+++
title = "Messing with keys"
date = "2012-04-09T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "keyboard" ]

+++


Context : as I mentioned in a [previous post]({{< ref
"blog/2006/20060510_get_the_keyboard_you_deserve.md" >}}), I use a slightly
modified qwerty map, where parentheses and brackets are switched.
Quite recently, for various reasons, I came to use
[rxvt-unicode](http://software.schmorp.de/pkg/rxvt-unicode.html) as
a terminal emulator.

Although being otherwise very nice, this little program used to suffer from an
annoying feature, namely the builtin iso-mode. Actually, it's not the iso-mode
itself that is annoying (after all, being able to compose rarely used
characters does not hurt) but the fact that it is hard-bound to the
control-shift combination.  There are at least 3 arguments against this
choice :

1.  such a sequence is easy to hit by accident
2.  anyone who would like to use emacs in console mode is screwed
3.  in any case, that feature should be an option

So I am very happy to hear that the default behavior has changed in the latest
version, and that iso-mode is now activated only in the `rxvt-unicode-ml`
flavor. I hope Debian will ship version 9.x soon.

Given that, the key handling in `rxvt-unicode` is still quite strange Remember
that I mentioned the switched brackets/parentheses ? This means that I have to
hit shift to compose a bracket. As you may know, the default escape character
for telnet (that enables you to close the session at the protocol level, for
example) is `^]` (that is `ctrl-]`). No need to say that with my configuration,
iso-mode was a real annoyance regarding this

Still, *without* iso-mode, the behavior is strange : hitting `^]` simply does
not work when `]` is not where expected (neither does it with `rxvt`, but
`xterm` is ok). Maybe it's related to my `~/.Xdefaults`, but I can't see why :

```
rxvt-unicode*background: DarkSlateGray
rxvt-unicode*foreground: Wheat
rxvt-unicode*cursorColor: Yellow
rxvt-unicode*font: xft:Dejavu Sans Mono-8
rxvt-unicode*reverseVideo: false
rxvt-unicode*scrollBar: false
rxvt-unicode*saveLines: 5000
rxvt-unicode*reverseWrap: true
rxvt-unicode.geometry: 80x25+20+20
rxvt-unicode*fullCursor: true
rxvt-unicode*scrollTtyOutput: off
rxvt-unicode*scrollKey: on
rxvt-unicode*titleBar: true
rxvt-unicode*color12: #84bdd2
```

Anyway, I've just accidentally discovered that `^}` works ! (I mean, in
`rxvt-unicode` **and** `xterm`) As far as I know, this is undocumented feature,
but I couldn't care less I'm at the point where I don't understand the problem,
nor the solution. Time to sleep I guess
