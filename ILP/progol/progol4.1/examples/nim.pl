:- set(c,2), set(h,10)?
:- modeh(1,won(+side,+nat,-nat))?
:- modeb(1,+nat= #nat)?
:- modeb(3,move(+nat,-nat))?
:- modeb(*,divisible(+nat,#nat))?
:- modeb(1,other(+side,-side))?
:- modeb(1,won(+side,+nat,-nat))?

won(b,1,0).
won(b,2,0).
won(b,3,0).
won(b,5,4).
won(b,6,4).
won(b,7,4).
won(b,9,8).
won(b,10,8).
won(b,11,8).
won(b,13,12).
won(w,5,4).
won(w,6,4).
won(w,7,4).
won(w,1,0).
won(w,2,0).
won(w,3,0).

:- won(b,2,1).
:- won(b,4,0).
:- won(b,4,1).
:- won(b,4,2).
:- won(b,4,3).
:- won(b,5,3).
:- won(b,8,4).

side(b). side(w).

other(b,w). other(w,b).

red(1). red(2). red(3).

move(X,Y) :- red(R), Y is X-R.

% won(_,X,0) :- move(X,0).
% won(S,X,Y) :- move(X,Y), other(S,S1), not won(S1,Y,_).

divisible(X,Y) :- nat(Y), 0 is X mod Y.

play(X) :- X=<3, write('White wins!'), nl.
play(X) :- won(w,X,Y), write(Y), write(': '), read(M), play(M).
play(_) :- write('White resigns.').
