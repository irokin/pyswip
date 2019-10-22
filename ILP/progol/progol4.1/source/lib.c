/* ####################################################################### */

/*                      PROGOL predefined predicates			   */
/*                      ---------------------------			   */

/* ####################################################################### */

#include        <stdio.h>
#include        "progol.h"

struct libstruc {
	STRING pattern;
	LONG nargs;
	PREDICATE repeat;
	PREDICATE (*func) ();
	STRING helpmsg;
};

extern struct libstruc clib[];
extern STRING plib[];
extern PREDICATE generate;

/* l_init - initialise the Progol built-in predicate library.
 *	Initialises 'psymtlib', 'lib' and 'ptab'.
 */

l_init()
	{
	struct libstruc *cptr;
	LONG cnt1,cnt2,psym,psym1,argn;
	ITEM atom,cclause,*fptr,*entry,clause,lib2=B_EMPTY,pseen=B_EMPTY;
	FUNC f;
	STRING *scl;
	/* Load built-in C-library */
	for(cptr=clib,cnt1=0l;cptr->pattern;cptr++,cnt1++) {
	  psym=QP_ston(cptr->pattern,argn=cptr->nargs);
	  b_add(psym,allpsyms);
	  *y_ins(psym,psymtlib)=cnt1;
	  b_add(psym,lib2);
	  if(!(cptr->func)) continue;
	  b_add(psym,lib);
	  if(cptr->repeat) b_add(psym,repeats);
	  if(argn) {
	    atom=i_create('f',f=f_create(argn+1l));
	    FNAME(f)=i_create('h',(POINTER)psym); cnt2=0l;
	    ARG_LOOP(fptr,f) *fptr=i_create('v',(POINTER)cnt2++);
	  }
	  else atom=i_create('h',(POINTER)psym);
	  cclause=i_tup4(i_dec(l_push(i_dec(atom),L_EMPTY)),
		i_dec(I_INT(argn)),i_dec(i_create('h',pdot0)),NULL);
	  if(!(*(entry=f_ins(psym,ptab)))) *entry=L_EMPTY;
	  l_push(i_dec(cclause),*entry);
	  b_add(b_num(cclause,spcls),bclauses);
	  if(!(*(entry=f_ins(psym,bptab)))) *entry=B_EMPTY;
	  b_add(cclause->extra,*entry);
	}
	/* Load built-in Prolog library */
	for(scl=plib;**scl;scl++) {
	  clause=ccl_sread(*scl);
	  cl_assert(clause,FALSE,TRUE,FALSE,TRUE,pseen);
	  i_delete(clause);
	}
	cl_psfirstarg(pseen);
	b_uni(lib1,b_uni(lib2,lib));
	i_deletes(lib2,pseen,(ITEM)I_TERM);
}

#define MAXARGS	10l

PREDICATE
l_interp(goal,subin,subout,callno,built,break1)
	ITEM goal,built;
	BIND subin,subout;
	BREAK *break1;
	LONG callno;
	{
	register LONG psym,libno;
	register FUNC f;
	ITEM args[MAXARGS],*arg,*fptr;
	BIND subs[MAXARGS],*sub;
	psym=PSYM(goal);
	if(callno && !b_memq(psym,repeats)) return(FALSE);
	if(goal->item_type=='f') {
	    f=(FUNC)I_GET(goal);
	    arg=args; sub=subs;
	    ARG_LOOP(fptr,f) {
		*arg = *fptr; *sub = subin;
		SKIPVARS(*arg,*sub);
		arg++; sub++;
	    }
	}
	libno= *y_ins(psym,psymtlib);
	return((*((clib+libno)->func))(args,subs,subout,
					callno,built,break1));
}

#define LIB(x)		PREDICATE x(args,subsin,subout,callno,built,break1) \
			ITEM args[],built; BIND subsin[],subout; \
			LONG callno; BREAK *break1;

LIB(l_asserta) {
	PREDICATE result;
	ITEM arg0=p_copy(args[0l],subsin[0l],TRUE),cl,ccl;
	cl=l_pctol(arg0); cl_vrenum(cl);
	ccl=i_tup4(cl,i_dec(I_INT(cl_vmax(cl))),idot0,i_dec(F_EMPTY));
	result=cl_assert(ccl,FALSE,FALSE,TRUE,FALSE,NULL);
	i_deletes(arg0,cl,ccl,(ITEM)I_TERM);
	return(result);
}

ITEM
l_cln(n,atomic,ccls)
	LONG n;
	ITEM ccls;
	{
	register ITEM ccl;
	LIST_DO(ccl,ccls)
	  if(b_memq(ccl->extra,bclauses)&&(!atomic||
		(l_length(F_ELEM(0l,ccl))==1l))&&(n--)<=0l) return(ccl);
	LIST_END
	return((ITEM)NULL);
}

LIB(l_clause) {
	PREDICATE result=FALSE,atomic;
	ITEM arg0,arg1,*def,ccl,head,cl,first;
	char itype;
	LONG cno,cnt,arity;
	BIND s=subsin[0l],sub;
	if((itype=(arg0=args[0l])->item_type)=='f'||itype=='h') {
	  PDEF(def,arg0,s);
	  if(!b_memq(PSYM(arg0),lib1)&&*def){
	    atomic=(((arg1=args[1l])->item_type)== 'h' &&PSYM(arg1)==ptrue);
	    if(ccl=l_cln(callno,atomic,*def)) {
	      sub=(BIND)(break1->next_term_stack);
	      arity=(LONG)I_GET(F_ELEM(1l,ccl));
	      break1->next_term_stack= /* Set up new variable frame */
		(char*)(break1->env_stack->reset=(BIND*)(sub+arity));
	      for(cnt=0l;cnt<arity;cnt++) {
		(sub+cnt)->term=(ITEM)NULL;
		(sub+cnt)->subst=(BIND)NULL;
	      }
	      head=l_pop(cl=F_ELEM(0l,ccl));
	      subout->term=head; subout->subst=sub;
	      if(L_EMPTYQ(cl))
		  (subout+1l)->term=i_create('h',(POINTER)ptrue);
	      else (subout+1l)->term=l_ltopc(cl);
	      (subout+1l)->subst=sub;
	      (subout+2l)->term=I_INT(ccl->extra);
	      (subout+2l)->subst=(BIND)NULL;
	      l_push(i_dec(head),cl);
	      l_push(i_dec((subout+1l)->term),built);
	      l_push(i_dec((subout+2l)->term),built);
	      result=TRUE;
	    }
	  }
	}
	return(result);
}

LIB(l_constant) {
	register PREDICATE result=FALSE;
	char itype;
	if((itype=(args[0l]->item_type))=='h'||itype=='i') result=TRUE;
	return(result);
}

LIB(l_determination) {
	STRING name;
	LONG arity,p,p1;
	if((p=fsym(args[0l],&name,&arity))==(LONG)I_ERROR ||
		(p1=fsym(args[1l],&name,&arity))==(LONG)I_ERROR) {
	    printf("[Command should have form determination(P1/A1,P2/A2)]\n");
	    return(TRUE);
	}
	cl_ddeclare(p,p1);
	return(TRUE);
}

LIB(l_plt) {
	PREDICATE result;
	ITEM arg0=p_copy(args[0l],subsin[0l],FALSE),
		arg1=p_copy(args[1l],subsin[1l],FALSE);
	namecmp=TRUE;
	result=(i_cmp(arg0,arg1)==LT);
	namecmp=FALSE;
	i_deletes(arg0,arg1,(ITEM)I_TERM);
	return(result);
}

LIB(l_equiv) {
	PREDICATE result;
	ITEM arg0=p_copy(args[0l],subsin[0l],FALSE),
		arg1=p_copy(args[1l],subsin[1l],FALSE);
	namecmp=TRUE;
	result=(i_cmp(arg0,arg1)==EQ);
	namecmp=FALSE;
	i_deletes(arg0,arg1,(ITEM)I_TERM);
	return(result);
}

LIB(l_hypothesis) {
	PREDICATE result=FALSE;
	BIND sub;
	LONG arity,cnt;
	ITEM cl,head;
	if(hyp1) {
	  sub=(BIND)(break1->next_term_stack);
	  arity=(LONG)I_GET(F_ELEM(1l,hyp1));
	  break1->next_term_stack= /* Set up new variable frame */
		(char*)(break1->env_stack->reset=(BIND*)(sub+arity));
	  for(cnt=0l;cnt<arity;cnt++) {
	    (sub+cnt)->term=(ITEM)NULL;
	    (sub+cnt)->subst=(BIND)NULL;
	  }
	  head=l_pop(cl=F_ELEM(0l,hyp1));
	  subout->term=head; subout->subst=sub;
	  if(L_EMPTYQ(cl))
	    (subout+1l)->term=i_create('h',(POINTER)ptrue);
	  else (subout+1l)->term=l_ltopc(cl);
	  (subout+1l)->subst=sub;
	  (subout+2l)->term=I_INT(hyp1->extra);
	  (subout+2l)->subst=(BIND)NULL;
	  l_push(i_dec(head),cl);
	  l_push(i_dec((subout+1l)->term),built);
	  l_push(i_dec((subout+2l)->term),built);
	  result=TRUE;
	}
	return(result);
}

LIB(l_lt) {
	register PREDICATE result=FALSE;
	register char arg0,arg1;
	register LONG s,c2,i0,i1,cno;
	register ITEM a0=args[0l],a1=args[1l],*e1,*e2;
	register float f,*fp;
	arg0=a0->item_type; arg1=a1->item_type;
	if(callno&& !generate) result=FALSE;
	else if ((arg0== 'i' ||arg0== 'r')&&(arg1== 'i' ||arg1== 'r')) {
	  if(!callno) result=(i_cmp(a0,a1)<EQ);
	}
	else if ((arg0== 'k'&&(arg1== 'i' ||arg1== 'r' ||arg1== 'k'))||
		 (arg1== 'k'&&(arg0== 'i' ||arg0== 'r'))) {
	  if((cno=catoms->extra)&&(*(e1=f_ins(cno,clt)))&&
		(*(e2=f_ins(b_num(a0,sple),*e1))))
	  result=b_memq(b_num(a1,sple),*e2);
	}
	else switch(arg0) {
	  case 'i':
	    switch(arg1) {
	      case 'v':
	        i1=(LONG)I_GET(a0);
	        f=i1+(1.0/c_fraction(callno+1l)-1.0);
	        (subout+1l)->term=i_create('r',(POINTER)r_create(f));
	        l_push(i_dec((subout+1l)->term),built);
	        result=TRUE;
	        break;
	      default: break;
	    }
	    break;
	  case 'r':
	    switch(arg1) {
	      case 'v':
	        fp=(float *)I_GET(a0);
	        f=(*fp)+(1.0/c_fraction(callno+1l)-1.0);
	        (subout+1l)->term=i_create('r',(POINTER)r_create(f));
	        l_push(i_dec((subout+1l)->term),built);
	        result=TRUE;
	        break;
	      default: break;
	    }
	    break;
	  case 'v':
	    switch(arg1) {
	      case 'i':
	        i1=(LONG)I_GET(a1);
	        f=i1-(1.0/c_fraction(callno+1l)-1.0);
	        subout->term=i_create('r',(POINTER)r_create(f));
	        l_push(i_dec(subout->term),built);
	        result=TRUE;
	        break;
	      case 'r':
	        fp=(float *)I_GET(a1);
	        f=(*fp)-(1.0/c_fraction(callno+1l)-1.0);
	        subout->term=i_create('r',(POINTER)r_create(f));
	        l_push(i_dec(subout->term),built);
	        result=TRUE;
	        break;
	      case 'v':
	        s=isqrt(c2=callno<<1l); i0=callno-((s*(s+1))>>1l);
	        if(i0<0) {i1=s; i0+=s;} else i1=s+1l;
	        subout->term=I_INT(i0);
	        (subout+1l)->term=I_INT(i1);
	        l_push(i_dec(subout->term),built);
	        l_push(i_dec((subout+1l)->term),built);
	        result=TRUE;
	        break;
	      default: break;
	    }
	    break;
	  case 'k':
	    break;
	  default: break;
	}
	return(result);
}

LIB(l_le) {
	register PREDICATE result=FALSE;
	register char arg0,arg1;
	register LONG s,c2,i0,i1,cno;
	register ITEM a0=args[0l],a1=args[1l],*e1,*e2;
	register float f,*fp;
	arg0=a0->item_type; arg1=a1->item_type;
	if(callno&& !generate) result=FALSE;
	else if ((arg0== 'i' ||arg0== 'r')&&(arg1== 'i' ||arg1== 'r')) {
	  if(!callno) result=(i_cmp(a0,a1)<=EQ);
	}
	else if ((arg0== 'k'&&(arg1== 'i' ||arg1== 'r' ||arg1== 'k'))||
		 (arg1== 'k'&&(arg0== 'i' ||arg0== 'r'))) {
	  if((cno=catoms->extra)&&(*(e1=f_ins(cno,cle)))&&
		(*(e2=f_ins(b_num(a0,sple),*e1))))
	  result=b_memq(b_num(a1,sple),*e2);
	}
	else switch(arg0) {
	  case 'i':
	    switch(arg1) {
	      case 'v':
	        i1=(LONG)I_GET(a0);
	        f=i1+(1.0/c_fraction(callno+1l)-1.0);
	        (subout+1l)->term=i_create('r',(POINTER)r_create(f));
	        l_push(i_dec((subout+1l)->term),built);
	        result=TRUE;
	        break;
	      default: break;
	    }
	    break;
	  case 'r':
	    switch(arg1) {
	      case 'v':
	        fp=(float *)I_GET(a0);
	        f=(*fp)+(1.0/c_fraction(callno+1l)-1.0);
	        (subout+1l)->term=i_create('r',(POINTER)r_create(f));
	        l_push(i_dec((subout+1l)->term),built);
	        result=TRUE;
	        break;
	      default: break;
	    }
	    break;
	  case 'v':
	    switch(arg1) {
	      case 'i':
	        i1=(LONG)I_GET(a1);
	        f=i1-(1.0/c_fraction(callno+1l)-1.0);
	        subout->term=i_create('r',(POINTER)r_create(f));
	        l_push(i_dec(subout->term),built);
	        result=TRUE;
	        break;
	      case 'r':
	        fp=(float *)I_GET(a1);
	        f=(*fp)-(1.0/c_fraction(callno+1l)-1.0);
	        subout->term=i_create('r',(POINTER)r_create(f));
	        l_push(i_dec(subout->term),built);
	        result=TRUE;
	        break;
	      case 'v':
	        s=isqrt(c2=callno<<1l); i0=callno-((s*(s+1))>>1l);
	        if(i0<0) {i1=s-1l; i0+=s;} else i1=s;
	        subout->term=I_INT(i0);
	        (subout+1l)->term=I_INT(i1);
	        l_push(i_dec(subout->term),built);
	        l_push(i_dec((subout+1l)->term),built);
	        result=TRUE;
	        break;
	      default: break;
	    }
	    break;
	  case 'k':
	    break;
	  default: break;
	}
	return(result);
}

LIB(l_name) {
	char mess[MAXMESS];
	PREDICATE result=FALSE;
	register char *c,type0; register LONG n;
	ITEM arg0,arg1,l,num;
	float *fp;
	if((type0=(arg0=args[0l])->item_type)== 'h'||
		type0=='i'||type0=='r') {
	  if(type0=='h') strcpy(mess,QP_ntos((LONG)I_GET(arg0)));
	  else if(type0=='i') sprintf(mess,"%d",(LONG)I_GET(arg0));
	  else {fp=(float *)I_GET(arg0); sprintf(mess,"%.3f",*fp);}
	  l=L_EMPTY;
	  STR_LOOP(c,mess) l_suf(i_dec(I_INT(n= *c)),l);
	  (subout+1l)->term=l_ltop(l);
	  l_push(i_dec((subout+1l)->term),built);
	  i_delete(l);
	  result=TRUE;
	}
	else if((arg1=args[1l])->item_type== 'f') {
	  if(l=l_ptol(arg1=p_copy(args[1l],subsin[1l],FALSE))) {
	    c=mess; result=TRUE;
	    LIST_DO(num,l)
	      if(num->item_type=='i'&&
		0<=(n=(LONG)I_GET(num))&&n<=255l) *c++ =n;
	      else result=FALSE;
	    LIST_END
	    *c= '\0';
	    if(number(mess)) {
	      *c++ = ' '; *c= '\0';
	      if(!(subout->term=exp_sread(mess))) result=FALSE;
	    }
	    else {
		subout->term=i_create('h',QP_ston(mess,0l));
		subout->subst=(BIND)NULL;
	    }
	    l_push(i_dec(subout->term),built);
	  }
	  i_deletes(l,arg1,(ITEM)I_TERM);
	}
	return(result);
}

LIB(l_int) {
	register PREDICATE result=FALSE;
	register ITEM arg,ans;
	LONG sign,p1;
	if((arg=args[0])->item_type=='i')
	  if(callno) FALSE;
	  else result=TRUE;
	else if (arg->item_type=='v') {
	  sign=(++callno&1l?-1l:1l);
	  ans=subout->term=I_INT(sign*(callno>>1l));
	  subout->subst=(BIND)NULL;
	  l_push(i_dec(ans),built);
	  result=TRUE;
	}
	return(result);
}

LIB(l_memo) {
	memo = TRUE;
	printf("[Search memo turned ON]\n");
	return(TRUE);
}

LIB(l_modeb) {
	ITEM det=args[0l],pred=args[1l];
	PREDICATE result=cl_mdeclare(det,pred,bmodes);
	return(result);
}

LIB(l_modeh) {
	ITEM det=args[0l],pred=args[1l];
	PREDICATE result=cl_mdeclare(det,pred,hmodes);
	return(result);
}

LIB(l_modes) {
	printf("Head modes\n");
	l_modes1(hmodes);
	printf("Body modes\n");
	l_modes1(bmodes);
	return(TRUE);
}

l_modes1(modes)
	ITEM modes;
	{
	ITEM *rec,mdec,t,io,atom;
	BIND sub;
	FUNC_DO(rec,modes)
	  if(*rec) LIST_DO(mdec,*rec)
	    atom=HOF(TOF((LIST)I_GET(F_ELEM(0l,F_ELEM(0l,mdec)))));
	    io=F_ELEM(1l,mdec); sub=(BIND)stack;
	    LIST_DO(t,io)
	      sub->subst=(BIND)NULL;
	      (sub++)->term=t;
	    LIST_END
	    printf("  mode(%d,",(LONG)I_GET(F_ELEM(2l,mdec)));
	    p_fwritesub(ttyout,atom,(BIND)stack);
	    printf(")\n");
	  LIST_END
	FUNC_END
}

LIB(l_nat) {
	register PREDICATE result=FALSE;
	register ITEM arg;
	if(callno&& !generate) result=FALSE;
	else if((arg=args[0])->item_type=='i' && (LONG)I_GET(arg)>=0l)
	  if(callno) result=FALSE;
	  else result=TRUE;
	else if (arg->item_type=='v') {
	  if(callno<MAXN) subout->term=F_ELEM(callno+MAXN,nums);
	  else {
	  	subout->term=I_INT(callno);
		l_push(i_dec(subout->term),built);
	  }
	  subout->subst=(BIND)NULL;
	  result=TRUE;
	}
	return(result);
}

LIB(l_set) {
	LONG psym,ival;
	ITEM limit=args[0l],value=args[1l];
	if(!(limit->item_type=='h' && value->item_type=='i' &&
	    (ival=(LONG)I_GET(value))>=0l))
		return(FALSE);
	psym=(LONG)I_GET(limit);
	if(psym==QP_ston("h",0l)) hlim=ival;
	else if(psym==QP_ston("i",0l)) vlim=ival;
	else if(psym==QP_ston("c",0l)) clim=ival;
	else if(psym==QP_ston("noise",0l)) noiselim=ival;
	else if(psym==QP_ston("nodes",0l)) pnlim=ival;
	else if(psym==QP_ston("verbose",0l)) verbose=ival;
	else return(FALSE);
	return(TRUE);
}

LIB(l_settings) {
	printf("h=%ld, i=%ld, c=%ld, nodes=%ld, noise=%ld, verbose=%ld\n",
		hlim,vlim,clim,pnlim,noiselim,verbose);
	return(TRUE);
}

extern ITEM store;

LIB(l_record) {
	ITEM arg0=args[0l],arg1=args[1l];
	PREDICATE result=TRUE;
	LONG a0;
	if(arg0->item_type!='i') result=FALSE;
	else {
	  a0=(LONG)I_GET(arg0);
	  if(a0==0l) {
	    if(!store) store=L_EMPTY;
	    l_suf(i_dec(p_copy(arg1,subsin[1l],TRUE)),store);
	  }
	  else if(a0==1l&&store) {
	    unsigned long int *vp;
	    LONG vmax=0l,vno,cnt;
	    ITEM term,plist=l_ltop(store),vnums=Y_EMPTY;
	    BIND sub=(BIND)(break1->next_term_stack);
	    TERM_DO(term,plist) /* Renumber variables */
	      if(term->item_type=='v') {
		if(!(*(vp=y_ins(vno=(LONG)I_GET(term),vnums)))) *vp= ++vmax;
		I_GET(term)= (POINTER)(*vp-1);
	      }
	    TERM_END
	    break1->next_term_stack= /* Set up new variable frame */
		(char*)(break1->env_stack->reset=(BIND*)(sub+vmax));
	    for(cnt=0l;cnt<vmax;cnt++) (sub+cnt)->term=(ITEM)NULL;
	    l_push(i_dec(plist),built);
	    i_deletes(store,vnums,(ITEM)I_TERM); store=(ITEM)NULL;
	    (subout+1l)->term=plist;
	    (subout+1l)->subst=sub;
	  }
	  else result=FALSE;
	}
	return(result);
}

LIB(l_sort) {
	ITEM arg0=args[0l],pl0,pl1,l;
	PREDICATE result=FALSE;
	if((arg0->item_type=='f'&&PSYM(arg0)==pdot2)||
		arg0->item_type=='h'&&PSYM(arg0)==pempty) {
	  if(l=l_ptol(pl0=p_copy(arg0,subsin[0l],FALSE))) {
	    i_psort(l);
	    (subout+1l)->term=pl1=l_ltop(l);
	    (subout+1l)->subst=subsin[0l];
	    l_push(i_dec(pl1),built);
	    result=TRUE;
	  }
	  i_deletes(pl0,l,(ITEM)I_TERM);
	}
	return(result);
}

LIB(l_stats) {
	a_pr_block_stats();
	return(TRUE);
}

LIB(l_tell) {
	register result=FALSE;
	if(plgout!=ttyout) frecdelete(plgout);
	if(args[0]->item_type=='h')
	  if(plgout=frecopen(QP_ntos((LONG)I_GET(args[0])),"w")) result=TRUE;
	  else plgout=ttyout;
	return(result);
}

LIB(l_trace) {
	trace = TRUE;
	printf("[Prolog interpreter trace ON]\n");
	return(TRUE);
}

LIB(l_search) {
	searchq = TRUE;
	printf("[Hypothesis search ON]\n");
	return(TRUE);
}

LIB(l_nosearch) {
	searchq = FALSE;
	printf("[Hypothesis search OFF]\n");
	return(TRUE);
}

LIB(l_split) {
	vsplit = TRUE;
	printf("[Variable splitting flag ON]\n");
	return(TRUE);
}

LIB(l_nosplit) {
	vsplit = FALSE;
	printf("[Variable splitting flag OFF]\n");
	return(TRUE);
}

LIB(l_reduce) {
	ITEM command=args[0l];
	LONG arity,psym;
	STRING name;
	if(fsym(command,&name,&arity)==(LONG)I_ERROR) {
	    printf("[Command should have form reduce(Pred/Arity)]\n");
	    return(FALSE);
	}
	d_treduce(psym=QP_ston(name,arity));
	return(TRUE);
}

LIB(l_reductive) {
	reductive = TRUE;
	printf("[Reductive hypotheses constraint ON]\n");
	return(TRUE);
}

LIB(l_noreductive) {
	reductive = FALSE;
	printf("[Reductive hypotheses OFF]\n");
	return(TRUE);
}

LIB(l_retract) {
	PREDICATE result=FALSE;
	ITEM arg0=args[0l],arg1=args[1l],def;
	register LIST elem,*elemp;
	char type0=arg0->item_type;
	LONG cno,psym;
	if((type0=='f'||type0=='h')&&arg1->item_type=='i'&&
		(def=F_ELEM(psym=PSYM(arg0),ptab))!=(ITEM)NULL) {
	  b_rem(cno=(LONG)I_GET(arg1),bclauses);
	  b_rem(cno,F_ELEM(psym,bptab));
	  elemp=L_LAST(def);
	  while(*elemp)
	    if(L_GET(*elemp)->extra==cno) {
	      elem= *elemp; *elemp=(*elemp)->next;
	      l_push(i_dec(L_GET(elem)),built); L_DEL(elem);
	    }
	    else elemp=&((*elemp)->next);
	  if(L_EMPTYQ(def)) {
	    i_delete(def);
	    def=F_ELEM(PSYM(arg0),ptab)=(ITEM)NULL;
	  }
	  else SUFLIST(def);
	  if(def) cl_pfirstarg(psym);
	  result=TRUE;
	}
	return(result);
}

LIB(l_nomemo) {
	memo = FALSE;
	printf("[Search memo turned OFF]\n");
	return(TRUE);
}

LIB(l_nospy) {
	i_delete(spies);
	spies=B_EMPTY;
	return(TRUE);
}

LIB(l_notrace) {
	trace = FALSE;
	printf("[Prolog interpreter trace OFF]\n");
	return(TRUE);
}

LIB(l_op) {
	ITEM astring,*entry,sym,assoc,prec;
	char mess[MAXWORD];
	LONG psym,passoc;
	prec=args[0l]; assoc=args[1l]; sym=args[2l];
	if(sym->item_type!='h') return(FALSE);
	else if(assoc->item_type!='h') return(FALSE);
	else if(prec->item_type!='i') return(FALSE);
	b_add(psym=(LONG)I_GET(sym),op);
	passoc=(LONG)I_GET(assoc);
	astring=i_create('s',strsave(QP_ntos(passoc)));
	if(strlen((STRING)I_GET(astring))==2l) psym=QP_ston(QP_ntos(psym),1l);
	if(*(entry=f_ins(psym,ops))) {
	    printf("[Redeclaration of operator %s ignored]\n",QP_ntos(psym));
	    i_delete(astring);
	    return(TRUE);
	}
	*entry=i_tup2(i_dec(astring),prec);
	return(TRUE);
}

LIB(l_ops) {
	FUNC f=(FUNC)I_GET(ops);
	ITEM *fptr;
	LONG operator=0l;
	FUNC_LOOP(fptr,f) {
	  if(*fptr) {
	    printf("  op(%d,%s,'%s')\n",(LONG)I_GET(F_ELEM(1l,*fptr)),
		(STRING)I_GET(F_ELEM(0l,*fptr)),QP_ntos(operator));
	  }
	  operator++;
	}
	return(TRUE);
}

LIB(l_repeat) {
	return(TRUE);
}

/*
LIB(l_right) {
	register LONG n,p,p1,x,y,z,z2;
	ITEM primes;
	if(args[0]->item_type!='i') return(FALSE);
	primes=B_EMPTY;
	n=(LONG)I_GET(args[0]);
	for(p=3l;p<=n;p+=2l) {
	  BIT_DO(p1,primes)
	    if(p1*p1>p) goto prime;
	    if(!(p%p1)) goto notprime;
	  BIT_END
	  prime:
	  b_add(p,primes);
	  notprime:
	}
	printf("There are %d primes less than or equal to %d\n",
		b_size(primes)+2l,n);
	for(x=3l;x<=n;x++)
	  for(y=x;y<=n;y++) {
	    z=isqrt(z2=x*x+y*y);
	    if(z*z==z2&&(b_memq(x,primes)||
			b_memq(y,primes)||b_memq(z,primes)))
		printf("%d %d %d\n",x,y,z);
	  }
	i_delete(primes);
	return(TRUE);
}
*/

LIB(l_see) {
	register result=FALSE;
	if(plgin!=ttyin) {freclose(plgin); plgin=ttyin;}
	if(args[0]->item_type=='h')
	  if(plgin=frecopen(QP_ntos((LONG)I_GET(args[0])),"r")) result=TRUE;
	  else plgin=ttyin;
	return(result);
}

LIB(l_spies) {
	printf("Spypoints are:\n");
	l_showp(spies);
	return(TRUE);
}

LIB(l_spy) {
	LONG arity;
	STRING name;
	ITEM command=args[0l];
	if(fsym(command,&name,&arity)==(LONG)I_ERROR) {
	    printf("[Command should have form spy(Pred/Arity)]\n");
	    return(TRUE);
	}
	else b_add(QP_ston(name,arity),spies);
	return(TRUE);
}

LIB(l_told) {
	register result=FALSE;
	if(plgout!=ttyout) {
	  freclose(plgout);
	  plgout=ttyout;
	  result=TRUE;
	}
	return(result);
}

LIB(l_commutative) {
	ITEM command=args[0l];
	LONG arity;
	STRING name;
	if(fsym(command,&name,&arity)==(LONG)I_ERROR) {
	    printf("[Command should have form commutative(Pred/Arity)]\n");
	    return(TRUE);
	}
	else b_add(QP_ston(name,arity),commutes);
	return(TRUE);
}

LIB(l_commutatives) {
	printf("Commutative predicates are:\n");
	l_showp(commutes);
	return(TRUE);
}

LIB(l_consult) {
	ITEM fname=args[0];
	STRING name;
	char file[MAXMESS];
	PREDICATE recon,result;
	if(fname->item_type== 'h')
		name=QP_ntos((STRING)I_GET(fname));
	else return(FALSE);
	sprintf(file,"%s.pl",name);
	recon=reconsult; reconsult=FALSE;
	result=cl_readrls(file);
	reconsult=recon;
	return(result);
}

LIB(l_reconsult) {
	ITEM fname=args[0];
	STRING name;
	char file[MAXMESS];
	PREDICATE recon,ms,result;
	if(fname->item_type== 'h')
		name=QP_ntos((STRING)I_GET(fname));
	else return(FALSE);
	sprintf(file,"%s.pl",name);
	recon=reconsult; reconsult=TRUE;
	ms=mseen; mseen=FALSE;
	result=cl_readrls(file);
	reconsult=recon; mseen=ms;
	return(result);
}

LIB(l_advise) {
	ITEM fname=args[0];
	STRING name;
	char file[MAXMESS];
	if(fname->item_type== 'h')
		name=QP_ntos((STRING)I_GET(fname));
	else return(FALSE);
	sprintf(file,"%s.pl",name);
	return(cl_writerls(file));
}

/* #include "lib1.c" */


PREDICATE getword();
PREDICATE getcommand();
PREDICATE getconnects();
PREDICATE prefix();
PREDICATE postfix();
int findlastslash();
int findchtype();

LIB(l_hasword) {
	ITEM i_ans;
	char alloc[MAXMESS];
	char *mess=alloc;
	if(!getword(mess)) {
	      i_ans=subout->term= i_create('h',QP_ston(mess,0l));
	      l_push(i_dec(i_ans),built);
	}
	else return(FALSE);
	return(TRUE);
}


LIB(l_connected) {
	ITEM i_ans;
	char alloc[MAXMESS];
	char *mess=alloc;
	if(!getconnects(mess)) {
	      i_ans=subout->term= i_create('h',QP_ston(mess,0l));
	      l_push(i_dec(i_ans),built);
	}
	else return(FALSE);
	return(TRUE);
}

typedef enum {LETTER, LANGLE, RANGLE, JUNK} chtype;
typedef enum {COMMAND, WORD, OTHER} statetype;

PREDICATE getword(word)
	char *word;
	{
	static statetype state=OTHER;
	chtype newtype;
	static char ch='#';
	int p=0;
	PREDICATE done=FALSE;

	newtype=findchtype(ch);

	while (ch!=EOF && !done)
	  {

	  switch (state)
	    {
    	    case COMMAND:
	      if (newtype==RANGLE)
		{
		state=OTHER;
		}
	      else
		{
	        newtype=findchtype(ch=getc(plgin->file));			
	        }
	    break;

	    case WORD:
	      if (newtype==LETTER)
		{
		word[p]=tolower(ch);
		p++;
		newtype=findchtype(ch=getc(plgin->file));
		}
	      else
		{
		state=OTHER;
		word[p]='\0';
		done=TRUE;
		}
	      break;	

	    case OTHER:
	      switch (newtype)
		{
		case LANGLE:
		  state=COMMAND;
		  newtype=findchtype(ch=getc(plgin->file));
		  break;
		case LETTER:
		  state=WORD;
		  break;
		default:
		  newtype=findchtype(ch=getc(plgin->file));
		}
	      break;
	    }
	  }

	if (ch==EOF) return TRUE;

	return FALSE;
	}

PREDICATE getcommand(word)
	char *word;
	{
	static statetype state=OTHER;
	chtype newtype;
	static char ch='#';
	int p=0;
	int done=FALSE;

	newtype=findchtype(ch);

	while (ch!=EOF && !done)
	  {

	  switch (state)
	    {
    	    case COMMAND:
	      if (newtype==RANGLE)
		{
		state=OTHER;
		word[p]='\0';
		done=TRUE;
		}
	      else
		{
		word[p]=tolower(ch);
		p++;		
	        newtype=findchtype(ch=getc(plgin->file));
		}	
	      break;

	    case WORD:
	      if (newtype==LETTER)
		{
		newtype=findchtype(ch=getc(plgin->file));
		}
	      else
		{
		state=OTHER;
		}
	      break;	

	    case OTHER:
	      switch (newtype)
		{
		case LANGLE:
		  state=COMMAND;
		  newtype=findchtype(ch=getc(plgin->file));
		  break;
		case LETTER:
		  state=WORD;
		  break;
		default:
		  newtype=findchtype(ch=getc(plgin->file));
		}
	      break;
	    }
	  }

	if (ch==EOF) return TRUE;

	return FALSE;
	}

int findchtype(in)
	char in;
	{
	
	switch (in)
		{
		case '<':
			return LANGLE;
			break;
		case '>':
			return RANGLE;
			break;
		default:
			if (ALPHA(in)) return LETTER;
		}

	return JUNK;
	}

PREDICATE getconnects(word)
	char *word;
	{
	PREDICATE end;
	char alloc[MAXMESS];
	char *add=alloc;

	do {
		end=getcommand(add);
		} while (!end && !(prefix("a href=", add) && postfix(".html", add)));

	if (!end) 
		{
		strcpy(add, (add+6));
		strcpy(word, (add+findlastslash(add)));
		word[strlen(word)-5]='\0';
		}

	return end;
	}


PREDICATE prefix(pre,prefix)
	char *pre,*prefix;
	{
	int result=TRUE, p=0;

	if (strlen(prefix)<=strlen(pre)) return FALSE;
	do result&=(pre[p]==prefix[p++]); while (pre[p]!='\0');

	return result;
	}

PREDICATE postfix(fix,postfix)
	char *fix,*postfix;
	{
	if (strlen(postfix)<=strlen(fix)) return FALSE;
	
	return (!strcmp(fix, (postfix+strlen(postfix)-strlen(fix))));
	}

int findlastslash(string)
	char *string;
	{
	int i=strlen(string);

	while (string[i]!='/' && i>0) i--;

	return ++i;
	}

#define CWIDTH 35

LIB(l_help0) {
	printf("The following system predicates are available:\n");
	l_showp(lib1);
	printf("Help for system predicates using help(Predicate/Arity)\n");
	return(TRUE);
}

LIB(l_help1) {
	ITEM command=args[0l];
	struct libstruc *cptr;
	STRING name;
	LONG arity,psym;
	char mess[MAXMESS];
	if(fsym(command,&name,&arity)==(LONG)I_ERROR) {
	    printf("[Command should have form help(Pred/Arity)]\n");
	    return(FALSE);
	}
	if (!b_memq(psym=QP_ston(name,arity),lib1)) {
	    predn(mess,name,arity);
	    printf("[Unknown system predicate - %s]\n",mess);
	    return(FALSE);
	}
	cptr=clib+((LONG)Y_ELEM(psym,psymtlib));
	predn(mess,cptr->pattern,cptr->nargs);
	printf("[%s - %s]\n",mess,cptr->helpmsg);
	return(TRUE);
}

LIB(l_label1) {
	ITEM clausen=args[0l];
	LONG cno;
	if(clausen->item_type!='i'||(cno=(LONG)I_GET(clausen))<0l)
	  return(FALSE);
	(*y_ins(cno,labels))++;
	return(TRUE);
}

LIB(l_label2) {
	ITEM clausen=args[0l],i_ans;
	LONG cno,ans;
	if(clausen->item_type!='i'||(cno=(LONG)I_GET(clausen))<0l)
	  return(FALSE);
	ans= *y_ins(cno,labels)+1l;
	i_ans=(subout+1l)->term=I_INT(ans);
	l_push(i_dec(i_ans),built);
	return(TRUE);
}

LIB(l_list0) {
	ITEM user=b_rem(pfalse0,b_sub(b_copy(allpsyms),lib1));
	LONG pno;
	BIT_DO(pno,user)
	  if(!F_ELEM(pno,ptab)) b_rem(pno,user);
	BIT_END
	printf("The following user predicates are defined:\n");
	l_showp(user);
	i_delete(user);
	return(TRUE);
}


LIB(l_list1) {
	ITEM command=args[0l];
	LONG arity,psym;
	STRING name;
	char mess[MAXMESS];
	if(fsym(command,&name,&arity)==(LONG)I_ERROR) {
	    printf("[Command should have form list(Pred/Arity)]\n");
	    return(FALSE);
	}
	predn(mess,name,arity);
	c_list1(psym=QP_ston(name,arity),mess);
	return(TRUE);
}

c_list1(psym,mess)
	LONG psym;
	char *mess;
	{
	ITEM *clauses,clause;
	if(b_memq(psym,lib))
	  printf("[No Prolog definition for library predicate %s]\n",mess);
	else {
	  clauses=f_ins(psym,ptab);
	  if(!(*clauses))
	    printf("[Predicate %s not defined]\n",mess);
	  else {
	    if(*clauses) {
	      LIST_DO(clause,*clauses)
	        if(b_memq(clause->extra,bclauses)) ccl_print(clause);
	      LIST_END
	      printf("[Total number of clauses = %d]\n",l_length(*clauses));
	    }
	    /*
	    {ITEM *entry; ULONG *pf;
	    if(*(entry=f_ins(psym,ptopf))) {
	      printf("Fsym indexed clauses:\n");
	      Y_DO(pf,*entry)
		if(*(clauses=f_ins(*pf,pftab))) {
		  printf("%d:%d\n",psym,*pf);
	          LIST_DO(clause,*clauses)
	            if(b_memq(clause->extra,bclauses)) ccl_print(clause);
	          LIST_END
		}
	      Y_END
	    }
	    if(*(clauses=f_ins(psym,pvtab))) {
	      printf("Var indexed clauses:\n");
	      LIST_DO(clause,*clauses)
	        if(b_memq(clause->extra,bclauses)) ccl_print(clause);
	      LIST_END
	    }
	    }
	    */
	 }
	}
}

PREDICATE c_gen1();

LIB(l_gen1) {
	ITEM command=args[0l];
	STRING name;
	LONG arity,psym;
	char mess[MAXMESS];
	PREDICATE oldsearchq=searchq,result;
	if(fsym(command,&name,&arity)==(LONG)I_ERROR) {
	    printf("[Command should have form gen(Pred/Arity)]\n");
	    return(FALSE);
	}
	predn(mess,name,arity);
	searchq=TRUE;
	result=c_gen1(psym=QP_ston(name,arity),mess);
	searchq=oldsearchq;
	return(result);
}


PREDICATE
c_gen1(psym,mess)
	LONG psym;
	char *mess;
	{
	char mess1[MAXMESS];
	PREDICATE result=TRUE;
	ITEM *clauses,bcl,bcl1,clause;
	LONG cno;
	if(b_memq(psym,lib1))
	  printf("[Cannot generalise library predicate %s]\n",mess);
	else if (bcl= *f_ins(psym,bptab)) {
	  bcl1=b_copy(bcl);
	  BIT_DO(cno,bcl1)
	    if(b_memq(cno,bclauses)) {
		ccl_swrite(mess1,clause=i_copy(i_dec(b_elem(cno,spcls))));
		printf("[Generalising %s]\n",mess1);
		c_sat(clause);
		i_delete(clause);
	    }
	  BIT_END
	  i_delete(bcl1);
	  c_list1(psym);
	  printf("\n");
	}
	else {
	  printf("[Predicate %s not defined]\n",mess);
	  result=FALSE;
	}
	return(result);
}

PREDICATE yates=TRUE;

#include <math.h>
extern DOUBLE gamma_func();

#define RIGHT	(AP+ap)
#define WRONG	(aP+Ap)
#define N	(RIGHT+WRONG)
#define P	(RIGHT/N)
#define Q	(1-P)
#define PACC	(P*100.0)
#define ERR	pow((P*Q)/N,0.5)
#define PERR	(ERR*100.0)
#define N1	(AP+aP)
#define N2	(Ap+ap)
#define N3	(AP+Ap)
#define N4	(aP+ap)
#define E1	((N1*N3)/N)
#define E2	((N1*N4)/N)
#define E3	((N2*N3)/N)
#define E4	((N2*N4)/N)
#define O1	(yates?(AP>E1?AP-0.5:(AP<E1?AP+0.5:AP)):AP)
#define O2	(yates?(aP>E2?aP-0.5:(aP<E2?aP+0.5:aP)):aP)
#define O3	(yates?(Ap>E3?Ap-0.5:(Ap<E3?Ap+0.5:Ap)):Ap)
#define O4	(yates?(ap>E4?ap-0.5:(ap<E4?ap+0.5:ap)):ap)
#define OE1	((O1-E1)*(O1-E1)/E1)
#define OE2	((O2-E2)*(O2-E2)/E2)
#define OE3	((O3-E3)*(O3-E3)/E3)
#define OE4	((O4-E4)*(O4-E4)/E4)
#define CHISQ	(OE1+OE2+OE3+OE4)
#define CHISQP	(gamma_func(0.5*((N3>0.0&&N4>0.0)?1.0:0.0),0.5*CHISQ))

LIB(l_leave) {
	ITEM command=args[0l],bpcls,bcls,cls,cls1,bred,one,bnegs,ccl;
	STRING name;
	LONG arity,psym,cno;
	double AP,aP,Ap,ap;
	char mess[MAXMESS],mess1[MAXMESS];
	PREDICATE oldsearchq=searchq,result=FALSE;
	AP=aP=Ap=ap=0.0;
	if((psym=fsym(command,&name,&arity))==(LONG)I_ERROR) {
	    printf("[Command should have form gen(Pred/Arity)]\n");
	    return(FALSE);
	}
	predn(mess,name,arity);
	searchq=TRUE;
	if(cls1= *f_ins(psym,ptab)) {
	  printf("[TESTING POSITIVES]\n");
	  cls=l_copy(cls1);
	  bpcls=b_copy(F_ELEM(psym,bptab));
	  bcls=b_copy(bclauses);
	  BIT_DO(cno,bpcls)
	      b_rem(cno,F_ELEM(psym,bptab)); b_rem(cno,bclauses);
	      b_lsub(F_ELEM(psym,ptab),one=b_add(cno,B_EMPTY));
	      c_gen1(psym,mess);	/* Generalise w/o clause */
	      i_delete(F_ELEM(psym,bptab));
	      F_ELEM(psym,bptab)=one; /* Test clause implied */
	      ccl_swrite(mess1,i_dec(b_elem(cno,spcls)));
	      if(b_emptyq(bred=d_tredundant(psym))) {
		Ap+=1.0;
		printf("[Wrong: %s]\n",mess1);
	  	  printf("[Partial accuracy= %.f/%.f]\n",RIGHT,N);
	      }
	      else {
	        AP+=1.0;		/* Add up and reset state */
		printf("[Right: %s]\n",mess1);
	  	  printf("[Partial accuracy= %.f/%.f]\n",RIGHT,N);
	      }
	      i_deletes(bred,F_ELEM(psym,ptab),F_ELEM(psym,bptab),
		bclauses,(ITEM)I_TERM);
	      F_ELEM(psym,ptab)=cls1=l_copy(cls);
	      SUFLIST(cls1);
	      F_ELEM(psym,bptab)=b_copy(bpcls);
	      bclauses=b_copy(bcls);
	      cl_pfirstarg(psym);
	  BIT_END
	  i_deletes(bpcls,cls,bcls,(ITEM)I_TERM);

	  printf("[TESTING NEGATIVES]\n");
	  if((psym!=pfalse0) && (bnegs= *f_ins(pfalse0,bptab))) {
	    cls=l_copy(*f_ins(psym,ptab));
	    bpcls=b_copy(F_ELEM(psym,bptab));
	    bcls=b_copy(bclauses);
	    BIT_DO(cno,bnegs)
	        b_rem(cno,bclauses);
	        c_gen1(psym,mess);	/* Generalise w/o clause */
	        ccl_swrite(mess1,ccl=i_dec(b_elem(cno,spcls)));
	        if(interp_quest(ccl)) {
		  aP+=1.0;
		  printf("[Wrong: %s]\n",mess1);
	  	  printf("[Partial accuracy= %.f/%.f]\n",RIGHT,N);
	        }
	        else {
	          ap+=1.0;			/* Add up and reset state */
		  printf("[Right: %s]\n",mess1);
	  	  printf("[Partial accuracy= %.f/%.f]\n",RIGHT,N);
	        }
	        i_deletes(F_ELEM(psym,ptab),F_ELEM(psym,bptab),
			bclauses,(ITEM)I_TERM);
	        F_ELEM(psym,ptab)=cls1=l_copy(cls);
	        SUFLIST(cls1);
	        F_ELEM(psym,bptab)=b_copy(bpcls);
	        bclauses=b_copy(bcls);
	        cl_pfirstarg(psym);
	    BIT_END
	    i_deletes(bpcls,cls,bcls,(ITEM)I_TERM);
	  }
	  c_chisq(AP,aP,Ap,ap);
	  result=TRUE;
	}
	else printf("[Predicate %s not defined]\n",mess);
	/* Count up and return result */
	searchq=oldsearchq;
	return(result);
}

/* Chi-square test on 2/2 contingency table
		 A ~A
	      P	AP aP
	     ~P Ap ap
 */

c_chisq(AP,aP,Ap,ap)
	DOUBLE AP,aP,Ap,ap;
	{
	printf("Contingency table=    ________A________~A\n");
	printf("                    P|%9.f|%9.f| %9.f\n",AP,aP,N1);
	printf("                     |(%7.1f)|(%7.1f)| \n",E1,E2);
	printf("                   ~P|%9.f|%9.f| %9.f\n",Ap,ap,N2);
	printf("                     |(%7.1f)|(%7.1f)|\n",E3,E4);
	printf("                      ~~~~~~~~~~~~~~~~~~~\n",Ap,ap);
	printf("                      %9.f %9.f  %9.f\n",N3,N4,N);
	printf("[Overall accuracy= %.2f%% +/- %.2f%%]\n",PACC,PERR);
	yates=TRUE;
	printf("[Chi-square = %.2f]",CHISQ);
	yates=FALSE;
	printf(" [Without Yates correction = %.2f]\n",CHISQ);
	yates=TRUE;
	printf("[Chi-square probability = %.4f]\n",CHISQP);
}

LIB(l_chisq) {
	double AP=(LONG)I_GET(args[0l]),aP=(LONG)I_GET(args[1l]),
		Ap=(LONG)I_GET(args[2l]),ap=(LONG)I_GET(args[3l]);
	c_chisq(AP,aP,Ap,ap);
	return(TRUE);
}

LIB(l_seen) {
	register result=FALSE;
	if(plgin!=ttyin) {
	  freclose(plgin);
	  plgin=ttyin;
	  result=TRUE;
	}
	return(result);
}

LIB(l_write) {
	noquotes=TRUE;
	p_fwritesub(plgout,args[0l],subsin[0l]);
	noquotes=FALSE;
	return(TRUE);
}

LIB(l_read) {
	register ITEM term;
	PREDICATE result=FALSE;
	term=exp_fread(plgin);
	if(term==(ITEM)I_TERM) term=i_inc(eof);
	if(term!=(ITEM)I_ERROR) {
	  subout->term=term;
	  subout->subst=(BIND)NULL;
	  l_push(i_dec(term),built);
	  result=TRUE;
	}
	return(result);
}

LIB(l_read1) {
	register ITEM term;
	PREDICATE result=FALSE;
	while((term=exp_fread(plgin))==(ITEM)I_ERROR);
	if(term!=(ITEM)I_TERM) {
	  subout->term=term;
	  subout->subst=(BIND)NULL;
	  l_push(i_dec(term),built);
	  result=TRUE;
	}
	return(result);
}


LIB(l_nl) {
	i_fnl(plgout);
	return(TRUE);
}

LIB(l_tab) {
	register PREDICATE result=FALSE;
	register LONG cnt;
	if(args[0l]->item_type=='i' && (cnt=(LONG)I_GET(args[0l]))>=0l) {
		for(;cnt;cnt--) i_fpr(plgout," ");
		result=TRUE;
	}
	return(result);
}

LIB(l_arg) {
	PREDICATE result=FALSE,numok=FALSE,numset=FALSE;
	ITEM arg0,arg1,arg2,sym;
	LONG n;
	BIND sub2;
	if((arg0=args[0])->item_type=='i') {
	    if((n=(LONG)I_GET(arg0)) >= 0l) numok=TRUE;
	}
	else if((arg0=args[0])->item_type=='v') {
	    subout->term=I_INT(n=callno);
	    subout->subst=(BIND)NULL;
	    l_push(i_dec(subout->term),built);
	    numok=TRUE;
	    numset=TRUE;
	}
	if(numok && (numset || !callno)) {
	    if ((arg1=args[1])->item_type== 'f' && n < FUNC_SIZE(arg1)) {
	      if(!n) {
		l_push(i_dec(sym=i_copy(F_ELEM(0l,arg1))),built);
		I_GET(sym)=(POINTER)QP_ston(QP_ntos((LONG)I_GET(sym)),0l);
		(sub2=subout+2l)->term=sym;
		(subout+2l)->subst=(BIND)NULL;
	      }
	      else (sub2=subout+2l)->term=F_ELEM(n,arg1);
	      sub2->subst=subsin[1l];
	      result=TRUE;
	    }
	    else if(arg1->item_type == 'h' && !n && !callno) {
		(sub2=subout+2l)->term=arg1;
		sub2->subst=subsin[1l];
		result=TRUE;
	    }
	}
	return(result);
}

LIB(l_float) {
	register PREDICATE result=FALSE;
	register ITEM arg,ans;
	float frac; 
	if(callno&& !generate) result=FALSE;
	else if((arg=args[0])->item_type=='r')
	  if(callno) result=FALSE;
	  else result=TRUE;
	else if (arg->item_type=='v') {
	  frac=c_fraction(callno);
	  ans=subout->term=i_create('r',(POINTER)r_create(frac));
	  l_push(i_dec(ans),built);
	  result=TRUE;
	}
	return(result);
}

LIB(l_functor) {
	PREDICATE result=FALSE;
	ITEM arg0,arg1,arg2;
	char type0;
	LONG arity,cnt;
	BIND sub;
	if((type0=(arg0=args[0l])->item_type)=='v') {
	  if((arg1=args[1l])->item_type=='h' &&
		(arg2=args[2l])->item_type=='i') {
	    if(!(arity=(LONG)I_GET(arg2))) subout->term=arg1;
	    else if((char *)(break1->env_stack)<=
		(char *)((sub=(BIND)(break1->next_term_stack))+arity))
	      return(FALSE);
	    else {
	      BIND sub=(BIND)(break1->next_term_stack);
	      arg0=i_create('f',f_create((LONG)I_GET(arg2)+1l));
	      F_ELEM(0l,arg0)=i_create('h',
		QP_ston(QP_ntos((LONG)I_GET(arg1)),arity));
	      for(cnt=0l;cnt<arity;cnt++) {
		F_ELEM(cnt+1l,arg0)=i_create('v',(POINTER)cnt);
		(sub+cnt)->term=(ITEM)NULL;
	      }
	      break1->next_term_stack= /* Set up new variable frame */
		(char*)(break1->env_stack->reset=(BIND*)(sub+arity));
	      l_push(i_dec(arg0),built);
	      subout->term=arg0;
	      subout->subst=sub;
	    }
	    result=TRUE;
	  }
	}
	else if(type0=='f') {
	  (subout+1l)->term=i_create('h',(POINTER)QP_ston(
			QP_ntos((LONG)I_GET(F_ELEM(0l,arg0))),0l));
	  (subout+2l)->term=i_create('i',(POINTER)(F_SIZE(arg0)-1l));
	  l_push(i_dec((subout+1l)->term),built);
	  l_push(i_dec((subout+2l)->term),built);
	  result=TRUE;
	}
	else if(type0=='h') {
	  (subout+1l)->term=arg0;
	  (subout+1l)->subst=(BIND)NULL;
	  (subout+2l)->term=i_create('i',(POINTER)0l);
	  (subout+2l)->subst=(BIND)NULL;
	  l_push(i_dec((subout+2l)->term),built);
	  result=TRUE;
	}
	return(result);
}

LIB(l_univ) {
	PREDICATE result=FALSE;
	ITEM arg0,arg1,l,term0,first,pl,new0,sym,first1;
	BIND sub1;
	LONG len;
	char type0;
	if((type0=(arg0=args[0l])->item_type)=='v') {
	  if((arg1=args[1l])->item_type=='f' &&
		(LONG)I_GET(F_ELEM(0l,arg1))==pdot2) {
	    if(l=l_ptol(arg1=p_copy(args[1l],subsin[1l],FALSE))) {
	      first=HOF((LIST)I_GET(l));
	      if((first=HOF((LIST)I_GET(l)))->item_type=='h') {
	        if((len=l_length(l))==1l) term0=first;
	        else {
		  first1=i_create('h',QP_ston(
			QP_ntos((LONG)I_GET(first)),len-1l));
		  i_delete(l_pop(l));
		  l_push(i_dec(first1),l);
	          term0=f_ltof(l);
	          l_push(i_dec(term0),built);
	        }
	        subout->term=term0;
	        subout->subst=subsin[1l];
	        result=TRUE;
	      }
	    }
	    i_deletes(l,arg1,(ITEM)I_TERM);
	  }
	}
	else if(type0=='f') {
	  l_push(i_dec(new0=i_copy(arg0)),built);
	  sym=F_ELEM(0l,new0);
	  I_GET(sym)=(POINTER)QP_ston(QP_ntos((LONG)I_GET(sym)),0l);
	  (sub1=subout+1l)->term=pl=l_ltop(l=f_ftol(new0));
	  sub1->subst=subsin[0l];
	  l_push(i_dec(pl),built);
	  i_delete(l);
	  result=TRUE;
	}
	else if(type0=='h'||type0=='i'||type0=='r') {
	  (sub1=subout+1l)->term=pl=l_ltop(l=l_push(arg0,L_EMPTY));
	  sub1->subst=subsin[0l];
	  l_push(i_dec(pl),built);
	  i_delete(l);
	  result=TRUE;
	}
	return(result);
}

LIB(l_test) {
	ITEM command=args[0l],def,ccl;
	LONG arity,psym;
	STRING name;
	if(fsym(command,&name,&arity)==(LONG)I_ERROR) {
	    printf("[Command should have form list(Pred/Arity)]\n");
	    return(FALSE);
	}
	if(def= *f_ins(psym=QP_ston(name,arity),ptab)) {
	  printf("The unguarded recursions are\n\n");
	  LIST_DO(ccl,def)
	    if(s_ugrecq(F_ELEM(0l,ccl))) ccl_print(ccl);
	  LIST_END
	}
	return(TRUE);
}

PREDICATE l_eval();

LIB(l_is) {
	float ans;
	ITEM i_ans;
	char itype;
	if(l_eval(args[1l],subsin[1l],&ans,&itype)) {
	  if(itype== 'i') {
	    LONG i=(LONG)ans;
	    if(-MAXN<=i&&i<MAXN) subout->term=F_ELEM(i+MAXN,nums);
	    else {
	      i_ans=subout->term= I_INT((LONG)ans);
	      l_push(i_dec(i_ans),built);
	    }
	  }
	  else {
	    i_ans=subout->term=i_create('r',r_create(ans));
	    l_push(i_dec(i_ans),built);
	  }
	  subout->subst=(BIND)NULL;
	  return(TRUE);
	}
	else return(FALSE);
}

PREDICATE
l_eval(t,s,ans,atype)
	ITEM t;
	BIND s;
	float *ans;
	char *atype;
	{
	LONG result=FALSE;
	LONG p,ix,iy;
	float fx,fy,*fp;
	double dx,dy;
	char xtype,ytype;
	PREDICATE i;
	SKIPVARS(t,s);
	switch(t->item_type) {
	  case 'f':
	    p=PSYM(t);
	    switch(F_SIZE(t)) {
	      case 2l:
	        if(l_eval(F_ELEM(1l,t),s,&fx,atype)) {
		  if(*atype=='i') ix=(LONG)fx;
		  if(p==pminus1) *ans=(float)(*atype=='i'?-ix:-fx);
		  else if(p==pplus1) *ans=fx;
		  else if(p==plog1) {
		    if(*atype== 'i'&&ix>0l) {*ans=log(dx=ix); *atype='r';}
		    else if(fx>0.0) *ans=log(dx=fx);
		    else return(FALSE);
		  }
		  else if(p==pexp1) {
		    if(*atype== 'i') {*ans=exp(dx=ix); *atype='r';}
		    else *ans=exp(dx=fx);
		  }
		  else if(p==psin1) {
		    if(*atype== 'i') {*ans=sin(dx=ix); *atype='r';}
		    else *ans=sin(dx=fx);
		  }
		  else if(p==pcos1) {
		    if(*atype== 'i') {*ans=cos(dx=ix); *atype='r';}
		    else *ans=cos(dx=fx);
		  }
		  else if(p==ptan1) {
		    if(*atype== 'i') {*ans=tan(dx=ix); *atype='r';}
		    else *ans=tan(dx=fx);
		  }
		  else if(p==pceil1) {
		    if(*atype== 'i') *ans=(float)ix;
		    else *ans=(float)(ix=ceil(dx=fx));
		    *atype='i';
		  }
		  else if(p==pfloor1) {
		    if(*atype== 'i') *ans=(float)ix;
		    else *ans=(float)(ix=floor(dx=fx));
		    *atype='i';
		  }
		  else return(FALSE);
		  result=TRUE;
		}
		break;
	      case 3l:
	        if(l_eval(F_ELEM(1l,t),s,&fx,&xtype)&&
			l_eval(F_ELEM(2l,t),s,&fy,&ytype)) {
		  if(xtype=='i'&&ytype=='r') { /* Type conversion */
		    ix=(LONG)fx; fx=ix; xtype='r'; *atype='r';
		  }
		  else if(xtype=='r'&&ytype=='i') {
		    iy=(LONG)fy; fy=iy; ytype='r'; *atype='r';
		  }
		  else if(xtype=='i'&&ytype=='i') {
		    ix=(LONG)fx; iy=(LONG)fy;
		    if(p==pdiv2) {fx=ix; fy=iy; *atype='r';}
		    else *atype='i';
		  }
		  else *atype='r';
		  i=(xtype=='i');
	          if(p==pplus2) *ans=(float)(i?ix+iy:fx+fy);
		  else if(p==pminus2) *ans=(float)(i?ix-iy:fx-fy);
		  else if(p==ptimes2) *ans=(float)(i?ix*iy:fx*fy);
		  else if(p==pdiv2&&fy!=0) *ans=fx/fy;
		  else if(p==phat2&&((i&&ix>=0)||(!i&&fx>=0.0))) {
		    if(i) {dx=ix;dy=iy;}
		    else {dx=fx;dy=fy;}
		    *ans=pow(dx,dy);
		    *atype='r';
		    /* if(i) {ix= *ans;*ans=(float)ix;} */
		  }
		  else if(p==pmod2&&i&&iy) *ans=(float)(ix%iy);
		  else if(p==por2&&i) *ans=(float)(ix|iy);
		  else if(p==pand2&&i) *ans=(float)(ix&iy);
		  else return(FALSE);
		  result=TRUE;
	        }
	        break;
	      default:
		break;
	    }
	    break;
	  case 'i':
	    ix=(LONG)I_GET(t);
	    *ans=(float)ix;
	    *atype='i';
	    result=TRUE;
	    break;
	  case 'r':
	    fp=(float *)I_GET(t);
	    *ans= *fp;
	    *atype='r';
	    result=TRUE;
	    break;
	  case 'h':
	    result=TRUE;
	    p=PSYM(t);
	    if(p==prand0) {
	      *ans=drand48();
	      *atype='r';
	    }
	    else if(p==pcputime0) {
	      *ans=(float)cputime();
	      *atype='r';
	    }
	    else result=FALSE;
	    break;
	  default:
	    break;
	}
	return(result);
}

LIB(l_var) {
	return(args[0]->item_type=='v');
}

LIB(l_vassert) {
	PREDICATE result=FALSE;
	LONG cno;
	if(args[0]->item_type=='i' && (cno=(LONG)I_GET(args[0]))>=0
		&& cno<(LONG)(I_GET(F_ELEM(2l,spcls)))) {
	  b_add(cno,bclauses);
	  result=TRUE;
	}
	return(result);
}

LIB(l_vretract) {
	PREDICATE result=FALSE;
	LONG cno;
	if(args[0]->item_type=='i' && (cno=(LONG)I_GET(args[0]))>=0) {
	  b_rem(cno,bclauses);
	  result=TRUE;
	}
	return(result);
}

LIB(l_xy) {
	LONG x,y;
	x=(LONG)I_GET(args[0l]);
	y=(LONG)I_GET(args[1l]);
	pf_ins(x,y,TRUE);
	xy_write();
	return(TRUE);
}

ITEM
l_namesp(preds,max,nc)
	ITEM preds;
	LONG *max,*nc;
	{
	char mess[MAXMESS];
	STRING name;
	LONG arity,psym,n;
	ITEM result=L_EMPTY,rec;
	BIT_DO(psym,preds)
	  rec=F_ELEM(psym,F_ELEM(0l,spsyms));
	  name=((STRING)I_GET(F_ELEM(0l,rec)));
	  arity=((LONG)I_GET(F_ELEM(1l,rec)));
	  predn(mess,name,arity);
	  if((n=strlen(mess))>*max) *max=n;
	  l_suf(i_dec(i_create('s',strsave(mess))),result);
	  if(rec= *f_ins(psym,bptab)) *nc+=b_size(rec);
	BIT_END
	return(i_sort(result));
}

l_showp(preds)
	ITEM preds;
	{
	LONG cwidth=0l,nc=0l;
	ITEM names=l_namesp(preds,&cwidth,&nc),name;
	LONG cnt1=0l,cnt2,columns=(78)/(cwidth+1l);
	STRING mess;
	LIST_DO(name,names)
	  if(!cnt1) printf("  ");
	  printf(mess=I_GET(name));
	  for(cnt2=(cwidth+1l)-strlen(mess);cnt2>0;cnt2--)
		printf(" ");
	  if(++cnt1 >= columns) {
		cnt1=0l;
		printf("\n");
	  }
	BIT_END
	if(cnt1) printf("\n");
	i_delete(names);
	if(preds!=lib1) printf("[Total number of clauses = %d]\n",nc);
}

ITEM c_choose(),c_kpart();

/* Below is an expression for generating random natural numbers
	using distribution 2^{x-1} */

/*

#define RNAT	(-log2(drand48()))

LIB(l_rand) {
	LONG y,nsyms=(LONG)I_GET(F_ELEM(2l,spsyms)),cnt1,cnt2;
	ITEM terms=F_ELEM(0l,spsyms),bterms,term;
	bterms=b_allones(nsyms);
	for(cnt1=20l;cnt1;cnt1--) {
	 for(cnt2=5l;cnt2;cnt2--) {
	  y= RNAT+1.0;
	  term=c_choose(y,terms,bterms);
	  at_fwrite(ttyout,term); printf("  ");
	  i_deletes(term,(ITEM)I_TERM);
	 }
	 i_fnl(ttyout);
	}
	i_delete(bterms);
	return(TRUE);
}
*/

/* c_fraction - returns the float constructed by reflecting (in binary)
 *	the given integer around the decimal point.
 *	eg. c_fraction(10)=0.01
 */

float
c_fraction(i)
	LONG i;
	{
	LONG val;
	float frac,res; 
	for(res=0.0,frac=0.5,val=i;val;frac/=2.0,val>>=1l)
	    if(val&1l) res+=frac;
	return(res);
}

ITEM
c_kpart(k,n)
	LONG k,n;
	{
	LONG cnt;
	ITEM result=Y_EMPTY;
	for(cnt=k;cnt>0l;cnt--) Y_PUSH(1l,result);
	for(cnt=n-k;cnt>0l;cnt--) Y_ELEM(MYRAND(0l,k-1l),result)++;
	return(result);
}

ITEM
c_range(lo,hi,terms,bterms)
	LONG lo,hi;
	ITEM terms,bterms;
	{
	LONG arity,psym;
	ITEM result=B_EMPTY;
	BIT_DO(psym,bterms)
	  arity=((LONG)I_GET(F_ELEM(1l,F_ELEM(psym,terms))));
	  if(lo<=arity && arity<=hi) b_add(psym,result);
	BIT_END
	return(result);
}

ITEM
c_choose(n,terms,bterms)
	LONG n;
	ITEM terms,bterms;
	{
	LONG m,r,psym,arity,i;
	ITEM bterms1,result,fsym,kpart,*subt;
	if(n<=1l) {
	  if(b_emptyq(bterms1=c_range(0l,0l,terms,bterms))) {
	    result=i_create('i',n);
	  }
	  else {
	    r=MYRAND(1,b_size(bterms1));
	    psym=b_ith(r,bterms1);
	    result=i_create('h',psym);
	  }
	}
	else {
	  if(b_emptyq(bterms1=c_range(1l,n-1l,terms,bterms))) {
	    result=i_create('i',n);
	  }
	  else {
	    r=MYRAND(1,b_size(bterms1));
	    psym=b_ith(r,bterms1);
	    fsym=i_create('h',psym);
	    arity=(LONG)I_GET(F_ELEM(1l,F_ELEM(psym,terms)));
	    result=i_create('f',f_create(arity+1l));
	    F_ELEM(0l,result)=fsym;
	    kpart=c_kpart(arity,n-1l);
	    for(i=1l;i<=arity;i++)
		F_ELEM(i,result)=c_choose(Y_ELEM(i-1l,kpart),terms,bterms);
	    i_delete(kpart);
	  }
	}
	i_delete(bterms1);
	return(result);
}

/* Table of library predicates
 */

struct libstruc clib[] = {
	"!", 0l, FALSE, NULL, "Prolog cut",
	",", 2l, TRUE, NULL, "(A, B) succeeds if A and B succeed",
	";", 2l, TRUE, NULL, "(A; B) succeeds if A or B succeed",
	"->", 2l, TRUE, NULL, "(A->B) if A then B",
	"=<", 2l, TRUE, l_le, "A =< B where A,B instantiated",
	">=", 2l, TRUE, NULL, "A >= B where A,B instantiated",
	"<", 2l, TRUE, l_lt, "A < B where A,B instantiated",
	">", 2l, TRUE, NULL, "A > B where A,B instantiated",
	"\\=", 2l, TRUE, NULL, "A \\= B where A,B instantiated",
	"\\==", 2l, TRUE, NULL, "A \\== B where A,B instantiated",
	"\\+", 1l, TRUE, NULL, "negation by failure",
	"@<", 2l, FALSE, l_plt, "A @< B term A lexicographically less than B",
	"@>", 2l, FALSE, NULL, "A @> B term A lexicographically greater than B",
	"@>=", 2l, FALSE, NULL, "A @>= B term A lexicographically greater or equal to B",
	"@=<", 2l, FALSE, NULL, "A @=< B term A lexicographically less or equal to B",
	"==", 2l, FALSE, l_equiv, "A == B if terms A and B identical",
	"=", 2l, FALSE, NULL, "A = B unifies A and B",
	"=..", 2l, FALSE, l_univ, "f(T1,..,Tn) =.. [f,T1,..,Tn]",
	".", 2l, FALSE, NULL, "[File|Files] reconsults File and Files",
	"advise", 1l, FALSE, l_advise, "writes user predicates to file",
	"any", 1l, FALSE, NULL, "any(X) succeeds on any term",
	"arg", 3l, TRUE, l_arg, "arg(N,T,T1) where T1 is the Nth argument of term T",
	"asserta", 1l, FALSE, l_asserta, "asserta(Clause) asserts the clause at beginning of definition",
	"bagof", 3l, FALSE, NULL, "bagof(X,P,B) bagof X such that P is B",
	"chisq", 4l, FALSE, l_chisq, "chisq(AP,aP,Ap,ap) prints chi-square statistics",
	"clause", 2l, TRUE, NULL, "clause(Head,Body) where Head must be instantiated",
	"clause", 3l, TRUE, l_clause, "clause(Head,Body,N) where N is the number of the clause",
	"commutative", 1l, FALSE, l_commutative, "commutative(Pred/Arity) means input arguments unordered",
	"commutatives", 0l, FALSE, l_commutatives, "show commutative predicates",
	"connected", 1l, TRUE, l_connected, "gets html files connected to present file",
	"constant", 1l, FALSE, l_constant, "constant(X) true if X is a constant",
	"consult", 1l, FALSE, l_consult, "consults file",
	"determination", 2l, FALSE, l_determination, "determination for predicate, eg. determination(mult/3,plus/3)",
	"element", 2l, FALSE, NULL, "element(X,L) if X is a member of list L",
	"float", 1l, TRUE, l_float, "float(F) is true if F is a floating point number",
	"functor", 3l, FALSE, l_functor, "functor(Term,Fsym,Arity) where Term has function symbol Fsym/Arity",
	"generalise", 1l, FALSE, l_gen1, "gen(Pred/Arity) generalises all clauses with head Pred/Arity",
	"hasword", 1l, TRUE, l_hasword, "gets words from html file",
	"help", 0l, FALSE, l_help0, "lists the system predicates, help available using help/1",
	"help", 1l, FALSE, l_help1, "gives help information on particular command, eg. help(tell/1)",
	"hypothesis", 3l, FALSE, l_hypothesis, "hypothesis(Head,Body,N) where N is number of hypothesis being tested",
	"int", 1l, TRUE, l_int, "int(X) succeeds when X is an integer",
	"is", 2l, FALSE, l_is, "X is Y where X is the evaluation of expression Y",
	"leave", 1l, FALSE, l_leave, "leave(Pred/Arity) gives predictive accuracy using leave-one-out",
	"label", 1l, FALSE, l_label1, "label(ClauseN) increments the label of the clause",
	"label", 2l, FALSE, l_label2, "label(ClauseN,N) N is the label of the given clause",
	"length", 2l, FALSE, NULL, "length(List,Length)",
	"listing", 0l, FALSE, l_list0, "list user-defined predicates",
	"listing", 1l, FALSE, l_list1, "list(Pred/Arity) prints the clauses of Pred/Arity",
	"memo", 0l, FALSE, l_memo, "turn ON memoing for search",
	"modeb", 2l, FALSE, l_modeb, "determinacy and body mode, eg. modeb(1,mult(+int,+int,-int))",
	"modeh", 2l, FALSE, l_modeh, "determinacy and head mode, eg. modeh(1,qsort([+int|+list],-list))",
	"modes", 0l, FALSE, l_modes, "show head and body modes of predicates",
	"name", 2l, FALSE, l_name, "name(X,L) where X is a constant and L is a list of asciis",
	"nat", 1l, TRUE, l_nat, "nat(X) succeeds when X is a natural number",
	"nl", 0l, FALSE, l_nl, "print new line",
	"nomemo", 0l, FALSE, l_nomemo, "turn OFF memoing for search",
	"noreductive", 0l, FALSE, l_noreductive, "turn OFF reductive constraint",
	"nosearch", 0l, FALSE, l_nosearch, "turn OFF hypothesis search",
	"nosplit", 0l, FALSE, l_nosplit, "turn OFF variable splitting flag",
	"nospy", 0l, FALSE, l_nospy, "turn OFF Prolog spy points",
	"not", 1l, FALSE, NULL, "negation by failure",
	"notrace", 0l, FALSE, l_notrace, "turn OFF Prolog interpreter trace flag",
	"number", 1l, TRUE, NULL, "number(X) if X is integer or float",
	"op", 3l, FALSE, l_op, "op(Precedence,Associativity,Symbol) same as Prolog",
	"ops", 0l, FALSE, l_ops, "show present operator precedences and associativities",
	"otherwise", 0l, FALSE, NULL, "otherwise always succeeds",
	/* "right", 1l, FALSE, l_right, "right(X) - print right triangles to X",*/
	/* "rand", 0l, FALSE, l_rand, "rand generates random terms with log distribution", */
	"read", 1l, FALSE, l_read, "read(X) reads X from input",
	"read1", 1l, TRUE, l_read1, "repeat read on recall until end_of_file",
	"read", 2l, TRUE, NULL, "read(File,X) - uses read1 to find X in File",
	"reconsult", 1l, FALSE, l_reconsult, "reconsults file",
	"record", 2l, FALSE, l_record, "used by bagof to record instances",
	"reduce", 1l, FALSE, l_reduce, "reduce the given predicate definition",
	"reductive", 0l, FALSE, l_reductive, "reductive hypotheses constraint",
	"repeat", 0l, TRUE, l_repeat, "succeeds indefinitely on recall",
	"retract", 1l, FALSE, NULL, "retract(Clause) removes clause from definitions",
	"retract", 2l, FALSE, l_retract, "retract(Head,N) retracts clause number n",
	"search", 0l, FALSE, l_search, "turn ON hypothesis search",
	"see", 1l, FALSE, l_see, "see(X) opens file X for reading",
	"seen", 0l, FALSE, l_seen, "seen closes file open for reading",
	"set", 2l, FALSE, l_set, "set(Limit,IntegerValue) where Limit is one of h, i, noise, nodes or verbose",
	"settings", 0l, FALSE, l_settings, "show present parameter settings",
	"setof", 3l, FALSE, NULL, "setof(X,P,S) S is the setof X such that P(X)",
	"sort", 2l, FALSE, l_sort, "sort(L1,L2) sorts list L1 to list L2",
	"spies", 0l, FALSE, l_spies, "show present spy points",
	"split", 0l, FALSE, l_split, "turn ON variable splitting flag",
	"spy", 1l, FALSE, l_spy, "spy(Pred/Arity) place spy point on Pred",
	"stats", 0l, FALSE, l_stats, "table of blocks being used in dynamic memory",
	"tab", 1l, FALSE, l_tab, "print n spaces",
	"tell", 1l, FALSE, l_tell, "tell(X) opens file X for writing",
	"test", 1l, FALSE, l_test, "used for development testing",
	"told", 0l, FALSE, l_told, "told closes file open for writing",
	"trace", 0l, FALSE, l_trace, "turn ON Prolog interpreter trace flag",
	"true", 0l, FALSE, NULL, "true always succeeds",
	"var", 1l, FALSE, l_var, "test if argument is a variable",
	"vassert", 1l, FALSE, l_vassert, "vassert(N) virtual assertion of clause N",
	"vretract", 1l, FALSE, l_vretract, "vretract(N) virtual retraction of clause N",
	"write", 1l, FALSE, l_write, "write(X) writes X to output",
	"xy", 2l, FALSE, l_xy, "add pair to xy table and print",

        0, 0, 0, 0
};

STRING plib[] = {
	"Term=Term.",
	"X>=Y :- Y=<X.",
	"X>Y :- Y<X.",
	"X\\=Y :- not(X=Y).",
	"X\\==Y :- not(X==Y).",
	"\\+(Atom) :- not(Atom).",
	"X@>Y :- Y@<X.",
	"X@=<Y :- Y@<X;X==Y.",
	"X@>=Y :- X@>Y;X==Y.",
	"[File] :- !, reconsult(File).",
	"[File|Files] :- reconsult(File), Files.",
	"not(Atom) :- Atom, !, fail.",
	"not(_).",
	"'!'.",
	"(Atom1, Atom2) :- Atom1, Atom2.",
	"(Atom;_) :- Atom.",
	"(_;Atom) :- Atom.",
	"(Atom1->Atom2) :- Atom1, !, Atom2.",
	"any(Term).",
	"bagof(X,P,_) :- P, record(0,X), fail.",
	"bagof(_,_,Bag) :- record(1,Bag), !.",
	"bagof(_,_,[]).",
	"clause(Head,Body) :- clause(Head,Body,_).",
	"element(X,[X|_]).",
	"element(X,[_|T]) :- element(X,T).",
	"length([],0).",
	"length([H|T],N) :- length(T,N1), N is N1+1.",
	"number(X) :- int(X).",
	"number(X) :- float(X).",
	"otherwise.",
	"read(File,X) :- see(File), read1(X).",
	"retract((Head :- Body)) :- !, bagof([Head,Body,N],clause(Head,Body,N),L), element([Head,Body,N],L), retract(Head,N).",
	"retract(Atom) :- bagof([Atom,N],clause(Atom,true,N),L), element([Atom,N],L), retract(Atom,N).",
	"setof(X,P,Set) :- bagof(X,P,Bag), sort(Bag,Set).",
	"true.",
	""
};
