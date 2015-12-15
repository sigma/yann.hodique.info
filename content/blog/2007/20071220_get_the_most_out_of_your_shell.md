+++
title = "Get the most out of your shell"
date = "2007-12-20T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "unix" ]

+++

There are some commands you use so often that you would not think there is room
for improvement there. Some time ago, I discovered
[most](http://www.jedsoft.org/most/), which is a pager meant as a replacement
for `less` (which was already a replacement for `more`, life is highly
cyclic). `most` is nice because of its sane features and keybindings (where
"sane" is a synonym for "emacs-like") Among other features, it allows to split
windows, to scroll horizontally, act as `tail` and so on. Also, it has a nice
ncurses interface that displays useful information and colors.  A standard
place for `most` is as the default pager, for example for viewing `man` pages.

```
export PAGER=most
```

This is what it looks like. Nice isn't it ?

![most in action](/media/blog/most.png)

And it allows editing. Well, not directly.  Ever wanted to just have a look at
a file, yes really, and then oh, if only I could change this line Let's quit
`less`, and open this file under
`emacs`. [Saint Ignucius](http://www.stallman.org/saint.html) would probably
say I'm not a **true believer** and that I deserve my fate it I'm not using
`emacs` as a pager, or even better as a shell. But still it happens to me all
the time.  So, `most` allows one to define a `MOST_EDITOR` to be launched when
hitting `e` while viewing a file. Now I have this in my `.zshrc` :

```
export MOST_EDITOR='(file=%s; line=%d; emacsclient -t +$line $file -a zile +$line)'
```

The expression is a bit more complex than it should be because of a strange
limitation : the filename (`%s`) must be **before** the line number (`%d`) in
`$MOST_EDITOR`, which is exactly the contrary of the requirements for using
`emacs` or similar programsAnyway, what this command does is launch
`emacsclient` in the current tty (using the neat multi-tty feature) in order to
edit the current file at the current position. In case `emacs` is not running
(which is admittedly rare), we fallback to `zile`, an emacs-like
lightweight editor.
