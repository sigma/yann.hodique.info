+++
categories = ["blog"]
date = "2016-03-11T18:15:57-08:00"
tags = ["hugo", "wercker"]
title = "Fixing blog deployment"

+++

Followup to {{< relref "blog-automated-deployment.md" >}}.

I've encountered a few difficulties today, when my
[Wercker](http://wercker.com/) setup started failing while installing the few
extra packages needed to deploy this blog.

I'm not sure exactly what happened, but I suspect that this is a case of the
`debian` docker image being cached too aggressively on Wercker, which made
packages installation more difficult (trying to access the wrong version or
something). Anyway, that was always a hack, and Docker images should be as
self-contained as possible.

Therefore, I built my own "box" (in Wercker lingo), that contains everything
that's needed to run the [Hugo](https://gohugo.io) build, and GitHub
pages deployment.

The image itself is very simple:

{{< highlight docker >}}
FROM ubuntu:15.10
MAINTAINER Yann Hodique <yann.hodique@gmail.com>

RUN apt-get update -q \
  && DEBIAN_FRONTEND=noninteractive apt-get install -qy git openssh-client python-pygments curl \
  && apt-get clean \
  && rm -rf /var/lib/apt
{{< /highlight >}}

On the blog side, it now looks like this:

{{< highlight yaml >}}
box: sigma/blog-builder:v0.1.3
build:
  steps:
    - arjen/hugo-build@1.8.1
  after-steps:
    - slack-notifier:
        url: $SLACK_URL
        username: wercker
deploy:
  steps:
    - lukevivier/gh-pages@0.2.1:
        token: $GIT_TOKEN
        domain: yann.hodique.info
        basedir: public
{{< /highlight >}}

Next step will be to figure out why my first Alpine-based attempt ended up
deploying something completely absurd...

Looks like there's no end to yak shaving.
