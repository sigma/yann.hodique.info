+++
title = "Magit extensions"
date = "2010-02-14T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "emacs", "magit", "git", "code" ]

+++


[Magit](http://zagadka.vm.bytemark.co.uk/magit/) is a great emacs tool to help
doing everyday's operations on [Git](http://git-scm.com/) repositories.
In particular, I really like the contextual approach, that splits the global
status in sections, meaning that the same high-level command can have subtle
semantics differences when applied to different objects.  As an example, the
high-level `magit-discard-item' function obviously takes different steps when
applied to an untracked file or to a staged hunk, or even to an entire branch.

The only disadvantage I see in the current implementation is the lack of
modularity in it: a heavy use of (really nice, btw) macros makes it mandatory
to have the complete set of features available when magit is loaded, so that
the generated code is complete.

Still, Git has lots of contributed helpers, which means the only stable way of
using them with Magit is to have them integrated upstream, which is not always
possible (take the example of custom helpers that cannot be distributed) or
does not even make sense (those helpers could be entirely bound to some
specificities of a project).

I've been working on an attempt to make Magit more modular, by allowing the
registration of so-called "extensions". See code here:
[http://github.com/sigma/magit/tree/extensions](http://github.com/sigma/magit/tree/extensions)

As a POC, I've extracted 2 features from the mainstream Magit, and converted
them to extensions: those are magit-svn and magit-topgit.  The way I see
things, those 2 features are exactly the kind I'd like to add to magit for my
daily work but that would not necessarily make sense as part of the core
project. As far as I know, the current behavior when those 2 extensions are
loaded is exactly the same as the regular upstream version (including placement
of the various sections)

The current code allows you to:

*  define new kind of sections (for example a  "topic" kind)
*  define how and where to display them (for example, right after the core
   "stashes" section)
*  hook new behavior for existing commands for those new sections (for example
   a way to discard such an item)
*  define new top-level commands, and associate menus

I tried to stay very close from the Magit way of doing things, and in
particular the way you express things in extensions is exactly the same as what
you do in the core code. Once again, tricky macros are at play to make the code
look the same.

What I'd like to see in the future (though I don't have much time to do it
right now :():

*  a magit-p4 (perforce integration),
*  maybe a magit-stgit (stgit in place of topgit for topic branches)
*  something to integrate with VMware's
   [ReviewBoard](http://www.reviewboard.org/)
*  code stabilization, and official proposal upstream (if there is an interest
   in this)
*  a way to activate those extensions on a per-project basis (I use both
   git-svn and git-p4 but never at the same time. same for topgit vs stgit)

Feel free to tell me what you think, any comment or advice is highly welcome !
