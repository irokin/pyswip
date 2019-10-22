%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Domain-independent library of predicates for generating
%	postscript files. This allows drawings of polygons,
%	circles, etc.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:- op(40,yfx,'@')?	% A coordinate <X,Y> is X@Y

ptell(File) :-
	ptell(File,50@0,565@780), !.

ptell(File,LLx@LLy,URx@URy) :-
	tell(File),
	pwrites(['%!PS-Adobe-2.0 EPSF-2.0']),
	pwrites(['%%BoundingBox:',LLx,LLy,URx,URy]),
	pwrites(['%%EndComments']),
	pwrites(['%%EndProlog']),
	pwrites([0.2,setlinewidth]), !.
ptold :-
	pwrites([showpage]),
	pwrites(['%%EOF']),
	told, !.

rect(LLx@LLy,URx@URy,Fill,Comment) :-	% Fill = stroke/fill
	polygon([LLx@LLy,URx@LLy,URx@URy,LLx@URy],Fill,Comment), !.

polygon(Clist,Fill,Comment) :-
	openpolygon1(Clist,Comment),
	closefill(Fill), !.

openpolygon(Clist,Comment) :-
	openpolygon1(Clist,Comment),
	stroke, !.
	
openpolygon1([Head|Tail],Comment) :-
	comment(Comment),
	moveto(Head),
	openpolygon1(Tail), !.
openpolygon1([]) :- !.
openpolygon1([Head|Tail]) :-
	lineto(Head),
	openpolygon1(Tail).

circle(Centre,Radius,Fill,Comment) :-
	comment(Comment),
	arc(Centre,Radius,0,360),
	closefill(Fill), !.

ellipse(LLx@LLy,URx@URy,Fill,Comment) :-
	comment(Comment),
	gsave,
	translate(LLx+(URx-LLx)/2@LLy+(URy-LLy)/2),
	scale(1,(URy-LLy)/(URx-LLx)),
	arc(0@0,(URx-LLx)/2,0,360),
	closefill(Fill),
	grestore, !.

line(A,B) :- moveto(A), lineto(B), stroke, !.

comment(Comment) :-
	pwrites(['%',tab(5),Comment]).

closefill(Fill) :-
	pwrites([closepath]),
	pwrites([Fill]), !.

text(Pos,Size,Text) :-
	pwrites(['/Palatino-Roman',findfont,Size,scalefont,setfont]),
	moveto(Pos),
	writes(['(']), writes(Text),
	pwrites([')',show]), !.
moveto(X@Y) :-
	X0 is X, Y0 is Y,
	pwrites([X0,Y0,moveto]), !.
lineto(X@Y) :-
	X0 is X, Y0 is Y,
	pwrites([X0,Y0,lineto]), !.
curveto(X1@Y1,X2@Y2,X3@Y3) :-
	X11 is X1, Y11 is Y1,
	X22 is X2, Y22 is Y2,
	X33 is X3, Y33 is Y3,
	pwrites([X11,Y11,X22,Y22,X33,Y33,curveto]), !.
translate(X@Y) :-
	X0 is X, Y0 is Y,
	pwrites([X0,Y0,translate]), !.
scale(X,Y) :-
	X0 is X, Y0 is Y,
	pwrites([X0,Y0,scale]), !.
arc(X@Y,Radius,Start,End) :-
	X0 is X, Y0 is Y, Radius0 is Radius,
	Start0 is Start, End0 is End,
	pwrites([X0,Y0,Radius0,Start0,End0,arc]), !.
stroke :- pwrites([stroke]), !.
gsave :- pwrites([gsave]), !.
grestore :- pwrites([grestore]), !.

pwrites([X]) :- write(X), nl.
pwrites([H|T]) :-
	pwrite(H), write(' '),
	pwrites(T).

pwrite(nl) :- nl, !.
pwrite(tab(X)) :- tab(X), !.
pwrite(not(X)) :- write('not('), pwrite(X), write(')'), !. % DOMAIN-SPECIFIC
pwrite(existsn(Rel,Train,_,Props,N)) :-			% DOMAIN-SPECIFIC
	writes(['|',Rel,'(',Train,',Car)']),
	propswrite(Props),
	writes(['| = ',N]), !.
pwrite(X) :- write(X), !.

propswrite([]) :- !.
propswrite([Prop|Props]) :-
	writes([', ',Prop,'(Car)']),
	propswrite(Props), !.

writes([X]) :- pwrite(X).
writes([H|T]) :-
	pwrite(H),
	writes(T).
