%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Sum of x from 0 to n. Thus sumx of 3=0+1+2+3. Background knowledge
%	can either be multiplication, division and incrementation
%	to give sumx(n)= (n*(n+1))/2 or it can be learned recursively
%	as sumx(n)=sumx(n-1)+n.

%%%%%%%%%%%%%%
% Mode declarations.
:- noreductive?

% Modes for closed form solution:	sumx(n)=(n*(n+1))/2
% :- modeb(1,-nat is (+nat+1))?
% :- modeb(1,mult(+nat,+nat,-nat))?
% :- modeb(6,div(+nat,#nat,-nat))?

% Modes for recursive solution:		sumx(n)=sumx(n-1)+n
:- modeb(1,-nat is (+nat-1))?
:- modeb(1,sumx(+nat,-nat))?
:- modeb(1,plus(+nat,+nat,-nat))?

:- modeh(1,sumx(+nat,-nat))?
:- commutative(mult/3)?
:- commutative(plus/3)?

%%%%%%%%%%%%%%
% Mode declaration for sumx(n)=sumx(n-1)+n
% :- modeb(1,-nat is (+nat-1))?
% :- modeb(1,sumx(+,-))?
% :- modeb(1,-nat is (+nat+(+nat)))?

%%%%%%%%%%%%%%
% Background knowledge
plus(X,Y,Z) :- Z is X+Y.

mult(X,Y,Z) :- Z is X*Y.

div(X,Y,Z) :- 2=<Y, Z is X/Y, X is Y*Z.

%%%%%%%%%%%%%%
% Positive examples

sumx(6,21).
sumx(5,15).
sumx(4,10).
sumx(3,6).
sumx(2,3).
sumx(1,1).
sumx(0,0).

%%%%%%%%%%%%%%
% Negative examples

:- sumx(1,2).
:- sumx(2,4).
:- sumx(3,3).
