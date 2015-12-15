+++
title = "LDAP authentication"
date = "2006-09-22T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "ldap" ]
+++


Switching to LDAP as an authentication mechanism is something that causes a bit
of stress. However, it is really more flexible than traditional UNIX
authentication. For example, it is really nice to be able to create accounts
for a Subversion server without granting shell access (`/bin/false` shells are
not a clean option).

Surprisingly enough, it was really easy once the right tutorial was found
[here](http://wiki.splitbrain.org/ldap) at my favorite Wiki provider's :-)

Many thanks to Andreas Gohr for this simple and complete introduction.
