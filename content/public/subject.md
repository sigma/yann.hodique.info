+++
title = "Validation de composants système"
date = "2012-04-09T17:48:00-00:00"

+++

## Contexte

Les systèmes embarqués tendent à évoluer vers les systèmes ouverts pour
accroître leur réactivité aux besoins de nouvelles applications. Dans
l'industrie des cartes à puce, par exemple, toutes les cartes étaient, il
y a encore quelques années, bâties sur le modèle de la carte bancaire, à savoir
une carte contenant une et une seule application. Aujourd'hui se développe une
nouvelle gamme de produits carte, dits multi-applicatifs. L'objet de ces cartes
est d'accroître la réactivité des logiciels embarqués en permettant le
chargement « *post issuance* » de nouveaux services : ce chargement de
nouvelles applications alors que les cartes sont déjà déployées dans les poches
de leurs utilisateurs soulève de nombreux problèmes dont celui, essentiel (pour
cette industrie), de la sécurité. Les cartes de la gamme JavaCard ont trouvé
dans le langage et surtout dans la machine virtuelle Java un moyen d'atteindre
cet objectif. La machine virtuelle se présente comme un support universel
capable de recevoir de nouveaux programmes à chaque instant, tout en
garantissant leur sécurité, en termes de confidentialité et d'intégrité. Le
caractère *universel* de la machine permet aux programmes de fonctionner sur
toutes sortes de microprocesseurs en définissant un ensemble d'*abstractions*
de haut niveau.

Cependant l'industrie a rapidement atteint la limite de cette démarche.
En effet, la machine virtuelle (vue comme un système d'exploitation à part
entière dans le cas des cartes à puce Java) acquitte l'industrie du problème de
la sécurité et de la portabilité des logiciels chargés « hors de
l'usine ». Toutefois, cette même machine virtuelle impose un modèle
d'abstraction : un paradigme d'applications à base d'objets persistants. Or les
applications cartes sont variées par exemple celles des cartes reposant sur
l'exploitation des normesISO 7816-4 voient en la carte un système de fichiers
(sécurisés), d'autres encore, reposant sur les cartes de norme ISO 7816-7
exploitent la carte en temps que base de données. Comment, dans ces conditions
distribuer des cartes en assurant qu'elles pourront supporter toutes sortes de
nouvelles applications ? Il est certes possible de réaliser un système de
fichiers à partir de la mémoire à objets persistants d'une carte Java, il est
de la même manière possible de réaliser sur ces fondements un système de
gestion de base de données, bien évidemment les performances en seront
profondément dégradées.

Pour dépasser ces difficultés des résultats de recherche récents ont promu un
ensemble de paradigmes architecturaux dans lesquels les systèmes évoluent par
adjonction de nouveaux « composants système ».  Ces architectures en
« exo-noyaux » ont pu être appliquées aux systèmes embarqués .  De tels
systèmes permettent la juxtaposition efficace d'abstractions et peuvent donc
s'adapter aux besoins de chaque application. Une application nécessitant un
nouveau paradigme système pourra demander le chargement du composant système
(appelé parfois *oslib*) au dessus d'une vue du matériel homogène (universelle)
mais non abstraite.

## Problématique

Les problèmes de fiabilité et de sécurité induits par l'extensibilité des
systèmes n'ont aujourd'hui que partiellement été adressés. En effet, la
sécurité apportée par les exo-noyaux embarqués, tels que `Camille`, n'adresse
que les problèmes d'innocuité du code : une extension système, au même titre
qu'une application, ne peut en effet pas obtenir les informations privées
appartenant à une autre application (garantie de confidentialité) ; elle ne
peut pas davantage fausser les résultats d'une opération extérieure (intégrité
des opérations). Néanmoins des problèmes de sécurité essentiels restent en
suspens, et cela tant en termes de disponibilité des ressources (éviter les
dénis de ressource), qu'en termes de temps de réponse garanti (assurer des
notions de « temps réel »).

Plus généralement les propriétés « non fonctionnelles » fournies aux
applications par les extensions du système ne sont pas aujourd'hui validées
formellement. Ainsi l'application doit nécessairement considérer les composants
systèmes additionnels qu'elle exploite comme des parties intégrantes de sa base
de confiance ( `Trusted Computing Base`).  Cela est dommageable à la
réutilisation des composants système embarqués par différentes applications qui
ne se font pas mutuellement confiance.

## Objectifs

Dans ce contexte, l'objectif de cette thèse est d'étudier les moyens d'assurer
la fiabilité des composants système chargés dynamiquement.  La possibilité
d'obtenir de telles garanties a été montrée dans de précédents travaux dans le
cadre de `Camille` sur un ensemble restreint de problèmes : les applications
qui utilisent de nouveaux composants leur font confiance, par contre, les
applications qui ne les utilisent pas voient leur intégrité et leur
confidentialité assurées. Il s'agit ici d'étendre ces premiers résultats à un
champ plus large de propriétés (et de systèmes) et de garantir la fiabilité des
composants système à toutes les applications.

On s'attachera dans un premier temps à exhiber et spécifier un échantillon de
propriétés caractéristiques des composants système servant de base
à l'expérimentation. Cette tâche, malgré son apparente trivialité, soulève de
nombreuses difficultés. Les éléments logiciels composant les abstractions
système sont en interaction avec des matériels qui imposent des règles d'usage
liées à leurs propriétés physiques. Dans ce contexte, une opération système
correcte sur le plan fonctionnel peut être biaisée par l'entremise d'un
comportement temporel (par exemple par le non respect du temps de rétention
minimal de sollicitation d'une page `EEPROM` ou, à l'inverse, par le non
respect d'une *deadline* critique imposée pour le temps de réponse à un
protocole de communication). Nous regroupons ces préoccupations temporelles
sous le terme de composants « temps réel ».

Aussi s'intéressera-t-on ensuite, d'une part, aux propriétés fonctionnelles des
composants et, d'autre part, à leurs interactions avec le reste du système :

*  pour les propriétés fonctionnelles, il s'agira d'adapter des travaux en
   cours dans l'équipe sur la spécification fonctionnelle de composants
   logiciels (en particulier les ramener dans le contexte du système peu de
   traitement des données par exemple pour en augmenter l'automatisation) ;
   certains composants système sont très difficiles à identifier en raison de
   leur très forte interaction avec les autres composants système et les
   applications elles-mêmes. Ils agissent sur les applications en changeant
   l'interprétation des opérations élémentaires exécutées. Il s'agit ici de
   parvenir à spécifier les modifications comportementales entraînées par le
   chargement d'un nouveau composant système ;

*  un autre type de composants système a un impact explicite sur le code
   applicatif. Le problème se rapproche alors d'un « tissage » d'un aspect
   « système » sur le code applicatif et le reste du code système. La
   spécification formelle des aspects et du tissage est aujourd'hui étudiée
   dans le cadre de la programmation de composants applicatifs mais peu dans le
   cadre de composants système (on peut citer par exemple les travaux autour
   des ordonnanceurs Bossa tissés dans le noyau Linux ). Il s'agira ici
   d'appliquer l'approche « tissage » aux composants système afin d'en
   spécifier les interactions avec les autres composants système et le
   code applicatif.

À plus long terme, en raison de la faible connectivité des systèmes embarqués
et de leurs faibles ressources, il est nécessaire qu'ils puissent, seuls,
s'assurer de la qualité des composants qu'ils chargent. Il sera donc nécessaire
d'envisager des solutions type `PCC` ( *Proof-Carrying Code*) . Pour que cela
soit possible par la suite, il est nécessaire de s'orienter, dès le début, vers
un type de preuves compactes et simples à valider. Rappelons que le principe de
base de `PCC` est qu'en règle générale, il est plus simple de vérifier la
validité d'une preuve préexistante que d'extraire cette preuve *ex
nihilo*. Cette préoccupation est essentielle pour que l'exo-noyau puisse
lui-même valider les composants chargés malgré les contraintes de performance
et de ressources (mémoire et communications) disponibles et devra donc guider
le choix des techniques utilisées.

## Contexte

Laboratoire : LIFL., USTL, Lille.

Équipe : STC (Spécification Test et Contraintes). 

Département d'enseignement : U.F.R d'IEEA (Informatique, Électronique, Électrotechnique et Automatique) de l'USTL.

## Collaborations

Ce sujet s'inscrit dans plusieurs actions en cours:

*  ATIP CNRS « Jeunes Chercheurs » STAC (Spécification et Test d'Applications
   à base de Composants logiciels), responsable Isabelle Ryl ;
*  ACI Sécurité « `SPOPS` : Sécurité et sûreté des systèmes d'exploitation
   ouverts pour petits objets portables de sécurité » notifiée en juillet 2003
   et coordonnée par Gilles Barthe (INRIA Sophia-Antipolis) ;
*  projet TACT-COLORS (COmposants LOgiciels Réutilisables et Sûrs), MOSAIQUES
   du contrat de plan état/région ;
*  projet « Composants Logiciels du Futur » de l'IRCICA (Institut de Recherche
   sur les Composants logiciels et matériels pour l'Information et la
   Communication Avancée), responsable Jean-Marc Geib.

Il s'effectuera également en collaboration avec :

*  Gemplus ;
