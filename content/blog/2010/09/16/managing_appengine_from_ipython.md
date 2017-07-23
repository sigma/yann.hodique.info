+++
title = "Managing AppEngine from IPython"
date = "2010-09-16T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "code", "python", "ipython" ]

+++


When developing a [Google Appengine](http://code.google.com/appengine/)
application, it can be a pain to manipulate the data in it, as you don't have
a console where to write python code (like you have in the
[dev appserver](http://code.google.com/appengine/docs/python/tools/devserver.html)).
Fortunately,
a
[remote api](http://code.google.com/appengine/articles/remote_api.html) exists.

I wanted a bit more, though:

* integration with [IPython](http://ipython.scipy.org), to benefit from code
  completion and other goodies
* ability to use my  `.netrc` as a credentials provider

So here is an IPython profile that does that (in addition, it wraps the
functionality of
[appcfg](http://code.google.com/appengine/docs/python/tools/uploadinganapp.html)).
Except for the bulkloader functionality, which is kind of ugly, the original
code from appengine was generic enough to allow that kind of manipulation in
a relatively clean way.


{{< gist sigma 556152 "ipy_profile_gae.py" >}}

And this is what it allows:
{{< gist sigma 556152 "ipython_session" >}}

Just in case, my  `~/.netrc` looks like:
{{< gist sigma 556152 "netrc" >}}

And of course, all those appengine libraries have to be somewhere in your python path. Here is the  `.pth` file for my dedicated [virtualenv](http://pypi.python.org/pypi/virtualenv)
{{< gist sigma 556152 "virtualenv_path_extensions.pth" >}}
