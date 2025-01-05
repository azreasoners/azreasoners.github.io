% --------------------------
% Display setting
% --------------------------
num(1..10).
#domain num(L).

evaluate_rule(R, Rq, Dec) <-
     rule(R, _, _, _) &
     evaluate(R, Rq, Dec).

evaluate_target(T, Rq, Dec) <-
    (rule(_, T, _, _) |
     policy(_, T, _, _, _) |
     policySet(_, T, _, _, _)) &
    evaluate(T, Rq, Dec).

evaluate_condition(C, Rq, Dec) <-
    rule(_, _, _, C) &
    evaluate(C, Rq, Dec).

evaluate_policy(P, Rq, Dec) <-
    (policy(P, _, _, _, _) |
     policySet(P, _, _, _, _)) &
    evaluate(P, Rq, Dec) & Dec != na.

evaluate_match(M, Rq, Dec) <-
     hasMatch(_, M) &
     evaluate(M, Rq, Dec).

evaluate_policy_ac(P, rq, Dec) <-
     evaluate(P, rq, Dec) &
     (policy(P, _, _, _, _) |
      policySet(P, _, _, _, _)).

access_request(rq, A, B, C, D, E) <-
     request(rq, A, B, C, D, E).

%admin_request(ar(ps2,rq,p), A, B, C, D, E) <-
%     request(ar(ps2,rq,p), A, B, C, D, E).

%evaluate_policy_ad(P, ar(generated_policy(0), rq, p), Dec) <-
%    evaluate_policy(P, ar(generated_policy(0), rq, p), Dec).

%evaluate_match_ac(M, rq, Dec) <-
%     evaluate_match(M, rq, Dec).

%applicable_ac(CID) <-
%      applicable(CID, rq).

%combined_decision_ac(P, rq, Dec) <-
%     combined_decision(P, rq, Dec).

%evaluate_target_ac(T, rq, Dec) <-
%      evaluate_target(T, rq, Dec).

#hide dec/1.
#hide combined_decision/3.
#hide evaluate/3.
#hide evaluate_match/3.
#hide evaluate_rule/3.
#hide evaluate_target/3.
#hide evaluate_condition/3.
#hide evaluate_policy/3.
#hide combining_algo/2.
#hide applicable/2.
#hide reduce/3.
#hide hasChild/3.
#hide path/5.
#hide indeterminate/1.
#hide pp_authorized/2.
#hide pi_authorized/2.
#hide dp_authorized/2.
#hide di_authorized/2.
#hide untrusted/1.
#hide evaluate_against/2.
#hide attrRetriever/5.
#hide attr_exist/2.
#hide evaluateConditionFunc/4.
#hide exist_allOf_not_nomatch/2.
#hide exist_anyOf_not_match/2.
#hide exist_match_not_true/2.
#hide hasAllOf/2.
#hide hasAnyOf/2.
#hide hasApplicableChild/2.
#hide hasMatch/2.
#hide match/5.
#hide num/1.
#hide policy/5.
#hide policySet/5.
#hide pre_evaluate/3.
#hide priority/3.
#hide rule/4.
#hide request/6.
#hide trusted/1.
#hide highest_priority/3.
%#hide access_request/6.

% --------------------------
% Lua function definition
% --------------------------

#begin_asp
#begin_lua
function evalMatchFunc(val1, val2, type1, type2, matchFunc)

	if matchFunc == "string-equal" then
		if type1 == "string" and val1 == val2 then
			return 1
		else
			return 0
		end
	else
		return 0
	end

end

function isDelegatedPrefixed(str)

	if string.find(str, "delegated:") == 1 then
		return 1
	else
		return 0
	end

end

function equalsDelegationInfo(str)

	if str == "delegate-info" then
		return 1
	else
		return 0
	end

end

function equalsDelegate(str)

	if str == "delegate" then
		return 1
	else
		return 0
	end

end

function addDelegatedPrefix(str)

	return "delegated:"..str

end

#end_lua.
#end_asp.

% --------------------------
% Condition evaluation
% --------------------------

#begin_asp

evaluate(CF, RQ, t) :-
	rule(_, _, _, CF),
	evaluateConditionFunc(CF, RQ, "true", "boolean").


evaluate(CF, RQ, f) :-
        rule(_, _, _, CF),
        evaluateConditionFunc(CF, RQ, "false", "boolean").

evaluate(null, RQ, t) :-
        request(RQ, _, _, _, _, _).

evaluate(CF, RQ, i) :-
        rule(_, _, _, CF),
	request(RQ, _, _, _, _, _),
        not evaluate(CF, RQ, t),
	not evaluate(CF, RQ, f).

evaluateConditionFunc(CF, RQ, VAL, TYPE) :-
	conditionFunc(CF, func(FUNC, ARG1, ARG2)),
	evaluateConditionFunc(ARG1, RQ, VAL1, TYPE1),
	evaluateConditionFunc(ARG2, RQ, VAL2, TYPE2),
	VAL = @evaluateConditionFunc(FUNC, VAL1, TYPE1, VAL2, TYPE2),
	TYPE = @getReturnType(FUNC).

evaluateConditionFunc(null, RQ, "null", "null") :-
	request(RQ, _, _, _, _, _).

%evaluateConditionFunc(AR, RQ, ATTRVAL, DATATYPE) :-
%       attr_returned(AR, RQ, ATTRVAL, DATATYPE).

evaluateConditionFunc(AR, RQ, ATTRVAL, DATATYPE) :-
	attr_returned(AR, RQ, ATTRVAL, DATATYPE),
	conditionFunc(_, func(_, AR, _)).

evaluateConditionFunc(AR, RQ, ATTRVAL, DATATYPE) :-
        attr_returned(AR, RQ, ATTRVAL, DATATYPE),
	conditionFunc(_, func(_, _, AR)).

attr_returned(AVID, RQ, VAL, TYPE) :-
	attr_value(AVID, TYPE, VAL),
	request(RQ, _, _, _, _, _).

#begin_lua
function evaluateConditionFunc(func, val1, type1, val2, type2)
	if func == "date-less-or-equal" then
                return "true"
        elseif func == "date-one-and-only" then
                return val1
        elseif func == "date-add-yearMonthDuration" then
                return val1
        elseif func == "and" then
                return "true"
        elseif func == "string-equal" then
                return "true"
        elseif func == "string-one-and-only" then
                return val1
        else
                return "undefined"
        end
end

function getReturnType(func)
	if func == "date-less-or-equal" then
		return "boolean"
	elseif func == "date-one-and-only" then
		return "date"
	elseif func == "date-add-yearMonthDuration" then
		return "date"
	elseif func == "and" then
		return "boolean"
	elseif func == "string-equal" then
		return "boolean"
	elseif func == "string-one-and-only" then
		return "string"
	else
		return "null"
	end
end

#end_lua.
#end_asp.

% --------------------------
% match evaluation
% --------------------------

% Case 1
evaluate(M, Rq, t) <-
    match(M, Func, AR, Type1, Val1) &
    attrRetriever(AR, AttrID, Cat,
       AttrIssuer, _) &
    request(Rq, AttrID, Cat, AttrIssuer,
       Type2, Val2) &
    #spatom{@evalMatchFunc(Val1, Val2, Type1, Type2, Func)} == 1.

% Case 2
attr_exist(M, Rq) <-
    match(M, _, AR, _, _) &
    attrRetriever(AR, AttrID, Cat,
      AttrIssuer, _) &
    request(Rq, AttrID, Cat, AttrIssuer, _, _).

evaluate(M, Rq, i) <-
    match(M, _, AR, _, _) &
    attrRetriever(AR, _, _, _, true) &
    request(Rq, _, _, _, _, _) &
    not attr_exist(M, Rq).

% Case 3
evaluate(M, Rq, f) <-
    match(M, _, _, _, _) &
    request(Rq, _, _, _, _, _) &
    not evaluate(M, Rq, t) &
    not evaluate(M, Rq, i).


% --------------------------
% allOf evaluation
% --------------------------

% Case 1
exist_match_not_true(ALL, Rq) <-
    hasMatch(ALL, M) &
    evaluate(M, Rq, Result) &
    Result != t.

evaluate(ALL, Rq, m) <-
    hasMatch(ALL, _) &
    request(Rq, _, _, _, _, _) &
    not exist_match_not_true(ALL, Rq).

% Case 2
evaluate(ALL, Rq, nm) <-
    hasMatch(ALL, M) &
    evaluate(M, Rq, f).

% Case 3
evaluate(ALL, Rq, i) <-
    hasMatch(ALL, _) &
    request(Rq, _, _, _, _, _) &
    not evaluate(ALL, Rq, m) &
    not evaluate(ALL, Rq, nm).


% --------------------------
% anyOf evaluation
% --------------------------

% Case 1
evaluate(ANY, Rq, m) <-
    hasAllOf(ANY, ALL) &
    evaluate(ALL, Rq, m).

% Case 2
exist_allOf_not_nomatch(ANY, Rq) <-
    hasAllOf(ANY, ALL) &
    evaluate(ALL, Rq, Result) &
    Result != nm.
      
evaluate(ANY ,Rq, nm) <-
    hasAllOf(ANY, _) &
    request(Rq, _, _, _, _, _) &
    not exist_allOf_not_nomatch(ANY, Rq).
			
% Case 3
evaluate(ANY, Rq, i) <-
    hasAllOf(ANY, _) &
    request(Rq, _, _, _, _, _) &
    not evaluate(ANY, Rq, m) &
    not evaluate(ANY, Rq, nm).


% --------------------------
% target evaluation
% --------------------------

% Case 1
exist_anyOf_not_match(T, Rq) <-
    (rule(_, T, _, _) |
     policy(_, T, _, _, _) |
     policySet(_, T, _, _, _)) &
    hasAnyOf(T, ANY) &
    evaluate(ANY, Rq, Result) &
    Result != m.

evaluate(T, Rq, m) <-
    (rule(_, T, _, _) |
     policy(_, T, _, _, _) |
     policySet(_, T, _, _, _)) &
    request(Rq, _, _, _, _, _) &
    not exist_anyOf_not_match(T, Rq).

% Case 2
evaluate(T, Rq, nm) <-
    (rule(_, T, _, _) |
     policy(_, T, _, _, _) |
     policySet(_, T, _, _, _)) &
    hasAnyOf(T, ANY) &
    evaluate(ANY, Rq, nm).

% Case 3
evaluate(T, Rq, i) <-
    (rule(_, T, _, _) |
     policy(_, T, _, _, _) |
     policySet(_, T, _, _, _)) &
    hasAnyOf(T, _) &
    request(Rq, _, _, _, _, _) &
    not evaluate(T, Rq, m) &
    not evaluate(T, Rq, nm).


% --------------------------
% rule evaluation
% --------------------------

% Case 1
evaluate(R, Rq, Effect) <-
    rule(R, T, Effect, Condition) &
    evaluate(T, Rq, m) &
    evaluate(Condition, Rq, t).

% Case 2
evaluate(R, Rq, na) <-
    rule(R, T, Effect, Condition) &
    (evaluate(T, Rq, nm) |
    evaluate(Condition, Rq, f)).

% Case 3
evaluate(R, Rq, i(Effect)) <-
    rule(R, _, Effect, _) &
    request(Rq, _, _, _, _, _) &
    not evaluate(R, Rq, Effect) &
    not evaluate(R, Rq, na).


% --------------------------
% combining algorithms
% --------------------------

combining_algo(P, Alg) <-
    (policy(P, _, Alg, _, _) |
     policySet(P, _, Alg, _, _)).

% permit-overrides and deny-overrides
priority(po, p, 1).
priority(po, i(dp), 2). 
priority(po, i(p), 3).		
priority(po, d, 4).
priority(po, i(d), 5).		
priority(po, na, 6).

priority(do, d, 1).			
priority(do, i(dp), 2).		
priority(do, i(d), 3).
priority(do, p, 4).		
priority(do, i(p), 5).		
priority(do, na, 6).

pre_evaluate(P, Rq, Dec) <-
	reduce(CP, Rq, Dec) &
        hasChild(P, CP, _).

highest_priority(P, Rq, N) <-
    N = #min[pre_evaluate(P, Rq, Dec):
        priority(Alg, Dec, X):
        combining_algo(P, Alg) = X] &
    pre_evaluate(P, Rq, _).

combined_decision(P, Rq, Dec) <-
    combining_algo(P, Alg) &
    highest_priority(P, Rq, N) &
    priority(Alg, Dec, N) &
    1 {pre_evaluate(P, Rq, X) : dec(X)} &
    (Alg = po | Alg = do).

dec(p; d; na; i(dp); i(d); i(p)).

combined_decision(P, Rq, na) <-
    combining_algo(P, Alg) &
    {pre_evaluate(P, Rq, X) : dec(X)} 0 &
    request(Rq, _, _, _, _, _) &
    (Alg = po | Alg = do).

% permit-unless-deny
% Case 1
combined_decision(P, Rq, d) <-
    combining_algo(P, pud) &
    hasChild(P, CP, _) &
    reduce(CP, Rq, d).

% Case 2
combined_decision(P, Rq, p) <-
    combining_algo(P, pud) &
    not combined_decision(P, Rq, d) &
    request(Rq, _, _, _, _, _).

% deny-unless-permit
% Case 1
combined_decision(P, Rq, p) <-
    combining_algo(P, dup) &
    hasChild(P, CP, _) &
    reduce(CP, Rq, p).
		
% Case 2
combined_decision(P, Rq, d) <-
    combining_algo(P, dup) &
    not combined_decision(P, Rq, p) &
    request(Rq, _, _, _, _, _).


% first-applicable
% Case 1
applicable(CP, Rq) <-
    reduce(CP, Rq, Dec) &
    Dec != na.
				
hasApplicableChild(P, Rq) <-
    hasChild(P, C, _) &
    applicable(C, Rq).

combined_decision(P, Rq, Dec) <-
    combining_algo(P, fa) &
    N = #min[applicable(C, Rq):
             hasChild(P, C, O)=O] &
    hasChild(P, CP, Order) &
    reduce(CP, Rq, Dec) &
    Order == N.
		
% Case 2
combined_decision(P, Rq, na) <-
    combining_algo(P, fa) &
    request(Rq, _, _, _, _, _) &
    not hasApplicableChild(P, Rq).


% only-one-applicable
% Case 1
combined_decision(P, Rq, na) <-
    combining_algo(P, ooa) &
    not hasApplicableChild(P, Rq) &
    request(Rq, _, _, _, _, _).

% Case 2
combined_decision(P, Rq, Dec) <-
    combining_algo(P, ooa) &
    applicable(CP, Rq) &
    hasChild(P, CP, _) &
    reduce(CP, Rq, Dec) &
    1{applicable(C, Rq):
          hasChild(P, C, _)}1.

% Case 3
combined_decision(P, Rq, i(dp)) <-
    combining_algo(P, ooa) &
    request(Rq, _, _, _, _, _) &
    2{applicable(C, Rq):
          hasChild(P, C, _)}.

% --------------------------
% policy/policy set evaluation
% --------------------------

reduce(R, Rq, Dec) <-
    evaluate(R, Rq, Dec) &
    rule(R, _, _, _).

% Case 1
evaluate(P, Rq, Dec) <-
    (policy(P, T, _, _, _) |
     policySet(P, T, _, _, _)) &
    evaluate(T, Rq, m) &
    combined_decision(P, Rq, Dec).
				
% Case 2
evaluate(P, Rq, na) <-
    (policy(P, T, _, _, _) |
     policySet(P, T, _, _, _)) &
    evaluate(T, Rq, nm).
		
% Case 3
evaluate(P, Rq, na) <-
    (policy(P, T, _, _, _) |
     policySet(P, T, _, _, _)) &
    evaluate(T, Rq, i) &
    combined_decision(P, Rq, na).

% Case 4
evaluate(P, Rq, i(Dec)) <-
    (policy(P, T, _, _, _) |
     policySet(P, T, _, _, _)) &
    evaluate(T, Rq, i) &
    combined_decision(P, Rq, Dec) &
    (Dec = p | Dec = d).

% Case 5
evaluate(P, Rq, i(Dec)) <-
    (policy(P, T, _, _, _) |
     policySet(P, T, _, _, _)) &
    evaluate(T, Rq, i) &
    combined_decision(P, Rq, i(Dec)).

% --------------------------
% reduction
% --------------------------

evaluate_against(rq, pdp).

evaluate_against(Rq, P) <-
     policySet(P, _, _, _, _) &
     hasChild(PS, P, _) &
     evaluate_against(Rq, PS).

evaluate_against(ar(P, Rq, Dec), SP) <-
    request(ar(P, Rq, Dec), _, _, _, _, _) &
    hasChild(PS, P, _) &
    hasChild(PS, SP, _) &
    policySet(SP) &
    SP != P.



% Generate administrative request
request(ar(P, Rq, Dec), AttrID, Cat,
AttrIssuer, Type, AttrVal) <-
    evaluate_against(Rq, PS) &
    policySet(PS, _, _, _, _) &
    (policy(P, _, _, _, _) | policySet(P, _, _, _, _)) &
    hasChild(PS, P, _) &
    request(Rq, AttrID, Cat, AttrIssuer,
      Type, AttrVal) &
    #spatom{@isDelegatedPrefixed(Cat)} == 1 &
    (Dec = d | Dec = p).

#spatom{request(ar(P, Rq, Dec), AttrID, @addDelegatedPrefix(Cat),
AttrIssuer, Type, AttrVal)} <-
    evaluate_against(Rq, PS) &
    policySet(PS, _, _, _, _) &
    (policy(P, _, _, _, _) | policySet(P, _, _, _, _)) &
    hasChild(PS, P, _) &
    request(Rq, AttrID, Cat, AttrIssuer,
      Type, AttrVal) &
    #spatom{@isDelegatedPrefixed(Cat)} == 0 &
    #spatom{@equalsDelegationInfo(Cat)} == 0 &
    #spatom{@equalsDelegate(Cat)} == 0 &
    (Dec = d | Dec = p).

#spatom{request(ar(P, Rq, Dec), AttrID, "delegate", 
AttrIssuer, Type, AttrVal)} <-
    evaluate_against(Rq, PS) &
    policySet(PS, _, _, _, _) &
    (policy(P, _, _, _, _) | policySet(P, _, _, _, _)) &
    hasChild(PS, P, _) &
    (policy(P, _, _, Issuer, _) |
     policySet(P, _, _, Issuer, _)) &
    hasAttr(Issuer, AttrID, _, AttrIssuer,
      Type, AttrVal) &
    (Dec = d | Dec = p).

#spatom{request(ar(P, Rq, Dec), "decision",
"delegation-info", null, "string", Dec)} <-
    evaluate_against(Rq, PS) &
    policySet(PS, _, _, _, _) &
    (policy(P, _, _, _, _) | policySet(P, _, _, _, _)) &
    hasChild(PS, P, _) &
    (Dec = d | Dec = p).

% construct the redunction graph

% PP edge and DP edge
path(Rq, PL, PH, pp, 1) <-
    evaluate(PH, ar(PL, Rq, p),
        p) &
    evaluate_against(Rq, PS) &
    policySet(PS, _, _, _, _) &
    hasChild(PS, PH, _) &
    hasChild(PS, PL, _) &
    PL != PH.

path(Rq, PL, PH, dp, 1) <-
    evaluate(PH, ar(PL, Rq, d),
        p) &
    evaluate_against(Rq, PS) &
    policySet(PS, _, _, _, _) &
    hasChild(PS, PH, _) &
    hasChild(PS, PL, _) &
    PL != PH.

% PI edge and DI edge
indeterminate(i(Dec)) <-
    Dec = p |
    Dec = d |
    Dec = dp.

path(Rq, PL, PH, pi, 1) <-
    evaluate(PH, ar(PL, Rq, p),
        Dec) &
    evaluate_against(Rq, PS) &
    policySet(PS, _, _, _, _) &
    hasChild(PS, PH, _) &
    hasChild(PS, PL, _) &
    PL != PH &
    indeterminate(Dec).

path(Rq, PL, PH, di, 1) <-
    evaluate(PH, ar(PL, Rq, d),
        Dec) &
    evaluate_against(Rq, PS) &
    policySet(PS, _, _, _, _) &
    hasChild(PS, PH, _) &
    hasChild(PS, PL, _) &
    PL != PH &
    indeterminate(Dec).

% Paths of arbitrary length
path(Rq, PL, PH, Type, L) <-
    path(Rq, PL, PM, Type, L1) &
    path(Rq, PM, PH, Type, L2) &
    PL != PH &
    L = L1 + L2.

path(Rq, PL, PH, pi, L) <-
    path(Rq, PL, PM, T1, L1) &
    path(Rq, PM, PH, T2, L2) &
    ((T1 == pp & T2 == pi) |
     (T1 == pi & T2 == pp)) &
    PL != PH &
    L = L1 + L2.

path(Rq, PL, PH, di, L) <-
    path(Rq, PL, PM, T1, L1) &
    path(Rq, PM, PH, T2, L2) &
    ((T1 == dp & T2 == di) |
     (T1 == di & T2 == dp)) &
    PL != PH &
    L = L1 + L2.

% Reduction process

trusted(P) <-
    (policy(P, _, _, Issuer, _) |
     policySet(P, _, _, Issuer, _)) &
    Issuer == null.
			
untrusted(P) <-
    (policy(P, _, _, Issuer, _) |
     policySet(P, _, _, Issuer, _)) &
    not trusted(P).

pp_authorized(Rq, P) <-
    path(Rq, P, TP, pp, L) &
    trusted(TP) &
    (policy(TP, _, _, _, Dep) |
     policySet(TP, _, _, _, Dep)) &
    L <= Dep.
		
pi_authorized(Rq, P) <-
    path(Rq, P, TP, pi, L) &
    trusted(TP) &
    (policy(TP, _, _, _, Dep) |
     policySet(TP, _, _, _, Dep)) &
    L <= Dep.
		
dp_authorized(Rq, P) <-
    path(Rq, P, TP, dp, L) &
    trusted(TP) &
    (policy(TP, _, _, _, Dep) |
     policySet(TP, _, _, _, Dep)) &
    L <= Dep.
		
di_authorized(Rq, P) <-
    path(Rq, P, TP, di, L) &
    trusted(TP) &
    (policy(TP, _, _, _, Dep) |
     policySet(TP, _, _, _, Dep)) &
    L <= Dep.

% Case 1
reduce(P, Rq, Dec) <-
    evaluate(P, Rq, Dec) &
    trusted(P).

% Case 2
reduce(P, Rq, p) <-
    untrusted(P) &
    evaluate(P, Rq, p) &
    pp_authorized(Rq, P).
		
% Case 3
reduce(P, Rq, d) <-
    untrusted(P) &
    evaluate(P, Rq, d) &
    dp_authorized(Rq, P).

% Case 4
reduce(P, Rq, i(p)) <-
    untrusted(P) &
    evaluate(P, Rq, p) &
    pi_authorized(Rq, P).

% Case 5
reduce(P, Rq, i(d)) <-
    untrusted(P) &
    evaluate(P, Rq, d) &
    di_authorized(Rq, P).

% Case 6
reduce(P, Rq, i(Dec)) <-
    untrusted(P) &
    evaluate(P, Rq, i(Dec)) &
    (pp_authorized(Rq, P) |
     pi_authorized(Rq, P) |
     dp_authorized(Rq, P) |
     di_authorized(Rq, P)).

% -------------------------
% final decision
% --------------------------

final_decision(Dec) <-
     evaluate(pdp, rq, Dec).
     evaluate_against(rq, pdp).
