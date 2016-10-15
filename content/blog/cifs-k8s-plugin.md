+++
title = "CIFS Kubernetes plugin"
categories = [
  "blog",
]
tags = [
  "k8s",
  "docker"
]
date = "2016-10-15T00:07:27-07:00"

+++

[Kubernetes](http://kubernetes.io/) comes
[loaded](http://kubernetes.io/docs/user-guide/volumes/) with all sorts of
volumes, but in some cases the right one is just not there: for example, CIFS
(SMB) shares are not natively supported. Fortunately, there's a convenient
extension system called
[flexvolume](https://github.com/kubernetes/kubernetes/blob/master/examples/volumes/flexvolume/README.md) that one can use to plug those holes.

<https://github.com/sigma/cifs_k8s_plugin> is an example of how to bundle such
a plugin into a container that can be used to populate the `volumeplugins`
directory.  It contains:

- a script that speaks the `flexvolume` protocol
- a `mount.cifs` binary to handle the mounting (because `hyperkube` image
  doesn't contain it)
- a static `jq` binary to unmarshal the data containing whatever is needed to
  implement the `flexvolume` actions

```bash
$ docker run -v /etc/kubernetes/volumeplugins/hodique.info~cifs:/target \
    sigma/cifs_k8s_plugin /target
```

Once this is in place (this is compatible with the CoreOS model of running
`kubelet` in a `rkt` container by the way) this can be used to make volumes out
of CIFS shares.

For example, a basic job making use of this possibility could look like this:

{{< highlight yaml >}}
---
apiVersion: v1
kind: Secret
metadata:
  name: cifs-secret
type: Opaque
data:
  password:
  username: Z3Vlc3Q= # guest
---
apiVersion: batch/v1
kind: Job
metadata:
  name: plop
spec:
  template:
    metadata:
      name: plop
    spec:
      containers:
        - name: plop
          image: "alpine:latest"
          command: ["/bin/ls", "-lah", "/mnt"]
          volumeMounts:
            - mountPath: /mnt
              name: video
      restartPolicy: Never
      volumes:
        - name: video
          flexVolume:
            driver: "hodique.info/cifs"
            secretRef:
              name: "cifs-secret"
            options:
              source: "//10.0.0.1/Share"
              mountOptions: "dir_mode=0755,file_mode=0644,noperm"
{{< /highlight >}}

Note that the volume will not be unmounted until the job is deleted, even if
the pod itself has long completed.

All in all, yet another example of `Kubernetes` being extremely
developer-friendly :)
