%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Concept to be tested (can be a multiple clause theory, such as Theory 3).

% Theory 1

east(T) :- has_car(T,C), short(C), closed(C).

% Theory 2

% east(T) :- infront(T,C,D), short(C), closed(D).

% Theory 3 - Progol rarely learns both clauses from only 20 examples.

% east(T) :- has_car(T,C), jagged(C).
% east(T) :- has_car(T,C), load(C,circle,1).
