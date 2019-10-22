/* ####################################################################### */

/*              PROGOL Include File for External Variables		   */
/*              -----------------------------------------		   */

/* ####################################################################### */

#ifdef MEMCHECK
extern ITEM all_items;
#endif

extern LONG memout;
extern LONG pdot2;	/* Hashed symbol value of ./2 */
extern LONG pempty;	/* Hashed symbol value of [] */
extern LONG plog1;	/* Hashed symbol value of log/1 */
extern LONG pexp1;	/* Hashed symbol value of exp/1 */
extern LONG psin1;	/* Hashed symbol value of sin/1 */
extern LONG pcos1;	/* Hashed symbol value of cos/1 */
extern LONG ptan1;	/* Hashed symbol value of tan/1 */
extern LONG pceil1;	/* Hashed symbol value of ceil/1 */
extern LONG pfloor1;	/* Hashed symbol value of floor/1 */
extern LONG prand0;	/* Hashed symbol value of random/0 */
extern LONG pcputime0;	/* Hashed symbol value of cputime/0 */
extern LONG pfalse0;	/* Hashed symbol value of false/0 */
extern LONG pminus;	/* Hashed symbol value of -/0 */
extern LONG pminus1;	/* Hashed symbol value of -/1 */
extern LONG pminus2;	/* Hashed symbol value of -/2 */
extern LONG pplus;	/* Hashed symbol value of +/0 */
extern LONG pplus1;	/* Hashed symbol value of +/1 */
extern LONG pplus2;	/* Hashed symbol value of +/2 */
extern LONG por2;	/* Hashed symbol value of \/ /2 */
extern LONG pand2;	/* Hashed symbol value of /\ /2 */
extern LONG phash;	/* Hashed symbol value of #/0 */
extern LONG phash1;	/* Hashed symbol value of #/1 */
extern LONG pif;	/* Hashed symbol value of ':-'/0 */
extern LONG pcomma;	/* Hashed symbol value of ','/0 */
extern LONG pimplies2;	/* Hashed symbol value of ':-'/2 */
extern LONG ple2;	/* Hashed symbol value of '=<'/2 */
extern LONG plt2;	/* Hashed symbol value of '<'/2 */
extern LONG pge2;	/* Hashed symbol value of '>='/2 */
extern LONG pgt2;	/* Hashed symbol value of '>'/2 */
extern LONG pcomma2;	/* Hashed symbol value of ','/2 */
extern LONG pdot0;	/* Hashed symbol value of '.'/0 */
extern LONG pdiv2;	/* Hashed symbol value of '/'/2 */
extern LONG ptimes2;	/* Hashed symbol value of '*'/2 */
extern LONG ptimes0;	/* Hashed symbol value of '*'/0 */
extern LONG phat2;	/* Hashed symbol value of '^'/2 */
extern LONG pmod2;	/* Hashed symbol value of 'mod'/2 */
extern LONG peq2;	/* Hashed symbol value of '='/2 */
extern LONG pcut;	/* Hashed symbol value of !/0 */
extern LONG pquest;	/* Hashed symbol value of ?/0 */
extern LONG ptrue;	/* Hashed symbol value of true/0 */
extern FILEREC *ttyin;
extern FILEREC *ttyout;
extern FILEREC *plgin;
extern FILEREC *plgout;
extern struct hashfns superhash[];
extern ITEM nums;	/* nat nums up to MAXN represented as ITEMS */
extern ITEM ops;	/* Operators */
extern ITEM fores;	/* Skolemised foreground ground atoms */
extern ITEM bfores;	/* The bitset of foreground ground atoms */
extern ITEM catoms;	/* Asserted Skolemised atoms */
extern ITEM cle;	/* LE relation for a's in skol clause */
extern ITEM clt;	/* LT relation for a's in skol clause */
extern ITEM sple;	/* LE object superset */
extern ITEM bclauses;	/* The bitset of clauses */
extern ITEM ptab;	/* Indexed definition-table for interpreter */
extern ITEM bptab;	/* Indexed bitset definitions */
extern ITEM spsyms;	/* Super-set of predicate/function symbols */
extern ITEM spatoms;	/* Super-set of atoms */
extern ITEM spcls;	/* Super-set of clauses */
extern ITEM bmodes;	/* table of body mode declarations */
extern ITEM hmodes;	/* table of head mode declarations */
extern ITEM determs;	/* Table of determination bitsets */
extern ITEM op;		/* Bitset of operators */
extern ITEM types;	/* Bitset of types used in mode declarations */
extern ITEM lib;	/* Bitset of Prolog library predicates */
extern ITEM lib1;	/* Bitset of C+Prolog library predicates */
extern ITEM allpsyms;	/* Bitset of all predicate symbols */
extern ITEM psymtlib;	/* Mapping from predicate symbol to lib function no*/
extern ITEM labels;	/* Labels for stochastic LPs */
extern ITEM hypothesis;	/* A clause */
extern ITEM hyp1;	/* Hypothesis being tested */
extern LONG vlim,pnlim,hlim,noiselim,clim;
extern LONG bitspos[BYTE_RNG] [10];
			/* Bit increment table */
extern unsigned char byte_sz[BYTE_RNG];
extern unsigned char llog2[BYTE_RNG]; /* Log table */
extern LONG verbose;	/* Verbosity level */
extern PREDICATE reconsult; /* Reconsulting file? */
extern PREDICATE mseen;	/* Modes seen? */
extern PREDICATE pdebug; /* Prolog trace debug flag */
extern PREDICATE interactive; /* Interactive session? */
extern LONG charlast; /* Used for printing terms */
extern ITEM comms;	/* Commands read from files */
extern PREDICATE memo; /* Search memo flag */
extern PREDICATE trace; /* Prolog interpreter trace flag */
extern PREDICATE noquotes; /* No quotes on Prolog constants */
extern PREDICATE vsplit; /* Prolog variable splitting flag */
extern PREDICATE reductive; /* Reductive constraint */
extern ITEM spies;	/* Spy points */
extern ITEM commutes;	/* Commutative predicates */
extern ITEM repeats;	/* Repeat library predicates */
extern ITEM indexed;	/* Predicates indexed on first arg */
extern char *stack;	/* Prolog interpreter stack */
extern LONG stack_size; /* Size of Prolog stack */
extern LONG varno;	/* Present variable number */
extern ITEM fileroot;	/* Progol fileroot from UNIX command level */
extern ITEM idot0;	/* . */
extern ITEM fhead;	/* False atom */
extern ITEM eof;	/* end_of_file atom */
extern ITEM gcall;	/* Ground calling clause */
extern CALL_ENV env_stack0; /* Start point for environment stack */
extern char *term_stack0;/* Start point for term stack */
extern PREDICATE namecmp;/* Name comparison in i_cmp */
extern LONG *hxy[],hxyn;/* Fast hash table */
extern ITEM pftab;	/* Pred/func sym indexed clause list */
extern ITEM pvtab;	/* Pred/var indexed clause list */
extern ITEM ptopf;	/* Pred->pred/func mapping */
extern ITEM cclauses;	/* Table of clauses */
extern ITEM osplit;	/* Tabulated variable partitions */
extern PREDICATE searchq;/* Hypothesis search? */
