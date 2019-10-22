%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% List element deletion.

:- modeh(1,delete(+constant,+clist,+clist))?
:- modeb(1,+constant = #constant)?
:- modeb(1,+clist = [-constant|-clist])?
:- modeb(1,delete(+constant,+clist,+clist))?

%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Types

clist([]).
clist([H|T]) :- constant(H), clist(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

%delete(0,[0],[]).
%delete(1,[1],[]).
%delete(2,[2],[]).
%delete(3,[3],[]).
delete(4,[4],[]).
%delete(1,[0,1],[0]).
%delete(0,[0,0],[0]).
%delete(0,[1,0],[1]).
%delete(0,[2,0],[2]).
%delete(1,[1,1],[1]).
delete(1,[2,1],[2]).
delete(2,[2,2],[2]).
%delete(2,[3,2],[3]).
delete(3,[2,3],[2]).
delete(4,[3,4,5],[3,5]).
delete(3,[4,2,3],[4,2]).
delete(4,[5,2,4],[5,2]).


%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

:- delete(X,[],X).
:- delete(X,[Y],Z), not(X==Y).
:- delete(0,[1,2],X).
:- delete(0,[1,2,3],X).
:- delete(0,[1,2,3,4],X).
:- delete(0,[1,0,3,4],[1,2]).
