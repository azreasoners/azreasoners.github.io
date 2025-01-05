% File 'publishing.cp'
% Publishing Papers ("Nonmonotonic Causal Theories," Figure 9)

:- sorts
  class; number.

:- objects
  conference, journal      :: class;
  1..100                   :: number. 

:- variables
  N                        :: number.

:- constants
  hasPublications,
  hasJournalPublications,
  hasLongPublications      :: inertialFluent;
  publish                  :: exogenousAction;
  kind                     :: attribute(class) of publish;
  length                   :: attribute(number) of publish.

constraint hasJournalPublications ->> hasPublications.
constraint hasLongPublications ->> hasPublications.

publish causes hasPublications.
publish causes hasJournalPublications if kind=journal.
publish causes hasLongPublications if length>30.

:- query 
maxstep :: 3;
0: -hasJournalPublications, -hasLongPublications;
0: kind=conference, length=10;
1: kind=conference, length=40;
2: kind=journal, length=71.

:- query
label :: 1;
maxstep :: 0.
