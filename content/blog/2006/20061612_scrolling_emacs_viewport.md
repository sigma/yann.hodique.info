+++
title = "Scrolling emacs viewport"
date = "2012-04-09T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "emacs" ]

+++


Sometimes I need to see more about the portion of text or code I am editing:
let's say I'm near the window's bottom, and I'd like to see what's after, but
without moving the point. The function `recenter` does that but it's not always
obvious to compute the argument you should give to it (for example `C-u -
1 0 C-l` would make sure the point is 10 lines before the bottom of the window,
but if you really meant 11 you must do it again).

This little code makes `<M-up>` and `<M-down>` scroll the "viewport", that is
exactly the opposite of regular scrolling: the point doesn't move, the
window does.

{{< highlight lisp >}}
;; viewport scrolling
(defun scroll-viewport (n)
  (let ((top (line-number-at-pos (window-start)))
        (cur (line-number-at-pos (point))))
    (recenter (+ (- cur top) n))))

(global-set-key (kbd "<M-up>") (lambda (arg) 
                                 (interactive "p")
                                 (or arg (setq arg 1))
                                 (scroll-viewport (- arg))))

(global-set-key (kbd "<M-down>") (lambda (arg) 
                                   (interactive "p")
                                   (or arg (setq arg 1))
                                   (scroll-viewport arg)))
{{< /highlight >}}
