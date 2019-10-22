:- set(c,3),set(i,2)?
:- modeb(1,((+int)=(#int)))?
:- modeb(1,dec(+int,-int))?
% :- modeb(1,mult(+int,+int,-int))?
:- modeb(1,plus(+int,+int,-int))?
:- modeh(1,mult(+int,+int,-int))?
:- commutative(mult/3), commutative(plus/3)?

mult(0,0,0).
mult(0,1,0).
mult(0,2,0).
mult(0,3,0).
mult(0,4,0).
mult(0,5,0).
mult(0,6,0).
mult(1,2,2).
mult(1,3,3).
mult(1,4,4).
mult(1,5,5).
mult(1,6,6).
mult(2,3,6).
mult(2,4,8).
mult(2,5,10).
mult(2,6,12).
mult(2,7,14).
mult(2,8,16).
mult(2,9,18).
mult(2,10,20).
mult(3,4,12).
mult(3,5,15).
mult(3,6,18).
mult(3,7,21).
mult(3,8,24).
mult(3,9,27).

:- mult(1,1,0).
:- mult(1,4,5).
:- mult(0,0,1).
:- mult(0,1,2).
:- mult(2,3,12).
:- mult(2,3,8).
:- mult(2,4,10).
:- mult(2,5,1).
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

plus(0,X,X).
plus(X,0,X).
plus(X,Y,Z) :- Z is X+Y.

dec(X,Y) :- Y is X-1, 0=<Y.
