+++
categories = ["blog"]
date = "2017-04-29T17:07:44-07:00"
tags = ["nix", "hack", "docker"]
title = "nix in custom location"

+++

A few weeks ago, I posted
a
[comment on Reddit](https://www.reddit.com/r/NixOS/comments/64ndq8/what_if_i_cant_create_nix_apologies_if_faq/dg4u8p3/) to
explain very briefly how I use [Nix](https://nixos.org/nix/) at work.

Long story short, I have access to some powerful Linux machines on which
I don't have root privileges. Therefore, the default location of Nix, `/nix`,
is not practical, and `$HOME/nix` is overall a better choice.

The NixOS wiki, although outdated,
contains
[some documentation on how to achieve this](https://nixos.org/wiki/How_to_install_nix_in_home_(on_another_distribution)), but it's a bit all over the place.

Since I've been doing so myself multiple times, I thought I'd streamline the
process and provide a simple enough recipe.

My approach is to start from a `nixos/nix` Docker container (which uses `/nix`
initially), and reconfigure/rebuild Nix in place so that it moves itself to its
desired location. Then I copy the bits to the machine I'm targeting, along with
the piece of configuration that defines the new home for Nix.

Here is the script at the heart of this container:

```bash
#!/bin/bash

# create target nix location
mkdir -p ${NIX_NEW_HOME}/{overlays,store,var}

# make sure we have the latest nix. In particular we want overlays (in nix 1.11.8+)
nix-channel --update && nix-env -u -j `nproc`

mkdir -p ${HOME}/.config/nixpkgs/
ln -s ${NIX_NEW_HOME}/overlays ${HOME}/.config/nixpkgs/overlays

# create overlay for custom paths
cat > ${HOME}/.config/nixpkgs/overlays/01-nix-path.nix <<EOF
self: super:
{
    nix = super.nix.override {
        storeDir = "${NIX_NEW_HOME}/store";
        stateDir = "${NIX_NEW_HOME}/var";
    };
}
EOF

# create a nix version that targets the custom location
nix-env -i nix nss-cacert -j `nproc`

# create a nix version that also *lives* in the custom location
nix-env -i nix nss-cacert -j `nproc`

# find the user-environment that just got created
USER_ENV=`find ${NIX_NEW_HOME}/store/ -name "*-user-environment"`

ln -s ${USER_ENV} ${NIX_NEW_HOME}/var/nix/profiles/default-1-link
ln -s ${NIX_NEW_HOME}/var/nix/profiles/default-1-link ${NIX_NEW_HOME}/var/nix/profiles/default

# package the whole thing
tar -czf /target/mynix.tgz --owner=${NIX_NEW_UID:-0} --group=${NIX_NEW_GID:-0} ${NIX_NEW_HOME}
```

In practice, I use it through Docker like this:

```bash
export NEW_HOME=/home/yann

docker run -ti -v `pwd`:/target \
  -e NIX_NEW_HOME=${NEW_HOME}/nix \
  -e NIX_NEW_UID=81050 \
  -e NIX_NEW_GID=20 sigma/mynix

<mynix.tgz ssh yann@server "cd / ; tar -zxf - ; ln -s ${NEW_HOME}/nix/var/nix/profiles/default ~/.nix-profile; mkdir -p ~/.config/nixpkgs; ln -s ${NEW_HOME}/nix/overlays ~/.config/nixpkgs/overlays"
```

On my laptop, it takes about 1.5 hours to complete (then again, it rebuilds
*everything*). YMMV

After that it's business as usual, making sure to source
`$HOME/.nix-profile/etc/profile.d/nix.sh` from your shell, and so on.

Of course, doing this means that I can't reuse packages from the Nix binary
cache. But since I typically have a *lot* of computing power on those servers,
building a bunch of stuff is not much of an issue (massive rebuilds involving
glibc and such I tend to do overnight). On smaller machines that would be more
of an issue.

Note: I could make the archive much smaller (probably ~30MB instead of ~600MB)
by garbage-collecting most of the stuff that's not needed by Nix itself. But
I fully expect to build much more things in the destination server, so I might
as well transfer those build-time dependencies as well, it's cheaper than
rebuilding them there.
