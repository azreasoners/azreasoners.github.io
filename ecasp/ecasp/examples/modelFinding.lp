% ModelFinding

agent(james).

fluent(awake(A)) :- agent(A).
event(wakeUp(A)) :- agent(A).

initiates(wakeUp(A), awake(A), T) :- agent(A).
:- happens(wakeUp(A),T), holdsAt(awake(A),T), agent(A).

:- releasedAt(F, 0).

0 {happens(E,T)} 1 :- T<maxstep.

hide.
show holdsAt(F, T), releasedAt(F, T), happens(E, T).
