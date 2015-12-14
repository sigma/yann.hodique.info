+++
title = "Python bindings for the VMware Orchestrator"
date = "2012-04-09T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "code", "python", "vmware" ]

+++


I've recently released [Python bindings](http://sigma.github.com/vmw.vco) for
the
[VMware vCenter Orchestrator](http://www.vmware.com/products/vcenter-orchestrator/)
under the MIT license.

Those bindings were developed as part of our test campaigns for the vCO SOAP
WebService, but were designed from the beginning as a general purpose
client. So that hopefully that might be useful to someone else.  A specificity
of these bindings is that they provide both a synchronous and an asynchronous
interfaces. This allows to write simple (synchronous) scripts, as well as more
complex applications using the [Twisted](http://twistedmatrix.com/)
framework. All this using a consistent API.

The bindings and their dependencies can be installed by:

{{< highlight shell >}}
$ pip install vmw.vco
{{< /highlight >}}

The source code is available here:
[https://github.com/sigma/vmw.vco](https://github.com/sigma/vmw.vco)

Basic example of use:

{{< highlight python >}}
>>> from vmw.vco.client import Client

>>> c = Client(url='http://vco-gae.appspot.com:80/vmware-vmo-webcontrol/webservice',
...            username='admin', password='admin')

>>> print c.echo('foo')
foo
{{< /highlight >}}

Note that this example uses an online resource,
a [vCO simulator](http://vco-gae.appspot.com/). More on that topic later.
