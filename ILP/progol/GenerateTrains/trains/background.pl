% Concept tester below emulates Michalski predicates.

has_car(T,C) :- member(C,T).

infront(T,C1,C2) :- append(_,[C2,C1|_],T).

ellipse(C) :- arg(2,C,ellipse).  hexagon(C) :- arg(2,C,hexagon).
rectangle(C) :- arg(2,C,rectangle).  u_shaped(C) :- arg(2,C,u_shaped).
bucket(C) :- arg(2,C,bucket).

long(C) :- arg(3,C,long).  short(C) :- arg(3,C,short).

double(C) :- arg(4,C,double).

has_roof(C,R) :- arg(5,C,R).

open(C) :- arg(5,C,none).  closed(C) :- not(open(C)).

has_wheel(C,w(NC,W)) :- arg(1,C,NC), arg(6,C,NW), nlist(1,NW,L), member(W,L).

has_load(C,Load) :- arg(7,C,l(_,NLoad)), nlist(1,NLoad,L), member(Load,L).

has_load0(C,nil) :- arg(7,C,nil).

has_load0(C,Shape) :- arg(7,C,l(Shape,N)), 1=<N.

has_load1(T,Shape) :- has_car(T,C), has_load0(C,Shape).

load(C,Shape,N) :- arg(7,C,l(Shape,N)).

wheels(C,N) :- arg(6,C,N).

none(C) :- has_roof(C,none).
flat(C) :- has_roof(C,flat).
jagged(C) :- has_roof(C,jagged).
peaked(C) :- has_roof(C,peaked).
arc(C) :- has_roof(C,arc).

member(X,[X|_]).
member(X,[_|T]) :- member(X,T).

nlist(N,N,[N]) :- !.
nlist(M,N,[M|T]) :-
	M=<N,
	M1 is M+1, nlist(M1,N,T), !.

% Metalogical predicates

existsn(Rel,Subj,N1,Plist,N) :-
	constant(Rel), !, plist(Plist),
	len1(Plist,N1),
	Call2=..[Rel,Subj,Obj],
	mkcalls(Plist,Obj,Calls),
	setof(Obj,(Call2,Calls),L),
	len1(L,N).
existsn(Rel,Subj,N1,Plist,N) :-
	setof(P,prop(P),Allp),
	rel(Rel),
	subset(Plist,Allp),
	len1(Plist,N1),
	Call2=..[Rel,Subj,Obj],
	mkcalls(Plist,Obj,Calls),
	setof(Obj,(Call2,Calls),L),
	len1(L,N).

mkcalls([Prop],Obj,Call) :-
	Call=..[Prop,Obj].
mkcalls([Prop|T],Obj,(Call,Calls)) :-
	Call=..[Prop,Obj],
	mkcalls(T,Obj,Calls).
mkcalls([],_,true).

notexistsa(Prop,Train) :-
	prop(Prop),
	Call1=..[Prop,Car],
	not(has_car(Train,Car), Call1).

existsa(Prop,Train) :-
	prop(Prop),
	Call1=..[Prop,Car],
	has_car(Train,Car), Call1.

has_wheel(Train,Wheel) :-
	train(Train),
	has_car(Train,Car),
	has_wheel(Car,Wheel).
has_load(Train,Load) :-
	train(Train),
	has_car(Train,Car),
	has_load(Car,Load).
has_roof(Train,Roof) :-
	train(Train),
	has_car(Train,Car),
	has_roof(Car,Roof).

subset([],_).
subset([H|T1],[H|T2]) :-
	subset(T1,T2).
subset(S,[_|T]) :-
	subset(S,T).

append([],L,L).
append([H|L1],L2,[H|L3]) :-
	append(L1,L2,L3).

%%%%%%%%%%%%%%%%%%%%%%%
% Meta-Types

rel(has_car).  rel(has_load1).  rel(has_wheel). rel(has_load).  rel(has_roof).

prop(short).  prop(closed).  prop(double).
prop(none).  prop(flat).  prop(jagged).  prop(peaked).  prop(arc).

plist([]).
plist([Prop|L]) :- prop(Prop), plist(L).

shape(circle).  shape(ellipse).  shape(hexagon).  shape(rectangle).
shape(triangle).  shape(utriangle).  shape(u_shaped). shape(diamond).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Types

train([c(_,_,_,_,_,_,_)|_]).
car(c(_,_,_,_,_,_,_)).

roof(r(none,_)).
roof(r(flat,_)).
roof(r(jagged,_)).
roof(r(peaked,_)).
roof(r(arc,_)).

% Generate foreground + background knowledge for trains

examples(File) :-
	tell(File),
	example, fail.
examples(_) :- told.

example :-
	east(T),
	writetrain(T,Tname,Tn,east),
	background(T,Tname,Tn).
example :-
	west(T),
	writetrain(T,Tname,Tn,west),
	background(T,Tname,Tn).

writetrain(T,Tname,Tn,Dir) :-
	tnumber(Tn),
	makename(Tname,[Dir,Tn]),
	write('%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'), nl,
	write('%'), nl,
	writedir(Dir,Tname),
	write(east(Tname)), write('.'), nl,
	write(train(Tname)), write('.'), nl.

writedir(east,Tname) :-
	write('% EASTBOUND TRAIN: '),
	write(Tname), nl, nl.
writedir(west,Tname) :-
	write('% WESTBOUND TRAIN: '),
	write(Tname), nl, nl,
	write(':- ').

background(T,Tname,Tn) :-
	has_car(T,C), carname(C,Tn,Cname),
	write(has_car(Tname,Cname)), write('.'), nl,
	write(car(Cname)), write('.'), nl,
	carprop(P,T,C), P,
	rewritep(P,Tname,Tn,Cname,P1),
	write(P1), write('.'), nl.
background(_,_,_) :- nl.

rewritep(infront(T,C1,C2),Tname,Tn,Cname1,infront(Tname,Cname1,Cname2)) :-
	carname(C2,Tn,Cname2), !.
rewritep(P,_,_,Cname,P1) :-
	P=..[Pname,_|Args],
	P1=..[Pname,Cname|Args], !.

carname(C,Tn,Cname) :-
	arg(1,C,Cn),
	makename(Cname,['car_',Tn,'_',Cn]).

tnumber(Tn) :-
	(retract(tnumber1(T0));T0=0),
	Tn is T0+1,
	asserta(tnumber1(Tn)), !.

carprop(infront(T,C,_),T,C).
carprop(long(C),_,C).
carprop(closed(C),_,C).
carprop(short(C),_,C).
carprop(open(C),_,C).
carprop(double(C),_,C).
carprop(jagged(C),_,C).
carprop(load(C,_,_),_,C).
carprop(wheels(C,_),_,C).
