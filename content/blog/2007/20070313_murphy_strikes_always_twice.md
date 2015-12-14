+++
title = "Murphy strikes always twice"
date = "2012-04-09T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "murphy", "fs" ]

+++


One more time, I've witnessed the incredible truth of Murphy's laws. I'm
currently in the process of writing the last modifications of my thesis,
according to the requirements of my examiners. Obviously, this is when bad
things happen : last week my beloved and trusted XFS-managed home directory
failed miserably, and even refused to give me any data I knew she had ! No need
to say this was the day I was supposed to send a new version. Some hours later,
after `xfs_repair` options magic, I was able to recover all my data, except for
the cache of firefox (which made me extremely happy when I realized this fact).

Then yesterday, I was again supposed to send another version and guess what ?
This time, my root partition, managed by Ext3, followed the same path, and was
silently remounted read-only. I don't know what was the error, but it took
a really long time to fix : thousands of files under `/lost+found`, I was quite
anxious about it And once again, I was extremely pleased to see that the lost
files were kernel sources ! (and nothing else). Lucky guy.

Of course I keep all my thesis data under subversion, don't underestimate
Murphy : the point is that this machine was the only one I could use to work
without spending hours to set up a decent environment. Actually I've started to
put all my home directory on a subversion server, but I'm not done with it yet
(I'll blog on this eventually). Odds are that I will never need it again once
it's done, anyway.

After some investigation, it looks like the hard disk is the real culprit, not
the filesystems. Sounds right, I need to purchase a new one and wait patiently
for the next failure (I'm still working on the thesis, it **will** happen).
