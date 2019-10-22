%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Domain-dependent predicates for drawing Michalski trains.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:- set(h,1000000)?
:- [ps,rtrain]?
:- randomseed?

engine(LRx@LRy,LRx@LRy) :-
	LRx1=LRx+32, LRy1=LRy-4,
	comment('DRAW AN ENGINE'),
	rect(LRx1-32@LRy1,LRx1-20@LRy1+14,stroke,'ENGINE BODY (BACK)'),
	rect(LRx1-31@LRy1+2,LRx1-26@LRy1+12,stroke,'LARGE WINDOW'),
	rect(LRx1-25@LRy1+8,LRx1-21@LRy1+12,stroke,'SMALL WINDOW'),
	rect(LRx1-20@LRy1,LRx1@LRy1+9,stroke,'ENGINE BODY (FRONT)'),
	polygon([LRx1-5@LRy1+9,LRx1-4.6@LRy1+15,
		 LRx1-7@LRy1+15,LRx1-6.6@LRy1+9],stroke,'SMOKESTACK'),
	circle(LRx1-5@LRy1-2,2,fill,'FRONT WHEEL'),
	circle(LRx1-23@LRy1-1,3,fill,'BACK WHEEL'), !.

car(_,rectangle,long,_,2,Roof,Load,X@Y,X+50@Y) :-
	basiclcar(X+50@Y,1,Roof),
	load0(X+50@Y,Load,1), !.
car(_,rectangle,long,_,3,Roof,Load,X@Y,X+50@Y) :-
	basiclcar(X+50@Y,1,Roof), load0(X+50@Y,Load,1),
	circle(X+20@Y-6,2,fill,'MIDDLE WHEEL'), !.
car(_,rectangle,short,not_double,2,Roof,Load,X@Y,X+34@Y) :-
	basiclcar(X+34@Y,0.6,Roof), load0(X+34@Y,Load,0.6), !.
car(_,rectangle,short,double,2,Roof,Load,X@Y,X+34@Y) :-
	basiclcar(X+34@Y,0.6,Roof), load0(X+34@Y,Load,0.6),
	openrectangle(X+1.5@Y-2.5,X+22.5@Y+7,'INNER BODY'), !.
car(_,u_shaped,Length,_,2,Roof,Load,X@Y,X+10+40*Xstretch@Y) :-
	xstretch(Length,Xstretch), Xend=X+40*Xstretch,
	comment('U_SHAPED CARRIAGE'),
	coupling(Xend+10@Y),
	line(X@Y+7,X@Y), arc(X+4@Y,4,180,270),
	line(X+4@Y-4,Xend-4@Y-4), arc(Xend-4@Y,4,270,0),
	line(Xend@Y,Xend@Y+7),
	circle(Xend-8*Xstretch@Y-6,2,fill,'FRONT WHEEL'),
	circle(X+8*Xstretch@Y-6,2,fill,'BACK WHEEL'),
	roof(Xend@Y+7,Xstretch,Roof),
	load0(Xend+10@Y,Load,Xstretch), !.
car(_,hexagon,short,_,2,_,Load,X@Y,X+34@Y) :-
	comment('HEX CARRIAGE'),
	coupling(X+34@Y),
	polygon([X+25.5@Y+1.5,X+20@Y+7,X+4@Y+7,X-1.5@Y+1.5,
		 X+4@Y-4,X+20@Y-4],stroke,'HEAXAGON'),
	load0(X+34@Y,Load,0.6),
	circle(X+20-4*0.6@Y-6,2,fill,'FRONT WHEEL'),
	circle(X+4+4*0.6@Y-6,2,fill,'BACK WHEEL'), !.
car(N,bucket,short,_,2,Roof,Load,X@Y,X+34@Y) :-
	comment('BUCKET CARRIAGE'),
	comment('LONG COUPLING'),
	line(X+34@Y,X+20.5@Y),
	((not(N=1))->line(X@Y,X+3.5@Y);true),
	openpolygon([X+24@Y+7,X+18.5@Y-4,X+5.5@Y-4,X@Y+7],'BUCKET'),
	roof(X+24@Y+7,0.6,Roof),
	load0(X+34@Y,Load,0.6),
	circle(X+16.5@Y-6,2,fill,'FRONT WHEEL'),
	circle(X+7.5@Y-6,2,fill,'BACK WHEEL'), !.
car(_,ellipse,short,_,2,_,Load,X@Y,X+34@Y) :-
	comment('ELLIPSE CARRIAGE'),
	coupling(X+34@Y),
	ellipse(X-0.285@Y-5.5,X+24.285@Y+8.5,stroke,'ELLIPSE'),
	load0(X+34@Y,Load,0.6),
	circle(X+24-7*0.6@Y-6,2,fill,'FRONT WHEEL'),
	circle(X+24-33*0.6@Y-6,2,fill,'BACK WHEEL'), !.

basiclcar(X@Y,Xstretch,Roof) :-
	comment('CARRIAGE'),
	coupling(X@Y),
	openrectangle(X-10-40*Xstretch@Y-4,X-10@Y+7,'CARRIAGE BODY'),
	circle(X-10-7*Xstretch@Y-6,2,fill,'FRONT WHEEL'),
	circle(X-10-33*Xstretch@Y-6,2,fill,'BACK WHEEL'),
	roof(X-10@Y+7,Xstretch,Roof), !.
	
coupling(X@Y) :-
	comment('COUPLING'),
	moveto(X@Y),
	lineto(X-10@Y),
	stroke, !.

openrectangle(LLx@LLy,URx@URy,Comment) :-
	openpolygon([LLx@URy,LLx@LLy,URx@LLy,URx@URy],Comment), !.

load0(_,nil,_) :- !.
load0(X@Y,l(rectangle,N),Xstretch) :- !,
	Length=((40*Xstretch-8)/N)-4,
	loads(X-10@Y-1.5,rectangle,N,Length,(40*Xstretch-Length*N)/(N+1)), !.
load0(X@Y,l(Shape,N),Xstretch) :-
	loads(X-10@Y-1.5,Shape,N,6,(40*Xstretch-6*N)/(N+1)), !.

loads(_,_,0,_,_) :- !.
loads(X@Y,Shape,N,Length,Sep) :-
	N1 is N-1,
	load1(X-Sep-Length@Y,X-Sep@Y+6,Shape),
	loads(X-Sep-Length@Y,Shape,N1,Length,Sep), !.

load1(LL,UR,circle) :-
	ellipse(LL,UR,stroke,'CIRCLE LOAD'), !.
load1(LL,UR,rectangle) :-
	rect(LL,UR,stroke,'RECTANGLE LOAD'), !.
load1(LL,UR,triangle) :-
	triangle(LL,UR,stroke,'TRIANGLE LOAD'), !.
load1(LL,UR,utriangle) :-
	utriangle(LL,UR,stroke,'UTRIANGLE LOAD'), !.
load1(LL,UR,diamond) :-
	diamond(LL,UR,stroke,'DIAMOND LOAD'), !.
load1(LL,UR,hexagon) :-
	hexagon(LL,UR,stroke,'HEXAGON LOAD'), !.

triangle(LLx@LLy,URx@URy,Fill,Comment) :-
	polygon([LLx@LLy,LLx+(URx-LLx)/2@URy,URx@LLy],Fill,Comment), !.
utriangle(LLx@LLy,URx@URy,Fill,Comment) :-
	polygon([LLx@URy,URx@URy,LLx+(URx-LLx)/2@LLy],Fill,Comment), !.
diamond(LLx@LLy,URx@URy,Fill,Comment) :-
	polygon([LLx@LLy+(URy-LLy)/2,LLx+(URx-LLx)/2@URy,
		 URx@LLy+(URy-LLy)/2,LLx+(URx-LLx)/2@LLy],Fill,Comment), !.
hexagon(LLx@LLy,URx@URy,Fill,Comment) :-
	polygon([LLx@LLy,LLx-(URx-LLx)/2.5@LLy+(URy-LLy)/2,LLx@URy,
		 URx@URy,URx+(URx-LLx)/2.5@LLy+(URy-LLy)/2,URx@LLy],
		 Fill,Comment), !.

roof(_,_,none) :- !.
roof(X@Y,Xstretch,flat) :-
	comment('FLAT ROOF'),
	moveto(X@Y),
	lineto(X-40*Xstretch@Y),
	stroke, !.
roof(X@Y,Xstretch,peaked) :-
	comment('PEAKED ROOF'),
	tan(15,T),
	moveto(X+3*Xstretch@Y-3*Xstretch*T),	% tan 15 deg ~= T
	lineto(X-20*Xstretch@Y+20*Xstretch*T),
	lineto(X-43*Xstretch@Y-3*Xstretch*T),
	stroke, !.
roof(Start,Xstretch,jagged) :-
	len1(Jags,44),
	jags(Start,Jags),
	openpolygon(Jags,'JAGGED ROOF'), !.

tan(15,0.268).

xstretch(short,0.6).
xstretch(long,1).

jags(_,[]).
jags(X@Y,[X@Y,X-0.909@Y-1,X-2.727@Y+1,X-3.636@Y|T]) :-
	jags(X-3.636@Y,T).

drawtrain(X@Y,T,X@Y-32) :- drawtrain(X@Y,T), !.

drawtrain(XY,[]) :- engine(XY,_), !.
drawtrain(XY,[c(N,Shape,Length,Double,Roof,Wheels,Load)|T]) :-
	car(N,Shape,Length,Double,Wheels,Roof,Load,XY,XY1),
	drawtrain(XY1,T), !.

% drawtrains/3 - does classification and prints at appropriate spots

drawtrains(East,West,[]) :-
	max(East,West,M),
	clause(east(T),Body),
	namevars((east(T):-Body)),
	ctol(Body,Body1),
	append([east(T),' :- '],Body1,L1),
	append(L1,['.'],L2),
	text(100@568-28*M,10,L2), !.
drawtrains(East,West,[H|T]) :-
	east(H), !, East1 is East+1,
	pwrites(['% EASTBOUND',East1]),
	text(55@598-28*East,7,[East1,'.']),
	drawtrain(75@600-28*East,H),
	drawtrains(East1,West,T), !.
drawtrains(East,West,[H|T]) :-
	West1 is West+1,
	pwrites(['% WESTBOUND',West1]),
	text(305@598-32*West,7,[West1,'.']),
	drawtrain(325@600-28*West,H),
	drawtrains(East,West1,T), !.

% drawtrains1/3 - list trains under given column point

drawtrains1(_,_,[]) :- !.
drawtrains1(X@Y,N,[H|T]) :-
	N1 is N+1,
	text(X-20@Y-2,7,[N1,'.']),
	drawtrain(X@Y,H),
	drawtrains1(X@Y-28,N1,T), !.

% drawtrains2/3 - list trains with check box

drawtrains2(_,_,[]) :- !.
drawtrains2(X@Y,N,[H|T]) :-
	N1 is N+1,
	text(X-20@Y-2,7,[N1,'.']),
	drawtrain(X@Y,H),
	LL=400@Y-4, UR=385@Y+11,
	rect(LL,UR,stroke,'CHECK BOX'),
	(ok(H) -> tick(LL,UR)
	;otherwise -> cross(LL,UR)),
	drawtrains2(X@Y-28,N1,T), !.

tick(LLx@LLy,URx@URy) :-
	moveto(LLx+2@LLy+(URy-LLy)/2),
	lineto(LLx+(URx-LLx)/2@LLy+2),
	lineto(URx+(URx-LLx)/2@URy),
	stroke, !.
cross(LLx@LLy,URx@URy) :-
	moveto(LLx+2@URy-2),
	lineto(URx-2@LLy+2), stroke,
	moveto(URx-2@URy-2),
	lineto(LLx+2@LLy+2),
	stroke, !.

max(X,Y,Y) :- X=<Y, !.
max(X,Y,X).

namevars(Term) :- namevars([Term],0,_), !.

namevars([],N,N) :- !.
namevars([Var|Terms],N1,N2) :-
	var(Var),
	namevar(N1,Var),
	namevars(Terms,N1+1,N2), !.
namevars([Term|Terms],N1,N2) :-
	Term=..[_|Subterms],
	namevars(Subterms,N1,N3),
	namevars(Terms,N3,N2), !.

namevar(N,V) :-
	Letter is N mod 26 + 65,
	Number is floor(N/26),
	((Number=0)->Numlist=[];otherwise->name(Number,Numlist)),
	name(V,[Letter|Numlist]), !.

ctol((Term1,Term2),[Term1,', '|T]) :- !, ctol(Term2,T), !.
ctol(Term,[Term]).

showexs(_,[]).
showexs(Dir,[H|T]) :-
	writes([Dir,'(',H,').',nl]),
	showexs(Dir,T), !.

split_trains([],[],[]) :- !.
split_trains([H|T],[H|E],W) :- east(H), !, split_trains(T,E,W), !.
split_trains([H|T],E,[H|W]) :- split_trains(T,E,W), !.

len1([],0).
len1([_|T],N) :-
	N1 is N-1,
	len1(T,N1).

makename(Name,List) :- flatten(List,List1), name(Name,List1), !.

flatten([],[]) :- !.
flatten([H|T],L) :-
	flatten(T,L1),
	name(H,HL),
	append(HL,L1,L), !.

min(X,Y,X) :- X=<Y, !.
min(X,Y,Y) :- Y<X, !.

make(N,Root) :-
 	makename(Ps,[Root,'.eps']),
 	makename(Es,[Root,'.pl']),
	len1(Ts,N), trains(Ts),
	split_trains(Ts,East,West),
	length(East,E), length(West,W), min(E,W,Min),
	len1(East1,Min), len1(West1,Min),
	append(East1,_,East), append(West1,_,West),
	Top is 28*Min+55,
	ptell(Ps,50@0,565@Top),
	text(75+30@Top-30,7,['TRAINS GOING EAST']),
	text(325+30@Top-30,7,['TRAINS GOING WEST']),
	pwrites(['% EASTBOUND']),
	drawtrains1(75@Top-55,0,East1),
	pwrites(['% WESTBOUND']),
	length(East1,NEast),
	drawtrains1(325@Top-55,NEast,West1),
	ptold,
	tell(Es),
	showexs(east,East1),
	showexs(west,West1),
	told.
