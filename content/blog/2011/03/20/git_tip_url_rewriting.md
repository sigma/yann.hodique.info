+++
title = "Git Tip: url rewriting"
date = "2011-03-20T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "git" ]
+++

I use `git submodule` quite a lot. In particular, for my
[dotfiles](https://github.com/sigma/dotfiles) repository, in which I have tons
of dependencies to external packages, for example from the
[emacsmirror](http://www.emacsmirror.org/) project.  Of course, I also wrote
some of the packages, or need to point a repository where my personal
changes live.

This I achieve by having submodules like the following (from my  `.gitmodules`)

```
[submodule "_emacs.d/packages-src/magit"]
        path = _emacs.d/packages-src/magit
        url = git://github.com/sigma/magit.git
```

Now something that's been bugging me is the choice between allowing people to
clone the entire thing seamlessly (meaning using the public URL of the
repository, as above), and being able to hack directly inside the submodule
(which would require to use the private URL, like
`git@github.com:sigma/magit.git`). Since submodules only have one URL, I had
no big hope.

 **But** the clever [git](http://git-scm.com/) developers actually have
   a solution (from git 1.6.5 apparently). I just had to add this to my
   `.gitconfig`, and everything works transparently !

```
[url "git@github.com:sigma/"]
        pushInsteadOf = git://github.com/sigma/
```

What it does is extremely simple actually whenever I'm trying to push to the
anonymous version of one of my repositories (they all live under `sigma/`), the
ssh-based version is used instead.  So I can only refer to anonymous URLs in my
submodules, and everyone is happy.  Note that there's a more general
`insteadOf` setting, that does the same operation for both read and write
operations. See
[git clone](http://www.kernel.org/pub/software/scm/git/docs/git-clone.html)
documentation for details.

Like it. A lot.
