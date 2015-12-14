+++
title = "Upgrading Dokuwiki"
date = "2012-04-09T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "dokuwiki" ]

+++

I just finished upgrading my Wiki to latest development version of Dokuwiki.
This was not an easy task because I've made quite a large number of
modifications that needed to be ported or incorporated.  I've decided to
maintain a Darcs branch of both my Dokuwiki and Arctic theme.

For now this is no complete success since I access the filesystem via
[sshfs](http://fuse.sourceforge.net/sshfs.html), and it looks like it lacks
a proper lock implementation. I'll try to find a better way :-)

By the way, I also upgraded several plugins to their latest version, so that
you (and I) may experience some weird and uncontrolled border effects. That's
the price for living on the bleeding edge.
