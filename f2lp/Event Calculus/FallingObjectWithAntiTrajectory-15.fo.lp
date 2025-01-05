time(0..maxstep).
offset(1..10).
height(0..10).
agent(nathan) :- true.
object(apple) :- true.
#domain object(Ob).
#domain height(H;H1;H2).
#domain agent(A).
#domain time(T).
#domain offset(Off).
fluent(falling(Ob);height(Ob,H)).
event(drop(A,Ob);hitGround(Ob)).
initiates(drop(A,Ob),falling(Ob),T) :- true.
terminates(hitGround(Ob),falling(Ob),T) :- true.
happens(hitGround(Ob),T) :- holdsAt(falling(Ob),T),holdsAt(height(Ob,0),T).
happens(drop(nathan,apple),0) :- true.
 :- H1!=H2,holdsAt(height(Ob,H1),T),holdsAt(height(Ob,H2),T).
trajectory(falling(Ob),T,height(Ob,H2),Off) :- holdsAt(height(Ob,H1),T),H2=(H1-Off).
antiTrajectory(falling(Ob),T,height(Ob,H),Off) :- holdsAt(height(Ob,H),T).
 :- {not holdsAt(falling(apple),0)}0,true.
holdsAt(height(apple,10),0) :- true.
 :- {not releasedAt(falling(Ob),0)}0,true.
releasedAt(height(Ob,H),0) :- true.
true.
:- false.
#hide true.
