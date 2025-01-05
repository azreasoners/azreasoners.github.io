time(0..maxstep).
height(0..18).
offset(1..18).
agent(nathan) :- true.
object(apple) :- true.
#domain time(T).
#domain height(H;H1;H2).
#domain offset(Off).
#domain agent(A).
#domain object(Ob).
fluent(falling(Ob);height(Ob,H)).
event(drop(A,Ob);hitGround(Ob)).
initiates(drop(A,Ob),falling(Ob),T) :- true.
releases(drop(A,Ob),height(Ob,H),T) :- true.
terminates(hitGround(Ob),falling(Ob),T) :- true.
initiates(hitGround(Ob),height(Ob,H),T) :- holdsAt(height(Ob,H),T).
happens(hitGround(Ob),T) :- holdsAt(falling(Ob),T),holdsAt(height(Ob,0),T).
happens(drop(nathan,apple),18) :- true.
 :- H1!=H2,holdsAt(height(Ob,H1),T),holdsAt(height(Ob,H2),T).
trajectory(falling(Ob),T,height(Ob,H2),Off) :- holdsAt(height(Ob,H1),T),H2=(H1-Off).
 :- {not holdsAt(falling(apple),0)}0,true.
holdsAt(height(apple,3),18) :- true.
 :- {not releasedAt(falling(Ob),0)}0,true.
 :- {not releasedAt(height(Ob,H),0)}0,true.
true.
:- false.
#hide true.
