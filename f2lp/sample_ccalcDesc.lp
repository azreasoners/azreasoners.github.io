name(john;mary).
coin(penny;nickel;dime).
time(0..maxstep).
#domain coin(C;C1;C2).
#domain time(T;T1).
#domain name(N;N1).
fluent(owner(C,N)) :- true.
action(give(C);throw(C)).
#domain fluent(F).
#domain action(A;A1).
h(owner(C,N),T+1) :- {not h(owner(C,N),T+1)}0,{not h(owner(C,N),T)}0,-h(owner(C,N),T+1),T<maxstep.
h(owner(C,N),T+1) :- {not h(owner(C,N),T+1)}0,{not h(owner(C,N),T)}0,not -h(owner(C,N),T+1),T<maxstep.
-h(owner(C,N),T+1) :- {not -h(owner(C,N),T+1)}0,{not -h(owner(C,N),T)}0,h(owner(C,N),T+1),T<maxstep.
-h(owner(C,N),T+1) :- {not -h(owner(C,N),T+1)}0,{not -h(owner(C,N),T)}0,not h(owner(C,N),T+1),T<maxstep.
h(give(C),T) :- {not h(give(C),T)}0,-h(give(C),T),T<maxstep.
h(give(C),T) :- {not h(give(C),T)}0,not -h(give(C),T),T<maxstep.
-h(give(C),T) :- {not -h(give(C),T)}0,h(give(C),T),T<maxstep.
-h(give(C),T) :- {not -h(give(C),T)}0,not h(give(C),T),T<maxstep.
h(throw(C),T) :- {not h(throw(C),T)}0,-h(throw(C),T),T<maxstep.
h(throw(C),T) :- {not h(throw(C),T)}0,not -h(throw(C),T),T<maxstep.
-h(throw(C),T) :- {not -h(throw(C),T)}0,h(throw(C),T),T<maxstep.
-h(throw(C),T) :- {not -h(throw(C),T)}0,not h(throw(C),T),T<maxstep.
h(owner(C,john),T+1) :- {not h(give(C),T)}0,{not h(owner(C,mary),T)}0,-h(owner(C,john),T+1),T<maxstep.
h(owner(C,john),T+1) :- {not h(give(C),T)}0,{not h(owner(C,mary),T)}0,not -h(owner(C,john),T+1),T<maxstep.
h(owner(C,mary),T+1) :- {not h(give(C),T)}0,{not h(owner(C,john),T)}0,-h(owner(C,mary),T+1),T<maxstep.
h(owner(C,mary),T+1) :- {not h(give(C),T)}0,{not h(owner(C,john),T)}0,not -h(owner(C,mary),T+1),T<maxstep.
-h(owner(C,N),T+1) :- {not h(give(C),T)}0,{not h(owner(C,N),T)}0,h(owner(C,N),T+1),T<maxstep.
-h(owner(C,N),T+1) :- {not h(give(C),T)}0,{not h(owner(C,N),T)}0,not h(owner(C,N),T+1),T<maxstep.
-h(owner(C,N),T+1) :- {not h(owner(C,N),T)}0,{not h(throw(C),T)}0,h(owner(C,N),T+1),T<maxstep.
-h(owner(C,N),T+1) :- {not h(owner(C,N),T)}0,{not h(throw(C),T)}0,not h(owner(C,N),T+1),T<maxstep.
false :- h(throw(C),T),not new_pred1(C,T).
false :- h(give(C),T),not new_pred1(C,T).
false :- h(A,T),h(A1,T),A!=A1.
ne(C,C1) :- C != C1.
ne(A,A1) :- A != A1.
ne(N,N1) :- N != N1.
lthan(T,T1) :- T < T1.
false :- h(owner(C,N),T),h(owner(C,N1),T),N!=N1.
h(F,0) :- {not h(F,0)}0,-h(F,0).
h(F,0) :- {not h(F,0)}0,not -h(F,0).
-h(F,0) :- {not -h(F,0)}0,h(F,0).
-h(F,0) :- {not -h(F,0)}0,not h(F,0).
h(owner(C,mary),0) :- true.
-h(owner(C,john),0) :- true.
false :- not new_pred2,true.
false :- not new_pred3,true.
#domain coin(NV1).
 :- {not h(owner(NV1,mary),maxstep)}0,true.
:- not h(F,T), not -h(F,T).
:- not h(A,T), not -h(A,T), T<maxstep.
new_pred1(C,T) :- h(owner(C,N),T).
#domain coin(NV2).
new_pred2 :- h(owner(C1,john),maxstep),h(owner(NV2,john),maxstep),C1!=NV2.
new_pred3 :- -h(owner(C,john),maxstep).
true.
:- false.
hide true.
