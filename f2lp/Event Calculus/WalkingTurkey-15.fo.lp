time ( 0 .. maxstep).
#domain time(T).
fluent (loaded;alive;walking).
event (load;shoot).
initiates(load,loaded,T) :- true.
terminates(shoot,alive,T) :- holdsAt(loaded,T).
terminates(shoot,walking,T) :- terminates(shoot,alive,T).
 :- {not releasedAt(loaded,0)}0,true.
 :- {not releasedAt(alive,0)}0,true.
 :- {not releasedAt(walking,0)}0,true.
holdsAt(alive,0) :- true.
holdsAt(loaded,0) :- true.
holdsAt(walking,0) :- true.
happens(shoot,5) :- true.
true.
:- false.
#hide true.
