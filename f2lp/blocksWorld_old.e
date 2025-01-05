objects(a;b;c;table).
time(0..maxstep).
#domain objects(X;Y).
#domain time(T).
fluent(on(X,Y);holding(X);clear(X)).
event(pickUp(X);stack(X,Y)).
#domain event(E1;E2).

% initiates and terminates formulas
T<maxstep -> initiates(pickUp(X),holding(X),T).
T < maxstep & holdsAt(on(X,Y),T) -> terminates(pickUp(X),on(X,Y),T).
T < maxstep -> initiates(stack(X,Y),on(X,Y),T).
T < maxstep -> terminates(stack(X,Y),holding(X),T).

% action precondition axioms
T < maxstep & happens(pickUp(X),T) -> holdsAt(clear(X),T) & -?[Y]:holdsAt(holding(Y),T) & X != table.
T < maxstep & happens(stack(X,Y),T) -> holdsAt(holding(X),T) & holdsAt(clear(Y),T) & X != table.

% event occurrence constraints
T<maxstep & happens(E1,T) & E1 != E2 -> -happens(E2,T).

% state constraints
X != table & holdsAt(clear(X),T) -> - ?[Y]:(holdsAt(on(Y,X),T)).
- ?[Y]:(holdsAt(on(Y,X),T))->holdsAt(clear(X),T) & X != table.
holdsAt(on(X,Y),T) -> X != table.
holdsAt(clear(table),T).

% observations
releasedAt(clear(X),0).
-releasedAt(holding(X),0).
-releasedAt(on(X,Y),0).

% needed for planning problems
{happens(E1,T)} :- T < maxstep.

% initial state
holdsAt(on(a,table),0).
holdsAt(on(b,c),0).
holdsAt(on(c,a),0).
-holdsAt(on(a,c),0).
-holdsAt(on(a,b),0).
-holdsAt(on(b,table),0).
-holdsAt(on(c,table),0).
-holdsAt(on(c,b),0).
-holdsAt(on(b,a),0).
-holdsAt(holding(X),0).

% goal
--(holdsAt(on(c,table),maxstep) & holdsAt(on(b,c),maxstep) & holdsAt(on(a,b),maxstep)).
