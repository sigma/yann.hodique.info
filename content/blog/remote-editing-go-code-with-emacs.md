+++
title = "Remote Editing Go Code With Emacs"
date = 2018-09-03T15:39:55-07:00
tags = [ "emacs", "code", "go", "hack" ]
categories = [ "blog" ]
+++

I've been in an uncomfortable place for a while: these days I primarily write Go code,
but on a machine where I don't have a Go toolchain (don't ask). As a result, I
basically SSH/tmux into a dev machine and do my work like that. When the
conditions are optimal that's ok-ish, but each network lag or brief
disconnection makes itself highly visible, and it's just not great. 

Also, that forces me to use a different Emacs instance than the one I use for
the rest of my regular activities, which is yet another annoyance I could easily
live without.

Of course Emacs has some support for that, namely TRAMP, which allows not only
for editing but also running tools on the remote side "transparently". That's
great, except that most Go-related Emacs modules I came to rely upon don't work
that well with TRAMP right now.
Long story short, they all use variants of `call-process` instead of the
TRAMP-aware `process-file` (which of course is no drop-in replacement either).

And so I've been silently suffering until today, when the magical combination of
will and time (yay, holidays !) appeared, and I started hacking my way around
the issue.

Tackled so far:

* go-eldoc (inline documentation for Go functions) [here](https://github.com/sigma/emacs-go-eldoc/commit/2bf260ea812321569be1297c0b9bc0f72068c1a3)
* git-gutter+ (git helper, not Go specific) [here](https://github.com/sigma/git-gutter-plus/commit/961f0257c4f33c8e603c64f4b53a89acbfa7a008)
* company-go (Go completion, plus documentation access) [here](https://github.com/sigma/gocode/commit/06ee386d3ad88774834ac5c98a29eb2e3d645e24) and [there](https://github.com/sigma/gocode/commit/da0cf3c538209da314180b7503607cd0dad86f7b)
* go-mode (top-level glue, including godef and gofmt) [here](https://github.com/sigma/go-mode.el/commit/d25f8d8cb267eaeee908590a30ad2942728f52c9) and [there](https://github.com/sigma/go-mode.el/commit/a49b051b11b9ec3bb08f9c33316e83cce58b49bc)

Note that for this last one I took a shortcut for `gofmt` and moved it from a
`before-write-hook` to an `after-write-hook`: I'm not entirely sure what this
dance with patches is about, seems simpler to just modify the file in place, and
it's way easier to handle remotely

So now at least I have the basics working. There's definitely more, but nothing
critical for me at least.

Oh and since I'm using spacemacs, the final integration looks like:

```lisp
(defun dotspacemacs/layers ()
  (setq-default
...
   dotspacemacs-additional-packages 
   `(
     (company-go :location 
                 (recipe :repo "sigma/gocode"
                         :fetcher github
                         :files ("emacs-company/company-go.el")
                         :version-regexp "v\\.\\(.*\\)"))
     (go-eldoc :location 
               (recipe :fetcher github
                       :repo "sigma/emacs-go-eldoc"))
     (git-gutter+ :location 
                  (recipe :fetcher github
                          :repo "sigma/git-gutter-plus"))
     (go-mode :location 
              (recipe :fetcher github
                      :repo "sigma/go-mode.el"
                      :files ("go-mode.el")))
    )
...))
```

After I convince myself that this is all working as intended, I shall clean up
those patches and propose them for upstream inclusion. In the meantime, it's
probably time to stop shaving that yak and actually work on the code I wanted to
write :).
