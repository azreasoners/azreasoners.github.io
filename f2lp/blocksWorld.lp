objects(a;b;c;table).
time(0..maxstep).
#domain objects(X;Y).
#domain time(T).
fluent(on(X,Y);holding(X);clear(X)).
event(pickUp(X);stack(X,Y)).
#domain event(E1;E2).
initiates(pickUp(X),holding(X),T) :- T<maxstep.
terminates(pickUp(X),on(X,Y),T) :- T<maxstep,holdsAt(on(X,Y),T).
initiates(stack(X,Y),on(X,Y),T) :- T<maxstep.
terminates(stack(X,Y),holding(X),T) :- T<maxstep.
#domain objects(NV1).
holdsAt(clear(X),T) :- T<maxstep,happens(pickUp(X),T).
 :- {not holdsAt(holding(NV1),T)}0,T<maxstep,happens(pickUp(X),T).
 :- X=table,T<maxstep,happens(pickUp(X),T).
holdsAt(holding(X),T) :- T<maxstep,happens(stack(X,Y),T).
holdsAt(clear(Y),T) :- T<maxstep,happens(stack(X,Y),T).
 :- X=table,T<maxstep,happens(stack(X,Y),T).
 :- {not happens(E2,T)}0,T<maxstep,happens(E1,T),E1!=E2.
#domain objects(NV2).
 :- {not holdsAt(on(NV2,X),T)}0,X!=table,holdsAt(clear(X),T).
holdsAt(clear(X),T) :- not new_pred1(X,T).
 :- X=table,not new_pred1(X,T).
 :- X=table,holdsAt(on(X,Y),T).
holdsAt(clear(table),T) :- true.
releasedAt(clear(X),0) :- true.
 :- {not releasedAt(holding(X),0)}0,true.
 :- {not releasedAt(on(X,Y),0)}0,true.
{happens(E1,T)} :- T < maxstep.
holdsAt(on(a,table),0) :- true.
holdsAt(on(b,c),0) :- true.
holdsAt(on(c,a),0) :- true.
 :- {not holdsAt(on(a,c),0)}0,true.
 :- {not holdsAt(on(a,b),0)}0,true.
 :- {not holdsAt(on(b,table),0)}0,true.
 :- {not holdsAt(on(c,table),0)}0,true.
 :- {not holdsAt(on(c,b),0)}0,true.
 :- {not holdsAt(on(b,a),0)}0,true.
 :- {not holdsAt(holding(X),0)}0,true.
false :- not holdsAt(on(c,table),maxstep),true.
false :- not holdsAt(on(b,c),maxstep),true.
false :- not holdsAt(on(a,b),maxstep),true.
new_pred1(X,T) :- holdsAt(on(Y,X),T).
true.
:- false.
hide true.
