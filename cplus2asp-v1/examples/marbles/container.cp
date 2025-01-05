% File 'container'

% Define maxCapacity in your dependent file.
% Define container objects and (optionally) expand on the setup below in your dependent file.

:- sorts
 container; amount.

:- objects
 0..maxCapacity :: amount.

:- variables
 C :: container;
 V :: amount.

:- constants
 capacity(container) :: 0..maxCapacity;
 contents(container) :: inertialFluent(amount);
 empty(container) :: exogenousAction;
 fill(container) :: exogenousAction.

:- constants
 abEmpty(container), abFill(container) :: abAction(boolean).

exogenous capacity(C).

caused contents(C)=0 after empty(C) unless abEmpty(C).
caused contents(C)=V if V=capacity(C) after fill(C) unless abFill(C).

nonexecutable empty(C) if contents(C)=0. % Why empty an empty glass?
nonexecutable fill(C) if contents(C)=capacity(C). % Why fill a full glass?