%%%%%%%%%%%%%%%%%%%%%%%%
% Eleusis parlour game 3. This is a guessing game involving cards and dice.
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

:- set(c,2)?

% Positive examples

% can_follow(card, suit, card, suit, number, number)
can_follow(5,d,4,h,1,1).
can_follow(8,c,5,d,1,2).
can_follow(j,s,8,c,1,1).
can_follow(2,c,j,s,1,2).
can_follow(5,s,2,c,1,3).
can_follow(a,c,5,s,1,4).
can_follow(5,s,a,c,1,5).
can_follow(0,h,5,s,1,6).
can_follow(3,c,6,s,1,2).

% Negative examples

:- can_follow(7,c,4,h,1,1).
:- can_follow(j,h,4,h,1,1).
:- can_follow(4,c,4,h,1,1).
:- can_follow(0,s,4,h,1,1).
:- can_follow(8,h,4,h,1,1).
:- can_follow(a,d,4,h,1,1).
:- can_follow(2,d,4,h,1,1).
:- can_follow(6,s,5,d,1,2).
:- can_follow(7,h,5,d,1,2).
:- can_follow(2,d,5,d,1,2).
:- can_follow(7,s,5,d,1,2).
:- can_follow(6,d,5,d,1,2).
:- can_follow(6,h,5,d,1,2).
:- can_follow(4,c,5,d,1,2).
:- can_follow(k,c,8,c,1,1).
:- can_follow(3,h,8,c,1,1).
:- can_follow(a,h,j,s,1,2).
:- can_follow(k,d,j,s,1,2).
:- can_follow(q,s,j,s,1,2).
:- can_follow(6,c,5,s,1,4).
:- can_follow(a,s,5,s,1,6).
