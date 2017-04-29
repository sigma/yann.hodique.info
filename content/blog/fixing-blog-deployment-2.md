+++
categories = ["blog"]
date = "2017-04-29T09:28:13-07:00"
tags = ["hugo", "wercker"]
title = "Fixing blog deployment, part 2"

+++

Followup to [this post]({{< relref "fixing-blog-deployment.md" >}}).

Better late than never I guess.

So I figured out a few things in that time, and the base image for the builder
of this blog is now alpine-based.

I'm also making use of the `solarized` theme for `pygments`. Since such plugins
are not supported by the `arjen/hugo-build` wercker step anyway, I need to put
those things in the base image for now.

Still, that reduced the size of the box by ~50%, which is always nice.

```docker
FROM alpine:3.5
MAINTAINER Yann Hodique <yann.hodique@gmail.com>

RUN apk add --no-cache bash git openssh-client py-setuptools py-pygments
RUN pip3 install --upgrade pip
RUN pip3 install pygments-style-solarized
```
