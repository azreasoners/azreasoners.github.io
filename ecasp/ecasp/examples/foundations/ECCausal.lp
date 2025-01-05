#domain fluent(Fluent), event(Event), time(Time).

%started(Fluent,Time).
%stopped(Fluent,Time).
%initiated(Fluent,Time).
%terminated(Fluent,Time).


initiated2(Fluent,Time) :- happens(Event,Time), initiates(Event,Fluent,Time).

holdsAt(Fluent,Time) :- started(Fluent,Time), not initiated2(Fluent,Time).

started(Fluent,Time) :- holdsAt(Fluent,Time).

started(Fluent,Time) :- happens(Event,Time), initiates(Event,Fluent,Time).

terminated2(Fluent,Time) :- happens(Event,Time), terminates(Event,Fluent,Time).

:- stopped(Fluent,Time), not terminated2(Fluent,Time), holdsAt(Fluent,Time).

stopped(Fluent,Time) :- not holdsAt(Fluent,Time).

stopped(Fluent,Time) :- happens(Event,Time), terminates(Event,Fluent,Time).

:- initiated(Fluent,Time), happens(Event,Time), terminates(Event,Fluent,Time).

started(Fluent,Time) :- initiated(Fluent,Time).

initiated(Fluent,Time) :- started(Fluent,Time), not terminated2(Fluent,Time).

:- terminated(Fluent,Time), happens(Event,Time), initiates(Event,Fluent,Time).

stopped(Fluent,Time) :- terminated(Fluent,Time).

terminated(Fluent,Time) :- stopped(Fluent,Time), not initiated2(Fluent,Time).


0{started(F,T)}1.
0{stopped(F,T)}1.
0{initiated(F,T)}1.
0{terminated(F,T)}1.



hide.

show holdsAt(F,T), happens(E,T), happens3(E,T,T2).
