%%%%%%%%%%%%%%%%%%%
% Learning a simple range concept such as 0=<X=<PI or -2PI=<X=<-PI.
%	This demonstrates the use of the recall number when using =</2.

:- modeb(16,(+any=< #any))?
:- modeb(16,(#any=< +any))?
:- modeh(1,inrange(+any))?
:- set(i,1),set(c,2)?

%%%%%%%%%%%%%%%%%%%
% Positive examples
inrange(-6.2).
inrange(-6.0).
inrange(-5.5).
inrange(-5.0).
inrange(-4.5).
inrange(-4.0).
inrange(-3.5).
inrange(-3.2).
inrange(0.1).
inrange(0.2).
inrange(0.5).
inrange(1.1).
inrange(1.5).
inrange(2.2).
inrange(2.8).
inrange(3.1).

%%%%%%%%%%%%%%%%%%%
% Negative examples
:- inrange(-6.3).
:- inrange(-3.1).
:- inrange(-0.1).
:- inrange(3.3).
:- inrange(6).
