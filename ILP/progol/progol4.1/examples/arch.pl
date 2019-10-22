%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Patrick Winston's arch problem. Learns a definition for an arch
%	in terms of support and touching relationships.
%
%							   / \
%	 ______				 _______	  /   \
%	|  11  |			|   31  |	 /  41 \
%	 ~~~~~~				 ~~~~~~~ 	 ~~~~~~~
%	|~|  |~|	|~|   |~|	  | | |		|~|   |~|
%	12|  |13	22|   |23	  32|33		42|   |43
%	| |  | |	|_|___|_|	  | | |		| |   | |
%	| |  | |	|   21  |	  | | |		| |   | |
%	 ~    ~          ~~~~~~~	   ~ ~		 ~     ~
%
%							   / \
%	 ______		    _______	_______  	  /   \
%	|  51  |	   |   61  |   |   71  |	 /  81 \
%	 ~~~~~~		    ~~~~~~~     ~~~~~~~  	 ~~~~~~~
%	|~|  |~|	|~|   |~|	  |~|   |~|	|~|   |~|
%	52|  |53	62|   |63	  72|   |73	82|   |83
%	| |  | |	| |   | |	  | |   | |	| |   | |
%	| |  | |	| |   | |	  | |   | |	| |   | |
%	 ~    ~          ~     ~	   ~     ~	 ~     ~

:- modeh(1,arch(+block,+block,+block))?
:- modeb(1,left_of(+block,+block))?
:- modeb(1,supports(+block,+block))?
:- modeb(1,not touches(+block,+block))?
:- modeb(1,brick(+block))?
:- modeb(1,wedge(+block))?
:- modeb(1,block(+block))?
:- op(40,xfx,left_of)?
:- op(40,xfx,supports)?
:- op(40,xfx,touches)?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Type information

block(Block) :- brick(Block).
block(Block) :- wedge(Block).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Examples

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Arch 1
%	 ______	
%	|  11  |
%	 ~~~~~~	
%	|~|  |~|
%	12|  |13
%	| |  | |
%	| |  | |
%	 ~    ~ 

arch(b11,b12,b13).

brick(b11).  brick(b12).  brick(b13).

b12 left_of b13.

b12 supports b11.  b13 supports b11.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Arch 2
%	|~|   |~|
%	22|   |23
%	|_|___|_|
%	|   21  |
%	 ~~~~~~~

:- arch(b22,b23,b24).

brick(b22).  brick(b23).  brick(b24).

b23 left_of b24.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Arch 3
%	 _______
%	|   31  |
%	 ~~~~~~~
%	  | | |
%	  32|33	
%	  | | |
%	  | | |
%	   ~ ~

:- arch(b31,b32,b33).

brick(b31).  brick(b32).  brick(b33).

b32 left_of b33.

b32 supports b31.  b33 supports b31.

b32 touches b33.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Arch 4
%          / \
%	  /   \
%	 /  41 \
%	 ~~~~~~~
%	|~|   |~|
%	42|   |43
%	| |   | |
%	| |   | |
%	 ~     ~

arch(b41,b42,b43).

brick(b42).  brick(b43).

b42 left_of b43.

b42 supports b41.  b43 supports b41.

wedge(b41).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Arch 5
%	 ______
%	|  51  |
%	 ~~~~~~	
%	|~|  |~|
%	52|  |53
%	| |  | |
%	| |  | |
%	 ~    ~

arch(b51,b52,b53).

brick(b51).  brick(b52).  brick(b53).

b52 left_of b53.

b52 supports b51.  b53 supports b51.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Arch 6
%	    _______
%	   |   61  |
%	    ~~~~~~~
%	|~|   |~|
%	62|   |63
%	| |   | |
%	| |   | |
%	 ~     ~

:- arch(b61,b62,b63).

brick(b61).  brick(b62).  brick(b63).

b62 left_of b63.

b63 supports b61.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Arch 7
%	 _______
%	|   71  |
%	 ~~~~~~~
%	   | |   |~|
%	   72|   |73
%	   | |   | |
%	   | |   | |
%	    ~     ~

:- arch(b71,b72,b73).

brick(b71).  brick(b72). brick(b73).

b72 left_of b73.

b72 supports b71.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Arch 8
%          / \
%	  /   \
%	 /  81 \
%	 ~~~~~~~
%	|~|   |~|
%	82|   |83
%	| |   | |
%	| |   | |
%	 ~     ~

arch(b81,b82,b83).

brick(b82).  brick(b83).

b82 left_of b83.

b82 supports b81.  b83 supports b81.

wedge(b81).
