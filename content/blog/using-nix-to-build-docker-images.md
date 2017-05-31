+++
categories = ["blog"]
date = "2016-04-17T18:52:58-07:00"
tags = ["nix", "docker"]
title = "using nix to build docker images"

+++

Luca Bruno has written
[a very complete post](http://lethalman.blogspot.com/2016/04/cheap-docker-images-with-nix_15.html)
on how to build Docker images with Nix.

I personally find it very interesting, especially since the "normal" way of
building Docker images rely on the Dockerfile mechanism, which is extremely
simplistic, and doesn't have a lot of good properties, especially around
composability and reproducibility.

More specifically, one thing that's always bugged me is the fact that all
package management aspects of container building typically take place within
the image that's being created. This leads to insane patterns like very long
"RUN" instructions that span package list update, install, and cleanup.
This is normal though, as all those package managers are meant to manage the
local machine...

Anyway, Nix is different in that it *defines* a system instead of making it
evolve, which makes it a good fit to define a system *externally*.
And that always gives opportunities to do things more efficiently.

Following this, I made a few changes to Luca's approach, further overriding the
package definitions to achieve:

* removal of unneeded binaries
* usage of su-exec instead of gosu (which, as a go binary, is not exactly slim)
* removal of glibc, in favor of musl

This lead to a final image weighing 1.2MB. For reference, that about 13 times
less than the smallest image available on dockerhub (15MB, based on alpine),
and whooping 147 times less than the default one (177MB, based on debian).

I guess the main point of this whole experiment is that Docker images size is
a real concern. Even though a 15MB is not a big issue in itself, at scale
wasting 92% of the bandwidth used to transfer those images is more than
significant...

It's about time we start making sure this wastage can be avoided.
