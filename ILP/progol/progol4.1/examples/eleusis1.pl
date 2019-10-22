%%%%%%%%%%%%%%%%%%%%%%%%
% Eleusis parlour game 1. This is a guessing game involving cards and dice.
% One player (the rule-maker) makes up game rules and presents the other
% player (the guesser) with a pair of cards and a pair of dice.
% The guesser is then told whether the hidden rule is true or not of the
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

% can_follow(card, suit, card, suit, number, number)

can_follow(7,c,a,h,1,1).
can_follow(6,c,7,c,1,1).
can_follow(9,s,6,c,2,2).
can_follow(0,h,9,s,1,3).
can_follow(7,h,0,h,1,1).
can_follow(0,d,7,h,2,2).
can_follow(j,c,0,d,1,3).
can_follow(a,d,j,c,1,1).
can_follow(4,h,a,d,1,1).
can_follow(8,d,4,h,1,2).
can_follow(7,c,8,d,1,3).
can_follow(9,s,7,c,1,1).
can_follow(0,c,9,s,1,2).
can_follow(k,s,0,c,1,3).
can_follow(2,c,k,s,1,4).
can_follow(0,s,2,c,1,5).
can_follow(j,s,0,s,1,6).

% Negative examples

:- can_follow(k,d,6,c,2,2).
:- can_follow(j,h,6,c,2,2).
:- can_follow(5,s,7,h,2,2).
:- can_follow(q,d,j,c,1,1).
:- can_follow(3,s,4,h,1,2).
:- can_follow(9,h,9,s,1,2).
:- can_follow(6,h,9,s,1,2).
:- can_follow(q,h,k,s,1,4).
:- can_follow(a,d,k,s,1,4).
