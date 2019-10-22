%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Integer multiplication. Learns mult(X,Y,Z) given decrementation and
%	plus as background knowledge.

:- modeb(1,dec(+int,-int))?
:- modeb(1,plus(+int,+int,-int))?
:- modeb(1,inc(+int,-int))?
:- modeb(1,mult(+int,+int,-int))?
:- modeb(1,plus(+int,+int,-int))?
:- modeh(1,plus(+int,+int,-int))?
:- modeh(1,mult(+int,+int,-int))?
:- determination(plus/3,dec/2), determination(plus/3,inc/2),
	determination(plus/3,plus/3)?
:- determination(mult/3,dec/2), determination(mult/3,plus/3),
	determination(mult/3,mult/3)?
:- commutative(mult/3), commutative(plus/3)?
:- set(c,3)?

%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

mult(4,X,Y) :- plus(X,X,Z), plus(X,Z,Z1), plus(X,Z1,Y).
mult(3,X,Y) :- plus(X,X,Z), plus(X,Z,Y).
mult(2,X,Y) :- plus(X,X,Y).
mult(1,X,X).
mult(0,X,0).

plus(4,X,Y) :- inc(X,U), inc(U,V), inc(V,W), inc(W,Y).
plus(3,X,Y) :- inc(X,U), inc(U,V), inc(V,Y).
plus(2,X,Y) :- inc(X,Z), inc(Z,Y).
plus(1,X,Y) :- inc(X,Y).
plus(0,X,X).
plus(X,0,X).

%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

:- mult(0,0,1).
:- mult(2,3,12).
:- mult(2,4,10).
:- mult(2,5,4).
:- mult(2,5,12).
:- mult(2,10,19).
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
