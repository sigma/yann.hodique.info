+++
title = "Python Webservice, part 1 : a Twisted solution"
date = "2010-02-17T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "code", "python", "twisted" ]
+++

As mentioned [previously]({{< ref "/blog/2009/11/15/https_traffic_logger.md"
>}}) I've been spending some time recently working on a SOAP server mock. For
this I've used Python [Twisted](http://twistedmatrix.com) and
[ZSI](http://pywebsvcs.sourceforge.net/zsi.html). Since the interaction between
the 2 seems rather poorly documented (even Google doesn't know much about it),
I thought I'd share what I've investigated and done.

Disclaimer: you should be familiar with SOAP and Python Twisted. I'll not cover
anything in these areas.

For the sake of simplicity, I'll take a classical minimal server: an
Echo service.

Let's start from the definition of such a service, as a WSDL. This one was
randomly picked from
[there](http://www.ioncannon.net/programming/180/soap-on-the-google-app-engine-platform/).
Anyway the implementation details are not so important here.

{{< highlight xml >}}
<?xml version="1.0" encoding="UTF-8"?>
<definitions
    xmlns="http://schemas.xmlsoap.org/wsdl/"
    xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/"
    xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/"
    xmlns:http="http://schemas.xmlsoap.org/wsdl/http/"
    xmlns:xsd="http://www.w3.org/2001/XMLSchema"
    xmlns:tns="urn:ZSI"
    targetNamespace="urn:ZSI" >
  <types>
    <xsd:schema elementFormDefault="qualified" targetNamespace="urn:ZSI">
      <xsd:element name="Echo">
        <xsd:complexType>
          <xsd:sequence>
            <xsd:element name="value" type="xsd:anyType"/>
          </xsd:sequence>
        </xsd:complexType>
      </xsd:element>
    </xsd:schema>
  </types>

  <message name="EchoRequest">
    <part name="parameters" element="tns:Echo" />
  </message>

  <message name="EchoResponse">
    <part name="parameters" element="tns:Echo"/>
  </message>
  <portType name="EchoServer">
    <operation name="Echo">
      <input message="tns:EchoRequest"/>
      <output message="tns:EchoResponse"/>
    </operation>
  </portType>

  <binding name="EchoServer" type="tns:EchoServer">
    <soap:binding style="document"
                  transport="http://schemas.xmlsoap.org/soap/http"/>
    <operation name="Echo">
      <soap:operation soapAction="Echo"/>
      <input>
        <soap:body use="literal"/>
      </input>
      <output>
        <soap:body use="literal"/>
      </output>
    </operation>
  </binding>

  <service name="EchoServer">
    <port name="EchoServer" binding="tns:EchoServer">
      <soap:address location="http://localhost:8080/echo"/>
    </port>
  </service>

</definitions>
{{< /highlight >}}

So really nothing fancy here. Just the definitions and shape of the
messages/operations (only trivial ones).

ZSI offers the `wsdl2py` tool, that can process such WSDL files, and produce
client bindings and server stub. In this article, the server part is what we
want. (note the `-w` flag will trigger Twisted code generation)

{{< highlight shell >}}
$ wsdl2py -w echo.wsdl
{{< /highlight >}}

`EchoServer_server.py` now contains the code for the service stub. Not much in
there, the main part being the following method:

{{< highlight python >}}
def soap_Echo(self, ps, **kw):
    request = ps.Parse(EchoRequest.typecode)
    return request,EchoResponse()
{{< /highlight >}}

This is the one we should implement to make the service concrete. Here is
a first attempt:

{{< highlight python >}}
class EchoService(EchoServer):

    def __init__(self):
        EchoServer.__init__(self)

    def soap_Echo(self, ps, **kw):
        request, _ = EchoServer.soap_Echo(self, ps, **kw)
        return request, request
{{< /highlight >}}

That will work. If you register this Service as a Twisted Resource, it will
provide a functional SOAP Echo service, on which you can execute the following.

{{< highlight shell >}}
$ POST http://localhost:8080/echo
Please enter content (application/x-www-form-urlencoded) to be POSTed:
<soapenv:Envelope 
    xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" 
    xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <soapenv:Body xmlns:ns1="urn:ZSI">
    <ns1:Echo>
      <ns1:value xsi:type="xsd:string">plop</ns1:value>
    </ns1:Echo>
  </soapenv:Body>
</soapenv:Envelope>
^D
<soapenv:Envelope xmlns:ZSI="http://www.zolera.com/schemas/ZSI/" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
<soapenv:Header></soapenv:Header><soapenv:Body xmlns:ns1="urn:ZSI"><ns1:Echo><ns1:value xsi:type="xsd:string">plop</ns1:value></ns1:Echo></soapenv:Body></soapenv:Envelope>
{{< /highlight >}}

Still, this is not very Twisted I mean we don't take advantage of the
asynchronous framework here, as the only method we wrote is definitely
synchronous and we treat each call after one another. Would the `soap_Echo()`
take time for whatever reason, that would introduce unacceptable delays for the
user.  Of course, there is no magic in the `Deferred` objects, which means than
simply returning a `Deferred` in `soap_Echo()` will absolutely not work.

Digging a bit more in the ZSI Twisted glue, one can find a promising piece of
code, appropriately named `DeferHandlerChain`. This is an alternative to the
`DefaultHandlerChain` that's used (by default) by `WSresource` objects (hence
`EchoServer` ones). The nice property of this handler chain is to put in place
a callback chain from `processRequest()` to `processResponse()`, actually
supporting `Deferred` objects returned by soap methods. In a word, what we need

The code can become something like:

{{< highlight python >}}
class EchoDeferHandlerChainFactory:
    protocol = DeferHandlerChain

    @classmethod
    def newInstance(cls):
        return cls.protocol(DefaultCallbackHandler, DataHandler)

class EchoService(EchoServer):

    def __init__(self):
        EchoServer.__init__(self)
        EchoServer.factory = EchoDeferHandlerChainFactory

    def soap_Echo(self, ps, **kw):
        request, _ = EchoServer.soap_Echo(self, ps, **kw)

        def _answer():
            return request

        # yeah, we have a very busy echo service... 5 seconds delay !
        d = deferLater(reactor, 5, _answer)
        return request, d
{{< /highlight >}}

You can now try triggering several parallel Echo calls, and see that they will
not be treated in sequence !
