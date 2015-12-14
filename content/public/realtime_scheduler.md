+++
title = "Realtime scheduler"
date = "2012-04-09T17:48:00-00:00"

+++

As an approach to my [thesis subject]({{< relref "subject.md" >}}), we chose to
focus on a particular widely used system component : the scheduler.
The initial context of application is CamilleRT, an real time exo-kernel for
embedded devices, and the problem is the following : given a set of periodic
tasks and a scheduler, how can we ensure the good behavior of the scheduler?
(an example of a bad behavior would be that the scheduler gives all the CPU
time to a particular task). This problem was studied in collaboration with
[Damien Deville](http://www.lifl.fr/~deville)

## Analysis of the problem

Since all the tasks are periodic and form a stable set (we consider that
bringing in a new task breaks the warranty, and thus requires a new proof of
good behavior), we can test the scheduler against a period of the set (the
*hyperperiod* of all the tasks), and then decide wether the scheduling plan is
acceptable or not (that is, is every deadline met?). From there, the problem is
reduced to a determinism property : if the behavior is identical on each
*hyperperiod* and the local behavior is acceptable, then the whole scheduling
plan is acceptable.  On the contrary, if the `schedule()` function is able to
cheat (for example, detecting wether it is being tested or not) we have no
warranty that the plan is correct.

If the `schedule()` function is a pure one (in the mathematical sense), the
determinism is trivial with the same entries, we obtain the same result. But to
write efficient scheduling functions, it is often necessary to keep an internal
state, such as the task progression, for the computation. Such an internal
state can be trivially used for cheating.

## Proposed solution

I proposed an algorithm that enables us to detect the potentiality of
cheating. The idea is to let the algorithm cheat, but to make this cheating
useless. A solution for this is to periodically reset every internal
state. Doing this, we can be sure that the algorithm will always behave
the same.

The principle of the algorithm is to follow the references (objects) that
appear in a method and infer a signature of the method. That signature will say
us for example that the second argument has been linked into the return value,
or to `this` object. The algorithm is incremental, with some restrictions due
to the dynamic binding of methods.
