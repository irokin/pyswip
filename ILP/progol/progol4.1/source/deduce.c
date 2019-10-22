#include <stdio.h>
#include "progol.h"

/*
 * #######################################################################
 *
 *			Clause deduction Routines
 *                      -------------------------
 *
 * #######################################################################
 */

/* d_pushfores - copy and skolemise all clauses of given predicate
 *	symbol. Body atoms asserted, head atoms placed in fores array.
 *	Each head atom numbered with clause number. This allows
 *	virtual clause retraction before calling. Array of numbers
 *	returned gives numbers of skolemised unit clauses to be
 *	retracted from each predicate definition by d_popfores.
 */

ITEM
d_pushfores(psym)
	LONG psym;
	{
	ITEM retract=Y_EMPTY,bcl,cclause,clause,head,pseen=B_EMPTY,
		nums=L_EMPTY;
	LONG cno,ano=0l;
	if(fores||bfores||catoms) d_error("d_pushfores - fores already set");
	fores=F_EMPTY; bfores=B_EMPTY; catoms=F_EMPTY;
	if(bcl= *f_ins(psym,bptab)) {
	  BIT_DO(cno,bcl)
	    cclause=i_dec(b_elem(cno,spcls));
	    clause=i_copy(F_ELEM(0l,cclause));
	    head=l_pop(cl_skolem(clause,TRUE));
	    cl_assatoms(clause,retract,pseen,cno,nums);
		/* Assert Skolemised body */
	    head->extra=cno;
	    b_add(ano,bfores);
	    *f_ins(ano++,fores)=head;
	    i_delete(clause);
	  BIT_END
	  BIT_DO(cno,bcl)
	    cclause=i_dec(b_elem(cno,spcls));
	    cl_leprop(cclause->extra,i_sort(nums));
	  BIT_END
	  cl_psfirstarg(pseen);
	}
	i_deletes(pseen,nums,(ITEM)I_TERM);
	return(retract);
}

d_popfores(retract)
	ITEM retract;
	{
	cl_retatoms(retract);
	i_deletes(fores,bfores,(ITEM)I_TERM);
	bfores=fores=(ITEM)NULL;
}

/* d_tredundant - returns bitset of logically redundant clauses
 *	having psym as head predicate symbol
 */

ITEM
d_tredundant(psym)
	LONG psym;
	{
	ITEM retract,brem;
	retract=d_pushfores(psym);
	brem=d_tredundant1();
	d_popfores(retract);
	i_delete(retract);
	return(brem);
}

ITEM
d_tredundant1()
	{
	register ITEM brem=B_EMPTY,atom,*entry;
	register LONG cno,ano;
	BIT_DO(ano,bfores)
	  b_rem(cno=(atom=F_ELEM(ano,fores))->extra,bclauses);
	  catoms->extra=cno;
	  if(*(entry=f_ins(cno,catoms))) b_uni(bclauses,*entry);
	  if(d_groundcall(atom)) b_add(cno,brem);
	  else b_add(cno,bclauses);
	  if(*entry) b_sub(bclauses,*entry);
	BIT_END
	b_uni(bclauses,brem);
	return(brem);
}

/* d_treduce - find redundant clauses in defn of psym and retract them.
 */

d_treduce(psym)
	LONG psym;
	{
	ITEM brem=d_tredundant(psym),defs=F_ELEM(psym,ptab);
	if(!b_emptyq(brem)) {
	  g_message(1l,"%d redundant clauses retracted",b_size(brem));
	  b_lsub(defs,brem);
	  b_sub(bclauses,brem);
	  b_sub(F_ELEM(psym,bptab),brem);
	}
	cl_pfirstarg(psym);
	i_delete(brem);
}

PREDICATE
d_groundcall(atom)
	ITEM atom;
	{
	ITEM *a;
	PREDICATE result;
	*(a=&(HOF(TOF((LIST)I_GET(F_ELEM(0l,gcall))))))=atom;
	result=interp_quest(gcall);
	*a=(ITEM)NULL;
	return(result);
}

/* d_gcpush - prepares call for cover testing.
 */

ITEM
d_gcpush(clause)
	ITEM clause;
	{
	ITEM result,head,eq;
	result=i_tup4(clause,i_dec(I_INT(cl_vmax(clause))),NULL,NULL);
	head=l_pop(clause);
	eq=i_tup3(i_dec(i_create('h',peq2)),NULL,i_dec(head));
	l_push(NULL,l_push(i_dec(eq),clause));
	return(result);
}

/* d_gcpop - undoes preparation of clause for cover testing.
 */

d_gcpop(clause)
	ITEM clause;
	{
	ITEM eq;
	l_pop(clause);
	eq=l_pop(clause);
	l_push(F_ELEM(2l,eq),clause);
	i_delete(eq);
}

/* cl_push - adds clause to end of definition table.
 */

LIST *
cl_push(clause)
	ITEM clause;
	{
	ITEM *entry,cclause;
	LONG psym=PSYM(HOF((LIST)I_GET(clause))),
		*cno= (LONG *)&(I_GET(F_ELEM(2l,spcls)));
	LIST *end;
	cclause=i_tup4(clause,i_dec(I_INT(cl_vmax(clause))),
		i_dec(i_create('h',(POINTER)pdot0)),NULL);
	if(!(*(entry=f_ins(psym,ptab)))) *entry=L_EMPTY;
	end=(LIST *)(*entry)->extra;
	l_suf(i_dec(cclause),*entry);
	cclause->extra=(*cno)++;
	cl_pfirstarg(psym);
	return(end);
}

/* cl_pop - remove last clause from definition table.
 */

int
cl_pop(end)
	LIST *end;
	{
	ITEM cclause=(ITEM)L_GET(*end);
	LONG psym=PSYM(HOF((LIST)I_GET(F_ELEM(0l,cclause)))),
		*cno= (LONG *)&(I_GET(F_ELEM(2l,spcls))),cno1=cclause->extra;
	ITEM *entry;
	if(!(*(entry=f_ins(psym,ptab))))
	  d_error("cl_pop - bad definition");
	i_delete(cclause);
	L_DEL(*end);
	*end=(LIST)NULL;
	(*entry)->extra=(ULONG)end;
	if(cno1==*cno+1l) --(*cno);
	cl_pfirstarg(psym);
}

/* cl_pcoverage - counts the number of clauses made redundant by the
 *	assertion of the new clause.
 */

#define ATMP(call) (&(F_ELEM(1l,HOF(TOF((LIST)I_GET(F_ELEM(0l,call)))))))

LONG
cl_pcoverage(call)
	ITEM call;
	{
	LONG cno,ano,result=0l;
	ITEM *atom=ATMP(call),*entry;
	BIT_DO(ano,bfores)
	  cno=(*atom=F_ELEM(ano,fores))->extra;
	  catoms->extra=cno;
	  if(*(entry=f_ins(cno,catoms))) b_uni(bclauses,*entry);
	  if(interp_quest(call)) result++;
	  if(*entry) b_sub(bclauses,*entry);
	BIT_END
	*atom=(ITEM)NULL;
	return(result);
}

/* cl_ncoverage - returns the number of headless clauses which succeed.
 */

LONG
cl_ncoverage(negq,call,ccl)
	PREDICATE negq;
	ITEM call,ccl;
	{
	LONG cnt=0l,cno;
	ITEM *entry,ccl1,cl,*atom=ATMP(call);
	if(negq) {
	  if(interp_quest(ccl)) cnt++;
	}
	else if(*(entry=f_ins(pfalse0,ptab))) {
	  LIST_DO(ccl1,*entry)
	    if(b_memq(ccl1->extra,bclauses)) {
	      if(l_length(cl=F_ELEM(0l,ccl1))==2) { /* Cover test atoms */
	        *atom=HOF(TOF((LIST)I_GET(cl)));
	        if(interp_quest(call)) cnt++;
	      }
	      else {				/* Assert clause and test */
		hyp1=ccl;			/* hypothesis being tested */
	        b_add(cno=ccl->extra,bclauses);
	        if(interp_quest(ccl1)) cnt++;
		hyp1=(ITEM)NULL;		/* hypothesis being tested */
	        b_rem(cno,bclauses);
	      }
	    }
	  LIST_END
	}
	*atom=(ITEM)NULL;
	return(cnt);
}

/* d_creduce - destructively reduces clause.
 */

ITEM
d_creduce(cclause)
	ITEM cclause;
	{
	ITEM clause0=F_ELEM(0l,cclause),clause,head0,retract=Y_EMPTY,atom,
		pseen=B_EMPTY,entry=(ITEM)NULL,term,bvars;
	register LIST *elemp;
	LONG cno=(LONG)I_GET(F_ELEM(2l,spcls));
	ct_bvars(head0=l_pop(clause0),bvars=B_EMPTY);
	LIST_DO(atom,clause0) /* Skolemise variables from head in body */
	  TERM_DO(term,atom)
	    if(term->item_type== 'v'&&b_memq((LONG)I_GET(term),bvars))
	      term->item_type= 'k';
	  TERM_END
	LIST_END
	cl_skolem(clause=i_copy(clause0),TRUE);
	cl_assatoms(clause,retract,pseen,cno,NULL); /* Assert Skolemised body */
	cl_psfirstarg(pseen);
	if(*f_ins(cno,catoms))  {
	  entry=b_uni(b_copy(*f_ins(cno,catoms)),bclauses);
	  i_swap(bclauses,entry);
	}
	l_push((ITEM)NULL,clause0);
	elemp= &(TOF((LIST)I_GET(clause0)));
	while(*elemp) {
	  atom=L_GET(*elemp);
	  b_rem(cno,bclauses);
	  if(interp_quest(cclause)) L_REM(elemp)
	  else {
	    b_add(cno,bclauses);
	    elemp = &((*elemp)->next);
	  }
	  cno++;
	}
	if(entry) i_swap(bclauses,entry);
	l_pop(clause0);
	l_push(head0,cl_skolem(clause0,FALSE));
	cl_retatoms(retract);
	i_deletes(entry,clause,retract,head0,pseen,bvars,(ITEM)I_TERM);
	return(cclause);
}
