%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Integer exponeniation. This example demonstrates incremental
%	learning in which each learned predicate definition is
%	used to learn the remaining ones. The recursive
%	definition of plus is learned first. Using this multiplication
%	is learned. Lastly, exponentiation is learned in terms
%	of multiplication.
%
%		exp/3
%
%		  /\
%		  ||
%
%		mult/3
%
%		  /\
%		  ||
%
%		plus/3
%
%	In general incremental learning of this kind requires
%	determinations to be used to avoid consideration of
%	irrelevant background knowledge.

:- modeh(1,plus(+int,+int,-int))?
:- modeh(1,mult(+int,+int,-int))?
:- modeh(1,exp(+int,+int,-int))?
:- modeb(1,dec(+int,-int))?
:- modeb(1,plus(+int,+int,-int))?
:- modeb(1,inc(+int,-int))?
:- modeb(1,mult(+int,+int,-int))?
:- modeb(1,exp(+int,+int,-int))?
:- determination(exp/3,dec/2), determination(exp/3,mult/3),
	determination(exp/3,exp/3)?
:- determination(plus/3,dec/2), determination(plus/3,inc/2),
	determination(plus/3,plus/3)?
:- determination(mult/3,dec/2), determination(mult/3,plus/3),
	determination(mult/3,mult/3)?
:- commutative(mult/3), commutative(plus/3)?
:- set(c,3), set(h,30)?

%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

exp(X,4,Y) :- mult(X,X,Z), mult(X,Z,Z1), mult(X,Z1,Y).
exp(X,3,Y) :- mult(X,X,Z), mult(X,Z,Y).
exp(X,2,Y) :- mult(X,X,Y).
exp(X,1,X).
exp(X,0,1).

mult(1,X,X).
mult(4,X,Y) :- plus(X,X,Z), plus(X,Z,Z1), plus(X,Z1,Y).
mult(3,X,Y) :- plus(X,X,Z), plus(X,Z,Y).
mult(2,X,Y) :- plus(X,X,Y).
mult(X,1,X).
mult(0,X,0).

plus(4,X,Y) :- inc(X,U), inc(U,V), inc(V,W), inc(W,Y).
plus(3,X,Y) :- inc(X,U), inc(U,V), inc(V,Y).
plus(2,X,Y) :- inc(X,Z), inc(Z,Y).
plus(1,X,Y) :- inc(X,Y).
plus(0,X,X).
plus(X,0,X).

%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

:- exp(0,1,1).
:- exp(1,2,3).
:- exp(2,2,1).
:- exp(3,2,8).
:- exp(4,3,3).

:- mult(0,0,1).
:- mult(2,3,0).
:- mult(2,3,12).
:- mult(2,4,10).
:- mult(2,5,4).
:- mult(2,5,12).
:- mult(2,10,19).
:- mult(3,3,3).
:- mult(3,5,12).
:- mult(3,4,6).
:- mult(3,4,3).
:- mult(3,4,10).
:- mult(3,4,16).
:- mult(3,4,8).
:- mult(3,4,14).
:- mult(4,7,11).
:- mult(4,11,40).
:- mult(4,11,55).
:- mult(5,11,50).
:- mult(2,2,2).
:- mult(2,2,6).
:- mult(3,3,6).
:- mult(3,10,20).

:- plus(0,0,1).
:- plus(1,2,1).
:- plus(2,2,1).
:- plus(3,2,2).
:- plus(4,3,3).

%%%%%%%%%%%%%%%%%%%%%%%%%
% Background knowledge

inc(X,Y) :- Y is X+1.

dec(X,Y) :- Y is X-1, 0=<Y.
