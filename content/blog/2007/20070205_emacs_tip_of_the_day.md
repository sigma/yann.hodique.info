+++
title = "Emacs tip of the day"
date = "2012-04-09T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "emacs" ]

+++


This one is for people whose brain is often preempted by urgent/attractive
tasks. Every once and then I find myself doing something that would help me to
perform the task I really should be spending time on. This can be the writing
of a powerful Emacs function for example or I suddenly realize I have to
replace occurrences of a certain pattern for consistency. When I'm done with
the subtask I'm no longer in the adequate state for switching back to the real
task, and sometimes I hardy remember what the task was, to start with. Here is
a small snippet that helps regarding this :

{{< highlight lisp >}}
;; Enter a recursive edit. C-M-c will bring back exactly there
(global-set-key (kbd "C-c r") (lambda ()
                                (interactive)
                                (save-window-excursion
                                  (save-excursion
                                    (recursive-edit)))))
{{< /highlight >}}

Hitting `C-c r` will put you in a "recursive editing mode", that is simply an
embedded call to the editing loop. The point here is that you can exit this
inner loop, which means that you return from the `recursive-edit`
function. This way, the recursive editing can be guarded by some context-saving
macros : here `save-window-excursion` and `save-excursion`. Once the user quits
the recursive edit, the context is restored, which means here that the windows
state, current buffer and position are restored : you're back in the state
where your brain was preempted without even needing to remember it.
