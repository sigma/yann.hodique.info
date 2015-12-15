+++
title = "A mocking framework for Emacs"
date = "2011-11-04T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "emacs", "mocking", "code" ]

+++


As I'm currently trying to introduce some unit tests for Magit (see
[the unit-test branch](https://github.com/magit/magit/tree/unit-tests))
I realize I really need some mocking capabilities. For some reason, even though
we now have a testing framework released with Emacs
([ERT](http://www.emacswiki.org/emacs/ErtTestLibrary)), no mocking library
found its way to the core (yet?).

In its manual, ERT refers to the 3rd party
[el-mock.el](http://www.emacswiki.org/emacs/EmacsLispMock), but I don't like
the DSL approach too much (feels un-lispy to me) and it has definitions
I strongly disagree with ( "Mocks are temporary functions which accept
specified arguments and return constant value.") that make my life harder as
a test writer.  In my opinion, mocks are supposed to work in a record/replay
way, which implies to be able to record different outputs for different (or
even same) inputs. Returning a constant value is more of a stub thing. Actually
I don't care too much about the naming, but I need the feature anyway :p

So, even though it is really nice for basic mocking (and achieves very low
verbosity), el-mock.el is just not flexible enough for my purpose.

Here comes [mocker.el](https://github.com/sigma/mocker.el), my attempt at
achieving greater mocking flexibility.  Admittedly, this is much more verbose,
but for some twisted reason I like it better this way :) Note that the
architecture is meant to be flexible, so in theory it would even be possible to
define a way to express mocking expressions in a DSL similar to el-mock.el's.

2 quick examples taken from the documentation:

{{< highlight lisp >}}
(mocker-let ((foo (x y z)
                  ((:input '(1 2 3) :output 4)
                   (:input '(4 5 6) :output 10)))
             (bar (x)
                  ((:input '(42) :output 4))))
  (+ (foo 1 2 3)
     (foo 4 5 6)
     (bar 42)))
     
;;; make `foo' generate the fibonacci suite, no matter how it's called
(mocker-let ((foo (x)
                  ((:input-matcher (lambda (x) t)
                    :output-generator (lexical-let ((x 0) (y 1))
                                        (lambda (any)
                                          (let ((z (+ x y)))
                                            (setq x y y z))))
                    :max-occur nil))))
  (* (foo 1) (foo 0) (foo 42)))
{{< /highlight >}}

For more details, please refer to the
[page of the project](https://github.com/sigma/mocker.el). Bottom line,
`mocker-let` is just a macro around `flet` that specializes in recording
expected behaviors, then consuming them.

For what matters, I'd like it a lot if a proper mocking solution (whatever it
may be) made its way to Emacs (as part of ERT or not). Maybe if there are
enough people interested in this we can come up with something together :)
