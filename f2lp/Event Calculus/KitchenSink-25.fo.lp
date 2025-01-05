time (0..maxstep).
event (tapOn;tapOff;overflow).
fluent (filling;height(H);spilling).
#domain time(T).
height(0..15).
offset(1..15).
#domain offset(O).
#domain height(H;H1;H2).
initiates(tapOn,filling,T) :- true.
terminates(tapOff,filling,T) :- true.
releases(tapOn,height(H),T) :- true.
initiates(tapOff,height(H),T) :- holdsAt(height(H),T).
terminates(overflow,filling,T) :- true.
initiates(overflow,height(H),T) :- holdsAt(height(H),T).
initiates(overflow,spilling,T) :- true.
trajectory(filling,T,height(H2),O) :- holdsAt(height(H1),T),H2=H1+O.
 :- H1!=H2,holdsAt(height(H1),T),holdsAt(height(H2),T).
happens(overflow,T) :- holdsAt(height(15),T),holdsAt(filling,T).
holdsAt(height(0),0) :- true.
 :- {not holdsAt(filling,0)}0,true.
 :- {not holdsAt(spilling,0)}0,true.
happens(tapOn,5) :- true.
 :- {not releasedAt(filling,0)}0,true.
 :- {not releasedAt(height(H),0)}0,true.
 :- {not releasedAt(spilling,0)}0,true.
true.
:- false.
#hide true.
