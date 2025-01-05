% level.lp
% updated on 11/29/08


% Formality Level(FL)
#const formal=1.
#const informal=0.
#domain flevel(F;F2).


% Intimacy Level(IL)
#const close=1.
#const distant=0.
#domain ilevel(I;I2).


% Position Level(PL)
#const p_higher=1.      % Speaker's rank is higher than Listener's rank
#const p_equal=0.       % Speaker's rank is equal to Listener's rank
#const p_noRel=-1.      % No position relationship
#domain plevel(P;P2).


% Kindred Level(KL)
#const k_higher=1.      % Degree of Speaker's kinship is higher than 
                        % degree of Listener's kinship
#const k_equal=0.       % Degree of Speaker's kinship is equal to
                        % degree of Listener's kinship
#const k_noRel=-1.      % No kindred relationship
#domain klevel(K;K2).


% Age
#const much_older=2.    % Speaker is much older than Listener
#const older=1.         % Speaker is older than Listener
#const equal=0.         % Speaker's age is equal to Listener's age
#domain ageDifference(G;G2).


% Part II
#const elder=4.           % 60~   years old
#const adult=3.           % 40~60 years old
#const young_adult=2.     % 20~40 years old
#const teenager=1.        % 10~20 years old
#const child=0.           %  0~10 years old
#domain ageLevel(V;V2).