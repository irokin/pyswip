%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Find the element of a list. Can be learned alone or with 
%	least/3 as background knowledge.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Declarations

:- modeh(1,min(+ilist,-int))?
:- modeb(1,min(+ilist,-int))?
:- modeb(1,least(+int,+int,-int))?
:- modeb(1,+any= #any)?
:- modeb(1,+ilist=[-int|-ilist])?
% :- modeb(1,+int =< +int)?
:- commutative(least/3)?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Types

ilist([]).
ilist([H|T]) :- int(I), ilist(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

min([3,1,2],1).
min([1,2,3],1).
min([5,2,4],2).
min([2,4,5],2).
min([5,3,2],2).
min([2,5,3],2).
min([5,3,3],3).
min([3,5,7],3).
min([3,2,1],1).
min([8,3,2],2).
min([2,8,3],2).

min([X],X).
min([X,Y],X) :- X=<Y.
min([X,Y],Y) :- Y=<X.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

:- min([1,2,3],3).
:- min([3,2,1],2).
:- min([2,1],2).
:- min([1,1],2).
:- min([1,2],2).
:- min([1,2],0).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Background knowledge

least(X,Y,X) :- X=<Y.
least(X,Y,Y) :- Y=<X.
