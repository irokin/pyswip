%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Learning legal chess moves. The moves of the chess pieces
%	
%	Pieces = {King, Queen, Bishop, Knight and Rook}
%
% are learned from examples. Each example is represented by
% a triple from the domain
%
%	Piece x (Rank x File) x (Rank x File)
%
% For instance, the following illustrates a knight (n) move example.
%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   n   .   .  |
%	3|  .   .   .   .|
%	2|. n .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h
%
% The only background predicate used is symmetric difference, i.e.
%
%	diff(X,Y) = absolute difference between X and Y
%
% Symmetric difference is defined separately on Rank and File.


:- set(c,2), set(i,1)?
:- modeh(1,move(#piece,pos(+file,+rank),pos(+file,+rank)))?
:- modeb(1,rdiff(+rank,+rank,-nat))?
:- modeb(1,fdiff(+file,+file,-nat))?
:- modeb(1,rdiff(+rank,+rank,#nat))?
:- modeb(1,fdiff(+file,+file,#nat))?

:- commutative(rdiff/3)?
:- commutative(fdiff/3)?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Types

piece(king).  piece(queen).  piece(bishop).
piece(knight).  piece(rook).  piece(pawn).

rank(1).  rank(2).  rank(3).  rank(4).
rank(5).  rank(6).  rank(7).  rank(8).

file(a).  file(b).  file(c).  file(d).
file(e).  file(f).  file(g).  file(h).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Background knowledge

rdiff(Rank1,Rank2,Diff) :-
	rank(Rank1), rank(Rank2),
	Diff1 is Rank1-Rank2,
	abs(Diff1,Diff).

fdiff(File1,File2,Diff) :-
	file(File1), file(File2),
	project(File1,Rank1),
	project(File2,Rank2),
	Diff1 is Rank1-Rank2,
	abs(Diff1,Diff).

abs(X,X) :- X>=0.
abs(X,Y) :- X<0, Y is -X.

project(a,1).  project(b,2).  project(c,3).  project(d,4).
project(e,5).  project(f,6).  project(g,7).  project(h,8).

%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   n   .   .  |
%	3|  .   .   .   .|
%	2|. n .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(knight,pos(b,2),pos(c,4)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   n   .   .|
%	2|. n .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(knight,pos(b,2),pos(d,3)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|. n .   .   .  |
%	5|  .   .   .   .|
%	4|.   n   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(knight,pos(c,4),pos(b,6)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|n .   .   .   .|
%	4|.   n   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(knight,pos(c,4),pos(a,5)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   . n .|
%	2|.   .   .   .  |
%	1|  .   .   n   .|
%	 |_______________|
%	  a b c d e f g h

move(knight,pos(f,1),pos(g,3)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   . n|
%	1|  .   .   n   .|
%	 |_______________|
%	  a b c d e f g h

move(knight,pos(f,1),pos(h,2)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   b   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   b   .|
%	 |_______________|
%	  a b c d e f g h

move(bishop,pos(f,1),pos(c,4)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   b   .  |
%	5|  .   .   .   .|
%	4|.   b   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(bishop,pos(c,4),pos(e,6)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   b   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|b   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(bishop,pos(e,6),pos(a,2)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|r   .   r   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(rook,pos(b,2),pos(e,2)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|r .   .   .   r|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(rook,pos(a,5),pos(h,5)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   r   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|.   . r .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(rook,pos(d,2),pos(d,5)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|r .   .   .   .|
%	4|r   .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(rook,pos(a,4),pos(a,5)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   q   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   q   .|
%	 |_______________|
%	  a b c d e f g h

move(queen,pos(f,1),pos(c,4)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   q   .  |
%	5|  .   .   .   .|
%	4|.   q   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(queen,pos(c,4),pos(e,6)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   q   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|q   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(queen,pos(e,6),pos(a,2)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|q   .   q   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(queen,pos(b,2),pos(e,2)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|q .   .   .   q|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(queen,pos(a,5),pos(h,5)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   q   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|.   . q .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(queen,pos(d,2),pos(d,5)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|q .   .   .   .|
%	4|q   .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(queen,pos(a,4),pos(a,5)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|q .   .   .   .|
%	4|q   .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(king,pos(a,4),pos(a,5)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   . k .   .  |
%	5|  .   k   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(king,pos(d,5),pos(d,6)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  k k .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(king,pos(b,5),pos(c,5)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   k k .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(king,pos(d,3),pos(e,3)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   . k .   .  |
%	3|  .   . k .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(king,pos(d,4),pos(e,3)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   . k .|
%	6|.   .   . k .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(king,pos(f,6),pos(g,7)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  . k .   .   .|
%	2|. k .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

move(king,pos(b,2),pos(c,3)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   n  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   n   .|
%	 |_______________|
%	  a b c d e f g h

:- move(knight,pos(f,1),pos(g,4)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|.   n   .   .  |
%	1|  .   .   n   .|
%	 |_______________|
%	  a b c d e f g h

:- move(knight,pos(f,1),pos(c,2)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   n|
%	2|.   .   .   .  |
%	1|  .   .   n   .|
%	 |_______________|
%	  a b c d e f g h

:- move(knight,pos(f,1),pos(h,3)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  b   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   b   .|
%	 |_______________|
%	  a b c d e f g h

:- move(bishop,pos(f,1),pos(b,3)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  r   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   r   .|
%	 |_______________|
%	  a b c d e f g h

:- move(rook,pos(f,1),pos(b,3)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  q   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   q   .|
%	 |_______________|
%	  a b c d e f g h

:- move(queen,pos(f,1),pos(b,3)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|. q .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   .   q   .|
%	 |_______________|
%	  a b c d e f g h

:- move(queen,pos(f,1),pos(b,4)).

%         _______________
%	8|. q .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|.   .   .   .  |
%	1|  .   . q .   .|
%	 |_______________|
%	  a b c d e f g h

:- move(queen,pos(e,1),pos(b,8)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   k   .   .|
%	2|. k .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

:- move(king,pos(b,2),pos(d,3)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   k   .   .  |
%	3|  .   .   .   .|
%	2|. k .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

:- move(king,pos(b,2),pos(c,4)).

%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|. k .   .   .  |
%	3|  .   .   .   .|
%	2|. k .   .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

:- move(king,pos(b,2),pos(b,4)).


%         _______________
%	8|.   .   .   .  |
%	7|  .   .   .   .|
%	6|.   .   .   .  |
%	5|  .   .   .   .|
%	4|.   .   .   .  |
%	3|  .   .   .   .|
%	2|. k . k .   .  |
%	1|  .   .   .   .|
%	 |_______________|
%	  a b c d e f g h

:- move(king,pos(b,2),pos(d,2)).
