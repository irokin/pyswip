%%%%%%%%%%%%%%%%%%%
% Learning a simple range concept such as 3<=X<=5. This demonstrates
%	the use of the recall number when using =</2.

:- modeb(4,+int =< #int)?
:- modeb(4,#int =< +int)?
:- modeh(1,inrange(+int))?
:- set(i,1)?

%%%%%%%%%%%%%%%%%%%
% Positive examples
inrange(2).
inrange(3).
inrange(4).
inrange(5).
inrange(10).
inrange(12).
inrange(15).

%%%%%%%%%%%%%%%%%%%
% Negative examples
:- inrange(0).
:- inrange(1).
:- inrange(6).
