%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Sorting numbers problem. The background knowledge is partition and append.
%	Learns insertion sort rather than quick-sort since it is
%	a shorter definition.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Mode declarations
:- set(i,2), set(c,3)?
:- modeb(1,qsort(+ilist,-ilist))?
:- modeb(1,part(+int,+ilist,-ilist,-ilist))?
:- modeb(1,append(+ilist,[+int|+ilist],-ilist))?
:- modeh(1,qsort([+int|+ilist],-ilist))?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Type information

ilist([]).
ilist([Head|Tail]) :- int(Head), ilist(Tail).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Background knowledge
part(X,[],[],[]).
part(X,[X|Tail],List1,List2) :- part(X,Tail,List1,List2).
part(X,[Head|Tail],[Head|Tail1],List2) :-
	Head < X, part(X,Tail,Tail1,List2).
part(X,[Head|Tail],List1,[Head|Tail2]) :-
	Head > X, part(X,Tail,List1,Tail2).

append([],List,List).
append([Head|Tail],List1,[Head|List2]) :- append(Tail,List1,List2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples
qsort([],[]).
qsort([3,2,1],[1,2,3]).
qsort([X],[X]).
qsort([X,Y],[X,Y]) :- X < Y.
qsort([Y,X],[X,Y]) :- X < Y.
qsort([X,Y,Z],[X,Y,Z]) :- X<Y, Y<Z.
qsort([Z,Y,X],[X,Y,Z]) :- X<Y, Y<Z.
qsort([X,Z,Y],[X,Y,Z]) :- X<Y, Y<Z.
qsort([Y,X,Z],[X,Y,Z]) :- X<Y, Y<Z.
qsort([Y,Z,X],[X,Y,Z]) :- X<Y, Y<Z.
qsort([Z,X,Y],[X,Y,Z]) :- X<Y, Y<Z.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples
:- qsort([0,2,1],[0,2,1]).
:- qsort([0,2,1],[0,1]).
:- qsort([1,0,2],[2,0,1]).
:- qsort([1,0,2],[2,1,0]).
:- qsort([1,2,0],[1,0,2]).
:- qsort([0,2,1],[2,1,0]).
:- qsort([2,1,0],[2,1,0]).
:- qsort([2,0,1],[2,1,0]).
:- qsort([2,1],[1]).
:- qsort([1],[2]).
:- qsort([0,1,2],[1,0,2]).
:- qsort([0,1],[1,0,1]).
