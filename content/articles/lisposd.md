+++
title = "libxosd sous guile"
date = "2009-02-22T17:48:00-00:00"
categories = ["articles"]
+++

## Présentation

Bien souvent il arrive au programmeur en quête de réutilisation de vouloir se
lier à une bibliothèque partagée existante, mais qui malheureusement se trouve
être écrite dans un autre langage que celui qu'il compte employer. Dans le cas
le plus commun, il s'agira probablement d'une bibliothèque écrite en C (ce qui
correspond aux recommandations du projet GNU). Le programmeur n'a alors d'autre
choix que de recourir à une couche de **liaison** (**binding** en anglais) pour
importer les fonctionnalités de ladite bibliothèque dans son langage favori.

Dans ce tutoriel, nous nous intéresserons à l'exploitation de la
[libxosd](http://www.ignavus.net/software.html) dans un programme écrit (ou
extensible) en [guile](http://www.gnu.org/software/guile/guile.html), le
langage d'extension officiel du projet GNU, qui est une implémentation de
scheme. On pourra notamment envisager l'utilisation de cette liaison pour
l'affichage de noms de bureaux virtuels dans [Scwm](http://scwm.mit.edu), la
génération d'un énorme message d'avertissement lorsque vos comptes (gérés par
gnucash) seront dans le rouge, ou simplement l'écriture de scripts guile
servant à notifier un événement quelconque.

Pour être complets il convient de noter que les langages considérés ne sont
pas neutres: en effet guile exporte une interface native  C, ce qui
simplifie notablement le portage de la bibliothèque choisie.

## De C à guile

### Encapsulation de la structure de données

Pour commencer, il est bon de se rappeler (ou d'apprendre) que guile manipule
un seul type de données: `SCM`, aussi les fonctions que nous allons écrire
manipuleront ce type en lieu et place des arguments C des fonctions
"normales". Nous aurons donc recours à un certain nombre d'assertions et de
conversions pour passer des arguments scheme aux arguments réels dans de bonnes
conditions (et vice versa).

La libxosd adopte un modèle objet centré autour d'une structure  `xosd`. Ce type
jouant un rôle central, nous devons tout d'abord lui donner un équivalent
guile. Guile permet de définir des  `smobs` (**small objects**), qui nous permettront
de faire ce que nous voulons avec les objets  `xosd`. La définition d'un  `smob` se
fait simplement en lui donnant un nom et une taille :

{{< highlight c >}}
xosd_tag = scm_make_smob_type(xosd,sizeof( xosd *));
{{< /highlight >}}

Remarquons que nous n'avons pas fait de lien entre le `smob` et la structure de
données que nous voulons encapsuler `(xosd *)`. Ce lien ne sera fait qu'à la
création d'un nouvel objet du type de ce `smob`.

Ainsi, `SCM_RETURN_NEWSMOB(xosd_tag, w);` où `w` est de type `xosd*` crée un
objet guile auquel l'identifiant `xosd_tag` est attaché, et dont on pourra
contrôler le type ultérieurement. Nous reviendrons sur cette étape lorsque nous
nous intéresserons à l'écriture d'un constructeur.

Passons à présent aux fonctions permettant conversion d'un `SCM` en son
véritable type C.

Nous aurons dans la majeure partie des cas à vérifier que le premier argument
de la fonction considérée est bien un pointeur vers un `xosd`. L'interface de
programmation des `smobs` nous fournit une macro pour cela:

{{< highlight c >}}
SCM_SMOB_PREDICATE(xosd_tag,osd)
{{< /highlight >}}

qui teste l'appartenance de `osd` (de type `SCM`) au type `smob` d'identifiant
`xosd_tag`. D'autres macros de test sont disponibles pour les types
"primitifs": ainsi on pourra écrire `SCM_INUMP(n)` pour tester l'appartenance
de `n` aux entiers naturels.

Pour effectuer un vrai test de validité d'appel, nous avons encore besoin de la
macro suivante

{{< highlight c >}}
SCM_ASSERT(condition,argument_effectif,
           argument_symbolique,fonction)
{{< /highlight >}}

qui se chargera d'interrompre l'exécution et de lever une erreur si
`condition`n'est pas respectée.

Enfin, lorsque toutes le vérifications sont passées, il nous reste encore
à exécuter réellement les conversions de types nécessaire. Nous avons vu
précédemment la macro `SCM_RETURN_NEWSMOB` qui nous permettait de convertir un
`xosd*` en son smob associé, il est temps de découvrir la macro
`SCM_SMOB_DATA(x)` qui, appliquée au résultat précédent, retourne le `xosd*` de
départ.  Dans la suite, nous utiliserons la macro suivante.

{{< highlight c >}}
#define XOSD(x) ((xosd *) SCM_SMOB_DATA(x))
{{< /highlight >}}

Encore une fois, il existe également des macros de conversion pour les types
"primitifs", ainsi `SCM_INUM(n)` appelé sur un `SCM` renverra un `int`, et la
conversion est valide sous réserve que `SCM_INUMP(n)` soit vrai.

### Encapsulation d'une fonction

Une fois les données correctement encapsulées, l'encapsulation d'une fonction
se fait assez facilement suivant un schéma constant:

*  vérification des arguments;
*  appel du code utile;
*  construction de la valeur de retour.

Voyons un exemple complet d'encapsulation de la fonction
`xosd_set_horizontal_offset(xosd * osd, int n)`

{{< highlight c >}}
static SCM _wrap_xosd_set_horizontal_offset(SCM osd, SCM n) {
    SCM_ASSERT(SCM_SMOB_PREDICATE(xosd_tag,osd), osd,
               SCM_ARG1, "xosd-set-horizontal-offset!");
    SCM_ASSERT(SCM_INUMP(n), n,
               SCM_ARG2, "xosd-set-horizontal-offset!");

    xosd_set_horizontal_offset(XOSD(osd),SCM_INUM(n));

    return SCM_UNSPECIFIED;
}
{{< /highlight >}}

La valeur `SCM_UNSPECIFIED` retournée peut être vue comme l'équivalent d'un
`void` C: une valeur inutilisable.

### Adaptation des interfaces

En passant d'un langage à un autre, il apparaît rapidement que certaines
constructions, parfaitement adaptées dans l'un, se trouvent tout à fait
inadaptées dans l'autre. Ainsi, la fonction suivante semble complexe, et peu
transposable en scheme:

{{< highlight c >}}
int xosd_display (xosd *osd, int line,
                  xosd_command command, ...);
// command appartient à l'ensemble
// {XOSD_percentage, XOSD_string, XOSD_printf, XOSD_slider}
{{< /highlight >}}

Il est bien sûr possible d'écrire une fonction scheme qui adopte le même
comportement, mais la simple présence d'arguments génériques rendrait la
solution inélégante. Si des adaptations mineures de l'interface sont
envisageables, on pourra faire le choix de séparer les différents comportements
(les différentes valeurs de `command`) en plusieurs fonctions. D'autre part la
commande `XOSD_printf` est clairement écrite comme un substitut à la fonction
`printf()` du C, par conséquent, il est logique de ne **pas** la transcrire
telle quelle, mais d'émuler son comportement à travers la fonction scheme
`format` et grâce à la commande `XOSD_string`. Ainsi les chaînes de formatage
acceptées ne seront pas nécessairement les mêmes, mais elles seront cohérentes
avec les habitudes liées au langage.

Nous écrirons donc trois fonctions: `xosd-display-percentage`,
`xosd-display-slider`, et `xosd-display-string`.  Une fois ceci fait, on pourra
sans problème écrire `(xosd-display-string (format "%d" n))` pour simuler une
fonction `(xosd-display-printf)`

{{< highlight c >}}
// affiche str à la ligne line
int xosd_display_string(xosd *osd, int line, char *str) {
    return xosd_display(osd,line,XOSD_string,str);
}
  
static SCM _wrap_xosd_display_string(SCM osd, SCM line, SCM str) {
    SCM_ASSERT(SCM_SMOB_PREDICATE(xosd_tag,osd), osd,
               SCM_ARG1, "xosd-display-string");
    SCM_ASSERT(SCM_INUMP(line), line,
               SCM_ARG2, "xosd-display-string");
    SCM_ASSERT(SCM_STRINGP(str), str,
               SCM_ARG3, "xosd-display-string");

    xosd_display_string(XOSD(osd),SCM_INUM(line),
                        SCM_STRING_CHARS(str));

    return SCM_UNSPECIFIED;
}
{{< /highlight >}}

Dans un registre différent, il arrive qu'en C on émule le retour de plusieurs
valeurs par le passage de pointeurs vers des variables locales. Ainsi la
fonction `xosd_get_colour` a le profil suivant:

{{< highlight c >}}
int xosd_get_colour(xosd *osd, int *red,
                    int *green, int *blue);
{{< /highlight >}}

Cette façon de faire est tout à fait inappropriée en scheme (puisque l'on n'a
pas de pointeurs), où l'usage dans des cas semblables est plutôt de renvoyer
une liste de résultats. Ainsi on voudra plutôt:

{{< highlight scheme >}}
(xosd-get-colour osd)
  -> (42 42 42)
{{< /highlight >}}

### Constructeurs/destructeurs

libxosd est une bibliothèque objet (bien qu'écrite en C) et fournit des
fonctions pour construire et détruire des instances. Le premier réflexe
(malheureux, évidemment) sera d'encapsuler ces méthodes comme les autres, et de
les utiliser en l'état en lisp. Bien que cela fonctionne au premier abord, il
subsiste un problème majeur: supposons que nous ayions écrit une fonction
`xosd-destroy` qui sert de destructeur. Que se passera-t-il si nous réutilisons
l'objet détruit? la même chose qu'en C, une **segfault**. D'autre part, que se
passera-t-il si nous perdons un object `xosd` en faisant par exemple `(set!
xosd nil)` (ceci a pour conséquence d'affecter `nil`, qui est la valeur
**nulle** de scheme à la variable `xosd`)? absolument rien, mais il nous est
désormais absolument impossible de récupérer la mémoire allouée pour cet objet
(**memory leak**). Ces deux facettes du même problème mettent en exergue le
fait que dans un programme scheme, la mémoire doit être gérée automatiquement,
il faut donc s'assurer que les objets passent bien par le ramasse-miettes
(**garbage collector**) pour ne pas avoir de problème.

Guile fournit une interface pour associer à un `smob` une fonction de
libération et une fonction de marquage. La première sert à libérer
explicitement la mémoire, la seconde sert à marquer d'éventuels objets à passer
au ramasse-miettes.

Ces fonctions sont installées lors de la phase d'initialisation du module par
les appels suivants :

{{< highlight c >}}
scm_set_smob_mark(xosd_tag,mark_xosd);
scm_set_smob_free(xosd_tag,free_xosd);
{{< /highlight >}}

ils permettent au ramasse-miettes de prendre conscience de l'existence d'objets
supplémentaires, et surtout de la manière dont il doit les gérer.

Puisque notre `smob` n'est en fait rien d'autre qu'un `xosd*` la fonction de
marquage est triviale. La fonction de libération, quant à elle, est simplement
un enrobage autour de `xosd_destroy()`. En ce qui concerne le constructeur en
revanche, rien de particulier, si ce n'est qu'il ne faut pas oublier que l'on
a un `smob` à créer (cf encapsulation de la structure).

{{< highlight c >}}
// crée un objet xosd à n lignes
static SCM make_xosd(SCM n) {
    xosd * w;

    SCM_ASSERT(SCM_INUMP(n), n, SCM_ARG1, "make-xosd");
    w = xosd_create(SCM_INUM(n));
    SCM_RETURN_NEWSMOB(xosd_tag, w);
}

// rien à marquer, le smob est simple
static SCM mark_xosd(SCM xosd_smob) {
    return SCM_BOOL_F;
}

// libération du smob
static size_t free_xosd(SCM xosd_smob) {
    xosd * osd = XOSD(xosd_smob);
    xosd_destroy(osd);
    return sizeof( xosd * );
}
{{< /highlight >}}

Insistons sur le fait que la fonction de libération n'a absolument pas vocation
a être appelée par l'utilisateur (d'ailleurs elle renvoie un `size_t` qui ne
signifie rien en `scheme`). L'appel sera exclusivement interne.

Une fois ceci fait, la désallocation se fera automatiquement lors d'un passage
du ramasse-miettes. Ainsi, si nous reprenons notre exemple de `(set! xosd
nil)`, au prochain passage du ramasse-miettes, l'ancien contenu de `xosd` sera
détecté comme désormais inutilisable (car non atteignable) et `free_xosd()`
sera appelé.

### Définitions et export de module

Une fois que toutes les fonctions de la bibliothèque d'origine ont été
encapsulées pour présenter un profil plus **lisp**, il est encore nécessaire de
les enregistrer auprès de l'interpréteur à l'aide de la fonction
`scm_c_define_gsubr()`.

{{< highlight c >}}
scm_c_define_gsubr("make-xosd", 1, 0, 0, make_xosd);
{{< /highlight >}}

les trois paramètres entiers représentent respectivement le nombre de
paramètres obligatoires, le nombre de paramètres optionnels, et une valeur
booléenne signalant l'existence ou non d'un "reste" d'arguments, mais ce n'est
pas le propos ici. Cette instruction associe au symbole d'identifiant
"make-xosd" la fonction C `make_xosd()`.

Le langage scheme possède un système de modules (que l'on peut voir comme une
série d'espaces de (re)nommage), chacun exportant une interface à destination
de l'utilisateur. Par défaut, les fonctions déclarées dans un module ne sont
pas exportées (elles sont privées en quelque sorte), aussi nous devons
explicitement exporter chacun des symboles de fonctions définis, et ce grace
à `scm_c_export()` qui accepte une série d'arguments terminée par `NULL`

{{< highlight c >}}
scm_c_export("make-xosd", ... , "xosd-display-string", NULL);
{{< /highlight >}}

### Compilation

Une simple compilation en bibliothèque partagée suffit à rendre les fonctions
créées accessibles depuis guile, sous réserve qu'il existe une fonction
particulière `scm_init_<name>_module` si la bibliothèque s'appelle
`lib<name>.so`et que cette fonction déclare l'ensemble des symboles et
fonctions définis.

{{< highlight c >}}
SCM scm_init_xosdguile_module (void) {
    SCM module;
    module = scm_c_define_module("xosdguile", init_helper, NULL);
    return SCM_UNSPECIFIED;
}
{{< /highlight >}}

où `init_helper` est une fonction chargée de la création du module proprement
dit, en d'autres termes, il s'agit de la fonction contenant les instructions de
définition et export vues précédemment.

Si notre fichier C s'appelle `xosd_wrap.c`, la ligne de compilation idoine est
la suivante:

{{< highlight shell >}}
gcc -Wall -ansi -pedantic -shared -o libxosdguile.so -lxosd xosd_wrap.c
{{< /highlight >}}

## Exemple

Donnons maintenant un petit exemple d'utilisation. Pour être original, on veut
afficher un "Hello World!" en bas de l'écran, centré avec un décalage
supplémentaire de 80 pixels vers la droite. En outre, on désire qu'il
disparaisse de lui-même au bout de 5 secondes.

{{< highlight scheme >}}
(use-modules (xosdguile)) ;; chargement du module
(define osd-size 5) ;; on veut 5 lignes
(define osd-timeout 5) ;; les messages disparaissent en 5 secondes
(define osd (make-xosd osd-size)) ;; création d'un xosd
(define osd-position '(80 . 0)) ;; offset horizontal/vertical
  
(xosd-set-timeout! osd osd-timeout) ;; activation du timeout
(xosd-set-pos! osd 'bottom) ;; placement en bas
(xosd-set-align! osd 'center) ;; et au centre
(xosd-set-horizontal-offset! osd (car osd-position)) ;; décalage horizontal
(xosd-set-vertical-offset! osd (cdr osd-position)) ;; et vertical

(xosd-display-string osd 0 "Hello World !") ;; affichage
{{< /highlight >}}

Voyons une capture d'écran de la bête en fonctionnement. Franchement vous
attendiez quoi d'un "Hello World" ? :)

{{< figure src="/media/articles/lisposd.png" title="Killer app" class="full" >}}

## Références

* [projet (osd)](https://github.com/sigma/lisposd)
* [guile](http://www.gnu.org/software/guile/guile.html)

© Yann Hodique
