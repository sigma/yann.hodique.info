+++
title = "A Community-based approach to Emacs packages Continuous Integration"
date = "2013-03-12T21:11:00-00:00"
categories = [ "blog" ]
tags = [ "emacs" ]

+++

In this post, I'd like to discuss some of my thoughts about the current state
of Emacs packages development, and in particular those related to proper
building/testing.

## The problems

Historically, Emacs packages have had a self-centered, optimistic view of the
world : not only did many packages reinvent the same wheel again and again to
avoid depending on other non-official packages (just because there was no good
user story for installing them), but most of them didn't have formal tests, let
alone automated ones.

I think that's vowed to change somewhat, with the recent introduction of the
package management facility
([package.el](http://repo.or.cz/w/emacs.git/blob_plain/HEAD:/lisp/emacs-lisp/package.el)
in Emacs 24) and the associated infrastructure
([GNU Elpa](http://elpa.gnu.org/), [Marmalade](http://marmalade-repo.org/),
[Melpa](http://melpa.milkbox.net/)). This opens the door to actually useful
libraries outside of Emacs core. Not that everything is perfect there (in
particular, the complete lack of proper namespaces is a still big obstacle),
but we're slowly going there.

Anyway, with packages being (hopefully) less and less self-contained comes
a big concern about reliability/reproducibility, the central question being
"how do I make sure my package works". This is partially adressed by the (also
quite recent) introduction of an
[official test framework](http://www.emacswiki.org/emacs/ErtTestLibrary) in
Emacs core.  But now, it's pretty clear that we need some better way of running
tests under various conditions.

To take an example, as a maintainer of [Magit](http://magit.github.com/magit/),
I'd like to make sure that the code runs as expected with:

*  any OS around
*  at least Emacs 23.x and Emacs 24.x
*  all git versions since something like 1.6.x

Ideally, all combinations should be tested, but I'd be quite happy already to
have a good subset.  Now of course, having systems covering all this is just
not realistic for me. I can afford running a couple of VMs from time to time,
covering some of the possibilities, but maintaining the whole thing is much
more work that I'm willing to do (not even considering the price).

Also, quite frankly, the activity of the vast majority of Emacs packages
development doesn't justify such a continuous integration infrastructure.
It's nice to have tests run automatically when code changes, but having test
slaves sitting there the whole day waiting for a commit is not.

We have 2 separate problems with (in my opinion) a common solution:

*  few package developers can afford doing proper testing for their packages
   (especially when those packages get complex)
*  virtually no projects are active enough that the cost of a complete setup
   would make any sense.

This calls for mutualization.

## A proposed solution

It's pretty clear there is an Emacs community (arguably several of them :)),
with people caring about the overall quality of the packages they're using.
Structures like [Marmalade](http://marmalade-repo.org/), or even the
[EmacsWiki](http://www.emacswiki.org) would not have taken off otherwise.

It's also very clear that some members wouldn't mind donating some CPU cycles
to run tests, as long as they don't have to think about it too much (hey, we're
**lazy** persons, right? :)). The only thing that's not clear is how many would
volunteer, and the level of involvement they'd accept. That's what I'd like to
discover :)

Now the good thing is that in this community, the main requirements are
fulfilled in each and every machine: have an OS, have Emacs installed (and
generally several versions of it). So basically, my point is that we have
a large pool of potential build/test slaves waiting to be used.  Again, this is
Emacs most of the packages are built and tested in a matter of seconds, with
very few resources consumed in the process.

As an example, my main computer, running Ubuntu Oneiric 64 bits is running on
average 10-12 hours a day. I also have a OSX Lion virtual machine that's
running about 6 hours a day. Both have development versions of Emacs 24
installed, and the Ubuntu one also has Emacs 23.3.1. I can definitely run 50
build jobs per day (and probably much more) on each machine without even
noticing. Actually I'm using them right now as test slaves in my current
implementation, and can't tell when jobs are triggered.  It'd would also be
very easy for me to setup some [LXC containers](http://lxc.teegra.net/) to
serve multiple OS versions, or maybe even to leverage
[virtualenv](http://pypi.python.org/pypi/virtualenv) to compose environments
with the proper application versions. One can go quite far with few resources.

So my idea is to put in relation those projects needing resources, and those
resources waiting to be used. To accomplish that, we need several things:

*  a way for projects to describe what they seek (Emacs 23 on Windows XP)
*  a *simple* way for slave owners to describe what they can offer (Emacs 24 on
   Debian Squeeze)
*  a central place in order to dispatch and monitor the jobs

Note that in that scenario, it's very important to *not* require build slaves
to be available all the time. That would raise a lot the entry barrier, and is
not compatible with a communitary approach. Instead, I'd prefer to rely on many
slaves being sporadically available to cover the "continuous" aspects of
continuous integration.

I have implemented something just like this on top of
[Buildbot](http://trac.buildbot.net/) (for no particular reason, except that it
sounded fun).  Before I go into details, you might want to have a quick look
there to see how it looks for
Magit:[http://dev.hodique.info/buildbot/waterfall?category=magit](http://dev.hodique.info/buildbot/waterfall?category=magit)

## Details of the current implementation

My current implementation relies on:

*  a heavily customized buildbot configuration (but it's only configuration,
   runs on regular buildbot) that separates slaves definition from the rest,
   for ease of management. That configuration is mirrored here:
   [https://github.com/sigma/emacs-ci](https://github.com/sigma/emacs-ci)
*  a [gitolite](https://github.com/sitaramc/gitolite) server that hosts both
   the buildbot master configuration, and the definitions for slaves (one
   repository per slave).

The idea here is that contrary to regular buildbot instances, the jobs to run
and the slaves on which to run them are administered completely separately:

*  the buildbot admins maintain the list of projects, their requirements, and
   the build steps to run.
*  each slave admin maintains the description of what his slave is able to do.

For now, each time something is modified on either side, buildbot tries to
reconfigure itself, by recomputing which combinations can be accomodated for
each project, and the list of available slaves for each combination.

Here is a very simple project definition, for one of my pet projects:
[https://github.com/sigma/emacs-ci/blob/master/emacs/projects/mocker.py](https://github.com/sigma/emacs-ci/blob/master/emacs/projects/mocker.py). The
`_project_combinations` member defines what features a slave should expose to
be of interest. If ever we decide that testing with Emacs 23 would be a good
idea, we can just add a new element there, and appropriate slaves will be
picked automatically, if they exist at all.

At the other end of the spectrum, a slave definition looks like this:

```
$ cat slave.yaml
password: sdoifio1pofsd

features:
  os: osx-lion
  arch: x64
  emacs: GNU Emacs 24.0.94.1

binaries:
  emacs: /usr/local/Cellar/emacs/HEAD/Emacs.app/Contents/MacOS/Emacs
```

This is a simple [YAML](http://yaml.org/) file that exposes a set of features,
and optionally where to find them (the emacs binary part). This way, the build
steps can be written in such a way that they adapt to the exact slave they're
running on. Of course, the buildbot slave itself still has to be created, but
it's a one-time job :)

Note that even the password to connect to the master is under the slave admin's
responsibility (and the slave name is just the repository name). This
potentially allows slaves to be created without any intervention from the
master admin. For example, were we to enable some gitolite wildcard
repositories, we could have some people create slaves entirely by themselves.

## Conclusion

Wow that was a long post. Thanks for reading this far :)

Again, this is a pretty prospective topic. I'm not saying any of this is the
only true way of dealing with the issue, I'm just saying that this issue needs
to be adressed somehow.

I'd love to hear from people who might be interested in taking part in the
discussion, so feel free to contact me if you have any idea of how you would
like to see things evolve. In addition to mail, I'm generally available on
IRC(channel #emacs, nickname Sigma)
