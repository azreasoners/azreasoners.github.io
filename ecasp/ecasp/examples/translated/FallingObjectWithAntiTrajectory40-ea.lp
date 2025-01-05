#domain time(Time), offset(Offset), object(Object), agent(Agent), height(Height;Height1;Height2).

time(0..40).
height(0..35).
offset(1..35).

object(apple).
agent(nathan).


fluent(falling(Object)).
fluent(hasHeight(Object,Height)).

event(drop(Agent,Object)).
event(hitGround(Object)).



initiates(drop(Agent,Object),falling(Object),Time).

terminates(hitGround(Object),falling(Object),Time).

happens(hitGround(Object),Time) :- holdsAt(falling(Object),Time), holdsAt(hasHeight(Object,0),Time).

happens(drop(nathan,apple),0).

:- holdsAt(hasHeight(Object,Height1),Time), holdsAt(hasHeight(Object,Height2),Time), Height1!=Height2.

trajectory(falling(Object),Time,hasHeight(Object,Height2),Offset) :- holdsAt(hasHeight(Object,Height1),Time), Height2==Height1-Offset.

antiTrajectory(falling(Object),Time,hasHeight(Object,Height),Offset) :- holdsAt(hasHeight(Object,Height),Time).

:- holdsAt(falling(apple),0).

holdsAt(hasHeight(apple,35),0).

:- releasedAt(falling(Object),0).

releasedAt(hasHeight(Object,Height),Time).

hide.

show holdsAt(F,T), happens(E,T), happens3(E,T,T2).
