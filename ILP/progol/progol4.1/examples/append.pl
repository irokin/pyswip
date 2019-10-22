%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Learning a recursive definition for appending lists.

:- set(c,5)?
:- modeh(1,append([+constant|+clist],+clist,[-constant|-clist]))?
:- modeh(1,append(+clist,+clist,-clist))?
:- modeb(1,+clist = [])?
:- modeb(1,append(+clist,+clist,-clist))?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Types

clist([]).
clist([H|T]) :- constant(H), clist(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

append([],[],[]).
append([],[a],[a]).
append([],[b],[b]).
append([],[a,b],[a,b]).
append([],[a,b,c],[a,b,c]).
append([a],[b],[a,b]).
append([c],[d],[c,d]).
append([d],[e],[d,e]).
append([a],[b,c],[a,b,c]).
append([b],[c,d],[b,c,d]).
append([c],[d,e],[c,d,e]).
append([a],[b,c,d],[a,b,c,d]).
append([c,d],[e,f],[c,d,e,f]).
append([d,e],[f,g],[d,e,f,g]).
append([d,e],[f,g,h],[d,e,f,g,h]).
append([d,e],[f,g,h,i],[d,e,f,g,h,i]).
append([1,2,3],[4,5,6],[1,2,3,4,5,6]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

:- append([],[b],[a]).
:- append([a],[],[]).
:- append([a],[b],[b]).
:- append([a],[b],[a]).
:- append([a],[b],[b,a]).
:- append([a],[b],[c,b]).
:- append([a,b],[c,d],[a,c,d]).
:- append([a,c],[c,d],[a,d]).
