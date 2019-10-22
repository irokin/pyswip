%%%%%%%%%%%%%%%%%%%%%%%%
% Learning a qualitative model of a U-tube. This is done using
%	Kuiper's QSIM (qualitative simulator). QSIM simulates
%	physical systems using naive physics. A QSIM simulator is
%	provided as background knowledge. 4 positive examples come 
%	from a QSIM trace of water in a U-tube settling from the following
%	initial state.
%
%	    la0 |_|       | |
%		| |       | |
%	      0 | |       | | 0
%		| |       | |
%		| |       |_| lb0
%		| |       | |
%	        \ \_______/ /
%                \_________/

%%%%%%%%%%%%%%%%%%%%%%%%
% Mode declarations

:- modeh(1,state(+qval,+qval,+qval,+qval))?
:- modeb(1,deriv(+qval,+qval))?
:- modeb(1,add(+qval,+qval,+qval,[]))?
:- modeb(1,minus(+qval,+qval,[]))?
:- op(30,xfy,:)?
:- op(10, xfx, ...)?
:- set(c,4), set(h,1000)?

%%%%%%%%%%%%%%%%%%%%%%%%
% Type declarations

qval(Var:Mag/Der) :-
	succ(Var,M1,M2),
	not Var = deriv,
	(Mag = M1 ; Mag = M1...M2),
	der(Der).


%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

%%%%%%%%%%%%%%%%%%%%%%%%
% State 1 (initial)
%
%	||  la0 |_|       | |
%	\/      | |       | |
%	      0 | |       | | 0
%		| |       | |       /\
%		| |       |_| lb0   ||
%		| |       | |
%	        \ \_______/ /
%                \_________/

state(la:la0/dec,lb:lb0/inc,fab:f0/dec,fba:mf0/inc).

state(la:la0...inf/dec,lb:lb0...inf/inc,fab:f0/dec,fba:mf0/inc).

%%%%%%%%%%%%%%%%%%%%%%%%
% State 2
%
%	    la0 | |       | |
%	||      |_|       | |
%	\/    0 | |       | | 0     /\
%		| |       |_|       ||
%		| |       | | lb0
%		| |       | |
%	        \ \_______/ /
%                \_________/

state(la:0...la0/dec,lb:lb0...inf/inc,fab:0...f0/dec,fba:mf0...0/inc).

%%%%%%%%%%%%%%%%%%%%%%%%
% State 3
%
%	    la0 | |       | |
%	=       |_|       | |
%	      0 | |       | | 0 
%		| |       |_|       = 
%		| |       | | lb0
%		| |       | |
%	        \ \_______/ /
%                \_________/

state(la:0...la0/std,lb:lb0...inf/std,fab:0/std,fba:0/std).

%%%%%%%%%%%%%%%%%%%%%%%%
% State 4 (final)
%
%	    la0 | |       | |
%	        | |       | |
%	=     0 |_|       |_| 0     =
%		| |       | |         
%		| |       | | lb0
%		| |       | |
%	        \ \_______/ /
%                \_________/

state(la:0/std,lb:0/std,fab:0/std,fba:0/std).



%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples
:- state(la:la0/inc,lb:lb0/inc,fab:f0/dec,fba:mf0/inc).
:- state(la:0...la0/dec,lb:lb0...inf/inc,fab:0...f0/inc,fba:mf0...0/inc).
:- state(la:0...la0/std,lb:lb0...inf/std,fab:0...f0/std,fba:0/std).
:- state(la:0...la0/std,lb:lb0...inf/std,fab:0/std,fba:mf0...0/std).
:- state(la:0/std,lb:0/dec,fab:0/std,fba:0/std).
% NEW NEGS FROM IVAN (5.5.90)
% In this example value lb:inf/std was changed to lb:minf/std 9.1.93
:- state(la:0/std,lb:minf/std,fab:0/std,fba:0/std).
% The folowing new neg. example addded 9.1.1993
:- state(la:0...la0/std,lb:lb0...inf/inc,fab:0...f0/std,fba:0/inc).
:- state(la:0...la0/dec,lb:lb0...inf/std,fab:0/dec,fba:0/inc).
% The folowing new neg. example addded 27.1.1993
:- state(la:0...la0/dec,lb:lb0...inf/inc,fab:0/dec,fba:0/inc).
% The folowing new neg. example addded 27.5.1993
:- state(la:0...la0/std,lb:lb0...inf/inc,fab:0/dec,fba:0/inc).
% The following added on 15.8.1994 - gives complete+correct+human solution
:- state(la:0...la0/dec,lb:lb0...inf/inc,fab:0...f0/dec,fba:mf0...0/std).
:- state(la:0...la0/dec,lb:lb0...inf/inc,fab:0/dec,fba:mf0...0/std).
:- state(la:0...la0/std,lb:lb0...inf/inc,fab:0...f0/dec,fba:0/inc).

%%%%%%%%%%%%%%%%%%%%%%%%%%
% Background knowledge - the QSIM simulator

der(dec).
der(std).
der(inc).

% ---------------------------------------------------------------------------
%
%                            .BOF. SI2_LOGIC
%                   Version with Alen's corrections Dec. 1990
%                   and Ivan Bratko's extensions January 1991
% ---------------------------------------------------------------------------


% ---------------------------------------------------------------------------
%
%                         DIRECTIVES AND OPERATORS
%
% ---------------------------------------------------------------------------








% ---------------------------------------------------------------------------
%
%                                TOP LEVEL
%
% ---------------------------------------------------------------------------




oscil( N) :-
%  consult(oscil),
  simulate([x:0/inc, v:v0/std, a:0/dec], N).


oscil2:-
  reconsult(oscil),
  write('Depth: '),
  read(N), nl,
  simulate([x:0/inc, v:vmax/std, a:0/dec], N).

% oscil(File): write trace on File

oscil(File) :-
  reconsult(oscil),
  write('Depth: '),
  read(N), nl,
  tell( File),
  simulatetofile([x:0/inc, v:v0/std, a:0/dec], N),
  told.

oscil2(File) :-
  reconsult(oscil),
  write('Depth: '),
  read(N), nl,
  tell( File),
  simulatetofile([x:0/inc, v:vmax/std, a:0/dec], N),
  told.

heat:-
  reconsult(heat),
  write('Depth: '),
  read(N), nl,
  simulate([ts:ts0/std, t:t0/inc, dt:dt0/dec, q:q0/dec], N).

tube( N) :-    % N is depth of simulation, N = 5 is OK
%  consult(tube),
  simulate([la:la0/dec, lb:lb0/inc, fab:f0/dec, fba: mf0/inc], N).

% tube( TubeModel): consult file with tube model and run it

tube( File) :-
  reconsult(File),
    write('Depth: '),
      read(N), nl,
	simulate([la:la0/dec, lb:lb0/inc, fab:f0/dec, fba: mf0/inc], N).
	 

tube2:-
  reconsult(tube2),
    write('Depth: '),
      read(N), nl,
	simulate([la:la0/dec, lb:lb0/inc, fab:f0/dec, fba: mf0/inc], N).

tube3:-
  reconsult(tube3),
    write('Depth: '),
      read(N), nl,
	simulate([la:la0/inc, lb:lb0/dec, fab:f0/inc, fba: mf0/dec], N).

tube4 :-
  reconsult(tube4),
    write('Depth: '),
      read(N), nl,
	simulate([la:la0/dec, lb:lb0/inc, fab:f0/dec, fba: mf0/inc], N).
	 

tube_all  :-   % Simulation runs from all legal initial states
  u_tube( LA,LB,FAB,FBA),   % A legal u-tube state
  simulate( [LA,LB,FAB,FBA], 6),
  nl, write('End of run'), nl,nl,
  fail
  ;
  write('End of all possible u-tube traces').

leak( N) :-
%  consult(leak),
  simulate([la:la0/dec, lb:lb0/inc, dab:dab0/dec,
            fab:fab0/dec, fba:fba0/inc, fxb:fbx0/dec, fbt : fbt0/dec], N).


polepos  :-
  reconsult( polepos),
  write( 'Depth: '),
  read( N), nl,
  simulate( [
    x:x0/std, dx:0/inc, ddx:0...inf/inc,
    th:th0/std, dth:0/dec, ddth:minf...0/dec,
    f:posf/std, m1th:0...inf/std, m2th:0...inf/std, ath:0...athmax/std, fath:0...inf/std], N).




% ---------------------------------------------------------------------------
%
%                                SIMULATOR
%
% ---------------------------------------------------------------------------

% simulate(InitialState, Depth)

simulate(S, N):-
  write(point:S), nl,
  simulate(point:S, 1, N).

simulate(_, _).



simulate(S, M, N):-
  M =< N,
  noninfinity(S),
  M1 is M+1,
  transition(S, S1),
  tab(M), write(S1), nl,
  simulate(S1, M1, N).


simulatetofile(S, N):-
  nl, reformat( S, SS), write( SS), write('.'),
  simulatetofile(point:S, 1, N).

simulatetofile(_, _).



simulatetofile(S, M, N):-
  M =< N,
  noninfinity(S),
  M1 is M+1,
  transition(S, S1),
  S1 = _:SS1, reformat( SS1, SSS1),
  tab(M), write(SSS1), write('.'), nl,
  simulatetofile(S1, M1, N).

reformat( VarList, StateTerm)  :-
  StateTerm =.. [state | VarList].


% ---------------------------------------------------------------------------
%
%                                KERNEL
%
% ---------------------------------------------------------------------------

%  Constraints
%
%    range( Func:Mag/Der, RangeMag/RangeDer) :
%      Mag is inside RangeMag, Der is inside RangeDer,
%      where RangeMag = LowestMag...HighestMag,
%            Der = LowestDer...HighestDer
%      Eg:  range( x:Mag/Der, x0...x0/std...std)   x is constant x0
%
%
%    deriv( F1:M1/D1, F2:M2/D2) :
%      d/dt(F1) =q F2
%
%
%    add( F1:M1/D1, F2:M2/D2, F3:M3/D3, CorrespValList) :
%      F1 + F2 =q F3  with corr. values in CorrValList
%      CorrValList = [ c(Val1,Val2,Val3), c(...), ...]
%
%
%    mult( F1:M1/D1, F2:M2/D2, F3:M3/D3, CorrValList)
%      F1 * F2 =q F3  with corr values in CorrValList
%
%
%    minus( F1:M1/D1, F2:M2/D2, CorrList)
%      F1 =q -F2  with corr. values in CorrList
%      CorrList  =  [ c(Val1, Val2), c(...), ...]
%
%
%   m_plus( F1:M1/D1, F2:M2/D2, CorrList) :
%      F1 is monotonically inc. function of F2 with corr. values in CorrList
%
%
%    m_minus( F1:M1/D1, F2:M2/D2, CorrList) :
%      F1 is monotonically dec. function of F2 with corr. values in CorrList
%
%
%    mA( F1:M1/D1, F2:M2/D2, CorrList) :
%      F2 is an A-monotonic inc. - dec. function of F1 with CorrList
%
%
%    mV( F1:M1/D1, F2:M2/D2, CorrList) :
%      F2 is a V-monotonic dec. - inc. function of F1 with CorrList


range(F:M/D, Rm/Rd):-
  inside_closed(F, M, Rm),
  inside_closed(deriv, D, Rd).

deriv(_:_/D1, F2:M2/_):-
  verify_range_deriv(D1, F2, M2).

add(F1:M1/D1, F2:M2/D2, F3:M3/D3, C):-
% If M1 is not 0 and M2 is a non-interval then M2 and M3 are diff.
  not M1 = 0, not M2 = L1...L2, M2 = M3, !, fail.
add(F1:M1/D1, F2:M2/D2, F3:M3/D3, C):-
  not M2 = 0, not M1 = L1...L2, M1 = M3, !, fail.

add(F1:M1/D1, F2:M2/D2, F3:M3/D3, C):-
  verify_add_inf_consistence(M1, M2, M3),
  verify_add_mag(F1, F2, F3, M1, M2, M3, C),
  verify_add_der(D1, D2, D3).

mult(F1:M1/D1, F2:M2/D2, F3:M3/D3, C):-
  verify_mult_zeroinf_consistence(M1, M2, M3),
  norm_mag(F1, M1, 0, X1),
  norm_mag(F2, M2, 0, X2),
  norm_mag(F3, M3, 0, X3),
  lookup_mult_sign_table(X1, X2, X3),
  verify_mult_mag_and_der(F1, F2, F3, M1, M2, M3, D1, D2, D3, C).

minus(F1:M1/D1, F2:M2/D2, C):-
  verify_minus_zeroinf_consistence(M1, M2),
  norm_mag(F1, M1, 0, X1),
  norm_mag(F2, M2, 0, X2),
  lookup_minus_sign_table(X1, X2),
  verify_minus_mag(F1, F2, M1, M2, C),
  verify_minus_der(D1, D2).

m_plus(F1:Mag1/Der1, F2:Mag2/Der2, C):-
  verify_m_plus_limit( F1, F2, Mag1, Mag2, C),
  m_plus_deriv(Der1, Der2),
  !.

m_minus(F1:Mag1/Der1, F2:Mag2/Der2, C):-
  verify_m_minus_limit(F1,F2,Mag1, Mag2, C),
  m_minus_deriv(Der1, Der2),
  !.

% mA( F1:Mag1/Der1, F2:Mag2/Der2, Corr) :
%   the A-monotonic constr, that is:
%   if Mag1 < 0 then m_plus else m_minus

mA( F1:Mag1/Der1, FQ2, Corr)  :-
  conc( CorrNeg, [ c(0,M) | CorrPos], Corr),     % Divide corr. vals. in neg. and pos.
  (
  less_than( F1, Mag1, 0),       % F1 negative
  m_plus( F1:Mag1/Der1, FQ2, CorrNeg)
  ;
  leseq_than( F1, 0, Mag1),        % F1 positive
  m_minus( F1:Mag1/Der1, FQ2, [ c(0,M) | CorrPos])
  ).


% mV( F1:Mag1/Der1, F2:Mag2/Der2, Corr) :
%   the V-monotonic constr., that is:
%   if Mag1 < 0 then m_minus else m_plus

mV( F1:Mag1/Der1, FQ2, Corr)  :- 
 conc( CorrNeg, [ c(0,M) | CorrPos], Corr),     % Divide corr. vals. in neg. and pos. 
 (
 less_than( F1, Mag1, 0),         % F1 negative
 m_minus( F1:Mag1/Der1, FQ2, CorrNeg)
 ;
 leseq_than( F1, 0, Mag1),        % F1 positive
 m_plus( F1:Mag1/Der1, FQ2, [ c(0,M) | CorrPos])
 ).
	      

% time_descriptor(Old_Td, New_Td)

time_descriptor(point, interval).
time_descriptor(interval, point).



% single_transition(Td, F:M0/D0, TransitionId, F:M/D)

single_transition(point, F:Mag/std,         p1,  F:Mag/std):-
  not Mag = _..._ .

single_transition(point, F:Mag/std,         p2,  F:Mag...Mag1/inc):-
  succ(F, Mag, Mag1).

single_transition(point, F:Mag/std,         p3,  F:Mag1...Mag/dec):-
  succ(F, Mag1, Mag).

single_transition(point, F:Mag/inc,         p4,  F:Mag...Mag1/inc):-
  succ(F, Mag, Mag1).

single_transition(point, F:Mag1...Mag2/inc, p5,  F:Mag1...Mag2/inc).

single_transition(point, F:Mag/dec,         p6,  F:Mag1...Mag/dec):-
  succ(F, Mag1, Mag).

single_transition(point, F:Mag1...Mag2/dec, p7,  F:Mag1...Mag2/dec).

single_transition(point, F:Mag1...Mag2/std, p8,  F:Mag1...Mag2/dec).

single_transition(point, F:Mag1...Mag2/std, p9,  F:Mag1...Mag2/std).

single_transition(point, F:Mag1...Mag2/std, p10, F:Mag1...Mag2/inc).


single_transition(interval, F:Mag/std,         i1, F:Mag/std).

single_transition(interval, F:_...Mag2/inc,    i2, F:Mag2/std).

single_transition(interval, F:_...Mag2/inc,    i3, F:Mag2/inc).

single_transition(interval, F:Mag1...Mag2/inc, i4, F:Mag1...Mag2/inc).

single_transition(interval, F:Mag1..._/dec,    i5, F:Mag1/std).

single_transition(interval, F:Mag1..._/dec,    i6, F:Mag1/dec).

single_transition(interval, F:Mag1...Mag2/dec, i7, F:Mag1...Mag2/dec).

single_transition(interval, F:Mag1...Mag2/inc, i8, F:Mag1...Mag2/std).

single_transition(interval, F:Mag1...Mag2/dec, i9, F:Mag1...Mag2/std).



% verify_range_deriv(QDer, Func, Mag):-

verify_range_deriv(dec, F, M):-
  less_than(F, M, 0).
verify_range_deriv(std, _, 0).
verify_range_deriv(inc, F, M):-
  less_than(F, 0, M).



% verify_add_inf_consistence(Mag1, Mag2, Mag3)

verify_add_inf_consistence(minf, inf, _).
verify_add_inf_consistence(inf, minf, _).
verify_add_inf_consistence(A, inf, X) :-
  not A = minf,
  X = inf.
verify_add_inf_consistence(A, minf, X) :-
  not A = inf,
  X = minf.
verify_add_inf_consistence(inf, A, X) :-
  not A = minf,
  X = inf.
verify_add_inf_consistence(minf, A, X) :-
  not A = inf,
  X = minf.
verify_add_inf_consistence(A, B, C) :-
  not A = inf,
  not A = minf,
  not B = inf,
  not B = minf,
  not C = inf,
  not C = minf.



% verify_add_mag(Func1, Func2, Func3, Mag1, Mag2, Mag3, Corr)
%   - check add-magnitude consistency.

verify_add_mag(F1, F2, F3, M1, M2, M3, []):-
  norm_mag(F1, M1, 0, A1),
  norm_mag(F2, M2, 0, A2),
  norm_mag(F3, M3, 0, A3),
  lookup_consist_table(A1, A2, A3).

verify_add_mag(F1, F2, F3, M1, M2, M3, [c(C1,C2,C3) |Corr]):-
  norm_mag(F1, M1, C1, A1),
  norm_mag(F2, M2, C2, A2),
  norm_mag(F3, M3, C3, A3),
  lookup_consist_table(A1, A2, A3),
  verify_add_mag(F1, F2, F3, M1, M2, M3, Corr).



% norm_mag(Func, Val1, Val2, Norm_val)  - Norm_val = sgn(Val1-Val2).

norm_mag(_, V, V, zero).

norm_mag(F, V1, V2, neg):-
  not V1 = V2,
  less_than(F, V1, V2).

norm_mag(F, V1, V2, pos):-
  not V1 = V2,
  less_than(F, V2, V1).



% lookup_consist_table(A1, A2, A3)  - add/mult-value-consistency table.

lookup_consist_table(neg,  neg,  neg ).
lookup_consist_table(neg,  zero, neg ).
lookup_consist_table(neg,  pos,  neg ).
lookup_consist_table(neg,  pos,  zero).
lookup_consist_table(neg,  pos,  pos ).
lookup_consist_table(zero, neg,  neg ).
lookup_consist_table(zero, zero, zero).
lookup_consist_table(zero, pos,  pos ).
lookup_consist_table(pos,  neg,  neg ).
lookup_consist_table(pos,  neg,  zero).
lookup_consist_table(pos,  neg,  pos ).
lookup_consist_table(pos,  zero, pos ).
lookup_consist_table(pos,  pos,  pos ).



% verify_add_der(Der1, Der2, Der3)  - chech add-deriv consistency.

verify_add_der(Der1, Der2, Der3):-
  norm_deriv(Der1, D1),
  norm_deriv(Der2, D2),
  norm_deriv(Der3, D3),
  lookup_consist_table(D1, D2, D3).



% norm_deriv(Deriv, Norm_deriv)  - normalise deriv.

norm_deriv(inc, pos ).
norm_deriv(std, zero).
norm_deriv(dec, neg ).



% verify_mult_mag_and_der(Func1, Func2, Func3,
%                         Mag1, Mag2, Mag3, Der1, Der2, Der3, Corr)
%   - check mult-magnitude/derivate consistency.

verify_mult_mag_and_der(F1, F2, _, M1, M2, _, D1, D2, D3, []):-
  norm_deriv(D1, Nd1),
  norm_deriv(D2, Nd2),
  norm_deriv(D3, Nd3),
  norm_mag(F1, M1, 0, Nm1),
  norm_mag(F2, M2, 0, Nm2),
  lookup_mult_sign_table(Nd1, Nm2, X1),
  lookup_mult_sign_table(Nd2, Nm1, X2),
  lookup_consist_table(X1, X2, Nd3).

verify_mult_mag_and_der(F1, F2, F3, M1, M2, M3,
                        D1, D2, D3, [c(C1,C2,C3) |Corr]):-
        norm_mag(F1, C1, 0, Nc1),
        norm_mag(F1, M1, C1, A1),
        norm_mag_sign_correction(A1, Nc1, Cnc1),
        norm_mag(F2, C2, 0, Nc2),
        norm_mag(F2, M2, C2, A2),
        norm_mag_sign_correction(A2, Nc2, Cnc2),
        norm_mag(F3, C3, 0, Nc3),
        norm_mag(F3, M3, C3, A3),
        norm_mag_sign_correction(A3, Nc3, Cnc3),
        lookup_consist_table(Cnc1, Cnc2, Cnc3),
        verify_mult_mag_and_der(F1, F2, F3, M1, M2, M3, D1, D2, D3, Corr).



% norm_mag_sign_correction(A, B, New_a)
%   - sign correction for mult-mag-normalisation.

norm_mag_sign_correction(pos, neg, neg).
norm_mag_sign_correction(neg, neg, pos).
norm_mag_sign_correction(A,   B,   A  ):-
  not B = neg.



% verify_mult_zeroinf_consistence(Mag1, Mag2, Mag3)

verify_mult_zeroinf_consistence(inf, inf, inf).
verify_mult_zeroinf_consistence(inf, minf, minf).
verify_mult_zeroinf_consistence(minf, inf, minf).
verify_mult_zeroinf_consistence(minf, minf, inf).

verify_mult_zeroinf_consistence(0, inf, _).
verify_mult_zeroinf_consistence(0, minf, _).
verify_mult_zeroinf_consistence(inf, 0, _).
verify_mult_zeroinf_consistence(minf, 0, _).

verify_mult_zeroinf_consistence(minf, A, X):-
  not A = inf,
  not A = 0,
  not A = minf,
  (
    X=minf
    ;
    X = inf
  ).
verify_mult_zeroinf_consistence(inf, A, X):-
  not A = inf,
  not A = 0,
  not A = minf,
  (
    X=minf
    ;
    X = inf
  ).
verify_mult_zeroinf_consistence(A, minf, X):-
  not A = inf,
  not A = 0,
  not A = minf,
  (
    X=minf
    ;
    X = inf
  ).
verify_mult_zeroinf_consistence(A, inf, X):-
  not A = inf,
  not A = 0,
  not A = minf,
  (
    X=minf
    ;
    X = inf
  ).

verify_mult_zeroinf_consistence(0, A, 0):-
  not A = inf,
  not A = minf.
verify_mult_zeroinf_consistence(A, 0, 0):-
  not A = inf,
  not A = 0,
  not A = minf.

verify_mult_zeroinf_consistence(A, B, C):-
  not A = inf,
  not A = 0,
  not A = minf,
  not B = inf,
  not B = 0,
  not B = minf,
  not C = inf,
  not C = 0,
  not C = minf.



% lookup_mult_sign_table(V1, V2, V3)  - mult-sign-consistency.

lookup_mult_sign_table(zero, _  ,  zero).
lookup_mult_sign_table(X   , zero, zero):-
  not X = zero.
lookup_mult_sign_table(pos,  pos,  pos ).
lookup_mult_sign_table(pos,  neg,  neg ).
lookup_mult_sign_table(neg,  pos,  neg ).
lookup_mult_sign_table(neg,  neg,  pos ).



% verify_minus_zeroinf_consistence(Mag1, Mag2)

verify_minus_zeroinf_consistence(0, 0).
verify_minus_zeroinf_consistence(inf, minf).
verify_minus_zeroinf_consistence(minf, inf).
verify_minus_zeroinf_consistence(A, B):-
  not A = inf,
  not A = 0,
  not A = minf,
  not B = inf,
  not B = 0,
  not B = minf.



% verify_minus_mag(Func1, Func2, Mag1, Mag2, Corr)
%   - check minus-magnitude consistency.

verify_minus_mag(_, _, _, _, []).

verify_minus_mag(F1, F2, M1, M2, [c(C1,C2) |Corr]):-
  norm_mag(F1, M1, C1, Norm),
  norm_mag(F2, C2, M2, Norm),
  verify_minus_mag(F1, F2, M1, M2, Corr).



% verify_minus_der(Der1, Der2)
%   - check minus-derivative consistency.

verify_minus_der(D1, D2):-
  lookup_minus_der_table(D1, D2).



% lookup_minus_sign_table(X1, X2)  - minus-sign consistency.

lookup_minus_sign_table(pos,  neg).
lookup_minus_sign_table(zero, zero).
lookup_minus_sign_table(neg,  pos).



% lookup_minus_der_table(X1, X2)  - minus-der consistency.

lookup_minus_der_table(dec, inc).
lookup_minus_der_table(std, std).
lookup_minus_der_table(inc, dec).



% verify_m_plus_limit(Func1, Func2, Mag1, Mag2, Corr_list)  - 
%   check monotone increasing limit approach.

verify_m_plus_limit(_, _, _, _, []).

verify_m_plus_limit( F1, F2,M1, M2, [c(A,B) |Corr]):-
  norm_mag( F1, M1, A, Nm),
  norm_mag( F2, M2, B, Nm),
  verify_m_plus_limit( F1, F2, M1, M2, Corr).

% verify_m_minus_limit( Func1, Func2, Mag1, Mag2, CorrList)
%   check monotone decreasing approach

verify_m_minus_limit(_, _, _, _, []).

verify_m_minus_limit( F1, F2, M1, M2, [c(A,B) |Corr]):-
  norm_mag( F1, M1, A, Nm), 
  norm_mag( F2, B, M2, Nm), 
  verify_m_minus_limit( F1, F2, M1, M2, Corr).
       

% The following (verify_m_limit) is probabbly no longer needed!!!

verify_m_limit(M1, M2, [c(A,B) |Corr]):-
  not M1 = A,
  not M2 = B,
  verify_m_limit(M1, M2, Corr).



% m_plus_deriv(Mag1, Mag2)
%   - verify m_plus deriv condition

m_plus_deriv(D, D).


% m_minus_deriv(Mag1, Mag2)
%   - verify m_minus deriv condition

m_minus_deriv(D1, D2):-
  lookup_minus_der_table(D1, D2).



% less_than(Func, Val1, Val2)  - Val1 < Val2 for Functionn.

less_than(_, _...A, A..._).
less_than(F, _...A2, B1..._):-
  less_than(F, A2, B1).

less_than(_, A, A..._):-
  not A = _..._.
less_than(F, A, B1..._):-
  not A = _..._,
  less_than(F, A, B1).

less_than(_, _...B, B):-
  not B = _..._.
less_than(F, _...A2, B):-
  not B = _..._,
  less_than(F, A2, B).

less_than(F, A, B):-
  not A = _..._,
  not B = _..._,
  succ(F, A, B).
less_than(F, A, B):-
  not A = _..._,
  not B = _..._,
  succ(F, A, C),
  less_than(F, C, B).



leseq_than(_, A, A).

leseq_than(F, A, B):-
  less_than(F, A, B).



% inside _closed(Func, Val, Range)
%   - is Val in the closed interval Range for Function.

inside_closed(F, V1...V2, R1...R2):-
  leseq_than(F, R1, V1),
  less_than(F, V1, V2),
  leseq_than(F, V2, R2).

inside_closed(_, V, R...R):-
  not V = _..._,
  V = R.

inside_closed(F, V, R1...R2):-
  not V = _..._,
  not R1 = R2,
  leseq_than(F, R1, V),
  leseq_than(F, V, R2).

inside_closed(_, V, V):-
  not V = _..._.



noninfinity(_:S):-
  finite_values(S).



finite_values([]).

finite_values([_:A/_ |S]):-
  not A = inf,
  not A = minf,
  finite_values(S).



global(_:S):-
  changed_state(S).



changed_state([i1:_ |S]):-
  changed_state(S).

changed_state([i4:_ |S]):-
  changed_state(S).

changed_state([i7:_ |S]):-
  changed_state(S).

changed_state([Id:_ |_]):-
  not Id = i1,
  not Id = i4,
  not Id = i7.

conc( [], L, L).

conc( [X | L1], L2, [X | L3])  :-
  conc( L1, L2, L3).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Tube definitions

% ---------------------------------------------------------------------------
%
%                                U-TUBE
%
% ---------------------------------------------------------------------------

succ(deriv, dec, std).
succ(deriv, std, inc).

succ(la, minf, 0).
succ(la, 0, la0).
succ(la, la0, inf).

succ(lb, minf, 0).
succ(lb, 0, lb0).
succ(lb, lb0, inf).

succ(fab, minf, 0).
succ(fab, 0, f0).
succ(fab, f0, inf).

succ(fba, minf, mf0).
succ(fba, mf0, 0).
succ(fba, 0, inf).



% Par = Fun:Mag/Der
transition(Td0:[LA0,LB0,FAB0,FBA0], Td:[LA,LB,FAB,FBA]):-
  time_descriptor(Td0, Td),
  single_transition(Td0, LB0, IdLB, LB),
  single_transition(Td0, FAB0, IdFAB, FAB),
  single_transition(Td0, LA0, IdLA, LA),
  add(LB, FAB, LA, [c(lb0,f0,la0)]),
  single_transition(Td0, FBA0, IdFBA, FBA),
  minus(FBA, FAB, [c(mf0,f0)]),
  deriv(LA, FBA),
  deriv(LB, FAB),
  global(Td:[IdLA:LA,IdLB:LB,IdFAB:FAB,IdFBA:FBA]).

% Old model
% state(A,B,C,D) :- deriv(A,D), deriv(B,C), add(B,C,A,[]).
% New model
% state(A,B,C,D) :- deriv(A,D), deriv(D,C), add(B,C,A,[]).

% state(A,B,C,D) :- deriv(A,D), deriv(B,B), add(A,D,C,[]), minus(A,D,[]).
