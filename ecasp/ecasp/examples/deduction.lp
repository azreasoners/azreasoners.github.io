% Deduction

agent(james).

fluent(awake(A)) :- agent(A).
event(wakeUp(A)) :- agent(A).

initiates(wakeUp(A), awake(A), T) :- agent(A).

:- holdsAt(awake(james), 0).
happens(wakeUp(james), 0).

:- releasedAt(F, 0).

hide.
show holdsAt(F, T), releasedAt(F, T), happens(E, T).
