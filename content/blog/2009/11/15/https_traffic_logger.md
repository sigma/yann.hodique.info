+++
title = "https traffic logger"
date = "2009-11-15T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "hack" ]

+++

I'm currently writing a Simulator (really a mock) for some server, and more
precisely for the SOAP interface it provides.  All traffic is done only in
https, and for debug purpose I needed to dump traces of the communication
between a client and the real WebService, to compare them with the
communication I have between the same client and the fake WebService.  So here
is a tiny script that acts as a proxy to unwrap/log/re-wrap SSL stuff.

{{< gist sigma 224171 >}}

I have the feeling all this should be doable by using ncat only, but did not
find how yet.
