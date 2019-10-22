#include <stdio.h>
#include "progol.h"
#include <math.h>

/*
 * #######################################################################
 *
 *			Command Routines
 *                      ----------------
 *
 * #######################################################################
 */

PREDICATE c_randseed();
PREDICATE c_fixedseed();

struct com commands[] = {
	"fixedseed", 0l, c_fixedseed, "initialises random number generator seed to 1",
	"randseed", 0l, c_randseed, "intitialises random number generator seed using time of day",

        0, 0, 0, 0
};

main_prompt()
	{
	ITEM c;
	char mess[MAXMESS],mess1[MAXMESS];
	float start;
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
	LONG type;
	STRING vname;
	if(cclause==(ITEM)I_ERROR)
	  return;
	if((type=(LONG)CTYPE(cclause))==pdot0) {	/* Assertion */
	  d_creduce(cclause);
	  printf("<Reduced clause is ");
	  ccl_fwrite(ttyout,cclause,NULL);
	  printf(">\n");
	  if(cl_assert(cclause,FALSE,TRUE,TRUE,FALSE,(ITEM)NULL)) {
	    printf("[<"); ccl_fwrite(ttyout,cclause,NULL);
	    printf("> added to clauses]\n");
	  }
	  CONTRA
	  return;
	}
	else if((type)==pcut) {			/* Example */
	  CTYPE(cclause)=(POINTER)pdot0;
	  cl_assert(cclause,FALSE,TRUE,TRUE,FALSE,(ITEM)NULL);
	  CONTRA
	  c_sat(cclause1=i_copy(cclause));
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
		  if(STREQ(vname=(STRING)I_GET(*entry),"_")) continue;
		  if(!first) printf("\n");
		  else first=FALSE;
		  printf("%s = ",(STRING)I_GET(*entry));
		  p_fwritesub(ttyout,*fptr,(BIND)term_stack0);
		}
		printf(" ");
		if(ttychline(ttyin)!= ';') {
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



extern ITEM ct_sat(),cl_vrenum(),r_outlook(),r_vdomains();

PREDICATE
c_sat(cclause)
	ITEM cclause;
	{
	ITEM atoio=F_EMPTY,otoa=F_EMPTY,head=(ITEM)NULL,hypothesis,
		retract,clause=F_ELEM(0l,cclause),outlook,cclause1,vdomains;
	LONG psym;
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
	  if(searchq) {
	    retract=d_pushfores(psym=PSYM(HOF((LIST)I_GET(clause))));
	    r_search(&hypothesis,atoio,otoa,outlook,vdomains);
	    if(hypothesis&& !L_EMPTYQ(hypothesis)) {
	      cl_unflatten(&hypothesis);
	      if(verbose>=2) {
	        g_message(2l,"Result of search is");
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
	        cl_vrenum(hypothesis);
	        cclause1=i_tup4(hypothesis,i_dec(I_INT(varno)),
			  idot0,i_dec(F_EMPTY));
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

extern ITEM ct_bestpr();


PREDICATE
c_doall(froot)
	ITEM froot;
	{
	STRING name;
	char mess[MAXMESS];
	LONG psym=0l;
	ITEM *hmode;
	float start;
	searchq=TRUE;
	if(froot->item_type== 'h')
		name=QP_ntos((STRING)I_GET(froot));
	else return(FALSE);
	sprintf(mess,"%s.pl",name);
	if(cl_readrls(mess,FALSE)) {
		/* Generalise each predicate in hmodes */
	  FUNC_DO(hmode,hmodes)
	    if(*hmode) {
	      start=cputime();
	      c_gen1(psym,"");
	      sprintf(mess,"Time taken %.3fs",cputime()-start);
	      g_message(1l,mess);
	    }
	    psym++;
	  FUNC_END
	}
	return(TRUE);
}

PREDICATE
c_randseed()
	{
	SRAND(datetime());
	printf("[The random number generator has been reset using the time of day]\n");
	return(TRUE);
}

PREDICATE
c_fixedseed()
	{
	SRAND(1);
	printf("[The random number generator has been reset to 1]\n");
	return(TRUE);
}
