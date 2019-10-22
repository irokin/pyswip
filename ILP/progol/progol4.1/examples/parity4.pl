%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Circuit design of a parity checker with limited time delay.
%	Examples represent a complete tabulation of the I/O relation.
%	The learned circuit looks as follows.
%
%			|| 		Output	(Delay=2)
%		     |~~~~~~|
%		     | NXOR |
%		      ~|~~|~
%		   ||~~~  ~~~||			(Delay=1)
%		|~~~~~~|  |~~~~~~|
%		| NXOR |  | NXOR |
%		 ~|~~|~    ~|~~|~
%		  a  b      c  d	Inputs	(Delay=0)
%

:- set(c,3), set(i,2), set(h,1000)?
:- modeh(1,parity(in(+bdn,+bdn,+bdn,+bdn),-bdn))?
:- modeb(1,nxor(+bdn,+bdn,-bdn))?

:- commutative(nxor/3)?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Type information

bdn(v(f,Delay,[_|_])) :- nat(Delay).
bdn(v(t,Delay,[_|_])) :- nat(Delay).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Background knowledge

nxor(v(I1,D1,N1),v(I2,D2,N2),v(O,D,N3)) :-
	nxor1(I1,I2,O), min(D1,D2,D3), D is D3+1,
	append(N1,N2,N4), sort(N4,N3).

nxor1(f,f,t).
nxor1(f,t,f).
nxor1(t,f,f).
nxor1(t,t,t).

min(X,Y,X) :- X=<Y, !.
min(X,Y,Y).

append([],X,X) :- !.
append([H|T],L,[H|R]) :- append(T,L,R), !.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Examples


parity(in(v(f,0,[a]),v(f,0,[b]),v(f,0,[c]),v(f,0,[d])),v(t,2,[a,b,c,d])).
parity(in(v(f,0,[a]),v(f,0,[b]),v(f,0,[c]),v(t,0,[d])),v(f,2,[a,b,c,d])).
parity(in(v(f,0,[a]),v(f,0,[b]),v(t,0,[c]),v(f,0,[d])),v(f,2,[a,b,c,d])).
parity(in(v(f,0,[a]),v(f,0,[b]),v(t,0,[c]),v(t,0,[d])),v(t,2,[a,b,c,d])).
parity(in(v(f,0,[a]),v(t,0,[b]),v(f,0,[c]),v(f,0,[d])),v(f,2,[a,b,c,d])).
parity(in(v(f,0,[a]),v(t,0,[b]),v(f,0,[c]),v(t,0,[d])),v(t,2,[a,b,c,d])).
parity(in(v(f,0,[a]),v(t,0,[b]),v(t,0,[c]),v(f,0,[d])),v(t,2,[a,b,c,d])).
parity(in(v(f,0,[a]),v(t,0,[b]),v(t,0,[c]),v(t,0,[d])),v(f,2,[a,b,c,d])).
parity(in(v(t,0,[a]),v(f,0,[b]),v(f,0,[c]),v(f,0,[d])),v(f,2,[a,b,c,d])).
parity(in(v(t,0,[a]),v(f,0,[b]),v(f,0,[c]),v(t,0,[d])),v(t,2,[a,b,c,d])).
parity(in(v(t,0,[a]),v(f,0,[b]),v(t,0,[c]),v(f,0,[d])),v(t,2,[a,b,c,d])).
parity(in(v(t,0,[a]),v(f,0,[b]),v(t,0,[c]),v(t,0,[d])),v(f,2,[a,b,c,d])).
parity(in(v(t,0,[a]),v(t,0,[b]),v(f,0,[c]),v(f,0,[d])),v(t,2,[a,b,c,d])).
parity(in(v(t,0,[a]),v(t,0,[b]),v(f,0,[c]),v(t,0,[d])),v(f,2,[a,b,c,d])).
parity(in(v(t,0,[a]),v(t,0,[b]),v(t,0,[c]),v(f,0,[d])),v(f,2,[a,b,c,d])).
parity(in(v(t,0,[a]),v(t,0,[b]),v(t,0,[c]),v(t,0,[d])),v(t,2,[a,b,c,d])).

:- parity(in(v(f,0,[a]),v(f,0,[b]),v(f,0,[c]),v(f,0,[d])),v(f,2,[a,b,c,d])).
:- parity(in(v(f,0,[a]),v(f,0,[b]),v(f,0,[c]),v(t,0,[d])),v(t,2,[a,b,c,d])).
:- parity(in(v(f,0,[a]),v(f,0,[b]),v(t,0,[c]),v(f,0,[d])),v(t,2,[a,b,c,d])).
:- parity(in(v(f,0,[a]),v(f,0,[b]),v(t,0,[c]),v(t,0,[d])),v(f,2,[a,b,c,d])).
:- parity(in(v(f,0,[a]),v(t,0,[b]),v(f,0,[c]),v(f,0,[d])),v(t,2,[a,b,c,d])).
:- parity(in(v(f,0,[a]),v(t,0,[b]),v(f,0,[c]),v(t,0,[d])),v(f,2,[a,b,c,d])).
:- parity(in(v(f,0,[a]),v(t,0,[b]),v(t,0,[c]),v(f,0,[d])),v(f,2,[a,b,c,d])).
:- parity(in(v(f,0,[a]),v(t,0,[b]),v(t,0,[c]),v(t,0,[d])),v(t,2,[a,b,c,d])).
:- parity(in(v(t,0,[a]),v(f,0,[b]),v(f,0,[c]),v(f,0,[d])),v(t,2,[a,b,c,d])).
:- parity(in(v(t,0,[a]),v(f,0,[b]),v(f,0,[c]),v(t,0,[d])),v(f,2,[a,b,c,d])).
:- parity(in(v(t,0,[a]),v(f,0,[b]),v(t,0,[c]),v(f,0,[d])),v(f,2,[a,b,c,d])).
:- parity(in(v(t,0,[a]),v(f,0,[b]),v(t,0,[c]),v(t,0,[d])),v(t,2,[a,b,c,d])).
:- parity(in(v(t,0,[a]),v(t,0,[b]),v(f,0,[c]),v(f,0,[d])),v(f,2,[a,b,c,d])).
:- parity(in(v(t,0,[a]),v(t,0,[b]),v(f,0,[c]),v(t,0,[d])),v(t,2,[a,b,c,d])).
:- parity(in(v(t,0,[a]),v(t,0,[b]),v(t,0,[c]),v(f,0,[d])),v(t,2,[a,b,c,d])).
:- parity(in(v(t,0,[a]),v(t,0,[b]),v(t,0,[c]),v(t,0,[d])),v(f,2,[a,b,c,d])).
