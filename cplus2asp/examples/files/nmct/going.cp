% File 'going.cp'
% Going to Work ("Nonmonotonic Causal Theories," Figure 6)

:- sorts
  object;
  location.

:- objects
  jack,car           :: object;
  home,work          :: location.

:- variables
  L                  :: location.

:- constants
  loc(object)        :: inertialFluent(location);
  go(location)       :: exogenousAction.

go(L) causes loc(jack)=L.
go(L) may cause loc(car)=L if loc(car)=loc(jack).
nonexecutable go(L) if loc(jack)=L.

:- query 
label :: 0;
maxstep :: 1;
0: loc(jack)=home;
maxstep: 
   loc(jack)=work.

:- query
label :: 1;
maxstep :: 0;
maxstep: loc(jack)=loc(car).

:- query
label :: 2;
maxstep :: 1;
0: loc(jack)=home, loc(car)=home;
0: -go(home), go(work).