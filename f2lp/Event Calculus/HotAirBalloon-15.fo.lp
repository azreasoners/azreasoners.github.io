time(0..maxstep).
height(0..6).
offset(1..6).
agent(nathan) :- true.
balloon(ba) :- true.
#domain time(T).
#domain height(H;H1;H2).
#domain offset(Off).
#domain agent(A).
#domain balloon(B).
fluent(heaterOn(B);height(B,H)).
event(turnOnHeater(A,B);turnOffHeater(A,B)).
initiates(turnOnHeater(A,B),heaterOn(B),T) :- true.
terminates(turnOffHeater(A,B),heaterOn(B),T) :- true.
happens(turnOnHeater(nathan,ba),0) :- true.
happens(turnOffHeater(nathan,ba),6) :- true.
 :- H1!=H2,holdsAt(height(B,H1),T),holdsAt(height(B,H2),T).
trajectory(heaterOn(B),T,height(B,H2),Off) :- holdsAt(height(B,H1),T),H2=(H1+Off).
antiTrajectory(heaterOn(B),T,height(B,H2),Off) :- holdsAt(height(B,H1),T),H2=(H1-Off).
holdsAt(height(ba,0),0) :- true.
 :- {not holdsAt(heaterOn(ba),0)}0,true.
releasedAt(height(B,H),T) :- true.
 :- {not releasedAt(heaterOn(B),0)}0,true.
true.
:- false.
#hide true.
