+++
title = "Python Webservice, part 2 : a twistd integration"
date = "2010-02-18T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "code", "python", "twisted" ]

+++

## Twistd

In my [previous post]({{< ref
"/blog/2010/02/17/python_webservice_part_1_a_twisted_solution.md" >}}) I've
explained how to generate a SOAP service from a WSDL file, and how to implement
the required methods properly. A solution to run such a service is to use the
Twisted Daemon, aka `twistd`.  Though not needed, strictly speaking, it
provides a nice
[set of features](http://twistedmatrix.com/documents/current/core/howto/basics.html)
aimed at easing complex applications development.

In particular, it allows to write so-called
[plugins](http://twistedmatrix.com/documents/current/core/howto/tap.html) that
make it possible to just launch services in the following way:

{{< highlight shell >}}
$ twistd echo
{{< /highlight >}}

To achieve that, one has to create a file `twisted/plugins/echo.py` accessible
somewhere from the python path (that is, the `twisted` directory should be
a subdir of an element of the path).

## Manhole

Twisted offers a nice service, that can be used to provide an inspection/debug
interface for your main application. This is called
[manhole](http://twistedmatrix.com/documents/9.0.0/api/twisted.manhole.html)
and provides a python interpreter interface. This way you can just telnet/ssh
the manhole, and execute appropriate commands to introspect the current state
of the application, or put it in the desired state.

A nice example of a really useful function to run from the manhole service is
the
[rebuild](http://twistedmatrix.com/documents/9.0.0/api/twisted.python.rebuild.html)
one. It tries very hard to provide in-place reloading of code (and
corresponding objects). Don't expect any wonder here, it will fail in some
corner cases. Still, it proves really useful most of the time, and helps a lot
not restarting the server every now and then.

## Example

Here is a fully working example of a twistd plugin for the Echo service, with
a functional manhole service in addition.

{{< highlight python >}}
from zope.interface import implements
from twisted.python import usage
from twisted.python.rebuild import rebuild
from twisted.plugin import IPlugin
from twisted.application.service import IServiceMaker, MultiService
from twisted.application.internet import TCPServer
from twisted.web.server import Site
from twisted.web.resource import Resource
from twisted.conch.manhole_tap import makeService as makeConsoleService

from EchoImpl import EchoService

class EchoOptions(usage.Options):
    optParameters = [["port", "p", 8080, "main port", int],
                     ["service-port", "s", 8081, "service port", int],
                     ["service-users", "u", "/etc/passwd",
                      "The path to a passwd-like authentication file"]]

class EchoServiceMaker(object):
    implements(IServiceMaker, IPlugin)
    tapname = "echo"
    description = "Sample Echo service"
    options = EchoOptions

    def makeService(self, options):
        # Create Service
        srv = EchoService()

        root = Resource()
        # we'll serve Echo at http://<ip>:<port>/echo
        root.putChild('echo', srv)
        siteFactory = Site(root)

        echo_service = TCPServer(options['port'], siteFactory)
        console_service = makeConsoleService(
            {"telnetPort": str(options['service-port']),
             "sshPort": None,
             "namespace": {"service": srv,
                           "rebuild": rebuild},
             "passwd": options['service-users']})

        svc = MultiService()
        echo_service.setServiceParent(svc)
        console_service.setServiceParent(svc)

        return svc

serviceMaker = EchoServiceMaker()
{{< /highlight >}}

The code is hopefully rather straightforward. It should be noted that the
manhole service (created by the `makeConsoleService()` call) needs to know
about a set of objects (the "namespace" key). Those objects are the roots for
everything accessible from the service. Here, the main service object (the
`srv` object) is available to the manhole under the name `service`. Here is how
it can be used:

{{< highlight shell >}}
$ telnet localhost 8081
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Username: yann
Password: x

>>> print service
<EchoImpl.EchoService object at 0x2fc8c10>
>>> 
{{< /highlight >}}

Hint: the password is really 'x', as I'm using /etc/passwd by default, which
contains 'x' for every "password" field, since the real passwords are in
/etc/shadow.  Convenient for a dev deployment ;)
