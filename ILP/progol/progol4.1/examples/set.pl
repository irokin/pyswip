%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Set membership problems. Learns recursive clauses for member(Element,List)
% This is then used to learn pair(E1,E2,List) and subset(S1,S2).

:- modeh(1,member(+constant,+clist))?
:- modeh(1,pair(+constant,+constant,+clist))?
:- modeh(1,subset(+clist,+clist))?
:- modeb(1,+constant = #constant)?
:- modeb(1,+clist = [-constant|-clist])?
:- modeb(1,member(+constant,+clist))?
:- modeb(1,subset(+clist,+clist))?

%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Types

clist([]).
clist([H|T]) :- constant(H), clist(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

member(0,[0]).
member(1,[0,1]).
member(0,[0,0]).
member(0,[1,0]).
member(0,[2,0]).
member(1,[1,1]).
member(1,[1]).
member(1,[2,1]).
member(2,[2,2]).
member(2,[2]).
member(2,[3,2]).
member(3,[3]).
member(3,[4,2,3]).
member(5,[3,4,5]).
member(4,[4]).

pair(a,b,[a,b,c]).
pair(b,a,[a,c,b]).
pair(1,2,[3,2,1,0]).

subset([],[c]).
subset([],[c,a]).
subset([],[c,a,b]).
subset([1],[2,1,3]).
subset([c],[a,c]).
subset([a],[b,a,c]).
subset([b],[b,a,c]).
subset([c],[b,a,c]).
subset([b,a],[a,b]).
subset([a,b],[a,b,c]).
subset([b,a],[c,a,b]).
subset([b,a,c],[c,a,b,d]).
subset([b,a,c],[c,a,b,d]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

:- member(X,[]).
:- member(X,[Y]), not(X==Y).
:- member(0,[1,2]).

:- pair(a,b,[a]).
:- pair(a,b,[b]).

:- subset([a,c],[a,b]).
:- subset([a],[b]).
:- subset([a],[c]).
:- subset([b,a],[b]).
:- subset([b,a,c],[b,c,d]).
:- subset([b,a],[b,c,d]).
:- subset([b,a],[c,b,d]).
:- subset([b,a],[c,b,a]).
