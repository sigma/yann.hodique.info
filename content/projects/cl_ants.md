+++
title = "ICFP 2004"
date = "2012-04-09T17:48:00-00:00"
categories = [ "projects" ]

+++

## Background

I took part in the [ICFP](http://www.cs.luc.edu/icfp/) code contest in 2004,
with a few friends of mine from [Melting-Pot](http://www.melting-pot.org).

For those who don't know of ICFP code contest, let's have a few words on the
rule. A subject is published on the web site at a given time, and from then you
have 72 hours to submit your solution. That's all. The problems are often very
funny, and the idea of staying in front of a computer for 72 hours is also (did
I mention my geekness?)

For informations about the particular 2004 issue, have a look at the
[official website](http://www.cis.upenn.edu/~plclub/contest/index.php)

Our solution is based on a compiler written in C++ which takes a small C-like
dedicated language as input and generates "ant-assembly" language.
The compiler itself is rather nice and involves a very expressive
preprocessor. Anyway, it took us about 2/3 of the total time to come up with
the compiler, so that the real programs (the ones to be compiled the ant
brains) were not that efficient.

The result was a 83th place in the general ranking. Not so bad, but not as good
as it may have been with more time to think about strategies instead of
compilation issues.

## Idea

C++ was obviously not the best choice to write a compiler in very limited
time. It's efficient, it's clean, but it was much too long to develop
and debug.

Later, during my holidays (remember my geekness :p), I felt the need to
experiment a better solution. As you may find out on the web or even on this
site, I'm a big fan of Lisp languages, and I had the strong feeling that
I could come up with a functional solution very quickly.

Of course I had already the scheme of the solution in mind (that is, the
general design of the language I would need). We were 4 persons the first time,
and the general idea of the language to be compiled took us a few hours.
Anyway, in 6 hours, I wrote alone a compiler that is much better than our
submission (more expressiveness, opimizations,) **without** any prior knowledge
of [Common Lisp](http://www.cs.cmu.edu/Groups/AI/html/cltl/cltl2.html)
specificities (I like learning a language by programming such things).

## Target language

(shamelessly extracted from the
[rules](http://www.cis.upenn.edu/~plclub/contest/ants.html))

The behavior of ants is defined by a simple, finite state machine. Informally,
the instructions of this state machine can be described as follows:

|     |     |
| --- | --- |
| instruction  |  effect  |
|  ` Sense sensedir st1 st2 cond ` | Go to state   ` st1 `  if   ` cond `  holds in   ` sensedir ` ; and to state   ` st2 `  otherwise. |
|  ` Mark i st ` | Set mark   ` i `  in current cell and go to   ` st ` . |
|  ` Unmark i st ` | Clear mark   ` i `  in current cell and go to   ` st ` . |
|  ` PickUp st1 st2 ` | Pick up food from current cell and go to   ` st1 ` ; go to   ` st2 `  if there is no food in the current cell. |
|  ` Drop st ` | Drop food in current cell and go to   ` st ` . |
|  ` Turn lr st ` | Turn left or right and go to   ` st ` . |
|  ` Move st1 st2 ` | Move forward and go to   ` st1 ` ; go to   ` st2 `  if the cell ahead is blocked. |
|  ` Flip p st1 st2 ` | Choose a random number x from 0 to   ` p ` -1; go to   ` st1 `  if x=0 and   ` st2 `  otherwise. |


That's all. Now that's possible to write a brain directly with these
instructions, but obviously one could benefit from an abstraction that would
make the code easier to write a compiler.

## Description of the language

The compiler is able to compile a small lisp-like language (Lisp is **very**
good at using its own representation).

### Top level

There are two kinds of functional units in this languages

*  `func`: they are some kind of lisp functions, but limitations in the target
   code prevent from having a notion of `return`. Thus, they all end with
   a `call` to another `func`

{{< highlight cl >}}
(func function-name
  <statement 1>
  ...
  <statement n>)
{{< /highlight >}}

*  `macro`: as usual,  `macro` allows to inline a block of code. It is essential for the same reason: there is no  `return`. Thus, code replication is unavoidable

{{< highlight cl >}}
(macro macro-name
  <statement 1>
  ...
  <statement n>)
{{< /highlight >}}

### Statements

The body of a top-level form is composed of statements.

*  `progn`: a classical sequence in Lisp. It reflects the execution of all its arguments

{{< highlight cl >}}
(progn
  <statement 1>
  ...
  <statement n>)
{{< /highlight >}}

*  `turn`: the primitive action of making the ant turn its head in the given direction ( `'left` or  `'right` or any variable that contains one of these symbols)

{{< highlight cl >}}
(turn 'left)
{{< /highlight >}}

*  `mark`,  `unmark`: the primitive action of putting/removing a mark at the current position

{{< highlight cl >}}
(mark 0)
{{< /highlight >}}

*  `if`: the choice betwwen two statements, based on a condition

{{< highlight cl >}}
(if <condition>
    <statement 1>
  <statement 2>)
{{< /highlight >}}

*  `when` and  `unless`: trivial variants of  `if`
*  `cond`: generalized  `if`

{{< highlight cl >}}
(cond (<condition 1>
       <statement 1>)
      ...
      (<condition n>
       <statement n>))
{{< /highlight >}}

*  `drop`: drop the food in-place

{{< highlight cl >}}
(drop)
{{< /highlight >}}

*  `call`: branch to the beginning of another function

{{< highlight cl >}}
(call 'another-function)
{{< /highlight >}}

*  `while`: classical loop structure

{{< highlight cl >}}
(while <condition>
  <statement>)
{{< /highlight >}}

*  `dotimes`: constant loop

{{< highlight cl >}}
(dotimes 3
  <statement>)
{{< /highlight >}}

*  any other symbol is treated as an invocation to a macro

### Conditions

Several instructions produce boolean results, to be used within the context of conditional statements.

*  `pickup`: try to pick food up there

{{< highlight cl >}}
(if (pickup)
  ...)
{{< /highlight >}}

*  `flip`: basic  *random* action. The highest the argument, the most unlikely the event.

{{< highlight cl >}}
(if (flip 3)
  ...)
{{< /highlight >}}

*  `move`: a movement may fail, due to a rock, another ant, 

{{< highlight cl >}}
(if (move)
  ...)
{{< /highlight >}}

*  `sense`: try to detect something somewhere

{{< highlight cl >}}
(if (sense 'left-ahead 'food)
  ...)
{{< /highlight >}}

*  `not`,  `or`,  `and`: classical composition operators

## Examples

The engine makes it possible to express in a lisp-like language strategies like the following

{{< highlight cl >}}
;; this will be inlined for each "invocation"
(macro uturn ()
  (dotimes 3 (turn left)))

;; entry point of the ant brain
(func main
  ;; let's randomize a bit the initial direction
  (if (flip 3)
      (turn 'left)
      (when (flip 2)
        (turn 'right)))
  ;; from now, search some food. Note that the "function" will never return
  ;; since it can't be expressed easily with the basic instructions
  ;; (plus it's not that useful)
  (call 'search-food))

(func search-food
  (while (!(pickup))
    (cond ((sense 'left-ahead 'food)
           (progn
             (turn 'left)
             (while (not (move)))
             (while (not (pickup)))
             (uturn)
             (while (not (move)))
             (turn 'right)
             (call 'back-to-home)))
          ((sense 'right-ahead 'food)
           (progn
             (turn 'right)
             (while (not (move)))
             (while (not (pickup)))
             (uturn)
             (while (not (move)))
             (turn 'left)
             (call 'back-to-home)))
          ((!(move))
           (turn 'left)
           (when (flip 2)
             (turn 'left)))))
  (uturn)
  (call 'back-to-home))

(func back-to-home
  (while (not (sense 'here 'home))
    (unless (move)
      (turn 'right)))
  (drop)
  (call 'go-out-from-home))

(func go-out-from-home
  (uturn)
  (while (sense 'here 'home)
    (when (move)))
  (call 'search-food))

;; compile everything from "main"
(make-program)

;; dump the result in a suitable format
(dump-program "primitive-ant")
{{< /highlight >}}

## Solution

Let's see the code of the compiler

{{< highlight cl >}}
(defpackage icfp
  (:use common-lisp)
  (:export func macro make-program dump-program))

(in-package icfp)

(defvar *program* nil)
(defvar *functions* nil)
(defvar *macros* nil)
(defvar *optimize* t)

(defmacro while (condition &body body) `(do () ((not ,condition) t) ,@body))

(defun indexes-of (elem list)
  (let ((counter -1))
    (loop for e in list
       do (incf counter)
       if (equal e elem) collect counter)))

(defun uniquify (list)
  (loop for elem in list
     if (not (member elem acc)) collect elem into acc
     finally (return acc)
       ))

(defmacro single-instr (&rest instr) `(list (list ,@instr)))

(defmacro relative-jump (n) `(cons 'relative ,n))

(defun relative-jump-p (elem)
  (and (consp elem) (eq (car elem) 'relative)))

(defmacro absolute-jump (n) `(cons 'absolute ,n))

(defun absolute-jump-p (elem)
  (and (consp elem) (eq (car elem) 'absolute)))

(defmacro extern-function (f) `(cons 'extern ,f))

(defun extern-function-p (elem)
  (let ((el elem))
    (and (consp el ) (eq (car el) 'extern))))

(defmacro marker-symbol (n) n)

;; Instructions
(defun compile-goto (where)
  (single-instr 'flip 1 where where))

(defun compile-turn-instr (direction)
  (assert (or (eq direction 'left)
              (eq direction 'right)))
  (single-instr 'turn direction (relative-jump 1)))

(defun compile-mark-instr (marker)
  (single-instr 'mark (marker-symbol marker) (relative-jump 1)))

(defun compile-unmark-instr (marker)
  (single-instr 'unmark (marker-symbol marker) (relative-jump 1)))

(defmacro mapindex (i f seq)
  `(let ((,i 0)) (mapcar #'(lambda (s) (apply ,f (list s)) (incf i)) ,seq)))

(defun compile-if-instr (condition &optional then &rest else)
  (let ((compiled-cond (compile-cond condition))
        (compiled-then (compile-instr then))
        (compiled-else (compile-block else)))

    (mapindex i #'(lambda (ins)
                    (nsubst (relative-jump (- (length compiled-cond) i))
                            'alt1 ins))
              compiled-cond)
    (mapindex i #'(lambda (ins)
                    (nsubst (relative-jump (- (+ (length compiled-cond)
                                                 (length compiled-then)
                                                 (if else 1 0)) i))
                            'alt2 ins))
              compiled-cond)
    (append compiled-cond
            compiled-then
            (if else (compile-goto (relative-jump (+ (length compiled-else) 1))))
            compiled-else)))

(defun compile-when-instr (condition &rest then)
  (compile-if-instr condition (cons 'progn then)))

(defun compile-unless-instr (condition &rest else)
  (apply #'compile-if-instr condition nil else))

(defun compile-cond-instr (&rest clauses)
  (let* ((compiled-clauses
          (loop for cl in clauses collect (list (compile-cond (car cl))
                                                (compile-block (cdr cl)))))
         (total (+ 1
                   (length clauses) ; jumps
                   (apply #'+ (mapcar #'(lambda (cl)
                                          (+ (length (car cl))
                                             (length (cadr cl)))) compiled-clauses)))))
    (mapcan #'(lambda (cl)
                (let ((condition (car cl))
                      (bl (cadr cl)))
                  (setq total (- total (length condition) (length bl) 1))

                  (mapindex i #'(lambda (ins)
                                  (nsubst (relative-jump (- (length condition) i))
                                          'alt1 ins))
                            condition)
                  (mapindex i #'(lambda (ins)
                                  (nsubst (relative-jump (- (+ (length condition)
                                                               (length bl) 1) i))
                                          'alt2 ins))
                            condition)
                  (append condition
                          bl
                          (compile-goto (relative-jump (+ total 1))))))
            compiled-clauses)))

(defun compile-while-instr (condition &rest body)
  (let* ((compiled-cond (compile-cond condition))
         (compiled-body (compile-block body))
         (l (+ (length compiled-cond) (length compiled-body))))

    (mapindex i #'(lambda (ins)
                    (nsubst (relative-jump (- (length compiled-cond) i))
                            'alt1 ins))
              compiled-cond)
    (mapindex i #'(lambda (ins)
                    (nsubst (relative-jump (- (+ l 1) i))
                            'alt2 ins))
              compiled-cond)
    (append compiled-cond
            compiled-body
            (compile-goto (relative-jump (- l))))))

(defun compile-dotimes-instr (n &rest body)
  (assert (numberp n))
  (let ((compiled-body (compile-block body))
        (res nil))
    (loop for x below n do (setq res (append res compiled-body)))
    res))

(defun compile-drop-instr ()
  (single-instr 'drop (relative-jump 1)))

(defun compile-call-instr (function)
  (single-instr 'flip 1 (extern-function function) (extern-function function)))

(defmacro macro-body (m) `(caddr (assoc ,m *macros*)))

(defmacro macro-args (m) `(cadr (assoc ,m *macros*)))

(defun args-subst (sym-args real-args body)
  (assert (equal (length sym-args) (length real-args)))
  (let ((fbody body))
    (loop for sym in sym-args for real in real-args
         do (nsubst real sym fbody))
    fbody))

(defun compile-macro-instr (m &rest args)
  (let ((body (macro-body m))
        (sym-args (macro-args m)))
    (assert body)
    (compile-block (args-subst sym-args args body))))

;; Conditions
(defun compile-pickup-cond ()
  (single-instr 'pickup 'alt1 'alt2))

(defun compile-flip-cond (value)
  (single-instr 'flip value 'alt1 'alt2))

(defun compile-move-cond ()
  (single-instr 'move 'alt1 'alt2))

(defun compile-sense-cond (place kind)
  (single-instr 'sense place 'alt1 'alt2 kind))

(defun compile-not-cond (condition)
  (let ((compiled-cond (compile-cond condition)))
    (nsubst 'alt3 'alt1 compiled-cond)
    (nsubst 'alt1 'alt2 compiled-cond)
    (nsubst 'alt2 'alt3 compiled-cond)
    compiled-cond))

(defun compile-or-cond (&rest conditions)
  (cond ((null conditions) nil)
        ((equal (length conditions) 1)
         (compile-cond (car conditions)))
        (t
         (let ((first (compile-cond (car conditions)))
               (rest (apply #'compile-or-cond (cdr conditions))))
           (nsubst (relative-jump (length first)) 'alt2 first)
           (append first rest)))))

(defun compile-and-cond (&rest conditions)
  (cond ((null conditions) nil)
        ((equal (length conditions) 1)
         (compile-cond (car conditions)))
        (t
         (let ((first (compile-cond (car conditions)))
               (rest (apply #'compile-and-cond (cdr conditions))))
           (nsubst (relative-jump (length first)) 'alt1 first)
           (append first rest)))))

(defun compile-t-cond ()
  (single-instr 'flip 1 'alt1 'alt1))

(defun compile-nil-cond ()
  (single-instr 'flip 1 'alt2 'alt2))

(defun expand-args (args)
  (mapcar #'(lambda (x)
              (cond ((and (consp x) (eq (car x) 'quote))
                     (cadr x))
                    ((symbolp x) (symbol-value x))
                    (t x)))
          args))

(defun build (f args)
  (apply f (expand-args args)))

(defun compile-cond (condition)
  (if (not (consp condition))
      (case condition
        ('nil (compile-nil-cond))
        ('t (compile-t-cond))
        (t (error "No such condition constant")))
      (let ((instr (car condition))
            (args (cdr condition)))
        (case instr
          ('pickup (build 'compile-pickup-cond args))
          ('flip (build 'compile-flip-cond args))
          ('move (build 'compile-move-cond args))
          ('sense (build 'compile-sense-cond args))
          ('not (build 'compile-not-cond args))
          ('! (build 'compile-not-cond args))
          ('or (build 'compile-or-cond args))
          ('and (build 'compile-and-cond args))
          (t (error "No such condition operator"))
          ))))

(defun compile-instr (instruction)
  (let ((instr (car instruction))
        (args (cdr instruction)))
    (when instr
      (case instr
        ('progn (mapcan #'compile-instr args))
        ('turn (build 'compile-turn-instr args))
        ('mark (build 'compile-mark-instr args))
        ('unmark (build 'compile-unmark-instr args))
        ('if (build 'compile-if-instr args))
        ('when (build 'compile-when-instr args))
        ('unless (build 'compile-unless-instr args))
        ('cond (build 'compile-cond-instr args))
        ('drop (build 'compile-drop-instr args))
        ('call (build 'compile-call-instr args))
        ('while (build 'compile-while-instr args))
        ('dotimes (build 'compile-dotimes-instr args))
        (t (apply 'compile-macro-instr instr args))
        ))))

(defun compile-block (body)
  (compile-instr (cons 'progn body)))

(defun compile-function (body)
  (let ((compiled-body (compile-block body)))
    (append compiled-body
            ;; protect from going out a bad designed func (a func should always
            ;; end by a call). This should NEVER be executed. In case it is
            ;; anyway, let's re-execute the function. The useless code will be
            ;; removed at optimization time anyway.
            (compile-goto (relative-jump (- (length compiled-body)))))))

(defmacro func (name &body body)
  `(let ((compiled-function (compile-function ',body)))
     (setq *functions* (cons (cons ',name compiled-function)
                             (delete-if #'(lambda (f) (eq ',name (car f))) *functions*)))))

(defmacro macro (name args &body body)
  `(setq *macros* (cons (list ',name ',args ',body)
                        (delete-if #'(lambda (f) (eq ',name (car f))) *macros*))))

(defmacro function-body (f) `(cdr (assoc ,f *functions*)))

(defun function-dependencies (f)
  (mapcan #'identity
          (let ((body (function-body f)))
            (loop for instr in body collect
                 (loop for elem in instr
                    if (and (consp elem ) (eq (car elem) 'extern))
                    collect (cdr elem))))))

(defmacro run-on-program-with-counter (counter f)
  `(let ((counter 0))
     (mapcar #'(lambda (instr)
                 (let ((res (apply ,f (list instr))))
                   (setq ,counter (+ 1 ,counter))
                   res))
             *program*)))

(defun transform-matching-instr-function (match op)
  #'(lambda (instr)
      (loop for elem in instr
         if (apply match (list elem))
         collect (apply op (list elem)) else collect elem)))

(defun resolve-extern (f offset)
  (setq *program*
        (run-on-program-with-counter
         counter
         (transform-matching-instr-function
          #'(lambda (elem)
              (and (extern-function-p elem)
                   (eq (cdr elem) f)))
          #'(lambda (elem)
              (declare (ignore elem))
              (absolute-jump offset))))))

(defun resolve-relative ()
  (setq *program*
        (run-on-program-with-counter
         counter
         (transform-matching-instr-function
          #'relative-jump-p
          #'(lambda (elem)
              (absolute-jump (+ (cdr elem) counter)))))))

(defun resolve-absolute ()
  (setq *program*
        (run-on-program-with-counter
         counter
         (transform-matching-instr-function
          #'absolute-jump-p
          #'cdr))))

(defun goto-alist ()
  (let ((l (delete
            nil
            (run-on-program-with-counter
             counter
             #'(lambda (instr)
                 (if (and (eq (car instr) 'flip)
                          (equal (nth 2 instr) (nth 3 instr)))
                     (cons counter
                           (if (consp (nth 2 instr))
                               ;; called before (resolve-absolute)
                               (cdr (nth 2 instr))
                               ;; called on final version
                               (nth 2 instr)))))))))
    (loop for j in l collect
         (let ((orig (car j))
               (from (car j)))
           (while (assoc from l)
             (setq from (cdr (assoc from l))))
           (cons orig from)))))

(defun simplify-jumps ()
  (setq *program*
        (let ((g (goto-alist)))
          (run-on-program-with-counter
           counter
           (transform-matching-instr-function
            #'absolute-jump-p
            #'(lambda (elem)
                (cons (car elem)
                      (or (cdr (assoc (cdr elem) g))
                          (cdr elem)))))))))

(defun following (instr)
  (loop for elem in instr
       if (absolute-jump-p elem) collect (cdr elem)))

(defun non-dead-code ()
  (let ((to-follow nil)
        (followed nil)
        (current 0))
    (while (setq to-follow
                 (append
                  (delete-if
                   #'(lambda (n) (or (member n followed)
                                     (member n to-follow)))
                   (uniquify (following (nth current *program*))))
                  to-follow))
      (setq followed (append followed (list current)))
      (setq current (car to-follow))
      (setq to-follow (cdr to-follow)))
    (setq followed (append followed (list current)))
    (run-on-program-with-counter
     counter
     #'(lambda (instr)
         (if (member counter followed)
             instr)))))

(defun kill-dead-code ()
  (setq *program* (non-dead-code))
  (setq *program*
        (delete
         nil
         (let* ((indexes (indexes-of nil *program*)))
           (run-on-program-with-counter
            counter
            (transform-matching-instr-function
             #'absolute-jump-p
             #'(lambda (elem)
                 (cons (car elem)
                       (- (cdr elem)
                          (length (delete-if
                                   #'(lambda (i) (> i (cdr elem)))
                                   (copy-list indexes))))))))))))

(defun optimize-program ()
  (simplify-jumps)
  (kill-dead-code))

(defun link-program (functions)
  (mapcar #'(lambda (f) (resolve-extern (car f) (cdr f)))
          functions)
  (resolve-relative)
  (if *optimize*
      (optimize-program))
  (resolve-absolute))

(defun make-program ()
  (setq *program* (function-body 'main))
  (let ((active-functions '((main . 0)))
        (deps (function-dependencies 'main)))
    (do () ((not deps) t)
      (let ((f (car deps))
            (l (length *program*)))
        (if (not (assoc f active-functions))
            (progn
              (setq deps (append deps (function-dependencies f)))
              (setq *program* (append *program* (function-body f)))
              (setq active-functions (cons (cons f l) active-functions)))))
      (setq deps (cdr deps)))
    (link-program active-functions)))

(defun print-program ()
  (loop for instr in *program* do
       (progn
         (loop for el in instr
            do (progn
                 (princ el)
                 (princ #\Space)))
         (princ  #\NewLine)
         )))

(defun dump-program (filename)
  (assert (stringp filename))
  (with-open-file (s filename :direction :output :if-exists :overwrite)
    (loop for instr in *program* do
         (progn
           (loop for el in instr
              do (format s "~S " el))
           (format s "~%")))))
{{< /highlight >}}

## Conclusion

*  Would we have written all this in Lisp from the beginning, we would have had at least two entire days to think about strategies to implement, instead of less than one :p
*  Lisp rocks !
