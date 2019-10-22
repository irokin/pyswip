%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Reversing of lists. Learns recursive solution for reversing lists
%	using list append as background knowledge.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Mode declarations
:- modeh(1,reverse(+list,-list))?
:- modeb(1,reverse(+list,-list))?
:- modeb(1,append(+list,[+int],-list))?
:- modeb(1,+any= #any)?
:- modeb(1,+list=[-int|-list])?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Type defintions
list([]).
list([H|T]) :- list(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Background knowledge
append([],X,X).
append([H|T],L1,[H|L2]) :-
	append(T,L1,L2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples
reverse([],[]).
reverse([1],[1]).
reverse([1,2,3],[3,2,1]).
reverse([2],[2]).
reverse([3],[3]).
reverse([4],[4]).
reverse([1,2],[2,1]).
reverse([1,3],[3,1]).
reverse([1,4],[4,1]).
reverse([2,2],[2,2]).
reverse([2,3],[3,2]).
reverse([2,4],[4,2]).
reverse([0,1,2],[2,1,0]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples
:- reverse([1],[]).
:- reverse([],[0]).
:- reverse([0,1],[0,1]).
:- reverse([0,1,2],[2,0,1]).
:- reverse([1,2,3],[2,3,1]).
:- reverse([1,2,3],[3,2,4]).
:- reverse([1,2,3],[4,2,1]).
