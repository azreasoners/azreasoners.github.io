% Abduction

agent(james).

fluent(awake(A)) :- agent(A).
event(wakeUp(A)) :- agent(A).

initiates(wakeUp(A), awake(A), T) :- agent(A).

:- holdsAt(awake(james), 0).
holdsAt(awake(james), 1).

:- releasedAt(F, 0).

{happens(E, T)} :- T<maxstep.

hide.
show holdsAt(F, T), releasedAt(F, T), happens(E, T).
