% WARNING: F2LP arrow '<-' was not found but implication was found. Treating '-' as default negation. Please use the option -c if you would like '-' to be treated as classical negation.
% This is the first scenario from "Physical Modeling using Temporal Action Logic"

time(0..maxstep).
#domain time(T).
 #domain time(T1).
 #domain time(T2).

% ----------------------------------------------------------------------------------------------------
% Value Domains
% ----------------------------------------------------------------------------------------------------

%#domain tankorreservoir :elements { reservoir, lowertank, upperleft, upperright }
%#domain pipeorpump :elements { leftpump, rightpump, leftpipe, rightpipe, lowerpipe }

%#domain pipe :parent pipeorpump :elements { [leftpipe  lowerpipe] }
%#domain pump :parent pipeorpump :elements { [leftpump  rightpump] }
%#domain tank :parent tankorreservoir :elements { [lowertank  upperright] }

pipe(leftpipe;rightpipe;lowerpipe).
pump(leftpump;rightpump).
tank(lowertank;upperleft;upperright).
#domain pipe(Pi).
#domain pump(Pu).
#domain tank(Ta).
value(Pi) :- true.
value(Pu) :- true.
value(Ta) :- true.
tankorreservoir(reservoir) :- true.
tankorreservoir(Ta) :- true.
pipeorpump(Pi) :- true.
pipeorpump(Pu) :- true.
#domain tankorreservoir(TR).
#domain pipeorpump(PP).
value(TR) :- true.
value(PP) :- true.
bool(true;false).
#domain bool(Bo).
value(Bo) :- true.
#domain value(V).
 #domain value(V1).
 #domain value(V2).


%// ----------------------------------------------------------------------------------------------------
%// Attachments
%// ----------------------------------------------------------------------------------------------------

%#propagation :split-by-time :constraints-by-time

%// We want to use fixed point numbers
%#domain fixedpoint :fixedpoint :lb 0 :ub 1000 :decimals 6
% using integers for simplicity
num(0..100).
#domain num(Nu).
$domain(0..100).

%// The time step  Decrease this value to simulate with greater precision
%// (less time between each discrete time step)  Unit:  Seconds
%#valueconst Ts :domain fixedpoint :parameter :default 125
%#timescale Ts
% simply provide this as run-time argument or use 1 for simplicity

%// We only specify the height of tanks  We assume implicitly that pipes and pumps
%// are defined in a way consistent with some 'real' arrangement of tanks  Pipes
%// and pumps go from the lowest level of one tank to the highest level of another
%// one  Pipes go from a higher level to a lower one   Pumps go from a lower level
%// to a higher one

%//  dh1 = -01040*Ts*sqrt(h1) + 00334*Ts*u1
%//  dhs = -00512*Ts*sqrt(hs) + 00343*Ts*us
%//  dh2 = -00750*Ts*sqrt(h2) + 01040*Ts*sqrt(h1) + 00656*Ts*sqrt(hs)
%//
%// d�r h1 - left tank
%//     hs - right tank
%//     h2 - lower tank
%//     u1 - left pump
%//     us - right pump
%//     Ts - samplingstiden (125, 4, el 1s) 
%//     dh - f�r�ndring i resp tank under Ts sekunder
%//

% scaling by mapping 001 to 1	
%#function alpha(pipe) :domain fixedpoint = {
%	<leftpipe> = $times(Ts, 01040), %using Ts*10 for f2lp
%	<rightpipe> = $times(Ts, 00512), %using Ts*5
%	<lowerpipe> = $times(Ts, 00750) %using Ts*7
%}
:- not alpha(leftpipe) $== timescale*10.
:- not alpha(rightpipe) $== timescale*5.
:- not alpha(lowerpipe) $== timescale*7.


%#function beta(pump) :domain fixedpoint = {
%	<leftpump> = $times(Ts, 00334), %using Ts*3
%	<rightpump> = $times(Ts, 00343) %using Ts*3
%}

:- not beta(leftpump) $== timescale*3.
:- not beta(rightpump) $== timescale*3.


%#function epsilon(pipe) :domain fixedpoint = {
%	<leftpipe> = $times(Ts, 01040), %Ts*10
%	<rightpipe> = $times(Ts, 00512), %Ts*5
%	<lowerpipe> = $times(Ts, 00750) %Ts*7
%}

:- not epsilon(leftpipe) $== timescale*10.
:- not epsilon(rightpipe) $== timescale*5.
:- not epsilon(lowerpipe) $== timescale*7.

%#function height(tankorreservoir) :domain fixedpoint = {
%	<reservoir> = 010, %using 10 for f2lp
%	<lowertank> = 5657, %using 565
%	<upperleft> = 5657, %using 565
%	<upperright> = 5657 %using 565
%}

:- not height(reservoir) $== 10.
:- not height(lowertank) $== 50.
:- not height(upperleft) $== 50.
:- not height(upperright) $== 50.

%#function source(pipeorpump) :domain tankorreservoir = {
%	<leftpump> = reservoir,
%	<rightpump> = reservoir,
%	<leftpipe> = upperleft,
%	<rightpipe> = upperright,
%	<lowerpipe> = lowertank
%}
% for compatibility with clingcon, we use predicates here
source(leftpump,reservoir) :- true.
source(rightpump,reservoir) :- true.
source(leftpipe,upperleft) :- true.
source(rightpipe,upperright) :- true.
source(lowerpipe,lowertank) :- true.


%#function dest(pipeorpump) :domain tankorreservoir = {
%	<leftpump> = upperleft,
%	<rightpump> = upperright,
%	<leftpipe> = lowertank,
%	<rightpipe> = lowertank,
%	<lowerpipe> = reservoir
%}
dest(leftpump,upperleft) :- true.
dest(rightpump,upperright) :- true.
dest(leftpipe,lowertank) :- true.
dest(rightpipe,lowertank) :- true.
dest(lowerpipe,reservoir) :- true.


%#feature pipeopen(pipe) :domain fixedpoint :showname
%#feature pumpvoltage(pump) :domain fixedpoint :showname

%#feature pipeflow(pipeorpump) :domain fixedpoint :showname

%#feature overflow(tankorreservoir) :domain boolean
%#feature empty(tankorreservoir) :domain boolean
%#feature waterlev(tankorreservoir) :domain fixedpoint :showname

%#feature inflow(tankorreservoir) :domain fixedpoint :showname
%#feature outflow(tankorreservoir) :domain fixedpoint :showname

feature(pipeopen(Pi);pumpvoltage(Pu);pipeflow(PP);overflow(TR);empty(TR);waterlev(TR);inflow(TR);outflow(TR)).
hFeature(pipeopen(T,Pi);pumpvoltage(T,Pu);pipeflow(T,PP);waterlev(T,TR);inflow(T,TR);outflow(T,TR)).
#domain feature(Fe).
#domain hFeature(HF).

%all are persistent
 :- pipeopen(T+1,Pi) $!= pipeopen(T,Pi),not occlude(T+1,pipeopen(Pi)),T<maxstep.

 :- pumpvoltage(T+1,Pu) $!= pumpvoltage(T,Pu),not occlude(T+1,pumpvoltage(Pu)),T<maxstep.

 :- pipeflow(T+1,PP) $!= pipeflow(T,PP),not occlude(T+1,pipeflow(PP)),T<maxstep.

#domain bool(NV_1).
holds(T,overflow(TR),NV_1) :- holds(T+1,overflow(TR),NV_1),not occlude(T+1,overflow(TR)),T<maxstep.
 :- {not holds(T+1,overflow(TR),NV_1)}0,not holds(T,overflow(TR),NV_1),not occlude(T+1,overflow(TR)),T<maxstep.
holds(T+1,overflow(TR),NV_1) :- holds(T,overflow(TR),NV_1),not occlude(T+1,overflow(TR)),T<maxstep.
 :- {not holds(T,overflow(TR),NV_1)}0,not holds(T+1,overflow(TR),NV_1),not occlude(T+1,overflow(TR)),T<maxstep.

#domain bool(NV_2).
holds(T,empty(TR),NV_2) :- holds(T+1,empty(TR),NV_2),not occlude(T+1,empty(TR)),T<maxstep.
 :- {not holds(T+1,empty(TR),NV_2)}0,not holds(T,empty(TR),NV_2),not occlude(T+1,empty(TR)),T<maxstep.
holds(T+1,empty(TR),NV_2) :- holds(T,empty(TR),NV_2),not occlude(T+1,empty(TR)),T<maxstep.
 :- {not holds(T,empty(TR),NV_2)}0,not holds(T+1,empty(TR),NV_2),not occlude(T+1,empty(TR)),T<maxstep.

:- waterlev(T+1,TR) $!= waterlev(T,TR),not occlude(T+1,waterlev(TR)),T<maxstep.
%waterlev(T+1,TR) $== waterlev(T,TR) :- not occlude(T+1,waterlev(TR)),T<maxstep.

 :- inflow(T+1,TR) $!= inflow(T,TR),not occlude(T+1,inflow(TR)),T<maxstep.

 :- outflow(T+1,TR) $!= outflow(T,TR),not occlude(T+1,outflow(TR)),T<maxstep.

%Unique value axioms
1{holds(T,overflow(TR),Boo):bool(Boo)}1 :- true.
1{holds(T,empty(TR),Boo):bool(Boo)}1 :- true.

%#attach	"logicguiplotPlotAttachment"
%	"pumpvoltage(leftpump), pumpvoltage(rightpump), waterlev(upperleft), waterlev(upperright), waterlev(lowertank)"

%#attach	SaveText
%	"pumpvoltage(leftpump), pumpvoltage(rightpump), waterlev(upperleft), waterlev(upperright), waterlev(lowertank)"

%#action setflow(pipe, fixedpoint)
%#action setvoltage(pump, fixedpoint)

%#action stopflow, stoppumps
action(setflow(Pi,Nu);setvoltage(Pu,Nu);stopflow;stoppumps).
#domain action(Ac).


%#valuevar flow, voltage :domain fixedpoint

%#obs [0] waterlev(reservoir) == 005 & !empty(reservoir)
%#obs [0] waterlev(lowertank) == 000 & empty(lowertank)
%#obs [0] waterlev(upperleft) == 000 & empty(upperleft)
%#obs [0] waterlev(upperright) == 000 & empty(upperright)
:- not waterlev(0,reservoir) $== 5.
:- not waterlev(0,lowertank) $== 0.
:- not waterlev(0,upperleft) $== 0.
:- not waterlev(0,upperright) $== 0.
 :- {not holds(0,empty(reservoir),true)}0,true.
holds(0,empty(lowertank),true) :- true.
holds(0,empty(upperleft),true) :- true.
holds(0,empty(upperright),true) :- true.

%#acs [t1,t2] setflow(pipe, flow)       -> R((t1,t2] pipeopen(pipe) == flow)
#domain time(NV_3).
occlude(NV_3,pipeopen(Pi)) :- NV_3>T1,NV_3<=T2,occurs(T1,T2,setflow(Pi,Nu)).
 :- NV_3>T1,NV_3<=T2,not occlude(NV_3,pipeopen(Pi)),occurs(T1,T2,setflow(Pi,Nu)).
 :- pipeopen(T2,Pi) $!= Nu,occurs(T1,T2,setflow(Pi,Nu)).

%%#acs [t1,t2] setvoltage(pump, voltage) -> R((t1,t2] pumpvoltage(pump) == voltage)
#domain time(NV_4).
occlude(NV_4,pumpvoltage(Pu)) :- NV_4>T1,NV_4<=T2,occurs(T1,T2,setvoltage(Pu,Nu)).
 :- NV_4>T1,NV_4<=T2,not occlude(NV_4,pumpvoltage(Pu)),occurs(T1,T2,setvoltage(Pu,Nu)).
 :- pumpvoltage(T2,Pu) $!= Nu,occurs(T1,T2,setvoltage(Pu,Nu)).

%#acs [t1,t2] stopflow  -> R((t1,t2] forall pump [ pumpvoltage(pump) == 000 ] & forall pipe [ pipeopen(pipe) == 000 ])
#domain time(NV_5).
occlude(NV_5,pumpvoltage(Pu)) :- NV_5>T1,NV_5<=T2,occurs(T1,T2,stopflow).
occlude(NV_5,pipeopen(Pi)) :- NV_5>T1,NV_5<=T2,occurs(T1,T2,stopflow).
 :- NV_5>T1,NV_5<=T2,not occlude(NV_5,pumpvoltage(Pu)),occurs(T1,T2,stopflow).
 :- NV_5>T1,NV_5<=T2,not occlude(NV_5,pipeopen(Pi)),occurs(T1,T2,stopflow).
 :- pipeopen(T2,Pi) $!= 0,occurs(T1,T2,stopflow).
 :- pumpvoltage(T2,Pu) $!= 0,occurs(T1,T2,stopflow).

%#acs [t1,t2] stoppumps -> R((t1,t2] forall pump [ pumpvoltage(pump) == 000 ])
#domain time(NV_6).
occlude(NV_6,pumpvoltage(Pu)) :- NV_6>T1,NV_6<=T2,occurs(T1,T2,stoppumps).
 :- NV_6>T1,NV_6<=T2,not occlude(NV_6,pumpvoltage(Pu)),occurs(T1,T2,stoppumps).
 :- pumpvoltage(T2,Pu) $!= 0,occurs(T1,T2,stoppumps).



%#occ [$scale( 40)-1,$scale( 40)] setvoltage(leftpump, 600)
%#occ [$scale( 57)-1,$scale( 57)] setvoltage(rightpump, 450)
%#occ [$scale( 87)-1,$scale( 87)] stoppumps
%#occ [$scale(200)-1,$scale(200)-0] stopflow
occurs(2,3,setvoltage(leftpump,6)) :- true.
occurs(3,4,setvoltage(rightpump,5)) :- true.
occurs(6,7,stoppumps) :- true.
occurs(15,16,stopflow) :- true.

%#dep X((0,$scale(200)]
%	forall pipeorpump [ pipeflow(pipeorpump) !== {} ] &
%	forall tank [ overflow(tank) !== {} ] &
%	forall tank [ empty(tank) !== {} ] &
%	forall tank [ waterlev(tank) !== {} ] &
%	forall tank [ inflow(tank) !== {} ] &
%	forall tank [ outflow(tank) !== {} ])
#domain time(NV_7).
occlude(NV_7,pipeflow(PP)) :- NV_7>0,NV_7<=16.
occlude(NV_7,overflow(Ta)) :- NV_7>0,NV_7<=16.
occlude(NV_7,empty(Ta)) :- NV_7>0,NV_7<=16.
occlude(NV_7,waterlev(Ta)) :- NV_7>0,NV_7<=16.
occlude(NV_7,inflow(Ta)) :- NV_7>0,NV_7<=16.
occlude(NV_7,outflow(Ta)) :- NV_7>0,NV_7<=16.


%// No tank is overflowing in state 0  No pumps are on, but all other pipes are open

%#obs [0] forall tankorreservoir [ !overflow(tankorreservoir) ]
%#obs [0] pumpvoltage(leftpump) == 500 & pumpvoltage(rightpump) == 000
%#obs [0] forall pipe [ pipeopen(pipe) == 1599 ]

 :- {not holds(0,overflow(TR),true)}0,true.
:- not pumpvoltage(0,leftpump) $== 5.
:- not pumpvoltage(0,rightpump) $== 0.
:- not pipeopen(0,Pi) $== 16.

%// Pipe flow depends on water levels  If there is no water, no water will flow out
%// If there is one level of water, then if there are two outgoing pipes, one will get
%// all the flow; if there is one outgoing pipe, it will get all the flow  If there
%// is enough water, both (all) pipes will get flow

%// Pipes from the reservoir (assumed to be pumps) always get as much water as they want

%#dom forall t, pump [ [t] pipeflow(pump) == $times(beta(pump), pumpvoltage(pump)) ]
:- not pipeflow(T,Pu) $== beta(Pu) * pumpvoltage(T,Pu).


%// Pipes not from the reservoir (assumed not to be pumps) get water depending on the
%// inflow and the water level

%// In this special case, there are no tanks with two outgoing pipes

%////////////////////////////////////////////////////////////////////////////////

%// Total inflow depends on pipeflow  The reservoir has no inflow and no outflow

%#obs [0] inflow(reservoir) == 000
%#obs [0] outflow(reservoir) == 000
:- not inflow(0,reservoir) $== 0.
:- not outflow(0,reservoir) $== 0.


%// For the other tanks, the inflow is equal to the sum of all inflows from
%// incoming pipes and all inflows from incoming pumps  Note that we need to
%// compensate for the fact that the alpha and epsilon of a pipe may not be
%// equal, even though they happen to be equal in this specific scenario

%#dom forall t, tank [
%	[t] inflow(tank) == $plus($sum(<pipe>, [t] dest(pipe) == tank, $times($div(epsilon(pipe),alpha(pipe)),pipeflow(pipe))),
%	                         $sum(<pump>, [t] dest(pump) == tank, pipeflow(pump)))
%]

:- not inflow(T,upperleft) $== pipeflow(T,leftpump).
:- not inflow(T,upperright) $== pipeflow(T,rightpump).
:- not inflow(T,lowertank) $== ((pipeflow(T,leftpipe) * (epsilon(leftpipe)/alpha(leftpipe))) +				(pipeflow(T,rightpipe) * (epsilon(rightpipe)/alpha(rightpipe)))).

%////////////////////////////////////////////////////////////////////////////////

%// Above we got the complete inflow to the upper level of tanks (those that have
%// only incoming pumps, no incoming pipes)  Now we calculate the pipeflow out
%// of those tanks  Doing this before calculating outflow means that we get
%// information more quickly -- we can calculate some of the outflow this loop,
%// instead of all outflow having to wait until next loop  This is about twice
%// as fast as if we had inverted the order of this and the next formula!

%#dom forall t, pipe [ [t] pipeflow(pipe) == $min(
%		  $plus(waterlev(source(pipe)), inflow(source(pipe))),
%		  $min($times(alpha(pipe), $sqrt(waterlev(source(pipe)))), pipeopen(pipe))) ]
% clingcon does not support sqrtso dividing by 2 instead
:- not dummy_sum(T,leftpipe) $== waterlev(T,upperleft) + inflow(T,upperleft).
:- not dummy_min(T,leftpipe) $== alpha(leftpipe)*(waterlev(T,upperleft)/2),   alpha(leftpipe)*(waterlev(T,upperleft)/2) $<= pipeopen(T,leftpipe).
:- not dummy_min(T,leftpipe) $== pipeopen(T,leftpipe),   pipeopen(T,leftpipe) $<= alpha(leftpipe)*(waterlev(T,upperleft)/2).

:- not dummy_sum(T,rightpipe) $== waterlev(T,upperright) + inflow(T,upperright).
:- not dummy_min(T,rightpipe) $== alpha(rightpipe)*(waterlev(T,upperright)/2),   alpha(rightpipe)*(waterlev(T,upperright)/2) $<= pipeopen(T,rightpipe).
:- not dummy_min(T,rightpipe) $== pipeopen(T,rightpipe),   pipeopen(T,rightpipe) $<= alpha(rightpipe)*(waterlev(T,upperright)/2).

:- not dummy_sum(T,lowerpipe) $== waterlev(T,lowertank) + inflow(T,lowertank).
:- not dummy_min(T,lowerpipe) $== alpha(lowerpipe)*(waterlev(T,lowertank)/2),   alpha(lowerpipe)*(waterlev(T,lowertank)/2) $<= pipeopen(T,lowerpipe).
:- not dummy_min(T,lowerpipe) $== pipeopen(T,lowerpipe),   pipeopen(T,lowerpipe) $<= alpha(lowerpipe)*(waterlev(T,lowertank)/2).

:- not pipeflow(T,Pi) $== dummy_sum(T,Pi),    dummy_sum(T,Pi) $<=  dummy_min(T,Pi).
:- not pipeflow(T,Pi) $== dummy_min(T,Pi),    dummy_min(T,Pi) $<=  dummy_sum(T,Pi).


%// Total outflow depends on pipeflow

%#dom forall t, tank [
%	[t] outflow(tank) == $sum(<pipe>, [t] source(pipe) == tank, pipeflow(pipe)) ]

:- not outflow(T,upperleft) $== pipeflow(T,leftpipe).
:- not outflow(T,upperright) $== pipeflow(T,rightpipe).
:- not outflow(T,lowertank) $== pipeflow(T,lowerpipe).

%////////////////////////////////////////////////////////////////////////////////

%// Dynamic fluent definition: empty

%#dom forall t, tank [ [t] empty(tank) -> waterlev(tank) == 000 ]

%////////////////////////////////////////////////////////////////////////////////

%// Dynamic fluent definition: empty

%#dom forall t, tank [ [t] empty(tank) ))
:- waterlev(T,Ta) $== 0, not holds(T,empty(Ta),true).
:- holds(T,empty(Ta),true), not waterlev(T,Ta) $== 0.

%// Dynamic fluent definition: waterlev  We calculate the newlevel as oldlevel+inflow-outflow
%// If this fits in the tank, the tank gets this new level and no overflow; if it doesn't fit,
%// the tank becomes full and overflows

%#dom forall t, tank [ [t] $lessequal($minus($plus(waterlev(tank), inflow(tank)), outflow(tank)), height%(tank)) ->
%	[t+1] waterlev(tank) == value(t, $minus($plus(waterlev(tank), inflow(tank)), outflow(tank))) & !overflow(tank)]
%:- Nu $== (waterlev(T,Ta) + inflow(T,Ta) - outflow(T,Ta)), height(Ta) $>= Nu,    not waterlev(T+1,Ta) $== Nu.
%:- Nu $== (waterlev(T,Ta) + inflow(T,Ta) - outflow(T,Ta)), height(Ta) $>= Nu,    holds(T+1,overflow(Ta),true).

:- height(Ta) $>= (waterlev(T,Ta) + inflow(T,Ta) - outflow(T,Ta)),                   
   not waterlev(T+1,Ta) $== (waterlev(T,Ta) + inflow(T,Ta) - outflow(T,Ta)).
:- height(Ta) $>= (waterlev(T,Ta) + inflow(T,Ta) - outflow(T,Ta)),
   holds(T+1,overflow(Ta),true).


%#dom forall t, tank [ [t] $greater($minus($plus(waterlev(tank), inflow(tank)), outflow(tank)), height(tank)) ->
%	[t+1] waterlev(tank) == height(tank) & overflow(tank)]

%:- Nu $== (waterlev(T,Ta) + inflow(T,Ta) - outflow(T,Ta)), height(Ta) $< Nu,    not waterlev(T+1,Ta) $== height(Ta).
%:- Nu $== (waterlev(T,Ta) + inflow(T,Ta) - outflow(T,Ta)), height(Ta) $< Nu,    not holds(T+1,overflow(Ta),true).

:- height(Ta) $< (waterlev(T,Ta) + inflow(T,Ta) - outflow(T,Ta)),
   not waterlev(T+1,Ta) $== height(Ta).
:- height(Ta) $< (waterlev(T,Ta) + inflow(T,Ta) - outflow(T,Ta)),
   not holds(T+1,overflow(Ta),true).

true.
:- false.

% just adding for readability
hFeature(pipeopen(T,Pi);pumpvoltage(T,Pu);pipeflow(T,PP);waterlev(T,TR);inflow(T,TR);outflow(T,TR)).
holdsAt(T,pipeopen(Pi),Nu) :- pipeopen(T,Pi) $== Nu.
holdsAt(T,pumpvoltage(Pu),Nu) :- pumpvoltage(T,Pu) $== Nu.
holdsAt(T,pipeflow(PP),Nu) :- pipeflow(T,PP) $== Nu.
holdsAt(T,waterlev(TR),Nu) :- waterlev(T,TR) $== Nu.
holdsAt(T,inflow(TR),Nu) :- inflow(T,TR) $== Nu.
holdsAt(T,outflow(TR),Nu) :- outflow(T,TR) $== Nu.
holdsAt(T,Fe,Bo) :- holds(T,Fe,Bo).

#hide true.
%#hide num/1.
%#hide time/1.
%#hide feature/1.
%#hide value/1.
%#hide action/1.
%#hide hFeature/1.
%#hide dummy_sum/2.
%#hide dummy_min/2.
#hide.
#show holdsAt/3.
#show occurs/3.
