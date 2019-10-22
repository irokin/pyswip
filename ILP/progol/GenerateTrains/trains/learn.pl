%%%%%%%%%%%%%%%%%%%%%%%
% Train problem. This involves 5 eastbound trains and
%	5 westbound trains. Learns simple rule which distinguishes
%	between eastbound and westbound trains.

%%%%%%%%%%%%%%%%%%%%%%%
% Declarations

:- set(h,1000)?
:- set(nodes,800)?
% :- set(c,5)?

:- modeh(1,east(+train))?
:- modeb(*,infront(+train,-car,-car))?
:- modeb(*,has_car(+train,-car))?
:- modeb(1,long(+car))?
:- modeb(1,closed(+car))?
:- modeb(1,short(+car))?
:- modeb(1,open(+car))?
:- modeb(1,double(+car))?
:- modeb(1,jagged(+car))?
:- modeb(1,load(+car,#shape,#int))?
:- modeb(1,wheels(+car,#int))?

shape(circle).  shape(ellipse).  shape(hexagon).  shape(rectangle).
shape(triangle).  shape(utriangle).  shape(u_shaped). shape(diamond).

:- [train1]?
