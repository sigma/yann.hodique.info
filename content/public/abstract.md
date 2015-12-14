+++
title = "Safety and optimization by typing in the context of open and constraint devices"
date = "2012-04-09T17:48:00-00:00"

+++

[this page is also available in [french]({{< relref "abstract_fr.md" >}})]

The generalization of embedded electronic devices implies growing needs for
adaptability regarding the software systems that are meant to be deployed on
them. Additionally, the combination of the fact that these devices are
typically low on resources and used in environments where their security cannot
be ensured makes it necessary to deal with performance and reliability issues.

The topic of this thesis is the conception and exploitation of type systems in
order to make software systems more adapted to these requirements. Through the
definition of well-suited type systems, we modify the way software is produced
in order to make it easier to fulfill the specific needs in embedded realm.

The scientific contributions expressed here take place in two distinct moments
of the life of software. The first one is when software is conceived : during
this period, we advocate the use of a type system that is flexible enough to
express high level concepts without generating a loss of performances. The
second one is when software is deployed: then, because it has reached a point
where its environment is supposed to be safe, we can take advantage of type
inference to adapt the interpretation of programs, aiming mainly
at optimization.
