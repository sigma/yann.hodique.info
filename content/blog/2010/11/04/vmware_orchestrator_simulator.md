+++
title = "VMware Orchestrator Simulator"
date = "2010-11-04T17:48:00-00:00"
categories = [ "blog" ]
tags = [ "code", "vmware", "python" ]

+++


In my [previous post]({{< ref
"/blog/2010/11/03/python_bindings_for_the_vmware_orchestrator.md" >}}) I briefly
introduced an online public vCO simulator, that can be used for test
purpose. To quote the [documentation](http://vco-gae.appspot.com/)

SOAP

* a public reference implementation against which client code can be tested.
* code examples explaining the various behaviors.

This simulator has been developed for the
[Google App Engine](http://vco-gae.appspot.com/code.google.com/appengine/)
platform.  Right now it is able to simulate the lifecycle of a small set of
reference "workflows", thus providing a good base for experimentation.

The documentation of the
[Python bindings](http://sigma.github.com/vmw.vco/index.html) uses that
simulator, so that running the examples is as simple as a copy/paste operation.

Technically speaking, it works by associating time slots to workflow states,
and reconsidering those slots when a new action (like canceling the workflow
run) "rewrites" the future. Therefore, there is no notion of "live process",
that could quickly become a limitation.

The simulator itself is free software, released under MIT license. The source
code is available [here](https://github.com/sigma/vmw.vco).  Hope it can prove
useful to someone !
