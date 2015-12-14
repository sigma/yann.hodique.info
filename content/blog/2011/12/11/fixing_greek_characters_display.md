+++
title = "fixing greek characters display"
date = "2013-03-12T21:11:00-00:00"
categories = [ "blog" ]
tags = [ "unix", "emacs" ]

+++


I generally like the
[Monaco](http://blog.mayfounder.net/2009/02/10/adding-a-new-ttf-font-to-linux/)
font, that I use for both Emacs and my rxvt-unicode terminals in GNU/Linux. In
order to achieve this, I had the following in my `~/.Xdefaults`:

```
emacs*font: Monaco:pixelsize=13

urxvt*font: xft:Monaco:pixelsize=13
```

Although it works perfectly fine for "normal text", it does a disastrous job
for fancier characters such as greek symbols. I don't know much about fonts
lookup, but I guess there's a fallback mechanism to elect another font when the
selected one does not contain the required symbols.

Anyway, as you can see in the following capture of my rxvt-unicode setup, it's
not exactly pretty (bottom left for the display before fixing). It gets mucc
better after modifying my `~/.Xdefaults` to contain:

```
urxvt*font: xft:Monaco:pixelsize=13,xft:Monospace
```

{{< figure src="/media/blog/2011/12/11/before_after_urxvt.png" link="/media/blog/2011/12/11/before_after_urxvt.png" alt="urxvt before and after fix" class="full" >}}

Similarly for Emacs, the default display was not great (although at least
readable). Putting something like the following in my `~/.emacs` solved the
issue for me.

```
(set-fontset-font "fontset-default" 'greek '("dejavu sans mono". "unicode-bmp"))
```

{{< figure src="/media/blog/2011/12/11/before_after_emacs.png" link="/media/blog/2011/12/11/before_after_emacs.png" alt="emacs before and after fix" class="full" >}}

And now my Lisp lambdas can finally look good :)
