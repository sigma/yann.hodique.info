+++
categories = ["blog"]
date = "2016-03-10T14:50:01-08:00"
tags = ["security", "gpg", "emacs", "yubikey"]
title = "Refreshing my GPG setup"

+++

Yesterday I started putting my [GPG](https://www.gnupg.org/) setup in
order... I should have done it a long time ago, as it was quite messy:

  * My main key was still an ancient 1024 bits DSA key from I create in '03
  * a few others were not used anymore
  * a more recent one was a smartcard-generated 2048 bits RSA key, which was
    way too easy to lose
  * I didn't have a proper backup plan for all this

In addition, I wanted to play with new
[Yubikeys](https://www.yubico.com/products/yubikey-hardware/yubikey4/) that
support OpenPGP from the get go, and to clean up my approach to development
machine bootstrapping, by using said yubikey as well.

Finally, I like the [keybase](https://keybase.io) approach to identity, and
want to use it in particular for [kbfs](https://keybase.io/docs/kbfs) once
it's ready.

So here are the components of this brand new shiny setup

  * 2 secure USB keys for backup
  * 2 Yubikeys 4
  * 1 master key that will be on secure storage only (on a daily basis I'm
    gonna use only subkeys)
  * 1 encryption subkey that's on secure storage *and* replicated on each
    yubikey
  * 1 signing and 1 authentication subkey per yubikey, generated on the
    yubikeys directly (they can die with them)
  * of course, revocation certificates for everything, stored on secure storage
  * [keychain](http://www.funtoo.org/Keychain) for managing my ssh- and
    gpg-agent
  * a keybase graph centered around the master key

## Secure USB keys ##

I purchased 2 of these bad boys: [Apricorn Aegis Secure Key](http://amzn.to/227gb13)

The idea is that I'm going to keep them in sync as much as possible, with one
being available at home and one in a secure location. I'm using them to hold
copies of my master key, revocation certificates, and a few other important
things. They are pin-protected, FIPS-compliant (military-grade) and will
destroy their content after a few (10 I think) attempts (which is why it's
important to keep of on them somewhere safe !)
All in all, good enough for me :)

## Yubikeys ##

On a daily basis, I use a low-profile [Yubikey 4 Nano](http://amzn.to/1TTRJMb)

As a backup, I also have a regular [Yubikey 4](http://amzn.to/1Rdciw2)

Of course, I don't use them only for GPG: U2F is super convenient for
protecting my Google/GitHub/Dropbox/... accounts. Still, in the context of this
post, the built-in support for OpenPGP is what I'm interested in. That and the
fact that they finally support [4096 bits RSA
keys](https://www.yubico.com/2015/12/yubikey-4096-you-asked-we-delivered/). Definitely
a step up from the 2048 bits max of the previous generation, and most likely
sufficient for the foreseeable future.

## GPG keys generation ##

I won't go into the details of the keys generation, as I pretty much applied
methods found elsewhere:

  * https://www.esev.com/blog/post/2015-01-pgp-ssh-key-on-yubikey-neo/ provides
    a very nice step-by-step tutorial to generate the kind of keys I'm
    interested in. Remember that the 2048 bits limitation doesn't apply to
    Yubikey 4, and that there's no need to run `ykpersonalize -m82` to
    enable OpenPGP on these.
  * https://wiki.debian.org/Subkeys describes good practices for keys
  * https://blog.josefsson.org/2014/06/23/offline-gnupg-master-key-and-subkeys-on-yubikey-neo-smartcard/
    also has great content (and goes a bit further, by using a completely
    offline machine)

As for key sizes, both my master and my encryption key are 4096 bits RSA
keys. All signing and authentication keys are 3072 bits RSA keys.

In the end, my key looks like this:

```
$ gpg2 --edit-key 2B412392
gpg (GnuPG/MacGPG2) 2.0.28; Copyright (C) 2015 Free Software Foundation, Inc.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Secret key is available.

pub  4096R/0xDC8615542B412392  created: 2016-03-11  expires: 2017-03-11  usage: C
                               trust: ultimate      validity: ultimate
sub  3072R/0x3F0AC4425EAFEC31  created: 2016-03-11  expires: 2017-03-11  usage: S
sub  3072R/0x03FCFA93348F287F  created: 2016-03-11  expires: 2017-03-11  usage: A
sub  3072R/0xA72C149B819C180C  created: 2016-03-11  expires: 2017-03-11  usage: S
sub  3072R/0x115C66B8C52959D4  created: 2016-03-11  expires: 2017-03-11  usage: A
sub  4096R/0xBE7F9034433375D0  created: 2016-03-11  expires: 2017-03-11  usage: E
[ultimate] (1). Yann Hodique <yann.hodique@gmail.com>
[ultimate] (2)  Yann Hodique <yann@hodique.info>
[ultimate] (3)  Yann Hodique <hodiquey@vmware.com>
[ultimate] (4)  Yann Hodique <sigma@keybase.io>
```

The "usage" column on the far right defines what those (sub-)keys are for:
Certification, Signature, Authentication and Encryption. As expected, 2 "S" and
2 "A".

## System integration ##

I use [Zsh](http://www.zsh.org/) as my shell of choice. In order to make sure
the right keys are made available, I'm running something like this in my
`.zshenv`:

```
eval `keychain -q --nogui --eval --agents ssh,gpg --inherit any-once id_rsa`
```

As I briefly mentioned, each Yubikey is also holding an authentication subkey,
which can be used an SSH key. Therefore, the content of my agent looks like
this in the end:

```
$ ssh-add -l
3072 SHA256:... cardno:000604163319 (RSA)
2048 SHA256:... /Users/hodiquey/.ssh/id_rsa (RSA)
```

The first line corresponds to the yubikey-held key. The public part of it can
be obtained with:

```
$ gpgkey2ssh 433375D0
ssh-rsa AAAA...
```

That part can then be uploaded to wherever (GitHub for example)

Now, shell integration is good, but most of the time I use all this from
[Emacs](https://www.gnu.org/software/emacs/) instead.

Just because I'm lazy, I piggy-back on my shell configuration with the
following snippet:

{{< highlight emacs-lisp >}}
(use-package exec-path-from-shell
    :ensure t
    :config
    (when (memq window-system '(mac ns))
      (exec-path-from-shell-initialize)

      (exec-path-from-shell-copy-envs
       '("PYTHONPATH" "GOPATH"
         "SSH_AUTH_SOCK" "SSH_AGENT_PID"
         "GPG_AGENT_INFO"))))
{{< /highlight >}}

Amongst other things, it makes sure `GPG_AGENT_INFO` is properly set, which is
what Emacs will need to communicate with my already-running `gpg-agent`.

The reason this is needed is that on OSX my Emacs is not run from a shell, so
it doesn't inherit all the right environment variables it should
have. [`exec-path-from-shell`](https://github.com/purcell/exec-path-from-shell)
is a nice module that works around that.

## Key publication ##

Obviously, GPG keys are meant to be used by others, so publication is key (ok,
that was a bad one).
I'm using pool.sks-keyservers.net most of the time, so I published my key there
as well. For all practical purposes it's just the right thing to do.

```
$ gpg2 --keyserver hkps://hkps.pool.sks-keyservers.net --send-key 2B412392
```

In addition, I'm playing with identity management at
[keybase](https://keybase.io). See my proven identities over there (including
the domain name of this blog): https://keybase.io/sigma

## Final cleanup ##

After that, all I needed to do was to revoke the bunch of old GPG keys I had
lying around. Good riddance.
