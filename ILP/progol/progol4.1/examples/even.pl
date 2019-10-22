%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Even parity. The argument list of 0s and 1s is even if it contains an
%	even number of 1s.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Mode declarations

:- modeh(1,even(+blist))?
:- modeb(1,+constant = #constant)?
:- modeb(1,+blist = [-bin|-blist])?
:- modeb(1,even(+blist))?
:- modeb(1,not(even(+blist)))?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Types

bin(0).
bin(1).

blist([]).
blist([X|Y]) :- bin(X), blist(Y).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

even([]).
even([0]).
even([0,0]).
even([1,1]).
even([0,0,0]).
even([0,1,1]).
even([1,0,1]).
even([1,1,0]).
even([0,0,0,0]).
even([0,0,1,1]).
even([0,1,0,1]).
even([1,0,0,1]).
even([0,1,1,0]).
even([1,0,1,0]).
even([1,1,0,0]).
even([1,1,1,1]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

:- even([1]).
:- even([0,1]).
:- even([1,0]).
:- even([0,0,1]).
:- even([0,1,0]).
:- even([1,0,0]).
:- even([1,1,1]).
:- even([0,0,0,1]).
:- even([0,0,1,0]).
:- even([0,1,0,0]).
:- even([1,0,0,0]).
:- even([0,1,1,1]).
:- even([1,0,1,1]).
:- even([1,1,0,1]).
:- even([1,1,1,0]).
