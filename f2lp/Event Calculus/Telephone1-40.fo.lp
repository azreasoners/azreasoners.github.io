time(0..maxstep).
agent(agent1;agent2).
phone(phone1;phone2).
#domain time(T).
#domain agent(A).
#domain phone(P;P1;P2).
fluent(ringing(P1,P2);dialTone(P);busySignal(P)).
fluent(idle(P);connected(P1,P2);disconnected(P)).
event(pickUp(A,P);setDown(A,P);dial(A,P1,P2)).
initiates(pickUp(A,P),dialTone(P),T) :- holdsAt(idle(P),T).
terminates(pickUp(A,P),idle(P),T) :- holdsAt(idle(P),T).
initiates(setDown(A,P),idle(P),T) :- holdsAt(dialTone(P),T).
terminates(setDown(A,P),dialTone(P),T) :- holdsAt(dialTone(P),T).
initiates(dial(A,P1,P2),ringing(P1,P2),T) :- holdsAt(dialTone(P1),T),holdsAt(idle(P2),T).
terminates(dial(A,P1,P2),dialTone(P1),T) :- holdsAt(dialTone(P1),T),holdsAt(idle(P2),T).
terminates(dial(A,P1,P2),idle(P2),T) :- holdsAt(dialTone(P1),T),holdsAt(idle(P2),T).
initiates(dial(A,P1,P2),busySignal(P1),T) :- holdsAt(dialTone(P1),T),not holdsAt(idle(P2),T).
terminates(dial(A,P1,P2),dialTone(P1),T) :- holdsAt(dialTone(P1),T),not holdsAt(idle(P2),T).
initiates(setDown(A,P),idle(P),T) :- holdsAt(busySignal(P),T).
terminates(setDown(A,P),busySignal(P),T) :- holdsAt(busySignal(P),T).
initiates(setDown(A,P1),idle(P1),T) :- holdsAt(ringing(P1,P2),T).
initiates(setDown(A,P1),idle(P2),T) :- holdsAt(ringing(P1,P2),T).
terminates(setDown(A,P1),ringing(P1,P2),T) :- holdsAt(ringing(P1,P2),T).
initiates(pickUp(A,P2),connected(P1,P2),T) :- holdsAt(ringing(P1,P2),T).
terminates(pickUp(A,P2),ringing(P1,P2),T) :- holdsAt(ringing(P1,P2),T).
initiates(setDown(A,P1),idle(P1),T) :- holdsAt(connected(P1,P2),T).
initiates(setDown(A,P1),disconnected(P2),T) :- holdsAt(connected(P1,P2),T).
terminates(setDown(A,P1),connected(P1,P2),T) :- holdsAt(connected(P1,P2),T).
initiates(setDown(A,P2),idle(P2),T) :- holdsAt(connected(P1,P2),T).
initiates(setDown(A,P2),disconnected(P1),T) :- holdsAt(connected(P1,P2),T).
terminates(setDown(A,P2),connected(P1,P2),T) :- holdsAt(connected(P1,P2),T).
initiates(setDown(A,P),idle(P),T) :- holdsAt(disconnected(P),T).
terminates(setDown(A,P),disconnected(P),T) :- holdsAt(disconnected(P),T).
happens(pickUp(agent1,phone1),0) :- true.
happens(dial(agent1,phone1,phone2),8) :- true.
happens(pickUp(agent2,phone2),32) :- true.
holdsAt(idle(P),0) :- true.
 :- {not holdsAt(dialTone(P),0)}0,true.
 :- {not holdsAt(busySignal(P),0)}0,true.
 :- {not holdsAt(ringing(P1,P2),0)}0,true.
 :- {not holdsAt(connected(P1,P2),0)}0,true.
 :- {not holdsAt(disconnected(P),0)}0,true.
 :- {not releasedAt(ringing(P1,P2),0)}0,true.
 :- {not releasedAt(dialTone(P),0)}0,true.
 :- {not releasedAt(busySignal(P),0)}0,true.
 :- {not releasedAt(idle(P),0)}0,true.
 :- {not releasedAt(connected(P1,P2),0)}0,true.
 :- {not releasedAt(disconnected(P),0)}0,true.
true.
:- false.
#hide true.
