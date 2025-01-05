% File 'marblebags'

% This puzzle involves trying to fit a certain number of colored marbles into a certain number of bags of various sizes.
% The challenge is that each color of marble is a different size, and the difficulty usually arises from there only being barely enough room for all the marbles in all the bags.

% contents(C) in this puzzle represents the current volume occupied by marbles in a given bag C,
% and maxCapacity(C) represents the maximum volume of marbles the bag C can hold.
% We use separate contents constants to keep track of the actual number of marbles of varying types in each bag (e.g., blackContents(C), etc.).

:- macros
 maxCapacity -> 21.
 
:- macros
 numBags -> 3;
 numBlackMarbles -> 6;
 numWhiteMarbles -> 4;
 sizeOfBlack -> 5;
 sizeOfWhite -> 3;
 maxTotalContents -> (sizeOfBlack*numBlackMarbles) + (sizeOfWhite*numWhiteMarbles).

:- sorts
 index.

:- objects
 1..numBags :: index;
 b(index) :: container.

:- variables
 C :: container;
 VTC :: 0..maxTotalContents.

:- constants
 blackContents(container) :: 0..numBlackMarbles; % Number of black marbles in a container
 whiteContents(container) :: 0..numWhiteMarbles; % Number of white marbles in a container
 totalContents :: 0..maxTotalContents.

exogenous contents(C).
exogenous blackContents(C).
exogenous whiteContents(C).
exogenous totalContents.
caused abFill(C) & abEmpty(C). % We don't need to fill or empty these bags in the classic way.

% This relates the number of marbles of varying colors to the total volume occupied by them in any given bag.
constraint contents(C)=V ->> V=(sizeOfBlack*blackContents(C))+(sizeOfWhite*whiteContents(C)) & V =< maxCapacity.
% This enforces the idea that we can't overfill the bags.
constraint contents(C)=V ->> capacity(C)>=V.
% Defining total contents for all bags.
constraint totalContents=VTC ->> VTC=(sizeOfBlack*(blackContents(b(1))+blackContents(b(2))))+(sizeOfWhite*(whiteContents(b(1))+whiteContents(b(2)))).
% Now the puzzle-making constraint that we always have to fit all marbles.
constraint totalContents=maxTotalContents.

% Starting setup: how big is each bag?
caused capacity(b(1))=21 & capacity(b(2))=21 & capacity(b(3))=0.

:- query
maxstep::0.