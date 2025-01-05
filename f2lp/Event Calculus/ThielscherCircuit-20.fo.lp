time (0..maxstep).
#domain time(T).
event (lightOn;close1;open2;closeRelay).
fluent (light;switch1;switch2;switch3;relay).
happens(lightOn,T) :- stopped(light,T),initiated(switch1,T),initiated(switch2,T).
happens(open2,T) :- started(switch2,T),initiated(relay,T).
happens(closeRelay,T) :- stopped(relay,T),initiated(switch1,T),initiated(switch3,T).
initiates(lightOn,light,T) :- true.
terminates(open2,switch2,T) :- true.
initiates(closeRelay,relay,T) :- true.
initiates(close1,switch1,T) :- true.
 :- {not releasedAt(light,0)}0,true.
 :- {not releasedAt(switch1,0)}0,true.
 :- {not releasedAt(switch2,0)}0,true.
 :- {not releasedAt(switch3,0)}0,true.
 :- {not releasedAt(relay,0)}0,true.
 :- {not holdsAt(switch1,0)}0,true.
holdsAt(switch2,0) :- true.
holdsAt(switch3,0) :- true.
 :- {not holdsAt(relay,0)}0,true.
 :- {not holdsAt(light,0)}0,true.
happens(close1,12) :- true.
true.
:- false.
#hide true.
