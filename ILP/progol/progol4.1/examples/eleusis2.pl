%%%%%%%%%%%%%%%%%%%%%%%%
% Eleusis parlour game 2. This is a guessing game involving cards and dice.
% One player (the rule-maker) makes up game rules and presents the other
% player (the guesser) with a pair of cards and a pair of dice.
% The guesser is then told whether the game rules are true or not of the
% cards and dice. eg. positive examples might look as follows
%
%		 ___    ___
%		|7  |  |A  |   _   _
%		| C |  | H |  |1| |1|
%		|  7|  |  A|   ~   ~
%		 ~~~    ~~~
%		 ___    ___
%		|2  |  |K  |   _   _
%		| C |  | S |  |1| |4|
%		|  2|  |  K|   ~   ~
%		 ~~~    ~~~
% and the rule might be that either the second dice is odd or the
% suits must have the same colour.
%
% The 3 different eleusis (eleusis1,eleusis2,eleusis3) games were
% obtained from J.R. Quinlan (University of Sydney). This file contains
% common background knowledge for all three games.

:- [eleusis]?	% Common background knowledge

% Positive examples

% can_follow(card, suit, card, suit, number, number).
can_follow(4,d,j,c,1,1).
can_follow(q,h,4,d,1,1).
can_follow(3,s,q,h,1,2).
can_follow(q,d,3,s,1,1).
can_follow(9,h,q,d,1,1).
can_follow(q,c,9,h,1,2).
can_follow(7,h,q,c,1,1).
can_follow(q,d,7,h,1,1).                  
can_follow(9,d,q,d,1,1).
can_follow(q,c,9,d,2,2).
can_follow(3,h,q,c,1,1).
can_follow(k,h,3,h,1,1).
can_follow(4,c,k,h,2,2).
can_follow(k,d,4,c,1,1).
can_follow(6,c,k,d,1,1).
can_follow(j,d,6,c,1,1).
can_follow(8,d,j,d,1,1).
can_follow(j,h,8,d,2,2).
can_follow(7,c,j,h,1,3).
can_follow(j,d,7,c,1,1).
can_follow(7,h,j,d,1,1).
can_follow(j,h,7,h,1,2).
can_follow(6,h,j,h,2,3).
can_follow(k,d,6,h,3,4).

% Negative examples

:- can_follow(k,c,j,c,1,1).
:- can_follow(5,s,4,d,1,1).
:- can_follow(7,s,4,d,1,1).
:- can_follow(4,s,9,h,1,2).
:- can_follow(0,d,7,h,1,1).
