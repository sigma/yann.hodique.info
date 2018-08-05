+++
title = "Get the keyboard you deserve !"
date = "2006-10-05T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "keyboard" ]

+++


The keyboard is (most of the time) the only communication device between you
and your beloved computer. Therefore it is quite evident that its importance
should not be underrated.  Think about it for one moment : is your keyboard
perfect? Do you enjoy its touch and feel? Do you like the way the keys are
placed? All in all is it optimal?  These are questions that you definitely
should be able to answer positively. And if you're not then enhance it !

## The hardware

The first thing of course is to find a nice basis for your keyboard. Should it
be large or small? Should it have a laptop touch? White? Black? What layout
would you consider the best starting point?  No answer is wrong, everyone has
different taste and expectations about a keyboard because it's so personal.

My preferred keyboard is
a
[Happy Hacking Keyboard Lite 2](https://www.amazon.com/Happy-Hacking-Keyboard-Lite2-White/dp/B000MTELL8). It
is small, it hasn't all those useless keys that are far too common, the control
key is in the right place. Also, it is a QWERTY keyboard. Even if I'm French,
I find the french AZERTY layout just insane, and I would not use it, at any
price. The reason for this is mostly that I'm a programmer, and since most of
the programming languages were developed using QWERTY, the placement of braces,
brackets, parentheses, backslashes, are very
important.

{{< figure src="https://images-na.ssl-images-amazon.com/images/I/41D3EhSc6NL.jpg" link="https://www.amazon.com/Happy-Hacking-Keyboard-Lite2-White/dp/B000MTELL8" alt="Happy Hacking Keyboard Lite 2">}}

## The software

Now that you have got your device, it is time to make it the powerful tool it
must be. Even if you have chosen a keyboard that suits your needs almost
perfectly, there will always be these small issues, nothing really, but
stillDon't let those annoy you, customize your keyboard !  After all, is it all
that important that what's written on the key matches what's written on screen
using it? I don't think so, most of the time you don't even look at the
keyboard, so who cares?

Once again I'll take my example. I'm French, right. QWERTY has no accents,
ok. Then I need a "Compose" key, and my best choice for it is the right diamond
key. Oh, and I use Emacs, hence the M-x things, so "Meta" will be the left
diamond (note that it's right under the `X`). What to do with "Alt"? For my
window manager I usually use "Super"-prefixed keys, so "Alt" becomes
"Super". That's it, here is the line im my `.xsession` that makes my keyboard
act this way:

```
setxkbmap -layout us -option -option compose:ralt,altwin:meta_super
```

Wait, I'm a lisp programmer, I love lisp, and thinking about it, the
parentheses are not so well-placed : second level keys are not like immediate
ones. Also, wouldn't it be better to have parentheses and square brackets
exchanged? After all, even on common conversation, you make use of parentheses
far more often than brackets. Also, most programming languages use them at
least as often.  Let's add a line to `.xsession`:

```
xmodmap ~/.Xmodmap
```

Finally, the content of  `.Xmodmap`:

```
keycode 0x12 =  9               bracketleft
keycode 0x13 =  0               bracketright

keycode 0x22 =  parenleft       braceleft
keycode 0x23 =  parenright      braceright
```

Thanks to
[Cliki and Marco](http://www.cliki.net/Editing%20Lisp%20Code%20with%20Emacs)
for the good advices !
