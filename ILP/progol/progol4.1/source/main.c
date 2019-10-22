/* ####################################################################### */

/*			PROGOL main functions				   */
/*			--------------------				   */

/* ####################################################################### */

#include <stdio.h>
#include "progol.h"

FILEREC *ttyin;
FILEREC *ttyout;
PREDICATE interactive=TRUE;
ITEM fileroot=(ITEM)NULL;
PREDICATE c_doall();

#ifdef SUNCHECK
main(argc,argv)
	LONG argc;
	STRING argv[];
	{
	printf("CProgol Version 4.1\n\n");
#ifdef	CHECK_SECURITY
	check_security();
#endif
	c_open();
	checkargs(argc,argv);
	l_init();			/* Initialise built-in predicates */
	if(interactive) main_prompt();
	else c_doall(fileroot);
	c_close();
}
#else
main()
	{
#ifdef	CHECK_SECURITY
	check_security();
#endif
	c_open();
	checkargs(argc,argv);
	l_init();			/* Initialise built-in predicates */
	main_prompt();
	c_close();
}
#endif

extern CALL_ENV env_stack0;

/*
 * checkargs/2 - expects command line
 *	progol [-i] fileroot
 */

checkargs(argc,argv)
	LONG argc;
	STRING argv[];
	{
	LONG argno,n;
	STRING sp;
	for(argno=1;argno<argc;argno++) {
	    if(*argv[argno] == '-') {
		STR_LOOP(sp,argv[argno]+1l)
		    switch(*sp) {
			case 's': /* Prolog stack size */
			    if(++argno<argc && sscanf(argv[argno],"%ld",&n))
			      stack_size=(n>=0l?n:0l);
			    else printf("[Bad stack size]\n");
			    break;
			case 'v': /* Verbose flag */
			    verbose=2l;
			    break;
			default:
			    printf("[Unrecognised flag option <%c>]\n",*sp);
		    }
	    }
	    else if (fileroot==(ITEM)NULL) {
		fileroot=i_create('h',(POINTER)QP_ston(argv[argno],0l));
		interactive=FALSE;
	    }
	    else printf("[Ignoring additional file <%s>]\n",argv[argno]);
	}
        if (!(stack=(char *)malloc(stack_size*sizeof(char))))
		d_error("checkargs - calloc failed");
        ALIGN8(stack);
	term_stack0=stack;
	env_stack0=((CALL_ENV)(stack+stack_size))-1l;
}
