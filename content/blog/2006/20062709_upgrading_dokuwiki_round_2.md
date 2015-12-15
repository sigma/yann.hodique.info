+++
title = "Upgrading Dokuwiki, round 2"
date = "2006-09-27T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "dokuwiki" ]
+++


It looks like a simple `export DARCS_SLOPPY_LOCKS=1` solves the locks
issues. I still wonder why sshfs doesn't provide this but... well of course, it
still doesn't work. Now the problem is with the following error

`darcs: _darcs/prefs/repos-0: renameFile: permission denied (Operation not permitted)`

Ooook first I can't figure why a rename would be needed, and second why the
hell would it be forbidden? My user id is correctly translated by sshfs, I can
definitely `mv` files and `touch` them. Very strange. I'll have to ask #darcs,
it it exists :-)

Update: it looks like a workaround has to be activated in sshfs (the one
mentioned in the FAQ as a cure when `mv` fails).  For the record, my mount
command is now the following:

```
sshfs user@host: mnt/point -o idmap=user -o workaround=rename
```
