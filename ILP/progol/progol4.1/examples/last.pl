%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Last in list problem. Learns recursive clauses for last(Element,List)

:- modeh(*,last(+constant,+clist))?
:- modeb(1,+constant = #constant)?
:- modeb(1,+clist = [-constant|-clist])?
:- modeb(*,last(+constant,+clist))?

%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Types

clist([]).
clist([H|T]) :- constant(H), clist(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

last(a,[a]).
last(b,[b]).
last(c,[c]).
last(a,[b,a]).
last(b,[a,b]).
last(c,[a,c]).
last(c,[b,c]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

:- last(X,[]).
:- last(X,[Y]), not(X==Y).
:- last(1,[1,2]).
:- last(a,[c,a,b]).
:- last(0,[1,2]).
