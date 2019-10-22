% Rui's book example

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Mode declarations
:- set(i,2)?
:- modeh(1,book(+nat,[-constant,-constant,-constant]))?
:- modeb(1,author(+nat,-constant))?
:- modeb(1,title(+nat,-constant))?
:- modeb(1,editor(+nat,-constant))?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Background knowledge

author(1,camoes).
author(2,pessoa).
author(3,vergilioFerreira).
author(4,saramago).
author(5,saramago).
author(6,eca).
author(7,eca).

title(1,lusiadas).
title(2,mensagem).
title(3,contaCorrente).
title(4,levantadoDoChao).
title(5,memorialDoConvento).
title(6,osMaias).
title(7,aReliquia).

editor(1,asa).
editor(2,asa).
editor(3,asa).
editor(4,caminho).
editor(5,caminho).
editor(6,lello).
editor(7,lello).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Positive examples

book(1,[camoes,lusiadas,asa]).
book(2,[pessoa,mensagem,asa]).
book(3,[vergilioFerreira,contaCorrente,asa]).
book(4,[saramago,levantadoDoChao,caminho]).
book(5,[saramago,memorialDoConvento,caminho]).
book(6,[eca,osMaias,lello]).
book(7,[eca,aReliquia,lello]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Negative examples

:- book(1,[camoes,lusiadas,caminho]).
:- book(1,[saramago,lusiadas,asa]).
:- book(2,[pessoa,lusiadas,asa]).
:- book(3,[vergilioFerreira,levantadoDoChao,asa]).
:- book(4,[saramago,contaCorrente,caminho]).
