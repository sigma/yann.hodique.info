+++
title = "Emacs tip of the day"
date = "2006-12-12T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "emacs" ]
+++

I write a lot of papers these days (articles, thesis, ...). Often I need to fix
issues/errors/typos from dead-tree-printed-version annotations to my
LaTeX sources.

Unfortunately, `isearch-forward` ( `C-s`) is not really convenient for this
purpose since I use `fill-paragraph` ( `M-q`) all the time, so that the
consecutive words I'm looking for may as well lay on different lines.

Here is a quick solution : set `search-whitespace-regexp` to ` "[ \t\r\n]+"`
(*via* `M-x customize-option RET search-whitespace-regexp RET`).  It won't help
for `C-s`, but `isearch-forward-regexp` (`C-M-s`) may do the trick as well. The
only drawback left is when you search something that contains regexp meta
characters, but I can live with it for now.

An obvious solution would be to write a command that performs
`isearch-forward-regexp` but with a slightly modifed `isearch-mode-map` that
would insert escaped meta characters automagically.
