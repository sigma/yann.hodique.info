+++
categories = ["blog"]
date = "2016-01-02T10:53:03-08:00"
tags = ["emacs", "go", "opinion"]
title = "How modern languages will save the text editor"

+++

I've been an [Emacs](https://www.gnu.org/software/emacs/) user for many years,
and a coder for just as long. Somewhere in the middle, something weird
happened: while a text editor was initially considered a perfectly appropriate
way of writing code (in C, Lisp, Perl, Shell, ...), that ceased to be true for
a while. With the gain in popularity of complex languages like Java, or C++,
coders started to crave "fancy" features like code completion, refactoring, ...

And quite unfortunately, these advanced features relied on complex tooling
living outside of the languages themselves. Worse, the languages were developed
in complete ignorance of these tools, which made them somewhat hostile to those
goals (anybody who tried to implement a correct C++ parser knows what I mean,
life before [clang](http://clang.llvm.org/) was just painful). As a result,
very complex tools making heavy use of specialized partial parsers, static
analysis, and crazy heuristics emerged. And they emerged as part of even more
complex development suites to combine them all in a coherent form: the IDE
was born.

To tell the truth, I've never been a fan of the concept, and even though
I contributed to [KDevelop](https://www.kdevelop.org/) for a while, I did so
from the comfort of my beloved Emacs... Still, it'd be dishonest to pretend
IDEs like Visual Studio, Eclipse and the likes don't have a great impact, even
today, on how people ultimately develop code.

I guess my main gripe with this abundant tooling is that it's an all-or-nothing
deal: there's just no (easy) way I can take some of that experience and
integrate it somewhere else. At a more fundamental level, it seemed simply
"odd": why would we introduce "deficiencies" at the language level, just to
hope somebody else would work around them the best they can in tools?
For those reasons, I never actually used an IDE. But then, I *also* stayed away
from the languages that were "requiring" them as much as possible (with mixed
results, as I ended up writing a ton of code in those as well :)). Not
everybody has that luxury though, so text editors declined.

But more recently, things took a different turn (for the best I think): a new
language emerged that was promoting a different paradigm:
[Go](https://golang.org/).  Instead of making the tooling an afterthought, it's
been pretty much there at some level since inception. It even shows in the
language grammar itself, which is *designed* to enable fast compilation,
partial parsing, and a whole bunch of analysis tools. So instead of designing
a language to give programmers arbitrary bells and whistles they're used to
(completely disregarding the resulting complexity in the larger ecosystem), the
Go creators have made careful choices to create the best *overall* experience,
which includes not only code writing, but also code reading, code maintaining
and so on. So while some "features" that people might expect are noticeably
missing (macros, generics, ...), it's not *just* because those guys are lazy
(although all great engineers are :)) but *also* because the balance to maintain
is very subtle.

I personally find the result quite brilliant (some people obviously disagree),
and it's a real pleasure to write code in Go, but more importantly to me, I can
take the tooling, and integrate it pretty much anywhere. So a complex IDE is
not even particularly useful: all you need from your editor is to be able to
display the information the tools give, and to act on it.

And then, just like that, Emacs is a perfect environment to develop in Go. It's
not exactly *free* of course, in the sense that some integration code has to be
developed. But it's definitely several orders of magnitude easier than getting
a proper environment for
[Java](http://www.emacswiki.org/emacs/JavaDevelopmentEnvironment) or
[C++](http://tuhdo.github.io/c-ide.html), and *even* Python, despite
[Elpy](https://elpy.readthedocs.org/en/latest/)'s greatness.

If anybody's curious, here's my
[Go configuration for Emacs](https://github.com/sigma/dotemacs/blob/master/lisp/config/go-config.el). Except
for some `GOPATH`-related gymnastics, it's pretty straighforward.

I believe this new ability to embrace whatever environment the developer is
most familiar with, and with minimal effort, is part of Go's appeal. I, for
one, am most excited about the prospect of many developer-friendly tools to
come, and I really hope Go is just the first of many languages to follow the
same approach.
