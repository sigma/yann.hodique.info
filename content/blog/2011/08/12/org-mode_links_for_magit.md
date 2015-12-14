+++
title = "Org-mode links for Magit"
date = "2012-04-09T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "emacs", "magit", "org" ]

+++


Since some people seem to find it useful, let's advertise it a bit :) I've
wrote a very small extension to [Org-Mode](http://orgmode.org/) and
[Magit](http://magit.github.com/magit/), that allows me to put links to magit
items in my org files.
See [https://github.com/sigma/org-magit](https://github.com/sigma/org-magit)

Currently 3 types of links are supported:

*  `magit:/path/to/repo::commit@<hash>`
*  `magit:/path/to/repo::status`
*  `magit:/path/to/repo::log`

But you probably don't want to bother, and use `org-store-link` from the
target buffer.

See the header of  `org-magit.el` for more information.

Within emacs, opening those links will just invoke magit with the proper
environment. When exporting to HTML, this extension tries to convert the links
to something meaningful, using a list of web frontend and their URL
schemes. Only [GitHub](http://github.com) is provided out of the box, but
nothing prevents from adding more. Actually I'm doing exactly that to point to
an internal [cgit](http://hjemli.net/git/cgit/about/) instance I have at work.

I'm using this as a set of bookmarks, and also to link actual commits to my
development tasks.

(once I'm done with a couple of improvements, I'll definitely push into magit
contrib)
