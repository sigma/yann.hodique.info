+++
title = "Getting things done with GWT"
date = "2012-04-09T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "code", "gwt" ]

+++

It's been a long time I wanted to give a try to
[Google's GWT](http://code.google.com/webtoolkit/). Also, lately I needed more
and more a robust and straightforward implementation of David Allen's
[Getting Things Done](http://www.davidco.com/) to help me organize my
work. There it is, let's combine those two whishes and produce a working tool.

## GWT in brief

This toolkit gives you the opportunity to code your web application entirely in
Java while deploying it under its compiled form (HTML + JavaScript / Java
servlet). The advantages are obvious : during the development, you have access
to all the facilities of a Java environment (nice enough language when compared
to JavaScript ;-), debugger), to its robustness, and so on.

I've always been attracted by compilers from one high-level language to another
lower-level ones. The GWT compiler is clearly a success in this area. It makes
it really easy to introduce a layer of AJAX in your web application without
even thinking about it.

GWT is composed mainly of the following components:

*  a graphical toolkit following roughly the way of AWT, QT, so you feel like
   home really fast
*  a minimal implementation of the most useful `java.lang` and `java.util`
   classes for client use (translated to JavaScript after compilation)
*  a services API that deals with AJAX (client-side) and Java servlets
   (server-side)

There are also other interesting aspects (JUnit integration for example), but
I'll leave them for another day. All this is integrated in a very clever and
usable way, which makes it easy to develop your application. During
development, all you're going to use is provably "hosted mode", which is a pack
provided by Google, that includes an embedded Tomcat server (for servlets), an
embedded browser (based on mozilla), so that you can have a sample of your
application running in one click without needing to deploy anything. Plus, this
"hosted mode" really runs your Java code, so that you may as well debug it in
the meantime. Brilliant !

A small issue that is worth noting : it looks like when you use "hosted mode",
some informations are transmitted to Google. I have to admit I'm a bit
disappointed about that.

## GTD methodology

GTD seems to be very popular these days. The number of articles (in blogs
mostly mine is no exception now ;)) is constantly increasing, and the number of
related tools grows in proportion. Still I did not find a single tool that
really satisfying.

Here are my expectations for a perfect GTD tools:

*  it should be mostly online : I can't rely on a single device (especially
   **not** a laptop, and a PDA is not reactive enough)
*  there should be an offline client/cache/whatever : I'm not always online,
   even if I am most of the time
*  it should be **fast**, because most of the time you just add really small
   tasks to your list
*  it should be nice to look at, because you'll use it a lot :-)

## GWTD started

With my friend [Alexandre](http://gnurou.org), we started to write the tool of
our dreams : GWTD. Well, the name is nearly obvious I guess still we will have
to give it a meaning for completeness purpose. The two goals are :

*  to discover the possibilities of GWT
*  to have a working implementation of GTD that fills our needs.

The project is still in very early stage of development (given the small amount
of time we can afford on that, it's no surprise), bu we hope to come up with
a release soon enough.  The code (distributed under Apache licence 2.0) is
available at [code.google.com](http://code.google.com/p/gwtd/), and
a [Google group](http://groups.google.com/group/gwtd-devel) has also been
created.  Stay tuned !
