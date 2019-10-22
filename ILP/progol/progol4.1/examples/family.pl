%%%%%%%%%%%%%%%%%%%%%%
% Family relationships example. This involves learning both
%	parent and grandfather. Owing to the fact that grandfather
%	is defined in terms of parent, the order in which these
%	predicates are learned is important.
%	Try generalising grandfather before parent.

:- set(i,2), set(h,20), set(c,2)?
:- modeh(1,parent_of(+person,-person))?
:- modeh(1,grandfather_of(+person,-person))?
:- modeh(1,grandparent_of(+person,-person))?
:- modeb(*,father_of(+person,-person))?
:- modeb(*,mother_of(+person,-person))?
:- modeb(*,parent_of(+person,-person))?
:- op(40,xfx,father_of)?
:- op(40,xfx,mother_of)?
:- op(40,xfx,grandfather_of)?
:- op(40,xfx,grandparent_of)?
:- op(40,xfx,parent_of)?
:- determination(grandfather_of/2,parent_of/2)?
:- determination(grandfather_of/2,mother_of/2)?
:- determination(grandfather_of/2,father_of/2)?
:- determination(grandparent_of/2,parent_of/2)?
:- determination(grandparent_of/2,mother_of/2)?
:- determination(grandparent_of/2,parent_of/2)?
:- determination(parent_of/2,father_of/2)?
:- determination(parent_of/2,mother_of/2)?

%%%%%%%%%%%%%%%%%%%%%%
% Type information

person(andrew).  person(bernard).  person(cathleen).  person(daphne).
person(edith).  person(fred).  person(george).  person(john).
person(louis).  person(oscar).  person(paul).  person(robert).
person(stephen).  person(sylvia).  person(william). person(ada).

%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

william parent_of sylvia.
oscar parent_of louis.
oscar parent_of daphne.
louis parent_of stephen.
louis parent_of andrew.
george parent_of oscar.
paul parent_of edith.
sylvia parent_of stephen.
sylvia parent_of andrew.
edith parent_of louis.
edith parent_of daphne.

william grandfather_of stephen.
william grandfather_of andrew.
paul grandfather_of louis.
paul grandfather_of daphne.
paul grandfather_of cathleen.
oscar grandfather_of andrew.
oscar grandfather_of robert.
george grandfather_of louis.
george grandfather_of daphne.
george grandfather_of cathleen.

william grandparent_of stephen.
william grandparent_of andrew.
paul grandparent_of louis.
paul grandparent_of daphne.
paul grandparent_of cathleen.
oscar grandparent_of andrew.
oscar grandparent_of robert.
george grandparent_of louis.
george grandparent_of daphne.
george grandparent_of cathleen.
edith grandparent_of stephen.
edith grandparent_of john.
ada grandparent_of andrew.

%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

:- X grandfather_of Y, Y grandfather_of X.
:- X grandfather_of Y, X parent_of Y.
:- edith grandfather_of stephen.
:- william grandfather_of louis.
:- paul grandfather_of stephen.
:- george grandfather_of stephen.
:- oscar grandfather_of louis.

:- X grandparent_of Y, Y grandparent_of X.
:- X grandparent_of Y, X parent_of Y.
:- william grandparent_of louis.
:- paul grandparent_of stephen.
:- george grandparent_of stephen.
:- oscar grandparent_of louis.

:- X parent_of Y, Y parent_of X.
:- john parent_of oscar.
:- sylvia parent_of oscar.
:- louis parent_of sylvia.

william father_of sylvia.
oscar father_of louis.
oscar father_of daphne.
oscar father_of cathleen.
oscar father_of fred.
oscar father_of bernard.
louis father_of stephen.
louis father_of andrew.
louis father_of robert.
louis father_of john.
george father_of oscar.
paul father_of edith.

sylvia mother_of stephen.
sylvia mother_of andrew.
sylvia mother_of robert.
sylvia mother_of john.
edith mother_of louis.
edith mother_of daphne.
edith mother_of cathleen.
edith mother_of fred.
edith mother_of bernard.
ada mother_of sylvia.
