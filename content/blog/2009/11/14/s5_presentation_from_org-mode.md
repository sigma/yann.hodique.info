+++
title = "s5 presentation from org-mode"
date = "2009-11-14T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "emacs", "org", "s5" ]

+++


Generating [S5](http://meyerweb.com/eric/tools/s5/) based presentations from an
[Org Mode](http://orgmode.org) file is something I've considered for a while.
I've just published some working code to do so:
[http://github.com/sigma/org-s5](http://github.com/sigma/org-s5)

The README file should give the basic steps to make it work.

Constraints I wanted to respect:

* no modification needed on Org-mode or S5
* easy to reuse the glue across multiple presentations
* adaptable enough to survive multiple S5 themes

This is probably far from being complete, but at least it's usable enough for
me to do presentations at work.

Hope it can be useful for someone else.
