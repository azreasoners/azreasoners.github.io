% honorific2.lp
% Updated on 11/29/08

#domain person(S;L). % S: Speaker, L: Listener

person(speaker; listener).


:- formality(S,S,F).
:- intimacy(S,S,I).
:- kindred(S,S,K).
:- position(S,S,P).
:- older(S,S,G). 



%%% Symmetric Relationship
formality(L,S,F) :- formality(S,L,F).
intimacy(L,S,I) :- intimacy(S,L,I).

% Degree of kinship is the same to each other.
kindred(L,S,k_equal) :- kindred(S,L,k_equal).
% No kindred relationship between Speaker and Listener
kindred(L,S,k_noRel) :- kindred(S,L,k_noRel).

% Position(Social rank) is the same to each other
position(L,S,p_equal) :- position(S,L,p_equal). 
% No relationship of social rank bewteen Speaker and Listener
position(L,S,p_noRel) :- position(S,L,p_noRel).

% The same age of each other
older(L,S,equal) :- older(S,L,equal).


% constraints for relationship between older and age
:- older(S,L,much_older), age(S,V), age(L,V2), V-V2<2.
:- older(S,L,older), age(S,V), age(L,V2), V-V2!=1, V-V2!=0.
:- older(S,L,equal), age(S,V), age(L,V2), V!=V2.

% If some constraints can handle, necessary?
%:- older(S,L,G), G>equal, older(L,S,G2), G2>equal.
%:- position(S,L,p_equal), position(L,S,p_equal).
%:- kindred(S,L,k_equal), kindred(L,S,k_equal).

% constraint for relationship between age and position
:- position(S,L,p_equal), age(S,V), V>=young_adult,
   age(L,V2), V2<=teenager.
:- position(L,S,P), age(S,V), V>=young_adult,
   age(L,V2), V2<=teenager, P!=p_noRel.


%% hapSho
% case 2) Used in a public place
% Assume that a child doesn't use this speech level.
hapSho(S,L) :- formality(S,L,formal),
               age(S,V), V>child.
               

%% haO
% case 1) Mainly used by male middle-aged or elder adults
%         without close relationship.
haO(S,L) :- intimacy(S,L,distant), age(S,V), age(L,V2),
           V>=adult, V2>=adult, older(S,L,G), G>=equal,
           not ab1(S,L).
ab1(S,L) :- position(L,S,p_higher), kindred(S,L,k_noRel).
ab1(S,L) :- kindred(L,S,k_higher).


%% haGe
% case 1 & 2 & 4) Mainly used by male middle-aged or
%                 elder adults with close relationship.
haGe(S,L) :- intimacy(S,L,close), age(S,V), age(L,V2),
           V>=adult, V2>=adult, older(S,L,G), G>=equal,
           not ab1(S,L).

% case 3) Used between an older teacher and a student
%         They are young adults, adults or elders.
haGe(S,L) :- teacher(S), student(L), age(S,V), V>=adult,
             age(L,V2), V2>=young_adult, older(S,L,G), G>=older,
             not kindred(L,S,k_higher).
position(S,L,p_higher) :- teacher(S), student(L).


%% haeRa
% case 1) Used to close friends 
%         (Strong assumption: same age of each other)
haeRa(S,L) :- intimacy(S,L,close), older(S,L,equal),
              not ab1(S,L).

% case 2 & 3) Used between a much older speaker and a listener
haeRa(S,L) :- older(S,L,much_older), not ab1(S,L).


%% haeYo
% case 2 & 4 & 5 & 6 & 7)
haeYo(S,L) :- not ab2(S,L), S!=L.
ab2(S,L) :- kindred(S,L,k_higher).
ab2(S,L) :- kindred(S,L,k_equal).
ab2(S,L) :- position(S,L,p_higher), kindred(S,L,k_noRel).


%% hae
% case 1 & 2) 
hae(S,L) :- intimacy(S,L,close), older(S,L,G), G>=equal,
            not ab1(S,L).



hide.
show hapSho(_,_), haeYo(_,_), haO(_,_), 
     haGe(_,_), haeRa(_,_), hae(_,_),
     age(_,_), older(_,_,_),
     intimacy(_,_,_), formality(_,_,_),
     position(_,_,_), kindred(_,_,_),
     ab1(_,_), ab2(_,_).

