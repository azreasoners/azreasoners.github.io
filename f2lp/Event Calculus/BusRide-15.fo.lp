time(0..maxstep).
fluent (hasTicket; onRed; onYellow).
event (buy;board;boardRed;boardYellow).
#domain time(T).
happens(boardRed,T)|happens(boardYellow,T) :- happens(board,T).
initiates(buy,hasTicket,T) :- true.
initiates(boardRed,onRed,T) :- holdsAt(hasTicket,T).
initiates(boardYellow,onYellow,T) :- holdsAt(hasTicket,T).
 :- {not holdsAt(onYellow,T)}0,holdsAt(onRed,T).
 :- {not holdsAt(onRed,T)}0,holdsAt(onYellow,T).
holdsAt(hasTicket,T) :- holdsAt(onRed,T).
holdsAt(hasTicket,T) :- holdsAt(onYellow,T).
 :- {not releasedAt(hasTicket,0)}0,true.
 :- {not releasedAt(onRed,0)}0,true.
 :- {not releasedAt(onYellow,0)}0,true.
holdsAt(onRed,15) :- true.
 :- {not holdsAt(hasTicket,0)}0,true.
happens(buy,5) :- true.
happens(board,13) :- true.
true.
:- false.
#hide true.
