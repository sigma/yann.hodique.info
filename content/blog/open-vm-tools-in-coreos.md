+++
categories = ["blog"]
date = "2016-03-30T12:21:15-07:00"
tags = ["coreos", "docker", "vmware"]
title = "open-vm-tools in CoreOS"

+++

I've been running [CoreOS](https://coreos.com) for quite some time now, on
[VMware](https://vmware.com) platforms for the most part. Recently I've been
working on
[PXE-booting](https://coreos.com/os/docs/latest/booting-with-pxe.html) the VMs
instead of running
[OVAs](https://coreos.com/os/docs/latest/booting-on-vmware.html) though (I'll
explain why in another post).

The main problem with this kind of setup is that
[open-vm-tools](https://github.com/vmware/open-vm-tools) are not available in
the official PXE images, since they're meant to be platform-agnostic.

Various people have attempted to run open-vm-tools in a container instead.
After all, that's how we're supposed to ship software for those platforms.
In particular, https://github.com/godmodelabs/docker-open-vm-tools works pretty
well for my use-case.

The only minor detail that bugs me is the fact that open-vm-tools starts
running after Docker. For various reason, including the fact that I want to
customize the docker daemon based on information that transits through
open-vm-tools (which would be an interesting chicken-and-egg problem), I prefer
to use [rkt](https://coreos.com/rkt/docs/latest/) for that.

So, my [cloud-config](https://coreos.com/os/docs/latest/cloud-config.html)
looks like this instead:

{{< highlight yaml >}}
#cloud-config

coreos:
  units:
    - name: open-vm-tools.service
      command: start
      content: |
        [Unit]
        Description=Service for virtual machines hosted on VMware
        Documentation=http://open-vm-tools.sourceforge.net/about.php
        ConditionVirtualization=vmware
        [Service]
        Restart=always
        ExecStart=/usr/bin/rkt run --volume systemd,kind=host,source=/run/systemd --mount volume=systemd,target=/run/systemd --insecure-options=image --net=host docker://godmodelabs/open-vm-tools
{{< /highlight >}}
