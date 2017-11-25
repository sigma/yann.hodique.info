+++
title = "Mirroring some opensource projects"
date = "2007-03-20T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "oss", "code" ]

+++

Sometimes, it is real pain to stay synchronized with some projects you're using
and/or modifying. Just because they do not provide revisions, or because they
do, but with a tool you can't/don't want to use.

 falls under the first category : it is frequently updated on EmacsWiki, but
 downloading the project is quite inconvenient (you need to use a script, avoid
 wiki download limits, you don't have easy revisions, and so on). Therefore,
 I have setup a [bzr](http://bazaar-vcs.org/) repository that mirrors daily
 changes to a file in this project. It's far from being perfect of course,
 since the granularity of the commits is quite arbitrary, but I find it easier
 anyway. You can get *via*

{{< highlight shell >}}
bzr branch http://archives.hodique.info/bzr/icicles
{{< /highlight >}}

Dokuwiki and
[Arctic](http://www.chimeric.de/projects/dokuwiki/template/arctic), which are
respectively the wiki engine and the theme I use for this website, fall under
the second category : they are managed by
[darcs](http://www.darcs.net/DarcsWiki), which is a very nice vcs, but suffers
from two drawbacks (at least for me)

1.  it's not my tool of choice, I prefer [bzr](http://bazaar-vcs.org/)
2.  this site is hosted, and I'd like to easily record the patches I introduce
    in these softwares

The second point is the most important, of course :) In previous posts
([here]({{< ref "blog/2006/20062409_upgrading_dokuwiki.md" >}}) and
[there]({{< ref "blog/2006/20062709_upgrading_dokuwiki_round_2.md" >}})),
I explained how do deal with [darcs](http://www.darcs.net/DarcsWiki) with the
aid of [sshfs](http://fuse.sourceforge.net/sshfs.html), but it is unbearably
slow. Deploying [bzr](http://bazaar-vcs.org/) on the server is much easier
since it is written in [Python](http://python.org).

When it comes to converting a repository from one vcs to another one,
[Tailor](http://progetti.arstecnica.it/tailor) is the right tool to
use. I won't say the migration and synchronization were without trouble, it
would be a lie, but the result is very satisfying, once everything is
properly setup.

For the record, here is the configuration file I use for tailoring those two
projects :

{{< highlight ini >}}
[DEFAULT]
verbose = True
debug = True
encoding = utf8
encoding-errors-policy = ignore
projects = dokuwiki arctic

[dokuwiki]
root-directory = ~yann/Projects/tailor/dokuwiki
source = darcs:dokuwiki
target = bzr:dokuwiki
state-file = dokuwiki.state
start-revision = 2007-03-01

[bzr:dokuwiki]
repository = ~yann/Projects/bzr/dokuwiki.d/dokuwiki.darcs

[darcs:dokuwiki]
repository = ~yann/Projects/darcs/dokuwiki

[arctic]
root-directory = ~yann/Projects/tailor/arctic
source = darcs:arctic
target = bzr:arctic
state-file = arctic.state

[bzr:arctic]
repository = ~yann/Projects/bzr/arctic.d/arctic.darcs

[darcs:arctic]
repository = ~yann/Projects/darcs/arctic
{{< /highlight >}}

After running tailor, all I have to do is to `push` the repositories to their
public place. You can get them *via*

{{< highlight shell >}}
bzr branch http://archives.hodique.info/bzr/dokuwiki/dokuwiki.darcs
bzr branch http://archives.hodique.info/bzr/arctic/arctic.darcs
{{< /highlight >}}

Other branches will be created eventually, to reflect my personal
modifications. They are intended for personal use, but feel free to have
a look. After all, it's all free software :)
