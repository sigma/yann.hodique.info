+++
categories = ["blog"]
date = "2015-12-26T08:24:31-08:00"
tags = [ "emacs", "code", "rant" ]
title = "Rant: Obfuscation in Emacs"

+++

There's been a bit of a discussion going on
[here](http://thread.gmane.org/gmane.emacs.devel/196537) regarding some very
weird beast in the Emacs code base. I find it quite interesting, cause it's yet
another instance that reveals the diversity of points of view and priorities in
FOSS. And like a lot of people, I have a strong opinion, so I'll rant a bit
about it :)

To give some context:

* by default, Emacs provides at startup information on Emacs and the GNU
  project (which is definitely fine, maybe useful even). In this case,
  a message saying "For information about GNU Emacs and the GNU system, type
  C-h C-a."
* Emacs being pretty much all about customization, it provides ways to disable
  that (again perfectly reasonable, after over 18 years spent in Emacs, I don't
  need the slightest reminder of what it is)
* Emacs being a flagship project of GNU, it goes out of its way to make sure
  that if that information is hidden, that's only because *you* didn't want it:
  you, not your administrator (do real people still have those?), and not the
  random guy you copied your Emacs configuration from (gosh, I miss the time
  when http://dotemacs.de was a reference)

That's this last part that's tricky. The code that does that follows, in all
it's "splendor". If you don't puke when reading this, chances are you don't
know Emacs lisp very well, or you have a very strong political agenda
(apparently RMS introduced that code back in 1994). Neither of those apply to
me, so I'll be back in a minute.

{{< highlight lisp >}}
(defun display-startup-echo-area-message ()
  (let ((resize-mini-windows t))
    (or noninteractive                  ;(input-pending-p) init-file-had-error
        ;; t if the init file says to inhibit the echo area startup message.
        (and inhibit-startup-echo-area-message
             user-init-file
             (or (and (get 'inhibit-startup-echo-area-message 'saved-value)
                      (equal inhibit-startup-echo-area-message
                             (if (equal init-file-user "")
                                 (user-login-name)
                               init-file-user)))
                 ;; Wasn't set with custom; see if .emacs has a setq.
                 (condition-case nil
                     (with-temp-buffer
                       (insert-file-contents user-init-file)
                       (re-search-forward
                        (concat
                         "([ \t\n]*setq[ \t\n]+"
                         "inhibit-startup-echo-area-message[ \t\n]+"
                         (regexp-quote
                          (prin1-to-string
                           (if (equal init-file-user "")
                               (user-login-name)
                             init-file-user)))
                         "[ \t\n]*)")
                        nil t))
                   (error nil))))
        (message "%s" (startup-echo-area-message)))))
{{< /highlight >}}

So, basically what it does (or tries to do) is to control tightly the only ways
to disable that message: either via `customize` or by putting in your init file
(`.emacs`, or `.emacs.d/init.el`) and nowhere else a particular expression, as
explained in the documentation of `inhibit-startup-echo-area-message`. In both
case, your current user name will be used.

```
Non-nil inhibits the initial startup echo area message.
Setting this variable takes effect
only if you do it with the customization buffer
or if your init file contains a line of this form:
 (setq inhibit-startup-echo-area-message "YOUR-USER-NAME")
If your init file is byte-compiled, use the following form
instead:
 (eval '(setq inhibit-startup-echo-area-message "YOUR-USER-NAME"))
Thus, someone else using a copy of your init file will see the
startup message unless he personally acts to inhibit it.
```

Now where people like me start having a problem with attempts like that is that
Emacs is pretty much trying to tell me what I should be doing with my
configuration:

* how I organize it. My init file is serious stuff, I don't want to have that
  silly form in there, instead I want it buried in an indirectly-loaded file
  (along with many other very minor tweaks) that I can pretty much forget about
* how I write it. Seriously? even `(set 'inhibit-startup-echo-area-message
  "YOUR-USER-NAME")` doesn't work. Let me emphasize this: source code that's
  supposedly *strictly equivalent*, or would even compile to the *exact same
  bytecode* will behave differently! That's beyond madness in my book: we've
  just intentionally made the *implementation* of Emacs lisp engine part of the
  *semantics* of Emacs lisp programs...
* how I replicate it. Yes I have multiple user names depending on the machine,
  no I don't want to modify my configuration for each case, or generate it just
  for that
* how I distribute it. I like to share my config publicly, and I dislike having
  stuff in there that's relevant to me only (it's bad enough that I probably
  have those anyway, I shouldn't be forced to)

To add insult to injury, the code that's attempting that is downright
disgusting, and of course flawed (everybody realizes this, but attempting to
constrain the user in Emacs is just doomed to fail), so it's pretty much
useless. Therefore, in exchange for hypothetically protecting users against
massively incompetent misguided ill-intentioned people (I'm personally more
worried about the competent ones, but that's probably just me), we hurt the
aesthetic sense of any developer who stumbles upon this monstrosity.  I guess
that's where Free Software and Open Source start diverging... but I personally
don't like trade-offs that result in shitty code.

Anyway, I'm still gonna use my right to publish free code that sidesteps that
abomination once and for all. I was briefly tempted to post it as a followup on
the `emacs-dev` mailing list, but that would be bad form (and pretty
useless). I'd rather let Google do their job. (bonus: disable the startup
screen as well). Oh yeah, I forgot to mention: I'd be *very* curious to know of
a single first-time user who will actually use `C-h C-a` to get information
about Emacs or GNU instead of asking Google these days. Imagining that just
because something is printed on screen people are actually gonna read it, or
act on it, is just wishful thinking. Just sayin'

{{< highlight lisp >}}
;; Yes, it's disgusting, but it's a portable way to disable that silly
;; message
(put 'inhibit-startup-echo-area-message 'saved-value
     (setq inhibit-startup-echo-area-message (user-login-name)))

;; and now for the startup screen
(setq inhibit-startup-screen t)
{{< /highlight >}}

There you go, you can put that somewhere in a dark corner of your Emacs
configuration and forget about it: it's portable, it just works, and it doesn't
open your init file a second time just for kicks (therefore makes the part
that's messing with the semantics of the language dead code). The second line
is more or less what I'd actually want that code to be. The first line is just
about "exploiting" a silly assumption in the "protection" code.

Note that another popular way of getting rid of that pesky message is the much
more brutal solution provided by the
[Emacs Wiki](http://www.emacswiki.org/emacs/EmacsNiftyTricks#toc8)

{{< highlight lisp >}}
(eval-after-load "startup" '(fset 'display-startup-echo-area-message 'ignore)))
{{< /highlight >}}

I don't like it too much though, cause unlike
`inhibit-startup-echo-area-message` which is part of the public interface of
Emacs, this function might change name without warning (although quite frankly
it's pretty unlikely). Well, mostly a matter of taste I guess.

Oh by the way, if you're an evil site administrator who wants to deny their
users the freedom to access information about Emacs, you can even put this in
`after-init-hook` (mostly for `inhibit-startup-screen` actually, reason left as
an exercise) and you'll be fine. There, I just enabled evil behavior, I guess
I should feel bad about it.

Seriously, let's be clear: if your users don't *know* they're in Emacs, or what
it is, or where to find the documentation, they're *never* gonna get the hang
of it (it's not like it's a particularly intuitive editor). So if you really
want to prevent them from using Emacs, just don't install it already.

All in all, I find the justification for that piece of code to be extremely
weak and unsubstantiated. Feels more like basic paranoia to me.

I sincerely hope that nobody will "iron" the stupid piece of code that
generates the need for this stupid hack... it's hopeless anyway.
Although the version control history shows that quite a few people have been
"improving" it over the years... *sigh*
