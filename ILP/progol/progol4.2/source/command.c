#include <stdio.h>
#include "progol.h"

/*
 * #######################################################################
 *
 *			Command Routines
 *                      ----------------
 *
 * #######################################################################
 */

main_prompt()
	{
	ITEM c;
	char mess[MAXMESS],mess1[MAXMESS];
	double start;
	for (;;) {
		printf("|- ");
		if((c=ccl_ttyread())==(ITEM)I_TERM) break;
		else if(c==(ITEM)I_ERROR) continue;
		ccl_swrite(mess,c);
		start=cputime();
		c_interp(c);
		sprintf(mess1,"%s - Time taken %.2fs",mess,cputime()-start);
		g_message(1l,mess1);
		i_delete(c);
	}
}

extern char ttychline();
extern PREDICATE interp();
PREDICATE c_sat();

c_interp(cclause)
	ITEM cclause;
	{
	ITEM vtable;
	ITEM *fptr,*entry,cclause1;
	FUNC f;
	PREDICATE succeeded,first;
	LONG type,psym;
	if(cclause==(ITEM)I_ERROR)
	  return;
	if((type=CTYPE(cclause))==pdot0) {	/* Assertion */
	  d_creduce(cclause);
	  printf("<Reduced clause is ");
	  ccl_fwrite(ttyout,cclause,NULL);
	  printf(">\n");
	  if(cl_assert(cclause,FALSE,TRUE,TRUE,FALSE,(ITEM)NULL)) {
	    printf("[<"); ccl_fwrite(ttyout,cclause,NULL);
	    printf("> added to clauses]\n");
	    CONTRA
	  }
	  return;
	}
	else if((type)==pcut) {			/* Example */
	  CTYPE(cclause)=pdot0;
	  cl_assert(cclause,FALSE,TRUE,TRUE,FALSE,(ITEM)NULL);
	  CONTRA
	  psym=PSYM(HOF((LIST)I_GET(F_ELEM(0l,cclause))));
	  c_sat(cclause1=i_copy(cclause),bcl_costs(*f_ins(psym,bptab)));
	  i_delete(cclause1);
	  return;
	}
	vtable=F_ELEM(3l,cclause);		/* Query */
	f=(FUNC)I_GET(HOF((LIST)I_GET(F_ELEM(0l,cclause))));
	succeeded=interp(cclause,TRUE,TRUE);
	while (succeeded) {
		if(!(f->arr_size-1l)) {
		  interp((ITEM)NULL,TRUE,TRUE);
		  break;
		}
		first=TRUE;
		ARG_LOOP(fptr,f) {
		  if(!(*(entry=f_ins((LONG)I_GET(*fptr),vtable))))
		    d_error("c_interp - bad variable table");
		  if(STREQ((STRING)I_GET(*entry),"_")) continue;
		  if(!first) printf("\n");
		  else first=FALSE;
		  printf("%s = ",(STRING)I_GET(*entry));
		  p_fwritesub(ttyout,*fptr,(BIND)term_stack0);
		}
		printf(" ");
		if(ttychline()!= ';') {
		  interp((ITEM)NULL,TRUE,TRUE);
		  break;
		}
		succeeded=interp((ITEM)I_TERM,TRUE,TRUE);
	}
	if(succeeded) printf("yes\n");
	else printf("no\n");
}

/* interp_quest - interprets a question. Succeeds at most once.
 */

PREDICATE
interp_quest(cclause)
	ITEM cclause;
	{
	PREDICATE result;
	if(result=interp(cclause,TRUE,FALSE))
	  interp((ITEM)NULL,TRUE,FALSE);	/* Reset interpreter */
	return(result);
}

/* c_sat - saturates clause, does top-down search, asserts result
 *	result if compressive and does theory reduction.
 */

extern ITEM ct_sat(),cl_vrenum(),r_outlook(),r_vdomains(),r_posonly();

PREDICATE
c_sat(cclause,nex)
	ITEM cclause;
	LONG nex;
	{
	ITEM atoio=F_EMPTY,otoa=F_EMPTY,head=(ITEM)NULL,hypothesis,*entry,
		retract,clause=F_ELEM(0l,cclause),outlook,cclause1,vdomains;
	LONG psym,varno;
	double oldnoise,fnex=nex;
	PREDICATE pos=FALSE;
	if(hypothesis=ct_sat(cclause,atoio,otoa,&head)) {
	  cl_symreduce(&hypothesis,atoio,head);
	  outlook=r_outlook(hypothesis,head,otoa,atoio);
	  vdomains=r_vdomains(otoa,atoio);
	  if(verbose>=2) {
	    g_message(2l,"Most specific clause is");
	    printf("\n");
	    cl_print(hypothesis);
	    printf("\n");
	  }
	  if(SETQ(searching)) {
	    retract=d_pushfores(psym=PSYM(HOF((LIST)I_GET(clause))));
	    if(r_posonly()) {
	      g_message(2l,"Learning %s/%d from positive examples",
		QP_ntos(psym),QP_ntoa(psym));
	      pos=TRUE;
	      oldnoise=noiselim;
	      noiselim=100.0;
	      c_updsamp(psym);
	    }
	    fnex=fnex*inflate/100.0;
	    r_search(&hypothesis,atoio,otoa,outlook,vdomains,fnex);
	    if(pos) noiselim=oldnoise;
	    if(hypothesis&& !L_EMPTYQ(hypothesis)) {
	      cl_unflatten(&hypothesis);
	      if(verbose>=1) {
	        g_message(1l,"Result of search is");
		printf("\n");
	        cl_print(hypothesis);
		printf("\n");
	      }
	    }
	    else {
	      printf("[No compression]\n\n");
	    }
	    d_popfores(retract);
	    if(hypothesis&& !L_EMPTYQ(hypothesis)) {
	      /*
	      printf("Assert <");
	      cl_fwrite(ttyout,hypothesis,NULL); printf(">? ");
	      if(ttychline()=='y') {
	      */
	        cl_vrenum(hypothesis,&varno);
	        cclause1=i_tup4(hypothesis,i_dec(I_INT(varno)),
			  idot0,i_dec(F_EMPTY));
		if(pos&&cover) c_updbsamp(psym,cclause1);
	        cl_assert(cclause1,TRUE,TRUE,TRUE,FALSE,(ITEM)NULL);
	        i_delete(cclause1);
	        d_treduce(psym);
	      /*
	      } */
	    }
	    i_delete(retract);
	  }
	  i_deletes(hypothesis,outlook,vdomains,(ITEM)I_TERM);
	}
	i_deletes(atoio,otoa,head,(ITEM)I_TERM);
	return(TRUE);
}

/* c_updsamp - conditions stochastic labels on positive examples and
 *	updates sample ground atoms of psym for learning
 *	from positive examples.
 */

c_updsamp(psym)
	LONG psym;
	{
	ITEM *entry,*entry1,atom,bcls;
	LONG psym1,cost=0l,cno,cnt=0l;
	if(!(*(entry=f_ins(psym,psamps)))) {
	  if(!(psym1=(LONG)*y_ins(psym,ptog)))
	    d_error("c_updsamp - predicate has no generator predicate");
	  if(condition) c_condition(psym,psym1);
	  bcls=F_ELEM(psym,bptab);
	  BIT_DO(cno,bcls)
	    cost+= *y_ins(cno,costs);
	  BIT_END
	  *entry=d_sample(psym1,4l*cost);
	  LIST_DO(atom,*entry)
	    if(atom->item_type=='f') I_GET(F_ELEM(0l,atom))=(POINTER)psym;
	    else I_GET(atom)=(POINTER)psym;
	    atom->extra=cnt++;
	  LIST_END
	}
	if(!cover) c_updbsamp(psym,(ITEM)NULL);
	else if(!(*(entry=f_ins(psym,bpsamps)))) *entry=B_EMPTY;
	/* c_updbsamp(psym); */
}

/* c_updbsamp - compute subsample which is not yet provable from
 *	theory so far. This makes calculation of clause generality more
 *	efficient. Cover testing is done if call arg is NULL.
 */

c_updbsamp(psym,call)
	LONG psym;
	ITEM call;
	{
	ITEM *entry,*entry1,atom;
	ITEM c1,call1,*atom1;
	if(call) {
	  call1=d_gcpush(c1=i_copy(F_ELEM(0l,call)));
	  atom1=ATMP(call1);
	}
	entry=f_ins(psym,psamps);
	if(!(*(entry1=f_ins(psym,bpsamps)))) *entry1=B_EMPTY;
	LIST_DO(atom,*entry)
	  if(!b_memq(atom->extra,*entry1)) {
	    if(call) {
	      *atom1=atom;
	      if(interp_quest(call1)) b_add(atom->extra,*entry1);
	    }
	    else if(d_groundcall(atom)) b_add(atom->extra,*entry1);
	  }
	LIST_END
	if(call) {
	  *atom1=(ITEM)NULL;
	  i_deletes(c1,call1,(ITEM)I_TERM);
	}
}

/* c_condition - uses examples to condition stochastic logic program
 *	associated with a particular predicate.
 */

c_condition(psym,psym1)
	LONG psym,psym1;
	{
	ITEM ccl,cl,*entry,head,pred;
	if(*(entry=f_ins(psym,ptab))) {
	  if(!(psym1=(LONG)*y_ins(psym,ptog))) {
	    d_error("c_condition - predicate has no generator predicate\n");
	  }
	  conditioning=TRUE;
	  LIST_DO(ccl,*entry)
	    if(l_length(cl=F_ELEM(0l,ccl))==1l) {
	      if((head=HOF((LIST)I_GET(cl)))->item_type== 'h') pred=head;
	      else pred=F_ELEM(0l,head);
	      (LONG)I_GET(pred)=psym1;
	      l_push((ITEM)NULL,cl);
	      interp_quest(ccl);
	      l_pop(cl);
	      (LONG)I_GET(pred)=psym;
	    }
	  LIST_END
	  conditioning=FALSE;
	}
}

PREDICATE
c_doall(froot)
	ITEM froot;
	{
	STRING name;
	char mess[MAXMESS];
	LONG psym=0l;
	ITEM *hmode;
	double start;
	SET(searching);
	if(froot->item_type== 'h')
		name=QP_ntos((LONG)I_GET(froot));
	else return(FALSE);
	if(STRSUFQ(name,".pl")) sprintf(mess,"%s",name);
	else sprintf(mess,"%s.pl",name);
	if(cl_readrls(mess)) {
		/* Generalise each predicate in hmodes */
	  FUNC_DO(hmode,hmodes)
	    if(*hmode) {
	      start=cputime();
	      c_gen1(psym);
	      sprintf(mess,"Time taken %.3fs",cputime()-start);
	      g_message(1l,mess);
	    }
	    psym++;
	  FUNC_END
	}
	return(TRUE);
}
