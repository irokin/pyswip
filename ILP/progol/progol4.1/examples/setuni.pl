%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Set union for ordered lists. This involves learning one extra clause.
%	to complete the definition.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Mode declarations
:- nosplit?
:- set(i,1), set(c,2), set(h,20)?
:- modeb(1,+int =< +int)?
:- modeb(1,setuni(+ilist,[+int|+ilist],-ilist))?
:- modeh(1,setuni([+int|+ilist],[+int|+ilist],[-int|-ilist]))?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Types
ilist([]).
ilist([H|T]) :- int(H), ilist(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples
setuni([],X,X).
setuni(X,[],X).
setuni([E1|S1],[E2|S2],[E2|S3]) :-
	E2=<E1, setuni([E1|S1],S2,S3).

setuni([1,3],[2],[1,2,3]).
setuni([2,4],[3],[2,3,4]).
setuni([1,3],[2,4],[1,2,3,4]).
setuni([2,4],[3,5],[2,3,4,5]).
setuni([1,7],[2,4,6],[1,2,4,6,7]).
setuni([0,7],[2,4,6],[0,2,4,6,7]).
setuni([0,9],[2,4,6],[0,2,4,6,9]).
setuni([0,10],[2,4,6,7],[0,2,4,6,7,10]).
setuni([0,3,4],[1,2],[0,1,2,3,4]).
setuni([1,3,5],[2,4,6],[1,2,3,4,5,6]).
setuni([X],[Y],[X,Y]) :- X=<Y.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples
:- setuni([1],[3],[1]).
:- setuni([1],[3],[3,1]).
:- setuni([1],[3],[3]).
:- setuni([3],[1],[]).
:- setuni([3],[1],[2]).
:- setuni([2,3],[1,4],[2,1,3,4]).
:- setuni([3,4],[2,5],[3,2,4,5]).
:- setuni([3,4],[2],[3,2,4]).
:- setuni([3,4],[2],[1,2,4]).
:- setuni([2],[3,4],[3,2,4]).
:- setuni([2],[3,4],[2,3,2,4]).
:- setuni([2],[3,4],[1,2,4]).
:- setuni([2],[3,4],[2,4]).
:- setuni([3,1],[2],[0,1,2]).
