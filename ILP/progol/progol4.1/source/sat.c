/* ####################################################################### */

/*                      PROGOL saturation construction routines	   	   */
/*                      ---------------------------------------		   */

/* ####################################################################### */

#include        <stdio.h>
#include        "progol.h"


/* ct_sat, Constructs saturated clause by calling interpreter to construct
 *	individual atoms.
 */

ITEM ct_tsubs(),cl_skolem();
int cl_retatoms();

ITEM
ct_sat(cclause,atoio,otoa,head)
	ITEM cclause,atoio,otoa,*head;
	{
	PREDICATE grnd;
	ITEM htab=h_create(6l),termtypes=F_EMPTY,terms,detms,*entry,
		bodyatoms,body,newatoms,newterms,pseen=B_EMPTY,nums=L_EMPTY,
		vars,retract,atom,clause=(ITEM)F_ELEM(0l,cclause),modes,
		oldterms=L_EMPTY;
	LONG psym,i,nextras,vno=cl_vmax(clause),maxct=0l,
		cno=(LONG)I_GET(F_ELEM(2l,spcls)),ano=0l,
		nvs=(LONG)I_GET(F_ELEM(1l,cclause));
	PREDICATE negq=(PSYM(HOF((LIST)I_GET(clause)))==pfalse0);
	if(negq) {modes=bmodes; i_delete(l_pop(clause));}
	else modes=hmodes;
	if(nvs) cl_skolem(clause,TRUE);
	atom=l_pop(clause); /* Skolemise */
	cl_assatoms(clause,retract=Y_EMPTY,pseen,cno,nums);
	cl_leprop(cno,i_sort(nums));
		/* Assert Skolemised body */
	cl_psfirstarg(pseen);
	catoms->extra=cno;
	if(*(entry=f_ins(cno,catoms))) b_uni(bclauses,*entry);
	psym=PSYM(atom);
	body=L_EMPTY;
	if(!(terms=ct_tsubs(atom,htab,head,&vno,&maxct,modes))) {
	  l_push(i_dec(atom),clause);
	  if(negq) l_push(fhead,clause);
	  if(nvs) cl_skolem(clause,FALSE); /* Deskolemise */
	  cl_retatoms(retract);	/* Retract Skolemised body */
	  printf("[No mode declaration for example ");
	  ccl_fwrite(ttyout,cclause,NULL); printf("]\n");
	  i_deletes(retract,body,htab,termtypes,pseen,nums,
		oldterms,(ITEM)I_TERM);
	  return((ITEM)NULL);
	}
	ct_type1(terms,termtypes);
	detms=i_inc((*(entry=f_ins(psym,determs))? *entry:allpsyms));
	bodyatoms=l_push(*head,L_EMPTY);
	for(i=1l,nextras=1l;nextras && i<=vlim;i++) {
	  newatoms=L_EMPTY; newterms=L_EMPTY;
	  ct_smkatoms(detms,termtypes,htab,newatoms,newterms,&vno,&ano,
		atoio,otoa,maxct,*head,oldterms);
	  set_sub(i_sort(newatoms),bodyatoms);
	  nextras=l_length(newatoms);
	  l_append(body,newatoms);
	  set_uni(bodyatoms,newatoms);
	  set_sub(i_sort(newterms),terms);
	  ct_type1(newterms,termtypes);
	  i_delete(oldterms); oldterms=l_copy(terms);
	  set_uni(terms,newterms);
	  i_deletes(newatoms,newterms,(ITEM)I_TERM);
	}
	l_push(*head,body);
	l_push(i_dec(atom),clause);
	if(negq) {l_push(fhead,clause); l_push(fhead,body);}
	if(nvs) cl_skolem(clause,FALSE); /* Deskolemise */
	cl_retatoms(retract);	/* Retract Skolemised body */
	i_deletes(bodyatoms,htab,termtypes,retract,(ITEM)I_TERM);
	i_deletes(terms,detms,pseen,nums,oldterms,(ITEM)I_TERM);
	return(body);
}

/* ct_type1 - checks the types of all terms and saves them in termtypes
 */


ct_type1(terms,termtypes)
	ITEM terms,termtypes;
	{
	ITEM *entry,term,args=L_EMPTY,atom,clause,call;
	LONG type;
	atom=i_tup2(i_dec(i_create('h',(POINTER)0l)),
			i_dec(i_create('v',(POINTER)0l)));
	clause=l_push((ITEM)NULL,l_push(atom,L_EMPTY));
	call=i_tup4(clause,i_dec(I_INT(1l)),idot0,NULL);
	BIT_DO(type,types)
	  if(!(*(entry=f_ins(type,termtypes))))
	    *entry=L_EMPTY;
	  I_GET(F_ELEM(0l,atom))=(POINTER)type;
	  LIST_DO(term,terms)
	    l_push(term,args);
	    if(ct_skolq(term) || ct_call(call,1l,args,NULL,NULL,NULL))
		l_push(term,*entry);
	    i_dec(l_pop(args));
	  LIST_END
	  i_sort(*entry);
	BIT_END
	i_deletes(args,atom,clause,call,(ITEM)I_TERM);
}

/* ct_tsubs - extracts all subterms from the clause head. These are
 *	inserted into the term->variable mapping (htab).
 *	The clause head contains only distinct variables.
 */

ITEM
ct_tsubs(atom,htab,head,vno,maxct,modes)
	ITEM atom,htab,*head,modes;
	LONG *vno,*maxct;
	{
	ITEM term,term1,*term2,term3,terms,*modelist,callio,call,call1,
		*entry,matom,eatom;
	LONG cost;
	BIND sub;
	PREDICATE found;
	ARG_DO(term2,atom)	/* Set maxct */
	  if((cost=ct_cost(*term2))> *maxct) *maxct=cost;
	ARG_END
	if(*(modelist=f_ins(PSYM(atom),modes)))
	  LIST_DO(callio,*modelist) /* Find first matching head mode */
	    matom=HOF(TOF((LIST)I_GET(F_ELEM(0l,call1=F_ELEM(0l,callio)))));
	    eatom=i_tup3(i_dec(i_create('h',peq2)),matom,atom);
	    call=i_tup2(i_dec(l_push(NULL,l_push(eatom,L_EMPTY))),
		  F_ELEM(1l,call1));
	    found=interp(call,TRUE,FALSE);
	    i_deletes(eatom,call,(ITEM)I_TERM);
	    if(found) { /* Match found */
	      interp((ITEM)NULL,TRUE,FALSE); /* Reset interpreter */
	      terms=L_EMPTY;
	      sub=(BIND)term_stack0;
	      *head=i_copy(matom);
	      TERM_DO(term,*head)
		if(term->item_type== 'v') {
		  term1=(sub+((LONG)I_GET(term)))->term;
		  if(IS(CONST,term->extra)) {
		    term3=i_copy(term1);
		    i_swap(term,term3);
		    i_delete(term3);
		  }
		  else {
		    if(!(*(entry=h_ins(term1,htab))))
		      *entry=i_create('v',(*vno)++);
		    I_GET(term)=I_GET(*entry);
		    if(!IS(OUT,term->extra)) l_suf(term1,terms);
		  }
		}
	      TERM_END
	      return(i_sort(terms));
	    }
	  LIST_END
	return((ITEM)NULL);
}

ct_smkatoms(detms,termtypes,htab,atoms,terms,vno,ano,atoio,otoa,maxct,head,
		oldterms)
	ITEM detms,termtypes,htab,atoms,terms,atoio,otoa,head,oldterms;
	LONG *vno,*ano,maxct;
	{
	LONG psym,det;
	ITEM callio,*modelist,ins=L_EMPTY,call,outs=B_EMPTY,consts=B_EMPTY;
	LIST iolist;
	BIT_DO(psym,detms)
	  if(*(modelist=f_ins(psym,bmodes)))
	    LIST_DO(callio,*modelist)
	      call=F_ELEM(0l,callio);
	      iolist=(LIST)I_GET(F_ELEM(1l,callio));
	      det=(LONG)I_GET(F_ELEM(2l,callio));
	      ct_smkatoms1(call,iolist,det,0l,0l,ins,outs,consts,termtypes,
		htab,atoms,terms,vno,ano,atoio,otoa,maxct,head,oldterms,TRUE,
		ct_nins(iolist));
	    LIST_END
	BIT_END
	i_deletes(ins,outs,consts,(ITEM)I_TERM);
}

ct_smkatoms1(call,iolist,det,n,m,ins,outs,consts,termtypes,htab,newatoms,
		newterms,vno,ano,atoio,otoa,maxct,head,oldterms,new,nins)
	ITEM call,ins,outs,consts,termtypes,htab,newatoms,newterms,
		atoio,otoa,oldterms;
	LIST iolist;
	LONG n,m,*vno,*ano,det,maxct,nins;
	PREDICATE new;
	{
	LONG tno,argn,type;
	LIST elem,insp,outsp;
	ITEM *terms,term,head1,otermss,oterms,callatom,
		newatom,bin,bout,htab1,terms0;
	PREDICATE skip,constq;
	if(!iolist) {
	  otermss=(ITEM)NULL;
	  l_reverse(ins);
	  if(!ct_call(call,det,ins,(ITEM)NULL,outs,&otermss)) {
	    l_reverse(ins);
	    i_delete(otermss);
	    return;
	  }
	  callatom=HOF(TOF((LIST)I_GET(F_ELEM(0l,call))));
	  LIST_DO(oterms,otermss)
	    skip=FALSE; tno=0l;
	    LIST_DO(term,oterms)	/* Filter bad bindings */
	      if(!term||(!(constq=b_memq(tno++,consts))&&reductive&&
			ct_cost(term)>maxct)||(constq&&ct_skolq(term)))
		{skip=TRUE; break;} /* No terms more complex than head */
	    LIST_END
	    if(skip) continue;
	    newatom=i_dec(i_copy(callatom));
	    *f_ins(newatom->extra=(*ano)++,atoio)=
		    i_tup3(newatom,i_dec(bin=B_EMPTY),i_dec(bout=B_EMPTY));
	    insp=(LIST)I_GET(ins); outsp=(LIST)I_GET(oterms);
	    ct_smkatom(&newatom,&insp,&outsp,htab,htab1=h_create(3l),
		vno,newatom,bin,bout,otoa);
	    i_delete(htab1);
	    if(ITMEQ(newatom,head)) continue;
	    l_suf(newatom,newatoms); tno=0l;
	    LIST_DO(term,oterms)
	      if(!b_memq(tno++,consts)) l_suf(term,newterms);
	    LIST_END
	  LIST_END
	  l_reverse(ins);
	  i_delete(otermss);
	}
	else {
	  if(PSYM(head1=HOF(iolist))==pplus1) {
	    type=QP_ston(QP_ntos((LONG)I_GET(F_ELEM(1l,head1))),1l);
	    if(*(terms=f_ins(type,termtypes))) {
	      if(new) {
		set_sub(terms0=l_copy(*terms),oldterms);
	        LIST_DO(term,terms0)
	          l_push(term,ins);
	          ct_smkatoms1(call,TOF(iolist),det,n+1l,m,ins,outs,
			consts,termtypes,htab,newatoms,newterms,vno,ano,
			atoio,otoa,maxct,head,oldterms,FALSE,nins-1l);
	          i_delete(l_pop(ins));
	        LIST_END
		i_delete(terms0);
		if(nins>1l) {
		  set_int(terms0=l_copy(*terms),oldterms);
	          LIST_DO(term,terms0)
	            l_push(term,ins);
	            ct_smkatoms1(call,TOF(iolist),det,n+1l,m,ins,outs,
			consts,termtypes,htab,newatoms,newterms,vno,ano,
			atoio,otoa,maxct,head,oldterms,TRUE,nins-1l);
	            i_delete(l_pop(ins));
	          LIST_END
		  i_delete(terms0);
		}
	      }
	      else {
	        LIST_DO(term,*terms)
	          l_push(term,ins);
	          ct_smkatoms1(call,TOF(iolist),det,n+1l,m,ins,outs,
			consts,termtypes,htab,newatoms,newterms,vno,ano,
			atoio,otoa,maxct,head,oldterms,new,nins-1l);
	          i_delete(l_pop(ins));
	        LIST_END
	      }
	    }
	  }
	  else {
	    b_add(n,outs);
	    l_push((ITEM)NULL,ins);
	    if(PSYM(head1)==phash1) b_add(m,consts);
	    ct_smkatoms1(call,TOF(iolist),det,n+1l,m+1l,ins,outs,consts,
		termtypes,htab,newatoms,newterms,vno,ano,atoio,otoa,
		maxct,head,oldterms,new,nins);
	    if(PSYM(head1)==phash1) b_rem(m,consts);
	    l_pop(ins);
	    b_rem(n,outs);
	  }
	}
}

ITEM ct_renumv();

ct_smkatom(term,ins,oterms,htab,htab1,vno,newatom,bin,bout,otoa)
	ITEM *term,htab,htab1,newatom,bin,bout,otoa;
	LIST *ins,*oterms;
	LONG *vno;
	{
	ITEM *subterm,subterm1,*entry;
	PREDICATE in;
	LONG vno1;
	switch((*term)->item_type) {
	  case 'f':
	    ARG_DO(subterm,*term)
	      ct_smkatom(subterm,ins,oterms,htab,htab1,vno,newatom,
			bin,bout,otoa);
	    ARG_END
	    break;
	  case 'v':
	    in=FALSE;
	    if(!(subterm1=HOF(*ins))) { /* Output term */
	      subterm1=HOF(*oterms);
	      *oterms=(*oterms)->next;
	    }
	    else in=TRUE;
	    *ins=(*ins)->next;
	    if(IS(CONST,(*term)->extra)) {	/* Constant term */
	      i_delete(*term);
	      *term=i_inc(ct_renumv(subterm1,htab1,vno));
	    }
	    else {
	      if(!(*(entry=h_ins(subterm1,htab)))) /* New variable */
	        *entry=i_create('v',(POINTER)(*vno)++);
	      vno1=(LONG)I_GET(*entry);
	      I_GET(*term)=(POINTER)vno1;
	      if(in) 	/* Input variable */
	        b_add(vno1,bin);
	      else {	/* Output variable */
	        b_add(vno1,bout);
	        if(!(*(entry=f_ins(vno1,otoa)))) *entry=B_EMPTY;
	        b_add(newatom->extra,*entry);
	      }
	    }
	    break;
	  default:
	    break;
	}
}

/*
 * ct_renumv - renumber the variables in a term using vno as
 *	the starting point for new names.
 */

ITEM
ct_renumv(term,htab,vno)
	ITEM term,htab;
	LONG *vno;
	{
	ITEM subterm,subterm1,*entry;
	TERM_DO(subterm,term)
	  if(subterm->item_type=='v') {
	    subterm1=i_copy(subterm);
	    if(!(*(entry=h_ins(subterm1,htab))))
	      *entry=i_create('v',(POINTER)(*vno)++);
	    I_GET(subterm)=(POINTER)I_GET(*entry);
	    i_delete(subterm1);
	  }
	TERM_END
	return(term);
}

/* ct_call - does up to n recalls of a given call
 */
PREDICATE ct_groundq();

PREDICATE
ct_call(call,n,args,retatom,outs,otermss)
	ITEM call,args,*retatom,outs,*otermss;
	LONG n;
	{
	PREDICATE result=FALSE;
	ITEM arg,atom,oterms,term;
	BIND sub=(BIND)term_stack0;
	LONG out,m;
	LIST_DO(arg,args)
	  sub->subst=(BIND)NULL;
	  (sub++)->term=arg;
	LIST_END
	if(outs) *otermss=L_EMPTY;
	while(n-- >0l && interp(call,FALSE,TRUE)) {
	  result=TRUE;
	  if(retatom)
	    *retatom=p_copy(HOF(TOF((LIST)I_GET(F_ELEM(0l,call)))),
		(BIND)term_stack0,FALSE);
	  if(outs) {
	    sub=(BIND)term_stack0;
	    l_suf(i_dec(oterms=L_EMPTY),*otermss);
	    BIT_DO(m,outs)
	      if(term=((sub+m)->term))
		if(!ct_groundq(term=i_dec(p_copy((sub+m)->term,
			(sub+m)->subst,FALSE)))) term=(ITEM)NULL;
	      if(!term) printf("[Warning: non-ground term derived]\n");
	      l_suf(term,oterms);
	    BIT_END
	  }
	}
	if(n<0l) interp((ITEM)NULL,FALSE,TRUE);	/* Reset interpreter */
	return(result);
}

/* cl_skolem - iterates through terms in clause and switches variables to skolem
 *	constants if skolem==TRUE and vice versa otherwise. Altered
 *	variables are added to the vars list.
 */

ITEM
cl_skolem(clause,skolem)
	ITEM clause;
	PREDICATE skolem;
	{
	ITEM atom,term;
	LONG *vno;
	LIST_DO(atom,clause)
	  TERM_DO(term,atom)
	    switch(term->item_type) {
	      case 'v': if(skolem) {
			  ON(SKOL,term->extra);
			  term->item_type= 'k';
			}
			break;
	      case 'k': if(!skolem) {
			  OFF(SKOL,term->extra);
			  term->item_type= 'v';
			}
			break;
	      default:  break;
	    }
	  TERM_END
	LIST_END
	return(clause);
}

/* ct_skolq - tests whether term contains skolem constants. If so,
 *	skolem flag is set.
 */

PREDICATE
ct_skolq(term)
	ITEM term;
	{
	PREDICATE result=FALSE;
	ITEM subt;
	if(IS(SKOL,term->extra)) return(TRUE);
	TERM_DO(subt,term)
	  if(IS(SKOL,subt->extra)) {
	    result=TRUE; break;
	  }
	TERM_END
	if(result) ON(SKOL,term->extra);
	return(result);
}

/* ct_groundq - tests whether term is ground.
 */

PREDICATE
ct_groundq(term)
	ITEM term;
	{
	register ITEM subt;
	TERM_DO(subt,term)
	  if(subt->item_type== 'v') return(FALSE);
	TERM_END
	return(TRUE);
}

/* cl_assatoms- Assert Skolemised body as unit clauses into rulebase
 */

/*
LTEQ AND LT TABLES SHOULD BE CONSTRUCTED HERE FOR EACH CCNO. NEW
GLOBAL ARRAYS NEEDED ANALOGOUS TO CATOMS STRUCTURE. DEVISE UNIQUE
NUMBER FOR EACH OBJECT TO BE COMPARED. MARK ITEMS WITH THIS NUMBER.
ALSO MARK LE,LT,GE,GT UNIT CLAUSE PSYMS WITH CLAUSE ORIGIN NUMBER. THIS WILL
BE USED BY L_LTEQ. COMPARISON TABLE IS BUILT BY NOTING EACH
LE,LT,GE,GT COMPARISON IN THE PASS THROUGH THE BODY. ALSO SAVE
NUMBERS IN A LIST. AT END OF PASS SORT NUMBERS AND NOTE ADJACENT
VALUE COMPARISONS IN COMPARISON TABLE. LASTLY PROPOGATE ALL LE
AND LT VALUES THROUGH TABLE.
LES = PURE LE CHAINS.
LTES = MIXED LE/LT CHAINS.
LTS = LTES-LES.
*/

#define	LTEMARK(a1,a2,tab) \
	{ITEM arg1,arg2; \
	 if(!(*(entry=f_ins(b_num(arg1=F_ELEM(a1,atom),sple),*tab)))) \
		b_add(arg1->extra,*entry=B_EMPTY); \
	 b_add(b_num(arg2=F_ELEM(a2,atom),sple),*entry); \
	 if(!(*(entry=f_ins(arg2->extra,*tab)))) \
		b_add(arg2->extra,*entry=B_EMPTY); \
	 if(arg1->item_type== 'i' || arg1->item_type== 'r') l_suf(arg1,nums); \
	 if(arg2->item_type== 'i' || arg2->item_type== 'r') l_suf(arg2,nums); \
	}

cl_assatoms(body,retract,pseen,ccno,nums)
	ITEM body,retract,pseen,nums;
	LONG ccno;
	{
	LONG psym,*cno= (LONG *)&(I_GET(F_ELEM(2l,spcls)));
	ITEM atom,*entry,clause,*ccs,*tle,*tlt;
	if(!catoms) catoms=F_EMPTY;
	catoms->extra=0l;
	if(!(*(ccs=f_ins(ccno,catoms)))) *ccs=B_EMPTY;
	if(nums) {
	  if(!cle) cle=F_EMPTY;
	  if(!(*(tle=f_ins(ccno,cle)))) *tle=F_EMPTY;
	  if(!clt) clt=F_EMPTY;
	  if(!(*(tlt=f_ins(ccno,clt)))) *tlt=F_EMPTY;
	}
	LIST_DO(atom,body)
	  psym=PSYM(atom);
	  if(!(*(entry=f_ins(psym,ptab)))) *entry=L_EMPTY;
	  clause=i_tup4(i_dec(l_push(atom,L_EMPTY)),i_dec(I_INT(0l)),NULL,NULL);
	  l_push(i_dec(clause),*entry);
	  (*y_ins(psym,retract))++;
	  b_add(psym,pseen);
	  b_add(clause->extra=(*cno)++,*ccs);
	  if(nums) {
	    if(psym==ple2) {LTEMARK(1l,2l,tle) LTEMARK(1l,2l,tlt)}
	    else if(psym==plt2) LTEMARK(1l,2l,tlt)
	    else if(psym==pge2) {LTEMARK(2l,1l,tle) LTEMARK(2l,1l,tlt)}
	    else if(psym==pgt2) LTEMARK(2l,1l,tlt)
	  }
	LIST_END
}

/* cl_retatoms - retract the given number of atoms from every non-zero
 *	predicate in the retract array. Also remove all asserted atoms
 *	from bclauses.
 */

cl_retatoms(retract)
	ITEM retract;
	{
	unsigned long int *n;
	LONG psym=0l,*cno;
	PREDICATE thisone=FALSE;
	cno= (LONG *)&(I_GET(F_ELEM(2l,spcls)));
	Y_DO(n,retract)
	  if(*n) thisone=TRUE;
	  for(;*n;(*n)--) {
	    i_delete(l_pop(F_ELEM(psym,ptab)));
	    b_rem(--(*cno),bclauses);
	  }
	  if(thisone) cl_pfirstarg(psym);
	  psym++;
	  thisone=FALSE;
	Y_END
	i_deletes(catoms,cle,clt,(ITEM)I_TERM);
	catoms=cle=clt=(ITEM)NULL;
}

/* cl_leprop - propogates less than or equal relationship over
 *	skolemised variables and numbers. tle and tlt contain
 *	respectively chains of only le and mixed le/lt. The lt
 *	relationship is constructed by subtracting only le chains
 *	from mixed le/lt chains. The mixed le/lt chains then form
 *	the le relation.
 */

cl_leprop(cno,nums)
	LONG cno;
	ITEM nums;
	{
	register LIST elem;
	ITEM *entry,*blt,ble1,*ble,*tle,*tlt;
	LONG n;
	tle=f_ins(cno,cle); tlt=f_ins(cno,clt);
	if(!(*tle)&& !(*tlt)) return;
	if(!(*tle)) *tle=F_EMPTY;
	if(!(*tlt)) *tlt=F_EMPTY;
	if(!L_EMPTYQ(nums)) {
	  PENL_LOOP(elem,(LIST)I_GET(nums)) {	/* Chain numbers */
	    if(!(*(entry=f_ins(L_GET(elem)->extra,*tlt)))) *entry=B_EMPTY;
	    b_add(L_GET(elem)->extra,b_add(L_GET(elem->next)->extra,*entry));
	  }
	  if(!(*(entry=f_ins(L_GET(elem)->extra,*tlt)))) *entry=B_EMPTY;
	  b_add(L_GET(elem)->extra,*entry);
	}
	cl_leprop1(*tle); cl_leprop1(*tlt);
	n=0l;
	FUNC_DO(blt,*tlt)	/* Separate pure le and mixed le/lt chains */
	  if(*blt) {
	    ble1=b_copy(*blt);
	    if(*(ble=f_ins(n,*tle)))
	      {b_sub(*blt,*ble); i_delete(*ble);}
	    *ble=ble1;
	  }
	  n++;
	FUNC_END
}

cl_leprop1(tl)
	ITEM tl;
	{
	ITEM *bl,bl1,bnew,bl2;
	register LONG m;
	if(!tl) return;
	FUNC_DO(bl,tl) /* For each object construct closure */ 
	  if(*bl) {
	    bnew=b_copy(*bl); bl1=B_EMPTY;
	    while(!b_emptyq(bnew)) {
	      BIT_DO(m,bnew)
	        if(bl2= *f_ins(m,tl)) b_uni(bl1,bl2);
	      BIT_END
	      b_sub(b_uni(bnew,bl1),*bl);
	      b_uni(*bl,bl1);
	    }
	    i_deletes(bnew,bl1,(ITEM)I_TERM);
	  }
	BIT_END
}

/*
 * ct_terms the set of all sub-terms in a given term. Flags
 *	are passed for VARIABLES_ONLY and ATOM.
 */

LIST *ct_terms1();

ITEM
ct_terms(term,varsonly,atom)
	ITEM term;
	PREDICATE varsonly,atom;
	{
	ITEM result=i_create('l',(POINTER)NULL);
	LIST *last=(LIST *)&(I_GET(result));
	ct_terms1(term,last,varsonly,atom);
	return(i_sort(result));
}

LIST *
ct_terms1(term,last,varsonly,atom)
	ITEM term;
	register LIST *last;
	PREDICATE varsonly,atom;
	{
	switch(term->item_type) {
	    case 'f': {
		FUNC f=(FUNC)I_GET(term);
		register ITEM *fptr;
		ARG_LOOP(fptr,f) last=ct_terms1(*fptr,last,varsonly,FALSE);
		}
		break;
	    case 'v':
		if (varsonly) last=l_end(i_inc(term),last);
		break;
	    case 'h': case 'i': case 'r':
		break;
	    default:
		d_error("ct_terms1 - bad term type");
	}
	if (!varsonly && !atom) last=l_end(i_inc(term),last);
	return(last);
}

/* cl_vars(cl) - collect the set of variables in the clause cl
 */

ITEM
cl_vars(cl)
	ITEM cl;
	{
	register ITEM result,literal;
	register LIST *last=L_LAST(result=L_EMPTY);
	LIST_DO(literal,cl)
	    last=ct_terms1(literal,last,TRUE,TRUE);
	LIST_END
	return(i_sort(result));
}

/* ct_bvars(term,bvars) - inserts all variable numbers in term into bvars
 */

ITEM
ct_bvars(term,bvars)
	ITEM term,bvars;
	{
	ITEM term1;
	TERM_DO(term1,term)
	  if(term1->item_type== 'v')
	    b_add((LONG)I_GET(term1),bvars);
	TERM_END
	return(bvars);
}

ct_nins(iolist)
	LIST iolist;
	{
	LIST elem;
	LONG result=0l;
	LIST_LOOP(elem,iolist) if(PSYM(L_GET(elem))==pplus1) result++;
	return(result);
}
