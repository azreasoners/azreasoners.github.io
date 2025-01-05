/*********************************************************************
    f2lp.c - This program turns first order formulas to 
             answer set programs under the stable model semantics.    

    version 1.0.1         4-Oct-2010

    Copyright (C) <2009>  <Joohyung Lee and Ravi Palla>
                All rights reserved.
    For information on how to contact the authors, please visit
    "http://reasoning.eas.asu.edu/f2lp".

    The authors hereby grant USER nonexclusive permission 
    to use, copy and/or modify this software for internal, 
    noncommercial, research purposes only. Any distribution, 
    including commercial sale or license, of this software, 
    copies of the software, its associated documentation and/or 
    modifications of either is strictly prohibited without the 
    prior consent of authors.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the complete copyright and warranty information, please read
    the "COPYRIGHT" file provided along with this software.
*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define VERSION "f2lp version 1.0"
#define USAGE "f2lp [input_file_1] . . . [input_file_n]\n"
#define NOINPUTFILE "if no input files are provided, then STDIN is considered.\n" 
#define OPTION_HELP "usage information: -h --h -help --help\n"
#define OPTION_VERSION "version number: -v --v -version --version\n"

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif


#ifndef MAX_PREDICATE_LENGTH
#define MAX_PREDICATE_LENGTH 500
#endif

/*
* maximum symbols other than alnum in a formula 
*/
#ifndef MAX_SYMBOLS
#define MAX_SYMBOLS 500
#endif

/*
* input to solver 
*/
FILE           *fpSolverInput = NULL; 

struct stack {
int             top;
char            item[MAX_SYMBOLS];
};

struct node {
struct node    *left;
struct node    *right;
char            val[MAX_PREDICATE_LENGTH];
int             prime;
};

#ifdef BAR_NEG_ATOM
struct baratom {
char            val[MAX_PREDICATE_LENGTH];
struct baratom *next;
};

struct negatom {
char            val[MAX_PREDICATE_LENGTH];
struct negatom *next;
};
#endif

struct qf {
	char *ptr;
	struct qf *next;
};

typedef struct node *NODEP;
#ifdef BAR_NEG_ATOM
struct negatom *root_negatom = NULL;
struct negatom *prev_negatom = NULL;

struct baratom *root_baratom = NULL;
struct baratom *prev_baratom = NULL;
#endif

struct qf *root_qf = NULL;

NODEP           root_prefix = NULL;


/*
* added these 
*/
int             reading_index = 0;
int 		other_found = 0; //for considering associativity
void            applyRules(NODEP root);
void            checkRuleForm(NODEP root, int *rule_flag,
	int ant_cons_flag);
void            format_and_write(NODEP root);
void            concat(char *str1, char *str2, char *dst);
int 		isPredicateSymbol(char c);
/*
* until here 
*/

void            push(struct stack *ps, char n);
void            pop(struct stack *ps);
int             isempty(struct stack *ps);
int             prcd(char stktop, char op);
void            toPrefix(char *input, char *output);
void            eliminate_imp(NODEP root);
int             index_prefix = 0;
#ifdef BAR_NEG_ATOM
int             no_baratoms = 0;
#endif
int 		var_index = 1;
int 		pred_index = 1;
int		added_dom = 0;


/*************
DEBUG info
*************/
int             debug_count = 0;

NODEP
prefix_to_tree(char *prefix, int dummy)
{
char            nextchar = (char) NULL;
char            litchar = (char) NULL;
int             i = 0;
int 		temp_index = 0;

NODEP           newnode = (NODEP) malloc(sizeof(struct node));
newnode->left = NULL;
newnode->right = NULL;

#if 0 //will use convert_NNF instead
if (dummy == 1) {
/*
* add false 
*/
newnode->val[0] = 'f';
newnode->val[1] = 'a';
newnode->val[2] = 'l';
newnode->val[3] = 's';
newnode->val[4] = 'e';
newnode->val[5] = '\0';

return newnode;
}
#endif
nextchar = prefix[index_prefix++];

/*
* Skip spaces. 
*/
// while((nextchar == ' ') || (nextchar == '\t'))
// nextchar = prefix[index++];
/*
* Echo the character so we have a record. 
*/
// putchar(nextchar);

if (nextchar == '\0') {
fprintf(stderr, "Premature end of formula.\n");
exit(1);
}
/*
* for negation - if negative literal, then do not add left child 
  
*/
//litchar = prefix[index_prefix]; //will use convert_NNF instead

/*
* If it's a variable, it is a prefix expression by itself. 
*/
if (isPredicateSymbol(nextchar)) {
do {
newnode->val[i++] = nextchar;
nextchar = prefix[index_prefix++];
} while (nextchar != '$');

/*
if (nextchar == '*') {
// printf("star atom %c\n", newnode->val[0]);
newnode->prime = 1;
} else {
newnode->prime = 0;
}*/
newnode->val[i] = '\0';

return newnode;
}

/*
* Else - must be an operator. 
*/
// printf("char %c\n",nextchar);
#if 0
if (litchar == '\0') {
fprintf(stderr, "Premature end of formula.\n");
exit(1);
}
#endif
/*
* Find the next complete prefix expression, then the one after that,
* and stick it all together. 
*/
#if 0
if (nextchar != '-') {
newnode->left = prefix_to_tree(prefix, 0);
}
#endif
/*
* if negative literal, then do not add left child 
*/
if ( (nextchar != '-') && 
	((nextchar != '!') && (nextchar != '?')) ) {
// printf("adding left child\n");
newnode->left = prefix_to_tree(prefix, 0);
}
if ( (nextchar == '!') || (nextchar == '?') )
{
	temp_index = index_prefix-1;
	/* proceed to char after quantifier */
	while(prefix[index_prefix] != ':')
		index_prefix++;
	
	index_prefix++;
}
//if ((nextchar != '-') || isPredicateSymbol(litchar)) {
// printf("adding right child with 0\n");
newnode->right = prefix_to_tree(prefix, 0);
//}
#if 0
else {
// printf("adding right child with 1\n");
newnode->right = prefix_to_tree(prefix, 1);
}
#endif
/*
* Turn negation to implication 
*/
if ( (nextchar == '!') || (nextchar == '?') )
{
	/* proceed to char after quantifier */
	while(prefix[temp_index] != ':')
	{
		newnode->val[i++] = prefix[temp_index++];
	}
	
	newnode->val[i++] = ':';
	newnode->val[i] = '\0';
}
else
{
	newnode->val[0] = nextchar;
	newnode->val[1] = '\0';
}
return newnode;
}


void
postfix(NODEP tree)
{
if (tree == NULL)
	return;
/*
* Print everything to left, then everything to right, then content of 
* this node. 
*/
postfix(tree->left);
postfix(tree->right);
// printf("%c",tree -> val[0]);
}

void
inorder_print(NODEP tree)
{
if (tree == NULL)
	return;

if (!isPredicateSymbol(tree->val[0])
	&& !isPredicateSymbol((char) ((int) tree->val[0] - 100))) {
	printf("%c", '(');
}
inorder_print(tree->left);
printf("%c", tree->val[0]);
inorder_print(tree->right);
if (!isPredicateSymbol(tree->val[0])
	&& !isPredicateSymbol((char) ((int) tree->val[0] - 100))) {
	printf("%c", ')');
}
}

void
inorder_print_nobr(NODEP tree)
{
if (tree == NULL)
	return;

inorder_print_nobr(tree->left);
printf("%c", tree->val[0]);
inorder_print_nobr(tree->right);
}



void
inorder_save_br(NODEP tree, char *infix)
{
	int             i = 0;
	if (tree == NULL)
		return;

	if ( (tree->left != NULL) || (tree->val[0] == '-') || 
		     (tree->val[0] == '!') ||  (tree->val[0] == '?') )
	{
		infix[reading_index++] = '(';
	}
		    		

	inorder_save_br(tree->left, infix);
	for (i = 0; tree->val[i] != '\0'; i++) {
		infix[reading_index++] = tree->val[i];
		    }
	
	infix[reading_index] = '\0';
	inorder_save_br(tree->right, infix);

	
	if ( tree->right != NULL )
	{
		infix[reading_index++] = ')';
	}
	
	infix[reading_index] = '\0';
}






void
inorder_save(NODEP tree, char *infix)
{
int             i = 0;
if (tree == NULL)
	return;
	
inorder_save(tree->left, infix);
for (i = 0; tree->val[i] != '\0'; i++) {
	infix[reading_index++] = tree->val[i];
}
infix[reading_index] = '\0';
inorder_save(tree->right, infix);
}


void
push(struct stack *ps, char n)
{
ps->top++;
ps->item[ps->top] = n;
}

void
pop(struct stack *ps)
{
ps->top--;
}

int
isempty(struct stack *ps)
{
if (ps->top == -1) {
	return (1);
} else {
	return (0);
}
}

int
prcd(char stktop, char op)
{

if ((stktop == ':') && ((op == '&') || (op == '|'))) {
	return 0;
}
if (((stktop == '&') || (stktop == '|')) && (op == ':')) {
	return 1;
}
#if 0
if (stktop == op) {
	return 0;
}
#endif

if (stktop == ')') {
	return 0;
}
#if 0
if ((stktop != '(') && (op == ')')) {
	return 0;
}
#endif

if (op == '(') {
	return 1;
}

if ( (stktop == '-') || (stktop == '_') ) {
	return 1;
}

if ( (op == '-') || (op == '_') ) {
	return 0;
}
return 0;
}

void
toPrefix(char *input, char *output)
{
struct stack    s;
char            optop,
		ans;
char           *p = input;
char 		op = (char)0;
int             i = 0,
	j = 0,
	k = 0,
	m;
int qf_index = 0, qf_base = 0;
struct qf *temp = NULL, *temp1 = NULL;
// int tuple_start = 0;
int             paran_count = 0;	// to take care of reification
s.top = -1;


while (*p != '\0') {
	p++;
	i++;
}
i--;			/* now size of infix expression is known
				* and counter 'i' indicates to end of
				* expression */

p = input;
while (*p != '\0') {
	if (p != input) {
	/*
	* indicate tuple start 
	*/
	if (((*p == '(') && isPredicateSymbol(*(p - 1))) && (paran_count == 0)) {
		// tuple_start = 1;
		paran_count = 1;
		j++;
		p++;
		continue;
	}
	}



	if (((*p == '(') && (paran_count == 0)) ||
	((*p == ')') && (paran_count == 0))) {

	p++;
	} else if ((*p == '(') && (paran_count != 0)) {
	paran_count++;
	j++;
	p++;
	} else if ((*p == ')') && (paran_count != 0)) {
	paran_count--;
	j++;
	p++;
	} else {
	j++;
	p++;
	}
}				/* now size of postfix expression is known 
				*/
m = j;
j--;			/* now j indicates end of prefix
				* expression */

// printf("infix %s with j val i val %d %d\n", input,j,i);



while (i >= 0) {

	if ((input[i] == ')')
	&& ((input[i + 1] == '$'))) {
	// tuple_start = 1;
	paran_count = 1;
	output[j] = input[i];
	j--;
	} else if ((isPredicateSymbol(input[i])) ||
		(input[i] == '$') ||
		(paran_count != 0)) {
	if (input[i] == '(') {
		// tuple_start = 0;
		paran_count--;
	}

	if (input[i] == ')') {
		// tuple_start = 0;
		paran_count++;
	}
	output[j] = input[i];
j--;
} else {
	/* check for quantifiers */
	if ((input[i] == ':') && (input[i-1] == ']') )
		op = '_';
	else
		op = input[i];

/* if stack top has more precedence */	 
while (!isempty(&s) && (prcd(s.item[s.top], op)))  
{
optop = s.item[s.top];
pop(&s);
//printf("popping %c op %c\n",optop,input[i]);
/* if stack top is _, then read the whole quantifier */
if ( optop == '_' )
{
	qf_index = 0;
	while(root_qf->ptr[qf_index] != '\0')
	{
		qf_index++;
	}
	qf_index--;
	while ( qf_index >= 0 )
		output[j--] = root_qf->ptr[qf_index--];
	root_qf = root_qf->next;
}
else
{
	output[j--] = optop;
}

}
if (isempty(&s) || input[i] != '(') {
	if ((input[i] == ':') && (input[i-1] == ']') )
	{
		//printf("pushing _\n");
		push(&s,'_');
		qf_index = i;
		while( (input[qf_index] != '!') && (input[qf_index] != '?') )
		{
			qf_index--;
		}
		/* store quantifier in the list */
		temp = (struct qf *) malloc(sizeof (struct qf));
		temp->ptr = (char *)malloc(i-qf_index+2);
		temp->ptr[i-qf_index+1] = '\0';
		temp->next = NULL;
		qf_base = qf_index;
		while( qf_index <= i )
		{
			temp->ptr[qf_index-qf_base] = input[qf_index];
			qf_index++; 
		}
		if ( root_qf == NULL )
		{
			root_qf = temp;
		}
		else
		{
			temp1 = root_qf;
			while(temp1->next != NULL)
				temp1 = temp1->next;
			
			temp1->next = temp;
		}
		/* ignore the rest of the quantifier */
		while ( (input[i] != '!') && (input[i] != '?') )
			i--;
	}
	else
	{
		//printf("pushing %c\n",input[i]);
		push(&s, input[i]);
	}
} else {
//printf("popping\n");
pop(&s);
}
}

i--;
}

while (!isempty(&s)) {
optop = s.item[s.top];
pop(&s);
//printf("popping %c\n",optop);
if ( optop == '_' )
{
	qf_index = 0;
	while(root_qf->ptr[qf_index] != '\0')
	{
		qf_index++;
	}
	qf_index--;
	while ( qf_index >= 0 )
		output[j--] = root_qf->ptr[qf_index--];
	root_qf = root_qf->next;
}
else
{
	output[j--] = optop;
}

}
// printf("\nprefix form:\t");

output[m] = '\0';
}

#ifdef BAR_NEG_ATOM
void
convert_neg_lp(char *inF, int lineNum, int *size_incr)
{
int             i = 0;
int             j = 0, k = 0;
int             neg_exist = 0;

struct negatom *newnode = NULL;
struct negatom *negnode = NULL;

int             neg_index = 0;
int             tuple_start = 0;
int             paran_count = 0;

for (i = 0; inF[i] != '\0'; i++) {

if ( inF[i] == '-' ) {
if ( i > 0 && inF[i-1] == ':' )
continue;

if (!isPredicateSymbol(inF[i + 1])) {
printf
("line %d - Strong negation can only be applied to an atom\n",
lineNum + 1);
exit(1);
}
/*
* create atoms ending with 'neg' 
*/
for (j = i; inF[j] != '\0'; j++) {
inF[j] = inF[j + 1];
}

newnode = (struct negatom *) malloc(sizeof(struct negatom));
newnode->next = NULL;

k = 0;
/*
* read the entire atom 
*/
while (isPredicateSymbol(inF[i])) {
newnode->val[k++] = inF[i++];
}
neg_index = i;
if (inF[i] == '(') {
paran_count = 1;
while (paran_count != 0) {
newnode->val[k++] = inF[i++];
if (inF[i] == '(')
paran_count++;
if (inF[i] == ')')
paran_count--;
}
newnode->val[k++] = ')';
// i++;
}

newnode->val[k] = '\0';
neg_exist = 0;
for (negnode = root_negatom; negnode != NULL;
negnode = negnode->next) {
if (!strcmp(negnode->val, newnode->val)) {
neg_exist = 1;
}
}

if (!neg_exist) {
if (prev_negatom != NULL) {
prev_negatom->next = newnode;
prev_negatom = newnode;
} else {
root_negatom = newnode;
prev_negatom = root_negatom;
}
}


/*
* shift all char right by 3 places 
*/
for (j = neg_index; inF[j] != '\0'; j++);
for (; j >= neg_index; j--) {
inF[j + 3] = inF[j];
}
/*
* add 'neg' 
*/
inF[neg_index++] = 'n';
inF[neg_index++] = 'e';
inF[neg_index] = 'g';
*size_incr = *size_incr+2;
// inF[i] = '~';
}

}

}
#endif

int isParan( char c )
{
	if ( (c == '(') || (c == ')' ) )
		     return 1;
	
	return 0;
}



void replaceSpecialPred ( char *inF, int lineNum )
{
	int start_index = -1;
	int i = 0, j = 0;
	int le = 0, ge = 0;
	int compare = 0;
	int paran_count = 0;
	
	for ( i = 0; inF[i] != '\0'; i++ )
	{
		compare = 0;
		switch (inF[i])
		{
			case '=':
				paran_count = 0;
				if ( i == 0 || (!isPredicateSymbol(inF[i-1]) && !isParan(inF[i-1])) )
				{
					printf("parse error at line %d\n",lineNum+1);
					exit(1);
				}
				
				if ( (!isPredicateSymbol(inF[i+1]) && !isParan(inF[i+1])) && (inF[i+1] != '=') )
				{
					printf("parse error at line %d\n",lineNum+1);
					exit(1);
				}
				
				if ( inF[i+1] == '=' )
					compare = 1;
				
				start_index = i-1;
				while ( (isPredicateSymbol(inF[start_index]) ||
					 isParan(inF[start_index]) || 
					 (paran_count != 0)) && 
					(start_index != 0))
				{
					if ( inF[start_index] == ')' )
						paran_count--;
					if ( inF[start_index] == '(' )
					{ 
						if (paran_count == 0)
							break;
						
						paran_count++;
					}
					start_index--;
				}
				
				if ( start_index == 0 )
				{
					if ( (paran_count == 0) && (inF[start_index] == '(') )
						start_index = 0;
					else
						start_index = -1;
				}
				
				j = i;
				/* shift by 4 characters */
				while ( inF[j] != '\0' )
					j++;
				
				while ( j != start_index )
				{
					if ( compare == 1 )
					{
						inF[j+3] = inF[j];
					}
					else
					{
						inF[j+4] = inF[j];
					}
					j--;
				}
				
				j++;
				inF[j++] = 'e';
				inF[j++] = 'q';
				inF[j++] = '(';
				
				if ( compare == 0 )
				{
					while ( inF[j+1] != '=' )
					{
						inF[j] = inF[j+1];
						j++;
					}
				}
				else
				{
					while ( inF[j] != '=' )
						j++;
				}
				
				inF[j] = ',';
				
				
				if ( !isPredicateSymbol(inF[j+2]) && !isParan(inF[j+2]) )
				{
					printf("parse error at line %d\n",lineNum+1);
					exit(1);
				}
				j++;
				
				paran_count = 0;
				while ( isPredicateSymbol(inF[j+1]) || isParan(inF[j+1]) || (paran_count != 0))
				{
					if ( inF[j+1] == ')' )
					{
						if ( paran_count == 0 )
							break;
						paran_count--;
					}
					
					if ( inF[j+1] == '(' )
					{ 
						paran_count++;
					}
					inF[j] = inF[j+1];
					j++;
				}
				inF[j] = ')';
				i = j+1;
				break;
				
			case '!':
				paran_count = 0;
				if ( inF[i+1] == '[' )
					break;
				if ( i == 0 || (!isPredicateSymbol(inF[i-1]) && !isParan(inF[i-1])) || (inF[i+1] != '=') )
				{
					printf("parse error at line %d\n",lineNum+1);
					exit(1);
				}
				start_index = i-1;
				while ( (isPredicateSymbol(inF[start_index]) || 
					 isParan(inF[start_index]) ||
					 (paran_count != 0)) && 
					(start_index != 0) )
				{
					if ( inF[start_index] == ')' )
						paran_count--;
					if ( inF[start_index] == '(' )
					{ 
						if (paran_count == 0)
							break;
						
						paran_count++;
					}
					start_index--;
				}
				
				if ( start_index == 0 )
				{
					if ( (paran_count == 0) && (inF[start_index] == '(') )
						start_index = 0;
					else
						start_index = -1;
				}
				
				j = i;
				/* shift by 4 characters */
				while ( inF[j] != '\0' )
					j++;
				
				while ( j != start_index )
				{
					inF[j+4] = inF[j];
					j--;
				}
				
				j++;
				inF[j++] = 'n';
				inF[j++] = 'e';
				inF[j++] = 'q';
				inF[j++] = '(';
				
				while ( inF[j] != '!' )
					j++;
				
				
				inF[j] = ',';
				if ( !isPredicateSymbol(inF[j+2]) && !isParan(inF[j+2]) )
				{
					printf("parse error at line %d %c\n",lineNum+1,inF[j+2]);
					exit(1);
				}
				j++;
				paran_count = 0;
				while ( isPredicateSymbol(inF[j+1]) || isParan(inF[j+1]) || (paran_count != 0))
				{
					if ( inF[j+1] == ')' )
					{
						if ( paran_count == 0 )
							break;
						paran_count--;
					}
					
					if ( inF[j+1] == '(' )
					{ 
						paran_count++;
					}
					inF[j] = inF[j+1];
					j++;
				}
				inF[j] = ')';
				i = j+1;
				break;
				
			case '<':
				paran_count = 0;
				if ( i == 0 || (!isPredicateSymbol(inF[i-1]) && !isParan(inF[i-1])) || 
								 ((inF[i+1] != '=') && (!isPredicateSymbol(inF[i+1]) && !isParan(inF[i+1])) ) )
				{
					printf("parse error at line %d\n",lineNum+1);
					exit(1);
				}
				start_index = i-1;
				while ( (isPredicateSymbol(inF[start_index]) || 
					 isParan(inF[start_index]) ||
					 (paran_count != 0)) && 
					(start_index != 0))
				{
					if ( inF[start_index] == ')' )
						paran_count--;
					if ( inF[start_index] == '(' )
					{ 
						if (paran_count == 0)
							break;
						
						paran_count++;
					}
					start_index--;
				}
				
				if ( start_index == 0 )
				{
					if ( (paran_count == 0) && (inF[start_index] == '(') )
						start_index = 0;
					else
						start_index = -1;
				}
				
				j = i;
				
				while ( inF[j] != '\0' )
					j++;
				/* shift by 4 characters if < */
				if ( isPredicateSymbol(inF[i+1]) || isParan(inF[i+1]) )
				{
					le = 0;
					while ( j != start_index )
					{
						inF[j+4] = inF[j];
						j--;
					}
				}
				else /* shift by 3 characters */
				{
					le = 1;
					while ( j != start_index )
					{
						inF[j+3] = inF[j];
						j--;
					}
				}
				
				j++;
				inF[j++] = 'l';
				if ( le == 1 )
					inF[j++] = 'e';
				else
					inF[j++] = 't';
				
				inF[j++] = '(';
				
				if ( le == 0 )
				{
					while ( inF[j+1] != '<' )
					{
						inF[j] = inF[j+1];
						j++;
					}
				}
				else
				{
					while ( inF[j] != '<' )
						j++;
				}
				
				
				inF[j] = ',';
				if ( !isPredicateSymbol(inF[j+2]) && !isParan(inF[j+2]) )
				{
					printf("parse error at line %d %c\n",lineNum+1,inF[j+2]);
					exit(1);
				}
				j++;
				paran_count = 0;
				while ( isPredicateSymbol(inF[j+1]) || isParan(inF[j+1]) || (paran_count != 0))
				{
					if ( inF[j+1] == ')' )
					{
						if ( paran_count == 0 )
							break;
						paran_count--;
					}
					
					if ( inF[j+1] == '(' )
					{ 
						paran_count++;
					}
					inF[j] = inF[j+1];
					j++;
				}
				inF[j] = ')';
				i = j+1;
				break;
			case '>':
				paran_count = 0;
				if ( i == 0 )
				{
					printf("parse error at line %d\n",lineNum+1);
					exit(1);
				}
				if ( inF[i-1] == '-' )
					break;
				
				if ( i == 0 || (!isPredicateSymbol(inF[i-1]) && !isParan(inF[i-1])) || 
								 ((inF[i+1] != '=') && (!isPredicateSymbol(inF[i+1]) && !isParan(inF[i+1])) ) )
				{
					printf("parse error at line %d\n",lineNum+1);
					exit(1);
				}
				start_index = i-1;
				while ( (isPredicateSymbol(inF[start_index]) || 
					 isParan(inF[start_index]) ||
					 (paran_count != 0)) && 
					(start_index != 0))
				{
					if ( inF[start_index] == ')' )
						paran_count--;
					if ( inF[start_index] == '(' )
					{ 
						if (paran_count == 0)
							break;
						
						paran_count++;
					}
					start_index--;
				}
				
				if ( start_index == 0 )
				{
					if ( (paran_count == 0) && (inF[start_index] == '(') )
						start_index = 0;
					else
						start_index = -1;
				}
				
				j = i;
				
				while ( inF[j] != '\0' )
					j++;
				/* shift by 4 characters if < */
				if ( isPredicateSymbol(inF[i+1]) || isParan(inF[i+1]) )
				{
					ge = 0;
					while ( j != start_index )
					{
						inF[j+4] = inF[j];
						j--;
					}
				}
				else /* shift by 3 characters */
				{
					ge = 1;
					while ( j != start_index )
					{
						inF[j+3] = inF[j];
						j--;
					}
				}
				
				j++;
				inF[j++] = 'g';
				if ( ge == 1 )
					inF[j++] = 'e';
				else
					inF[j++] = 't';
				
				inF[j++] = '(';
				
				if ( ge == 0 )
				{
					while ( inF[j+1] != '>' )
					{
						inF[j] = inF[j+1];
						j++;
					}
				}
				else
				{
					while ( inF[j] != '>' )
						j++;
				}
				
				
				inF[j] = ',';
				if ( !isPredicateSymbol(inF[j+2]) && !isParan(inF[j+2]) )
				{
					printf("parse error at line %d %c\n",lineNum+1,inF[j+2]);
					exit(1);
				}
				j++;
				paran_count = 0;
				while ( isPredicateSymbol(inF[j+1]) || isParan(inF[j+1]) || (paran_count != 0))
				{
					if ( inF[j+1] == ')' )
					{
						if ( paran_count == 0 )
							break;
						paran_count--;
					}
					
					if ( inF[j+1] == '(' )
					{ 
						paran_count++;
					}
					inF[j] = inF[j+1];
					j++;
				}
				inF[j] = ')';
				i = j+1;
				break;
				
#if 0
				
			case '^':
				
				if ( i == 0 || !isPredicateSymbol(inF[i-1]) || !isPredicateSymbol(inF[i+1]))
				{
					printf("parse error at line %d\n",lineNum+1);
					exit(1);
				}
				start_index = i-1;
				while ( isPredicateSymbol(inF[start_index]) && (start_index != 0))
					start_index--;
				
				if ( start_index == 0 )
					start_index = -1;
				
				j = i;
				
				while ( inF[j] != '\0' )
					j++;
				/* shift by 5 characters */
				
				while ( j != start_index )
				{
					inF[j+5] = inF[j];
					j--;
				}
				
				j++;
				inF[j++] = 'x';
				inF[j++] = 'o';
				inF[j++] = 'r';
				inF[j++] = '(';
				
				
				while ( inF[j+1] != '^' )
				{
					inF[j] = inF[j+1];
					j++;
				}
				
				
				inF[j] = ',';
				if ( !isPredicateSymbol(inF[j+2]) )
				{
					printf("parse error at line %d %c\n",lineNum+1,inF[j+2]);
					exit(1);
				}
				j++;
				while ( isPredicateSymbol(inF[j+1]) )
				{
					inF[j] = inF[j+1];
					j++;
				}
				inF[j] = ')';
				i = j+1;
				break;
#endif	
		}
	}
			
	
}

#ifdef BAR_NEG_ATOM
void
convert_neg(char *inF, int lineNum)
{
int             i = 0;
int             j = 0, k = 0;
int             neg_exist = 0;

struct negatom *newnode = NULL;
struct negatom *negnode = NULL;

int             neg_index = 0;
int             tuple_start = 0;
int             paran_count = 0;

for (i = 0; inF[i] != '\0'; i++) {

if ( inF[i] == '~' ) {
	if (!isPredicateSymbol(inF[i + 1]) || (i >0 && isPredicateSymbol(inF[i-1])) ) {
printf
("line %d - Strong negation can only be applied to an atom\n",
lineNum + 1);
exit(1);
}

/*
* create atoms ending with 'neg' 
*/
for (j = i; inF[j] != '\0'; j++) {
inF[j] = inF[j + 1];
}

newnode = (struct negatom *) malloc(sizeof(struct negatom));
newnode->next = NULL;


k = 0;
/*
* read the entire atom 
*/
while (isPredicateSymbol(inF[i])) {
newnode->val[k++] = inF[i++];
}
neg_index = i;
if (inF[i] == '(') {
paran_count = 1;
while (paran_count != 0) {
newnode->val[k++] = inF[i++];
if (inF[i] == '(')
paran_count++;
if (inF[i] == ')')
paran_count--;
}
newnode->val[k++] = ')';
// i++;
}

newnode->val[k] = '\0';
neg_exist = 0;
for (negnode = root_negatom; negnode != NULL;
negnode = negnode->next) {
if (!strcmp(negnode->val, newnode->val)) {
neg_exist = 1;
}
}

if (!neg_exist) {
if (prev_negatom != NULL) {
prev_negatom->next = newnode;
prev_negatom = newnode;
} else {
root_negatom = newnode;
prev_negatom = root_negatom;
}
}


/*
* shift all char right by 3 places 
*/
for (j = neg_index; inF[j] != '\0'; j++);
for (; j >= neg_index; j--) {
inF[j + 3] = inF[j];
}
/*
* add 'neg' 
*/
inF[neg_index++] = 'n';
inF[neg_index++] = 'e';
inF[neg_index] = 'g';
// inF[i] = '~';
}

}

}
#endif

int isPredicateSymbol(char c)
{
	
	switch (c)
	{
		//case '=':
		//case '<':
		//case '>':
		case '^':
		case '@':
		case '+':
		case '*':
		case '/':
		case '`': //mod is replaced by this
			return 1;
	}
	
	if ( ( (c == '~') || isalnum(c) ) || (c=='_') )
		return 1;
	
	return 0;
	
}



/*
* take care of empty antecedents or consequents 
*/
void
add_tf(char *inF)
{
int             i = 0;

/*
* if empty antecedent, add true 
*/
if ((inF[0] == '-') && (inF[1] == '>')) {
for (i = 0; inF[i] != '\0'; i++);

for (; i >= 0; i--) {
inF[i + 4] = inF[i];
}

inF[0] = 't';
inF[1] = 'r';
	inF[2] = 'u';
	inF[3] = 'e';
}

/*
* if empty consequent, add false 
*/
for (i = 0; inF[i] != '\0'; i++);

if (inF[i - 1] == '>') {
	inF[i] = 'f';
	inF[i + 1] = 'a';
	inF[i + 2] = 'l';
	inF[i + 3] = 's';
	inF[i + 4] = 'e';
	inF[i + 5] = '\0';
}

}


#if 0
/*
* reverse the string replacing ':' by '>', ',' by '&' and ';' by '|' 
*/
void
convert_arb(char *inF, int lineNum)
{
char            first[MAX_F_SIZE] = { (char) NULL };
char            second[MAX_F_SIZE] = { (char) NULL };
int             i = 0,
	j = 0;
int             tuple_start = 0;

// printf("formula after entering %s\n", inF );
for (i = 0; inF[i] != ':'; i++) {
	/*
	* indicate tuple start 
	*/
	if ((i > 0) && ((inF[i] == '(') && isPredicateSymbol(inF[i - 1]))) {
	tuple_start = 1;
	}

	/*
	* indicate tuple end 
	*/
	if ((inF[i] == ')') && tuple_start) {
	tuple_start = 0;
	}

	if ((inF[i] == ',') && (!tuple_start)) {
	first[i] = '&';
	} else if (inF[i] == ';') {
	first[i] = '|';
	} else {
	/*
	* store first part 
	*/
	first[i] = inF[i];
	}
}
first[i] = '\0';
i++;

/*
* check the implication 
*/
if (inF[i] != '-') {
	printf("parse error at line %d\n", lineNum + 1);
	exit(1);
}

i++;

/*
* store second part 
*/
while (inF[i] != '\0') {
	/*
	* indicate tuple start 
	*/
	if ((i > 0) && ((inF[i] == '(') && isPredicateSymbol(inF[i - 1]))) {
	tuple_start = 1;
	}

	/*
	* indicate tuple end 
	*/
	if ((inF[i] == ')') && tuple_start) {
	tuple_start = 0;
	}

if ((inF[i] == ',') && (!tuple_start)) {
second[j++] = '&';
i++;
} else if (inF[i] == ';') {
second[j++] = '|';
i++;
} else {
second[j++] = inF[i++];
}
}
second[j] = '\0';


// printf("first second %s %s\n", first, second);
/*
* reverse the implication 
*/
for (i = 0; second[i] != '\0'; i++) {
inF[i] = second[i];
}

inF[i++] = '-';
inF[i++] = '>';

/*
* now copy the second part 
*/
for (j = 0; first[j] != '\0'; j++) {
inF[i++] = first[j];
}
inF[i] = '\0';
// printf("formula %s\n",inF);

}
#endif

#if 0
void
parse_display()
{
FILE           *fp = NULL;
FILE           *fpOut = NULL;
char            predicate[MAX_PREDICATE_LENGTH] = { (char) NULL };
int             i = 0;
int             j = 0;
char            readBuf = (char) NULL;
char            writeBuf = (char) NULL;
int             ret = 0;
int             one_atom = 0;
int             paran_count = 0;

fp = fopen(".dlv_output", "r");
if (fp == NULL) {
printf("opening output file failed\n");
exit(1);
}

fpOut = fopen(".dlv_finaloutput", "w");
if (fpOut == NULL) {
printf("opening final output file failed\n");
exit(1);
}


while (1) {
one_atom = 0;
ret = fread(&readBuf, 1, 1, fp);
if (ret == 0) {
if (feof(fp)) {
fclose(fpOut);
system("cat .dlv_finaloutput");
printf("\n");
exit(1);
}
printf("read from output file failed\n");

exit(1);
}

if (readBuf == '{') {

while (readBuf != '}') {
i = 0;
if ((readBuf == '{') || isPredicateSymbol(readBuf)) {
writeBuf = readBuf;
ret = fwrite(&writeBuf, 1, 1, fpOut);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}
}

do {
ret = fread(&readBuf, 1, 1, fp);
if (ret == 0) {
printf("read from output file failed\n");
fclose(fpOut);
system("cat .dlv_finaloutput");
printf("\n");
exit(1);
}
predicate[i++] = readBuf;
} while (isPredicateSymbol(readBuf));

predicate[--i] = '\0';

if (negated_atom(predicate)) {
// printf("coming here hahaha\n");
/*
* convert predicate to ~predicate 
*/
/*
* shift all by one char 
*/
for (i = 0; predicate[i] != '\0'; i++);
j = i;
for (; i >= 0; i--) {
predicate[i + 1] = predicate[i];
}
/*
* remove 'neg' 
*/
predicate[j - 2] = '\0';
predicate[0] = '~';
}
//printf("predicate after %s\n", predicate);

if (can_write(predicate)) {
for (i = 0; predicate[i] != '\0'; i++) {
writeBuf = predicate[i];
ret = fwrite(&writeBuf, 1, 1, fpOut);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}
}

if (readBuf == '(') {
paran_count = 1;
while (paran_count != 0) {
/*
	* write all tuples 
	*/
writeBuf = readBuf;
ret = fwrite(&writeBuf, 1, 1, fpOut);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}


ret = fread(&readBuf, 1, 1, fp);
if (ret == 0) {
printf
	("read from DLV output file failed\n");
exit(1);
}

if ( readBuf == '(' )
paran_count++;
if ( readBuf == ')' )
paran_count--;

}

writeBuf = ')';
ret = fwrite(&writeBuf, 1, 1, fpOut);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}
/*
* read the ,or '}' 
*/
ret = fread(&readBuf, 1, 1, fp);
if (ret == 0) {
printf("reading , or } failed\n");
exit(1);
}
}

one_atom = 1;
writeBuf = ',';
ret = fwrite(&writeBuf, 1, 1, fpOut);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}

writeBuf = ' ';
ret = fwrite(&writeBuf, 1, 1, fpOut);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}
} else {
/*
* ignore all tuples 
*/
if (readBuf == '(') {
paran_count = 1;
while (paran_count != 0) {
ret = fread(&readBuf, 1, 1, fp);
if (ret == 0) {
printf("reading tuples failed\n");
exit(1);
}
if ( readBuf == '(' )
paran_count++;
if ( readBuf == ')' )
paran_count--;
}
}
}



}

/*
* if atleast one atom has been written, remove the final ',' 
*/

if (one_atom) {
ret = fseek(fpOut, -2, SEEK_CUR);
if (ret != 0) {
printf("seek back to remove , failed\n");
exit(1);
}
}

writeBuf = '}';
ret = fwrite(&writeBuf, 1, 1, fpOut);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}

writeBuf = '\n';
ret = fwrite(&writeBuf, 1, 1, fpOut);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}
}


}


}





void
parse_display_cmodels()
{
	FILE           *fp = NULL;
	FILE           *fpOut = NULL;
	char            predicate[MAX_PREDICATE_LENGTH] = { (char) NULL };
	int             i = 0;
	int             j = 0;
	char            readBuf = (char) NULL;
	char            writeBuf = (char) NULL;
	int             ret = 0;
	int             one_atom = 0;
	int             paran_count = 0;

	fp = fopen(".dlv_output", "r");
	if (fp == NULL) {
		printf("opening output file failed\n");
		exit(1);
	}

	fpOut = fopen(".dlv_finaloutput", "w");
	if (fpOut == NULL) {
		printf("opening final output file failed\n");
		exit(1);
	}


	while (1) {
		one_atom = 0;
		ret = fread(&readBuf, 1, 1, fp);
		if (ret == 0) {
			if (feof(fp)) {
				fclose(fpOut);
				system("cat .dlv_finaloutput");
				printf("\n");
				exit(1);
			}
			printf("read from output file failed\n");

			exit(1);
		}

		if (readBuf == ':') {

			while (readBuf != '}') {
				i = 0;
				if ((readBuf == '{') || isPredicateSymbol(readBuf)) {
					writeBuf = readBuf;
					ret = fwrite(&writeBuf, 1, 1, fpOut);
					if (ret == 0) {
						printf("write to output file failed\n");
						exit(1);
					}
				}

				do {
					ret = fread(&readBuf, 1, 1, fp);
					if (ret == 0) {
						printf("read from output file failed\n");
						fclose(fpOut);
						system("cat .dlv_finaloutput");
						printf("\n");
						exit(1);
					}
					predicate[i++] = readBuf;
				} while (isPredicateSymbol(readBuf));

				predicate[--i] = '\0';

				if (negated_atom(predicate)) {
// printf("coming here hahaha\n");
/*
					* convert predicate to ~predicate 
*/
/*
					* shift all by one char 
*/
					for (i = 0; predicate[i] != '\0'; i++);
					j = i;
					for (; i >= 0; i--) {
						predicate[i + 1] = predicate[i];
					}
/*
					* remove 'neg' 
*/
					predicate[j - 2] = '\0';
					predicate[0] = '~';
				}
//printf("predicate after %s\n", predicate);

				if (can_write(predicate)) {
					for (i = 0; predicate[i] != '\0'; i++) {
						writeBuf = predicate[i];
						ret = fwrite(&writeBuf, 1, 1, fpOut);
						if (ret == 0) {
							printf("write to output file failed\n");
							exit(1);
						}
					}

					if (readBuf == '(') {
						paran_count = 1;
						while (paran_count != 0) {
/*
							* write all tuples 
	*/
							writeBuf = readBuf;
							ret = fwrite(&writeBuf, 1, 1, fpOut);
							if (ret == 0) {
								printf("write to output file failed\n");
								exit(1);
							}


							ret = fread(&readBuf, 1, 1, fp);
							if (ret == 0) {
								printf
										("read from DLV output file failed\n");
								exit(1);
							}

							if ( readBuf == '(' )
								paran_count++;
							if ( readBuf == ')' )
								paran_count--;

						}

						writeBuf = ')';
						ret = fwrite(&writeBuf, 1, 1, fpOut);
						if (ret == 0) {
							printf("write to output file failed\n");
							exit(1);
						}
/*
						* read the ,or '}' 
*/
						ret = fread(&readBuf, 1, 1, fp);
						if (ret == 0) {
							printf("reading , or } failed\n");
							exit(1);
						}
					}

					one_atom = 1;
					writeBuf = ',';
					ret = fwrite(&writeBuf, 1, 1, fpOut);
					if (ret == 0) {
						printf("write to output file failed\n");
						exit(1);
					}

					writeBuf = ' ';
					ret = fwrite(&writeBuf, 1, 1, fpOut);
					if (ret == 0) {
						printf("write to output file failed\n");
						exit(1);
					}
				} else {
/*
					* ignore all tuples 
*/
					if (readBuf == '(') {
						paran_count = 1;
						while (paran_count != 0) {
							ret = fread(&readBuf, 1, 1, fp);
							if (ret == 0) {
								printf("reading tuples failed\n");
								exit(1);
							}
							if ( readBuf == '(' )
								paran_count++;
							if ( readBuf == ')' )
								paran_count--;
						}
					}
				}



			}

/*
			* if atleast one atom has been written, remove the final ',' 
*/

			if (one_atom) {
				ret = fseek(fpOut, -2, SEEK_CUR);
				if (ret != 0) {
					printf("seek back to remove , failed\n");
					exit(1);
				}
			}

			writeBuf = '}';
			ret = fwrite(&writeBuf, 1, 1, fpOut);
			if (ret == 0) {
				printf("write to output file failed\n");
				exit(1);
			}

			writeBuf = '\n';
			ret = fwrite(&writeBuf, 1, 1, fpOut);
			if (ret == 0) {
				printf("write to output file failed\n");
				exit(1);
			}
		}


	}


}


#endif





int
negated_atom(char *predicate)
{
int             i = 0;
int             j = 0;

// printf("predicate %s\n",predicate);

if (!isPredicateSymbol(predicate[i]))
return 0;

for (i = 0; predicate[i] != '\0'; i++);
j = i;

if (j < 4)
return 0;

if ((predicate[j - 1] != 'g') || (predicate[j - 2] != 'e')
|| (predicate[j - 3] != 'n'))
return 0;

return 1;
}


int
can_write(char *predicate)
{
int             i = 0;
int             j = 0;

if ( !strcmp(predicate,"true") )
return 0;


if ((!isPredicateSymbol(predicate[i]) && (predicate[i] != '-')) && (predicate[i] != '~'))
return 0;

for (i = 0; predicate[i] != '\0'; i++);
j = i;

if (j < 4)
return 1;

if ((predicate[j - 1] != 'r') || (predicate[j - 2] != 'a')
|| (predicate[j - 3] != 'b'))
return 1;

return 0;
}

int isArithmeticOp ( char c )
{
	switch (c)
	{
		case '+':
		case '*':
		case '@': // since we replace - with @
		case '/':
		case '^':
		case '`': // since we replace mod with `
			return 1;
		default:
			return 0;
	}
				
}

int isVarChar ( char c )
{
	if (isalnum(c))
		return 1;
		
	if ( c == '_' )
		return 1;
		
	return 0;
}

void
parse_exp(char *inF, int lineNum)
{
char            prev = (char) NULL;
int             i = 0;
int             paran_count = 0;
int             tuple_start = 0;
int      tuple_paran_count = 0;

for (i = 0; inF[i] != '\0'; i++) {
if (isPredicateSymbol(inF[i])) {
	if (!isArithmeticOp(inF[i]) && (prev == ')')) {
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}
/* if arithmetic operator, check for parse errors */
if ( isArithmeticOp(inF[i]) )
{
	if ( i==0 )
	{
		printf("parse error at line %d\n", lineNum + 1);
		exit(1);
	}

	if ( (inF[i] != '-') && 
	     (!isalnum(inF[i-1]) && (inF[i-1] != ')')) ) 
	{
		printf("parse error at line %d\n", lineNum + 1);
		exit(1);
	}
	
	if ( inF[i] == '-' )
	{
		if ( (!isalnum(inF[i-1]) && (inF[i-1] != ')')) &&
	        (inF[i-1] != ',') ) 
	   {
		   printf("parse error at line %d\n", lineNum + 1);
			exit(1);
	   }
	}
	
	if ( !isalnum(inF[i+1]) && (inF[i+1] != '(') )
	{
		printf("parse error at line %d\n", lineNum + 1);
		exit(1);
	}
}
prev = inF[i];
continue;
}

switch (inF[i]) {
case ':':
case '&':
case '|':
if ( i==0 )
{
	printf("parse error at line %d\n", lineNum + 1);
	exit(1);
}
if ((prev == ':') || (prev == '&') || (prev == '|')
|| (prev == '-') || (prev == '(') || (tuple_paran_count != 0)) {
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}
prev = inF[i];
break;

case '(':
if ( isPredicateSymbol(prev) && (tuple_paran_count == 0) )
{
tuple_paran_count = 1;
}
else if ( tuple_paran_count != 0 )
{
tuple_paran_count++;
}

paran_count++;
if ( prev == ')' )  {
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}

prev = inF[i];
break;

case ')':
if ( tuple_paran_count != 0 )
	{
tuple_paran_count--;
	}
paran_count--;
if (paran_count < 0) {
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}

if ( (prev == ':') || (prev == '&') || (prev == '|')
|| (prev == '-') || (prev == '(') ) {
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}

prev = inF[i];
break;

case '-':
if (tuple_paran_count == 0) {
if (isPredicateSymbol(prev) || (prev == ')')) {
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}
} else {
	if ((!isalnum(prev) && (prev != ')')) && ((prev != '(') && (prev != ','))) {
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}
}
prev = inF[i];
break;
case ',':
if (tuple_paran_count == 0) {
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}
prev = inF[i];
break;
case '+':
case '*':
case '/':
if (tuple_paran_count == 0)
{
	printf("parse error at line %d: +,* and / are not allowed outside tuples\n", lineNum+1);
	exit(1);
}
	if (!isalnum(prev) && (prev != ')') ) {
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}

/*
if (inF[i] == '*') {
inF[i] = '^';
}*/
prev = inF[i];
break;

case '~':
	if ( (!isPredicateSymbol(inF[i + 1]) || isPredicateSymbol(prev) ) 
		      || ( (tuple_paran_count != 0) || ( prev == ')') ) )
		{
			printf
					("line %d - Strong negation can only be applied to an atom\n",
					 lineNum + 1);
			exit(1);
		}
		prev = inF[i];
		break;
		
case '!':
case '?':
	if ( isPredicateSymbol(prev) || (prev == ')') || (tuple_paran_count != 0) )
	{
		printf("Parse error at line %d\n",lineNum+1);
		exit(1);
	}
	
	i++;
	if ( inF[i] != '[' )
	{
		printf("Parse error at line %d. Quantifier should be followed by a list [...] of variables\n",lineNum+1);
		exit(1);
	}
	if ( !isupper((int)inF[i+1]) )
	{
		printf ("Parse error at line %d. Quantification can only be applied to a list of variables\n",lineNum+1);
		exit(1);
	}
		
	i++;
	while ( inF[i] != ']' )
	{
		if ( (inF[i] == ',') && !isupper((int)inF[i+1]) )
		{
			printf ("Parse error at line %d. Quantification can only be applied to a list of variables\n",lineNum+1);
			exit(1);
		}
			
		if (inF[i] == '\0' )
		{
			printf("Parse error at line %d. No matching ] found\n",lineNum+1);
			exit(1);
		}
		if ( !isVarChar(inF[i]) && (inF[i] != ',') )
		{
			printf ("Parse error at line %d. Quantification can only be applied to a list of variables\n",lineNum+1);
			exit(1);
		}
		i++;
	}
	
	/* check for : and (*/
	if ( inF[i+1] != ':' )
	{
		printf("Parse error at line %d. No : found after [...]\n",lineNum+1);
		exit(1);
	}
	i++;
	//prev = ':'; //do not update prev here
	break;

default:
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}


}

if ( (paran_count != 0) || (tuple_paran_count != 0) ) {
printf("parse error at line %d. No matching parenthesis found\n", lineNum + 1);
exit(1);
}

}




void
add_dollar(char *inF)
{
int             i = 0,
j = 0,
k = 0,
l = 0;
int             fol_flag = 0;
int             paran_count = 0;

while (inF[i] != '\0') {

/* if quantifier, ignore */
if ( (inF[i] == '!') || (inF[i] == '?') )
{
	while ( inF[i] != ':' )
		i++;
}
	
if (isPredicateSymbol(inF[i])) {
fol_flag = 0;
for (l = i + 1; isPredicateSymbol(inF[l]); l++);

/*
* check for tuples 
*/
if (inF[l] == '(') {
paran_count = 1;
fol_flag = 1;
while (paran_count != 0) {
l++;
if (inF[l] == '(')
paran_count++;
if (inF[l] == ')')
paran_count--;
}
}

/*
* take care of the case where you are already at '\0' 
*/
if (!fol_flag)
l--;

for (j = l + 1; inF[j] != '\0'; j++);
for (j = j + 1; j > l + 1; j--) {
inF[j] = inF[j - 1];
}

inF[j] = '$';
i = j;

} else {
i++;
}
}




}




/*
* this function also checks for illegal characters 
*/
void
convert_imp(char *inF, int lineNum)
{
int             i = 0;
int             j = 0;
while (inF[i] != '\0') {
if (!isPredicateSymbol(inF[i])) {
switch (inF[i]) {
case '&':
break;
case '|':
break;
case '-':
break;
case '>':
break;
case '(':
break;
case ')':
break;
case '!':
break;
case '?':
break;
case '[':
break;
case ']':
break;
case ':':
if ( (i == 0) || (inF[i-1] != ']') )
{
	printf("parse error at line %d\n", lineNum + 1);
	exit(1);
}
break;
case '$':
case ',':
case '+':
case '*':
case '/':
break;
default:
	printf("unexpected use of %c in line %d \n", inF[i], lineNum + 1);
exit(1);
}
}



/*
if (inF[i] == '>') {
	if ((i == 0) || (inF[i - 1] != '-')) {
printf("parse error at line %d\n", lineNum + 1);
exit(1);
}
inF[i - 1] = ':';
*/

if (inF[i] == '>') {
	if (i == 0){
		printf("parse error at line %d\n", lineNum + 1);
		exit(1);
	}

if ( inF[i-1] == '-' )
{
	
inF[i - 1] = ':';
/*
* move characters to the left 
*/
for (j = i; inF[j] != '\0'; j++) {
inF[j] = inF[j + 1];
}
i--;
}
}
i++;
}

}


void
		convert_specialSymbols(char *inF, int lineNum)
{
	int             i = 0;
	int             j = 0;
	while (inF[i] != '\0') {
		/*
		if ( (inF[i] == '!') && (inF[i+1] != '[') )
		{
			inF[i] = '#';
		}
		*/
		/*
		if ( inF[i] == '@' )
		{
			printf("illegal character in line %d : %c\n", lineNum + 1,inF[i]);
			exit(1);
		}
		*/
		
		if ( (inF[i] == '-') && (inF[i+1] == '>') )
		{
			i++;
			continue;
		}
		
		if ( (i > 0) && ( (inF[i] == '-') && (isPredicateSymbol(inF[i-1]) || (inF[i-1] == ')')) ) )
		{
			if ( (inF[i+1] != '(') && !isPredicateSymbol(inF[i+1]) )
			{
				printf("parse error at line %d\n",lineNum+1);
				exit(1);
			}
			inF[i] = '@';
		}
		i++;
	}
	

}


void rewrite_NNF_1 ( NODEP root )
{
	//printf("rewrite 1\n");
	strcpy(root->val,"false");
	root->left = NULL;
	root->right = NULL;
}

void rewrite_NNF_2 ( NODEP root )
{
	//printf("rewrite 2\n");
	strcpy(root->val,"true");
	root->left = NULL;
	root->right = NULL;
}

void rewrite_NNF_3 ( NODEP root )
{
	//printf("rewrite 3\n");
	root->right = root->right->right->right;
}

void rewrite_NNF_4 ( NODEP root )
{
	NODEP temp1 = NULL, temp2 = NULL;
	//printf("rewrite 4\n");
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp1->val[0] = '-';
	temp1->val[1] = '\0';
	temp2->val[0] = '-';
	temp2->val[1] = '\0';
	
	temp1->left = NULL;
	temp1->right = root->right->left;
	
	temp2->left = NULL;
	temp2->right = root->right->right;
	
	root->val[0] = '|';
	root->left = temp1;
	root->right = temp2;
}


void rewrite_NNF_5 ( NODEP root )
{
	NODEP temp1 = NULL, temp2 = NULL;
	//printf("rewrite 5\n");
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp1->val[0] = '-';
	temp1->val[1] = '\0';
	temp2->val[0] = '-';
	temp2->val[1] = '\0';
	
	temp1->left = NULL;
	temp1->right = root->right->left;
	
	temp2->left = NULL;
	temp2->right = root->right->right;
	
	root->val[0] = '&';
	root->left = temp1;
	root->right = temp2;
}

void rewrite_NNF_6 ( NODEP root )
{
	NODEP temp1 = NULL, temp2 = NULL, temp3 = NULL;
	//printf("rewrite 6\n");
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp3 = (NODEP) malloc(sizeof(struct node));
	temp1->val[0] = '-';
	temp1->val[1] = '\0';
	temp2->val[0] = '-';
	temp2->val[1] = '\0';
	temp3->val[0] = '-';
	temp3->val[1] = '\0';
	
	temp1->left = NULL;
	temp1->right = root->right->left;
	
	temp2->left = NULL;
	temp2->right = root->right->right;
	
	temp3->left = NULL;
	temp3->right = temp1;
	
	root->val[0] = '&';
	root->left = temp3;
	root->right = temp2;
}

void convert_NNF ( NODEP temp )
{
	int dneg = 0;
	
	if (temp == NULL)
		return;
	
	
	if ( (temp->val[0] == '-')  && !strcmp(temp->right->val,"true") ) 
	{
		rewrite_NNF_1 ( temp );
	}
			
	if ( (temp->val[0] == '-') && !strcmp(temp->right->val,"false") )
	{
		rewrite_NNF_2 ( temp );
	}
			
	if ( (temp->val[0] == '-') && (temp->right->val[0] == '-') )
	{
		if (temp->right->right->val[0] == '-')
		{
			//inorder_print(temp);
			rewrite_NNF_3 ( temp );
			//inorder_print(temp);
			convert_NNF (temp);
		}
		else
		{
			if (!isPredicateSymbol(temp->right->right->val[0]))
				dneg=1;
		}
			
	}
			
	if ( (temp->val[0] == '-') && (temp->right->val[0] == '&') )
	{
		rewrite_NNF_4 (temp);
	}
			
	if ( (temp->val[0] == '-') && (temp->right->val[0] == '|') )
	{
		rewrite_NNF_5 (temp);
	}
			
	if ( (temp->val[0] == '-') && (temp->right->val[0] == ':') )
	{
		rewrite_NNF_6 (temp);
	}		
	
	
	convert_NNF(temp->left);
	convert_NNF(temp->right);
	if (dneg == 1) // in case of double negation
		convert_NNF(temp);
	//printf("exiting NNF\n");
}



void find_quantifier (NODEP root, struct node **qPtr, struct node **qParentPtr, int *sign, int dummy, int *dir, int sPositive)
{
	if ((root == NULL) || (*qPtr != NULL))
		return;
	
	if ( root->val[0] == '!' )
	{
		*qPtr = root;
		if (sPositive )
			*sign = dummy + 5; //to distinguish bet. positve and S.P
		else
			*sign = dummy;
		//printf("found universal quantification %s \n",(*qPtr)->val);
		return;
	}
	if ( root->val[0] == '?' )
	{
		*qPtr = root;
		if (sPositive)
			*sign = dummy + 5;
		else
			*sign = dummy;
		//printf("found existential quantification %s parent %s \n",(*qPtr)->val, (*qParentPtr)->val);
		return;
	}
	if ( *qPtr == NULL )
		*qParentPtr = root;
	
	if ( (root->val[0] == ':') && (*qPtr == NULL) )
	{
		*dir = 0;
		find_quantifier (root->left, qPtr, qParentPtr, sign, (dummy==1)?0:1, dir, 0);
	}
	else if ( (root->val[0] != '-') && (*qPtr == NULL) )
	{
		*dir = 0;
		/* dummy call since left is always NULL */
		find_quantifier (root->left, qPtr, qParentPtr, sign, dummy, dir, sPositive);
	}
	
	if ( *qPtr == NULL )
		*qParentPtr = root;
	
	if ( (root->val[0] == '-') && (*qPtr == NULL) )
	{
		*dir = 1;
		find_quantifier (root->right, qPtr, qParentPtr, sign, (dummy==1)?0:1, dir, 0);
	}
	else if (*qPtr == NULL)
	{
		*dir = 1;
		find_quantifier (root->right, qPtr, qParentPtr, sign, dummy, dir,sPositive);
	}
}



char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}

int no_of_char(int var_index)
{
	char *str = NULL;
	int i = 0;
	str = itoa(var_index,10);
	while ( str[i] != '\0' )
		i++;
				
	return i;
}


void check_and_addDom ( char *qPtr, char *newVar )
{
	int ret = 0;
	char readBuf = (char)NULL;
	char dom[LINE_MAX] = {(char)NULL};
	int i = 0, j = 0, start_index = 0;
	int match = 1;
	
	/* close and open so that EOF is written */
	fclose(fpSolverInput);
	fpSolverInput = fopen(".solver_input.lp","r+");
	if ( fpSolverInput == NULL )
	{
		printf("opening temporary file in rw mode failed. Check permissions.\n");
		exit(1);
	}
	
	while (1)
	{
		
		if (fgets(dom, LINE_MAX, fpSolverInput) == NULL) {
			if (!feof(fpSolverInput)) {
				printf("read line failed here\n");
				exit(1);
			}
			break;
		}
#ifdef DEBUG
		printf("searching line %s for domain variable declarations\n",dom);
#endif	
		
		if ( dom[0] != '#' )
			continue;
		
		i = 0;
		while ( (dom[i] != '(') && (dom[i] != '\0') )
			i++;
		
		if (dom[i] == '\0')
		{
			continue;
		}
		
		start_index = i;
		
		i++;
		match = 1;
		j = 0;
		while ( dom[i] != '\0' )
		{
			if ( dom[i] == ';' || dom[i] == ')' )
			{
				if ((qPtr[j] == '\0') && (match == 1))
				{
					i = 0;

					/* found match. write to file and return */
					fseek(fpSolverInput,(long)0,SEEK_END);
					
					while ( i < start_index )
					{
						readBuf = dom[i];
						ret = fwrite(&readBuf,1,1,fpSolverInput); 
						if ( ret == 0 )
						{
							printf("write to output file failed\n");
							exit(1);
						}
						i++;
					}
					
					readBuf = '(';
					ret = fwrite(&readBuf,1,1,fpSolverInput); 
					if ( ret == 0 )
					{
						printf("write to output file failed\n");
						exit(1);
					}
					
					readBuf = 'N';
					ret = fwrite(&readBuf,1,1,fpSolverInput); 
					if ( ret == 0 )
					{
						printf("write to output file failed\n");
						exit(1);
					}
					
					readBuf = 'V';
					ret = fwrite(&readBuf,1,1,fpSolverInput); 
					if ( ret == 0 )
					{
						printf("write to output file failed\n");
						exit(1);
					}
					
					
					j = 0;
					while ( newVar[j] != '\0' )
					{
						readBuf = newVar[j];
						ret = fwrite(&readBuf,1,1,fpSolverInput); 
						if ( ret == 0 )
						{
							printf("write to output file failed\n");
							exit(1);
						}
						j++;
					}
					
					readBuf = ')';
					ret = fwrite(&readBuf,1,1,fpSolverInput); 
					if ( ret == 0 )
					{
						printf("write to output file failed\n");
						exit(1);
					}
					
					readBuf = '.';
					ret = fwrite(&readBuf,1,1,fpSolverInput); 
					if ( ret == 0 )
					{
						printf("write to output file failed\n");
						exit(1);
					}
					
					readBuf = '\n';
					ret = fwrite(&readBuf,1,1,fpSolverInput); 
					if ( ret == 0 )
					{
						printf("write to output file failed\n");
						exit(1);
					}
					
					return;
					
				}
				else
				{
					j = 0;
					match = 1;
					i++;
					continue;
				}
			}
			if ( qPtr[j] != dom[i] )
				match = 0;
			
			i++;
			if ( qPtr[j] != '\0' )
				j++;
		}

	}	
		
}



void replace ( char *qStr, NODEP root )
{
	int i = 0, j = 0;
	char *newVar = NULL;
	int temp = 0;
	int varSize = 0;
	int start_index = 0;
	
	
	if ( root == NULL )
	{
		return;
	}
	
	while ( ((root->val[i] != '(') && (root->val[i] != '\0')) && (root->val[i] != '[') )
		i++;
	 
	if ( (root->val[i] == '(') || (root->val[i] == '[') )
	{
		//printf("found (\n");
		while ( root->val[i] != '\0' )
		{
			j = 0;
			if ( (root->val[i] == qStr[j]) && 
			     (!isVarChar(root->val[i-1])) )
			{
				//printf("possible match\n");
				/* possible match */
				start_index = i;
				while ( root->val[i++] == qStr[j++] );
				if ( qStr[j-1] == '\0' )
				{
					/* match if it is a variable */
					if ( !isVarChar(root->val[i-1]) )
					{
						//printf("found match %d %d\n",start_index,i-2);
						j = 0;
						varSize = no_of_char(var_index)+2;
						//printf("var size %d %d\n",	varSize, var_index);
						newVar = (char *)malloc(varSize-2+1);
						newVar = itoa(var_index,10);
						//printf("var index %d new Var %s\n",var_index,newVar);
						
						if ( varSize > i-2-start_index+1)
						{
							/* shift to the right */
							temp = i;
							while ( root->val[temp] != '\0' )
								temp++;
							
							while ( temp > i-2 )
							{
								root->val[temp+varSize - (i-2-start_index+1)] = root->val[temp];
								temp--;
							}
							i = i+varSize - (i-2-start_index+1); 
						}
						else if ( varSize < i-2-start_index+1)
						{
							/* shift to the left */
							temp = i-2+1;
							while ( root->val[temp] != '\0' )
							{
								root->val[temp-(i-2-start_index+1-varSize)] = 
										root->val[temp];
								temp++;
							}
							/* end it with '\0' */
							root->val[temp-(i-2-start_index+1-varSize)] = '\0';
							i = i-(i-2-start_index+1-varSize);
							
						}
						//printf("after shifting %s\n", root->val);
						root->val[start_index++] = 'N';
						root->val[start_index++] = 'V';
						temp = 0;
						while ( newVar[temp] != '\0' )
							root->val[start_index++] = newVar[temp++];
						
						/* check if the replaced variable is a domain variable 
						and add the new variable to the domain if it is the case */
						if ( !added_dom ) /* so that we dont add it multiple times */
						{
							check_and_addDom ( qStr, newVar );
							added_dom = 1;
						}
						
						//printf("after subs %s\n", root->val);
					}
				}
			}
			else
			{
				i++;
			}
		}
					
	}
	
	replace(qStr,root->left);
	replace(qStr,root->right);
}


void replace_variables (NODEP qPtr)
{
	int qIndex = 0;
	int qBase = 0;
	char *qStr = NULL;
	int i = 0;
	
	qIndex = 2;
	qBase = 2;
	while ( qPtr->val[qIndex] != ']' )
	{
		qIndex++;
		i = 0;
		
		while ( (qPtr->val[qIndex] != ',') && (qPtr->val[qIndex] != ']') )
		{
			qIndex++;
		}
		qStr = (char *)malloc(qIndex-qBase+1);
		while ( qBase < qIndex )
			qStr[i++] = qPtr->val[qBase++];
	
		qStr[i] = '\0';
		//printf("variable %s\n",qStr);
		added_dom = 0;
		replace(qStr,qPtr->right);
		var_index++;
		free(qStr);
		
		qBase = qIndex+1;	
	}
}



int compareVar ( char *var, char *qList, char *freeVar )
{
	int j = 0, start_index = 0, i = 0;
	char *subStr = NULL;
	int flag = 1; //indicates var not in qList and freeVar
	
	while ( qList[j] != '\0' )
	{
		if ( isupper((int)qList[j]) && !isVarChar(qList[j-1]) )
		{
			start_index = j;
			while ( isVarChar(qList[j]) )
				j++;
			
			subStr = (char *)malloc(j-start_index);
			if ( subStr == NULL )
			{
				printf("out of heap memory \n");
				exit(1);
			}
			
			j = start_index; i = 0;
			while ( isVarChar(qList[j]) )
			{
				subStr[i++] = qList[j++];
			}
			subStr[i] = '\0';
			//printf("comparing %s %s\n",subStr,var);
			if ( !strcmp(subStr,var) )
				flag = 0;
			
			free(subStr);
		}
		else
		{
			j++;
		}
	}
	
	j = 0; i = 0;
	while ( freeVar[j] != '\0' )
	{
		if ( isupper((int)freeVar[j]) && ( j == 0 || !isVarChar(freeVar[j-1]) ) )
		{
			start_index = j;
			while ( isVarChar(freeVar[j]) )
				j++;
			
			subStr = (char *)malloc(j-start_index);
			if ( subStr == NULL )
			{
				printf("out of heap memory \n");
				exit(1);
			}
			
			j = start_index; i = 0;
			while ( isVarChar(freeVar[j]) )
			{
				subStr[i++] = freeVar[j++];
			}
			subStr[i] = '\0';
			//printf("comparing %s %s\n",subStr,var);
			if ( !strcmp(subStr,var) )
				flag = 0;
			
			free(subStr);
		}
		else
		{
			j++;
		}
	}
	
	return flag;
}





/* returns all free variables, separated by comma and terminated by '\0' */
void find_freeVar ( NODEP root, char *qList, char *freeVar )
{
	int i = 0, j = 0, start_index  = 0;
	char *temp = NULL;
	
	if (root == NULL)
		return;
	
	/* collect quantifiers as you go */
	if ( (root->val[0] == '?') || (root->val[0] == '!') )
		concat(qList,root->val,qList);
	
	//printf("qList %s\n",qList);
	
	if ( isPredicateSymbol(root->val[0]) )
	{
		//printf("found predicate %s\n",root->val);
		i = 0;
		while ( (root->val[i] != '(') && (root->val[i] != '\0') )
			i++;
		
		if ( root->val[i] == '(' )
		{
			i++;
			while ( root->val[i] != '\0' )
			{
				if ( isupper((int)root->val[i])  && (!isVarChar(root->val[i-1])) )
				{
					//printf("found variable %c\n",root->val[i]);
					/* found a variable. Now compare it with all quantified variables */
					start_index = i;
					while(isVarChar(root->val[i]))
						i++;
					
					temp = (char *)malloc(i-start_index);
					
					i = start_index; j = 0;
					while(isVarChar(root->val[i]))
						temp[j++] = root->val[i++];
					
					temp[j] = '\0';
					//printf("comparing %s %s\n",temp,qList);
					if ( compareVar ( temp,qList,freeVar) )
					{
						/* if not in the quantified var list */
						
						concat(freeVar,temp,freeVar);
						concat(freeVar,",",freeVar);
						//printf("free Var %s \n",freeVar);
					}
					free(temp);
					
				}
				else
				{
					i++;
				}
			}
		}
	}

	find_freeVar ( root->left, qList, freeVar );
	find_freeVar ( root->right, qList, freeVar );
}






void elim_quantifier (struct node **root)
{
	NODEP qPtr = NULL, qParentPtr = NULL, temp1 = NULL, temp2=NULL;
	int sign = 1; /* 1 indicates pos. and 0 indicates negative */
	int dir = 0; /* direction from parent (left or right) */
	int i = 0, j = 0;
	int ret = 0;
	int index = 0;
	int sPositive = 0;
	char *freeVar = NULL;
	char *inorderF = NULL;
	char *newAtom = NULL;
	char *qList = NULL;
	char readBuf = (char)NULL;
	FILE *fp = NULL;
	inorderF = (char *)malloc(LINE_MAX);
	qList = (char *)malloc(LINE_MAX/2);
	freeVar = (char *)malloc(LINE_MAX/2);
	
	while(1) 
	{
		qPtr = NULL;
		qParentPtr = NULL;
		sign = 1;
		/* find quantifier and occurrence (pos, neg)*/
		find_quantifier (*root, &qPtr, &qParentPtr, &sign, 1, &dir, 1);

		/* if S.P occurrence, then subtract 5 */
		if ( sign >= 5 )
		{
			sign = sign - 5;
			sPositive = 1;
		} 
		/* if no quantifier found, return */
		if ( qPtr == NULL )
			break;
		/* remove all positive forall and rename quantified variables to the right of the node */
		/* replace negative forall with - exists - */
		if ( qPtr->val[0] == '!' )
		{
			if (sign == 1)
			{
				/* replace quantified variables to the right of qPtr */
				replace_variables (qPtr);
				/* remove the forall quantification */
				if ( qParentPtr == NULL )
				{
					//printf("setting root\n");
					*root = qPtr->right;
				}
				else
				{
					//printf("dir is %d parent %s\n",dir,qParentPtr->val);
					if ( dir == 0 )
						qParentPtr->left = qPtr->right;
					else
						qParentPtr->right = qPtr->right;
				}
#ifdef DEBUG
				reading_index = 0;
				inorder_save_br(*root,inorderF);
				printf("pos forall %s\n",inorderF);
#endif		
			}
			else
			{
				/* rewrite forall as -exists- */
				temp1 = (NODEP)malloc(sizeof (struct node));
				temp2 = (NODEP)malloc(sizeof (struct node));
				if ( temp1 == NULL || temp2 == NULL )
				{
					printf("out of heap memory\n");
					exit(1);
				}
				temp1->val[0] = '-';
				temp1->val[1] = '\0';
				temp2->val[0] = '-';
				temp2->val[1] = '\0';
				temp1->left = NULL;
				temp2->left = NULL;
				
				qPtr->val[0] = '?';
				
				temp2->right = qPtr->right;
				qPtr->right = temp2;
				temp1->right = qPtr;
				if ( dir == 0 )
					qParentPtr->left = temp1;
				else
					qParentPtr->right = temp1;
				
#ifdef DEBUG
				reading_index = 0;
				inorder_save_br(*root,inorderF);
				printf("neg forall %s\n",inorderF);
#endif
			}
		}
		
		/* if existential quantifier */
		else if ( qPtr->val[0] == '?' )
		{
			/* if negative occurrence, then just replace variables and eliminate it */
			if ( sign == 0 )
			{
				/* replace quantified variables to the right of qPtr */
				replace_variables (qPtr);
				
				if ( dir == 0 )
					qParentPtr->left = qPtr->right;
				else
					qParentPtr->right = qPtr->right;
				
#ifdef DEBUG
				reading_index = 0;
				inorder_save_br(*root,inorderF);
				printf("neg exist %s\n",inorderF);
#endif
			}
			else
			{
				/* introduce new predicates */
				/* first find all the free variables in the formula */
				strcpy(qList, qPtr->val);
				freeVar[0] = '\0';
				//printf("calling find_freeVar\n");
				find_freeVar ( qPtr->right, qList, freeVar );
				
				i = 0;
				while ( freeVar[i] != '\0' )
					i++; 
				
				if ( freeVar[0] != '\0' )
				{
					/* to get rid of the extra , */
					freeVar[i-1] = '\0';
				}
				i--;
				
				newAtom = (char *)malloc(8+no_of_char(pred_index)+2+i+1);
				concat ("new_pred",itoa(pred_index,10),newAtom);
				
				if ( freeVar[0] != '\0' )
				{
					concat(newAtom,"(",newAtom);
					concat(newAtom,freeVar,newAtom);
					concat(newAtom,")",newAtom);
				}

				ret = fwrite("#hide ", 1, sizeof("#hide ") - 1, fpSolverInput);
				if (ret == 0) {
					printf("write to output file failed\n");
					exit(1);
				}
				
				for ( index = 0; newAtom[index] != '\0'; index++ )
				{
					readBuf = newAtom[index];
					ret = fwrite(&readBuf, 1, 1, fpSolverInput);
					if (ret == 0) {
						printf("write to output file failed\n");
						exit(1);
					}
				}
				ret = fwrite(".\n", 1, sizeof(".\n") - 1, fpSolverInput);
				if (ret == 0) {
					printf("write to output file failed\n");
					exit(1);
				}

				reading_index = 0;
				inorder_save_br(qPtr->right,inorderF);
#ifdef DEBUG
				printf("saving %s\n", inorderF);
#endif
				concat(inorderF,"->",inorderF);
				concat(inorderF,newAtom,inorderF);
				concat(inorderF,".\n",inorderF);
				
				/* turn : to -> */
				i = 0;
				while (inorderF[i] != '\0')
				{
					if ( (inorderF[i] == ':') && (inorderF[i-1] != ']') )
					{
						j = i;
						while ( inorderF[i] != '\0' )
							i++;
						
						while ( i > j )
						{
							inorderF[i+1] = inorderF[i];
							i--;
						}
						inorderF[i] = '-';
						inorderF[i+1] = '>';
						i++;
					}
					i++;
				}
				
				pred_index++;
				/* write this formula to file */
#ifdef DEBUG
				printf("new formula %s\n", inorderF);
#endif	
				
				fp = fopen(".f2lp_input.fof","r+");
				fseek(fp,(long)0,SEEK_END);
				ret = fputs(inorderF,fp);
				if ( ret == EOF )
				{
					printf("writing to tmp file failed. Check permissions\n");
					exit(1);
				}
				fclose(fp);
				
				/* replace quantified formula by --new_pred / new_pred (if not S.P) */
				if ( sPositive ) {

				temp1 = (NODEP)malloc(sizeof (struct node));
				temp2 = (NODEP)malloc(sizeof (struct node));
				if ( temp1 == NULL || temp2 == NULL )
				{
					printf("out of memory\n");
					exit(1);
				}
				temp1->val[0] = '-';
				temp1->val[1] = '\0';
				temp2->val[0] = '-';
				temp2->val[1] = '\0';
				temp1->left = NULL;
				temp2->left = NULL;

				}
				strcpy(qPtr->val,newAtom);
				qPtr->right = NULL;
				qPtr->left = NULL;
				
				if ( sPositive ) {
				temp1->right = temp2;
				temp2->right = qPtr;
				}
				
				if ( qParentPtr == NULL )
				{
					if (sPositive) 	{
					*root = temp1;
					}
					else {
					*root = qPtr;
					}
					
				}
				else
				{
					//printf("parent pointer right %s\n",qParentPtr->right->val);
					if ( dir == 0 )
					{
						if (sPositive) {
						qParentPtr->left = temp1;
						}
						else {
						qParentPtr->left = qPtr;
						}
					}
					else
					{
						if (sPositive) {
						qParentPtr->right = temp1;
						}
						else {
						qParentPtr->right = qPtr;
						}
					}
				}
				
				free(newAtom);
#ifdef DEBUG
				reading_index = 0;
				inorder_save_br(*root,inorderF);
				printf("pos exist %s\n",inorderF);
#endif
			}
		}
	}
}




/* checks for extensional predicates */
int isExtensional ( char *buf, int index )
{
	char extString[12] = {(char)NULL};
	int i = 0;

	while ( (buf[index] != '\0') && (i != 11) )
		extString[i++] = buf[index++];

	extString[11] = '\0';

	if (!strcmp(extString,"extensional") && ((buf[index] == ' ') && isPredicateSymbol(buf[index+1])))
		return 1;

	return 0;
}

/* checks if the rule is completely in LP syntax */
int lpSyntax ( char *buf, int rule_arrow_index )
{
	int i = 0;
	int tuple_start = 0;
	int tuple_paran = 0;
	int paran_start = 0;
	int paran_count = 0;
	
	/* check for formula symbols */
	while ( buf[i] != '\0' )
	{
		switch(buf[i])
		{
			case '&':
			case '?':
			case '~':
				return 0;
			
			case '!':
				if ( buf[i+1] == '[' )
					return 0;

				break;
				
			case '|':
				/* since LP can contain | in the head */
				if ( i > rule_arrow_index )
					return 0;
				/* nested within - */
				if ( (paran_start != 0) && (tuple_start == 0) )
					return 0;	
				
			case '-':
				/* rule arrow */
				if ( (i != 0) && (buf[i-1] == ':') )
					break;
					
				/* implication */
				if (buf[i+1] == '>')
					return 0;
					
				/* double negation */
				if ( buf[i+1] == '-' )
					return 0;
				if ( (paran_start != 0) && (tuple_start == 0) )
					return 0; 

				break;
				
			case '(':
				/* for nested negation */	
				if (paran_start == 1)
					paran_count++;
					
				if ((i != 0) && (buf[i-1] == '-'))
				{
					if (paran_start == 0)
					{
						paran_start = 1;
						paran_count = 1;
					}
				}	
				
				/* so that negation within tuples is treated as minus */
				if (tuple_start == 1)
					tuple_paran++;
				
				if ((i != 0) && isPredicateSymbol(buf[i-1]))
				{
					if (tuple_start == 0)
					{
						tuple_start = 1;
						tuple_paran = 1;
					}
				}	
				break;
				
			case ')':
				if (paran_start == 1)
				{
					paran_count--;
					if (paran_count == 0)
						paran_start = 0;
				}
					
				if (tuple_start == 1)
				{
					tuple_paran--;
					if (tuple_paran == 0)
						tuple_start = 0;
				}
					
				break;
						
			default:
				break;
		}
		i++;
	}
	return 1;
}


int
main(int argc, char *argv[])
{
FILE           *fp = NULL, *fpInput = NULL, *fpOutput = NULL;
char            readBuf = { (char) NULL };
char            line[LINE_MAX] = { (char) NULL };

unsigned int    line_index = 0,
line_size = 0;
unsigned int    i = 0;
int             ret = 0,
k = 0;
char           *prop_str = "--pl";
char           *fol_str = "--fol";

NODEP           temp_prefix = NULL;
NODEP           top_left = NULL;

#ifdef BAR_NEG_ATOM
struct baratom *barnode = NULL;
struct negatom *negnode = NULL;
#endif

char           *toWrite = NULL;
char           *barString = NULL;
char           *barStringbar = NULL;

int             orig_lineNum = 0;
int             dlv_rule = 0;
int             extensional = -1;
int             paran_count = 0;
int             paran_mismatch_line = 0;
int             size_incr = 0;
int             solver_supported = 0;
int             comment = 0;
int             fileIndex = 0;
int 				 temp_buffer_index = 0;
int 				 rule_index = 0;
int 				 rule_arrow_index = 0;
int 				 do_not_continue = 0;

char           *inF = NULL; // (char *)&formula;
char           *prefixF = NULL;
char           *inorderF = NULL;
char 				*temp_buffer = NULL;


#ifdef DEBUG
printf("Max Line Size %d\n",LINE_MAX);
printf("Max Predicate Length %d\n",MAX_PREDICATE_LENGTH);
printf("Max Symbols %d\n",MAX_SYMBOLS);
#endif


/* print out the version number and usage information */
if (argc > 1)
{
	if ( !strcmp(argv[1],"-version") || 
             !strcmp(argv[1],"--version") ||
             !strcmp(argv[1],"-v") ||
             !strcmp(argv[1],"--v") )
	{
		printf("%s\n",VERSION);
		exit(0);
	}
	
	if ( !strcmp(argv[1],"-help") || 
             !strcmp(argv[1],"--help") ||
             !strcmp(argv[1],"-h") ||
             !strcmp(argv[1],"--h") )
	{
		printf("%s\n",VERSION);
		printf("usage:\n");
		printf("%s\n",USAGE);
		printf("%s\n",NOINPUTFILE);
		printf("options:\n");
		printf("%s\n",OPTION_HELP);
		printf("%s\n",OPTION_VERSION);
		exit(0);
	}
}

/*
* check for correct number of arguments 
*/
#if 0
if (argc < 2) {
printf("format - f2lp inputFile1 inputFile2 ... inputFileN\n");
// printf("logic - pl for propositional logic\n");
// printf(" - fol for first order logic\n");
exit(1);
}
#endif

#if 0
/*
* check for correct arguments 
*/
if (strcmp(argv[1], prop_str) && strcmp(argv[1], fol_str)) {
printf("format - ./a.out --logic filename\n");
printf("logic  - pl for propositional logic\n");
printf("       - fol for first order logic\n");
exit(1);
}
#endif
fpOutput = fopen(".f2lp_input.fof","w");
if (NULL == fpOutput) {
	printf("Input file open failed\n");
	return 1;
}

if ( argc < 2 )
{

while (1)
{
	ret = fread(&readBuf,1,1,stdin);
	if ( ret == 0 )
	{
		if (!feof(stdin))
		{
			printf("read from input failed\n");
			exit(1);
		}
		break;
	}
#if 0	
	if ( (readBuf == '@') || (readBuf == '`') )
	{
		printf("parse error. Unexpected character found %c\n",readBuf);
		exit(1);
	}
#endif
	
	if ( readBuf == '\r' )
		continue;
	
	ret = fwrite(&readBuf,1,1,fpOutput);
	if ( ret == 0 )
	{
		printf("write failed. Check permissions\n");
		exit(1);
	}
}

}
for (fileIndex = 1; fileIndex < argc; fileIndex++)
{

/* copy the file into a temporary file */
fpInput = fopen(argv[fileIndex],"r");
if (NULL == fpInput) {
	printf("Input file open failed\n");
	return 1;
}


while (1)
{
	ret = fread(&readBuf,1,1,fpInput);
	if ( ret == 0 )
	{
		if (!feof(fpInput))
		{
			printf("read from input failed\n");
			exit(1);
		}
		break;
	}
#if 0	
	if ( (readBuf == '@') || (readBuf == '`') )
	{
		printf("parse error. Unexpected character found %c\n",readBuf);
		exit(1);
	}
#endif
	
	if ( readBuf == '\r' )
		continue;
	
	ret = fwrite(&readBuf,1,1,fpOutput);
	if ( ret == 0 )
	{
		printf("write failed. Check permissions\n");
		exit(1);
	}
}

fclose(fpInput);

}
fclose(fpOutput);
/*
* open the file containing the arbitrary formulas 
*/
fp = fopen(".f2lp_input.fof", "r");
if (NULL == fp) {
printf("file open failed\n");
return 1;
}

//if ( argc == 2 )
fpSolverInput = fopen(".solver_input.lp", "w");
//else 
//fpSolverInput = fopen(argv[2], "w");

if (fpSolverInput == NULL) {
printf("opening output file failed - check permissions\n");
return 1;
}

/*
* until the file is complete 
*/
while (1) {
i = 0;
k = 0;
dlv_rule = 0;
extensional = -1;
do_not_continue = 0;

/*
if (fgets(line, LINE_MAX, fp) == NULL) {
if (!feof(fp)) {
printf("read line failed\n");
exit(1);
}
break;
}
*/
line_index = 0;
comment = 0;
paran_count = 0;
	
do
{
	ret = fread(&readBuf,1,1,fp);
	if ( ret == 0 )
	{
		if (!feof(fp))
		{
			printf("read from input failed\n");
			exit(1);
		}
		
		break;
	}
	if ( readBuf == '%' )
		comment = 1;
	
	if ( readBuf == '\n' )
	{
		comment = 0;
		orig_lineNum++;
	}

	if ( (readBuf == '(') && (comment == 0) )
	{
		paran_count++;
	}

	if ( (readBuf == ')') && (comment == 0) )
	{
		paran_count--;
	}	
	
	
	if ( ( ( (readBuf != '.') || (paran_count != 0) )  && (comment == 0)) && (readBuf != '\n') )
		line[line_index++] = readBuf;
	
}while((readBuf != '.')||(comment == 1) || (paran_count != 0));


/* if EOF and read failed */
if (ret == 0)
{
	if ( paran_count != 0 )
	{
		printf ("parse error at line %d : no matching parenthesis found.\n", paran_mismatch_line+1);
		exit(1);
	}
	break;
}

paran_mismatch_line = orig_lineNum + 1;

paran_count = 0;	
line[line_index] = '\0';
//printf("line %s\n",line);
		
/*
line_index = 0;
while (line[line_index] != '\n')
line_index++;
*/

line_size = line_index;

// printf("%d\n",line_size);
/*
* create memory for input formula 
*/
inF = (char *) malloc(LINE_MAX);
prefixF = (char *) malloc(LINE_MAX);
temp_buffer = (char *) malloc(LINE_MAX);
if (inF == NULL || prefixF == NULL || temp_buffer == NULL) {
printf("out of memory\n");
exit(1);
}
line_index = 0;

/*
* some initial processing for comments and LP rules 
*/
while (line_index < line_size) {
readBuf = line[line_index++];

if ( readBuf == '\n' )
	continue;
/*
* check for comments 
*/
if (readBuf == '%')
{
	/* ignore until end of line */
	while ( line[line_index] != '\n' )
	{
		line_index++;
		if (line_index == line_size)
			break;
	}
	continue;
}

/*
* check for LP rules 
*/
if ( (readBuf == '#') || (readBuf == ';') )
{
	if ( (readBuf == '#') && isExtensional(line,line_index) )
	{
		extensional = line_index;
	}
	dlv_rule = 1;
} 

if ( (readBuf == '.') && (line[line_index] == '.') )
	dlv_rule = 1;

if ( ( ( (readBuf == ':') && (line[line_index] == '-')) && ((line_index == 1) || (line[line_index-2] != ']')) )
	  || (readBuf == '{')) {
/*
* set the flag 
*/
dlv_rule = 1;
/* mark the index */
rule_arrow_index = i;
}



if ( (((readBuf == ' ') || (readBuf == '\t')) && ((line[line_index] == 'm') || (line[line_index] == 'M'))) &&
       ( ((line[line_index+1] == 'o') || (line[line_index+1] == 'O')) &&
       ((line[line_index+2] == 'd') || (line[line_index+2] == 'D')) ) )
{
	if ((line[line_index+3] == ' ') || (line[line_index+3] == '\t'))
	{
		/* found mod */
		/* replace with ` */
		readBuf = '`';
		line_index = line_index + 4;
		
	}
} 

/*
* ignore spaces 
*/
if ((readBuf == ' ') || (readBuf == '\t'))
continue;

/*
* read the char 
*/
inF[i++] = readBuf;


#if 0
/*
* check for 'not' 
*/
if (readBuf == 'n') {
if ((i == 0) || !isPredicateSymbol(inF[i - 1])) {
/*
* it is possible that 'not' is present 
*/
if (line_index < line_size - 3) {
if ((line[line_index] == 'o' &&
	line[line_index + 1] == 't') &&
line[line_index + 2] == ' ') {
/*
	* not is present 
	*/
inF[i++] = '~';
line_index = line_index + 3;
} else {
/*
	* read the char 
	*/
inF[i++] = readBuf;
}
} else { /* if line_index >= line_size - 3 */

/*
* read the char 
*/
inF[i++] = readBuf;
}
} else { /* if i != 0 && some name before */

/*
* read the char 
*/
inF[i++] = readBuf;
}
}
#endif


}
inF[i] = '\0';
/*
* if LP rule, write the line to output file and continue 
*/
if (dlv_rule == 1)
{
	if ( extensional != -1 )
	{
		/* parse the entire construct "extensional" */
		while ( line[extensional] != ' ' )
			extensional++;

		extensional++;
		
		readBuf = '{';
		ret = fwrite(&readBuf, 1, 1, fpSolverInput);
		if (ret == 0) {
			printf("writing LP rule to output file failed\n");
			exit(1);
		}

		while (line[extensional] != '\0')
		{
			readBuf = line[extensional++];
			ret = fwrite(&readBuf, 1, 1, fpSolverInput);
			if (ret == 0) {
				printf("writing LP rule to output file failed\n");
				exit(1);
			}
		}	
		ret = fwrite("}.\n", sizeof("}.\n")-1, 1, fpSolverInput);
		if (ret == 0) {
			printf("writing LP rule to output file failed\n");
			exit(1);
		}
	}
	else
	{
		/* check if it is not completely in LP syntax */
		if (!lpSyntax(inF, rule_arrow_index))
		{
			/* write in formula syntax */
			rule_index = rule_arrow_index+2;
			temp_buffer_index = 0;
			while (inF[rule_index] != '\0')
				temp_buffer[temp_buffer_index++] = inF[rule_index++];

			/* add implication */
			temp_buffer[temp_buffer_index++] = '-';
			temp_buffer[temp_buffer_index++] = '>';		
		
			rule_index = 0;
			while (rule_index != rule_arrow_index)
				temp_buffer[temp_buffer_index++] = inF[rule_index++];
				
			temp_buffer[temp_buffer_index] = '\0';
			
			/* copy formula to inF */			
			temp_buffer_index = 0;
			while (temp_buffer[temp_buffer_index] != '\0')
			{
				inF[temp_buffer_index] = temp_buffer[temp_buffer_index]; 	
				temp_buffer_index++;
			}	
			inF[temp_buffer_index] = '\0';
			do_not_continue = 1;
		}
		else
		{
			//convert_neg_lp (inF, orig_lineNum, &size_incr); 
			//ret = fwrite(inF, i+size_incr, 1, fpSolverInput);
			ret = fwrite(line, line_size, 1, fpSolverInput);
			if (ret == 0) {
				printf("writing LP rule to output file failed\n");
				exit(1);
			}
			ret = fwrite(".", 1, 1, fpSolverInput);
			if (ret == 0) {
				printf("writing LP rule to output file failed\n");
				exit(1);
			}
			ret = fwrite("\n", 1, 1, fpSolverInput);
			if (ret == 0) {
				printf("writing LP rule to output file failed\n");
				exit(1);
			}
		}
	}
		//orig_lineNum++;
	if (!do_not_continue)
		continue;
}



//printf("inF %s\n",inF);
/*
* empty lines 
*/
if (i == 0)
{
	//orig_lineNum++;
	continue;
}

#if 0
/*
* if dlv rule, convert to arb. formula 
*/
if (dlv_rule) {
convert_arb(inF, orig_lineNum);
}
#endif

/*
* take care of empty antecedents or consequents 
*/
add_tf(inF);
/*
* convert '~' to '-' and '-' to '~' 
*/
//convert_neg(inF, orig_lineNum);

// printf("after convert_neg %s\n", inF);
/* convert - to @ */
convert_specialSymbols(inF, orig_lineNum);
#ifdef DEBUG
printf("after coverting special symbols %s\n",inF);
#endif

/* replace special predicates by normal predicates */
replaceSpecialPred(inF, orig_lineNum);
#ifdef DEBUG
printf("after replacing special predicates %s\n",inF);
#endif
/*
* convert -> to : 
*/
convert_imp(inF, orig_lineNum);


/*
* parse expression 
*/
parse_exp ( inF, orig_lineNum ); 

add_dollar(inF);
//printf("after dollar %s\n",inF);

toPrefix(inF, prefixF);
#ifdef DEBUG
printf("prefix %s\n",prefixF);
#endif

temp_prefix = prefix_to_tree(prefixF, (index_prefix = 0));
inorderF = (char *) malloc(LINE_MAX);


#ifdef DEBUG
reading_index = 0;
inorder_save(temp_prefix, inorderF);
printf("inorderF %s\n",inorderF);
#endif
/* eliminate quantifiers */
elim_quantifier (&temp_prefix);


#ifdef DEBUG
reading_index = 0;
inorder_save_br(temp_prefix, inorderF);
printf("elim quantifier %s\n",inorderF);
#endif
//while(1);


//printf("calling NNF\n");
/* convert to NNF - requirement to apply the transformation */
convert_NNF(temp_prefix);
#ifdef DEBUG
reading_index = 0;
inorder_save_br(temp_prefix, inorderF);
printf("convert NNF %s\n",inorderF);
#endif
//inorder_print(temp_prefix);
//printf("\n");
//while(1);

/*
* rewrite F as true -> F if not already in implication format
*/
if ( temp_prefix->val[0] != ':' )
{
	root_prefix = (NODEP) malloc(sizeof(struct node));
	top_left = (NODEP) malloc(sizeof(struct node));
	
	top_left->val[0] = 't';
	top_left->val[1] = 'r';
	top_left->val[2] = 'u';
	top_left->val[3] = 'e';
	top_left->val[4] = '\0';
	top_left->left = (NODEP) NULL;
	top_left->right = (NODEP) NULL;
	
	root_prefix->val[0] = ':';
	root_prefix->val[1] = '\0';
	root_prefix->left = top_left;
	root_prefix->right = temp_prefix;
}
else
{
	root_prefix = temp_prefix;
}

// inorder_print(root_prefix);
// root_prefix = root;
// check_exp ( root, lineNum );

/*
* apply rules L1-L5 and R1-R5 
*/
applyRules(root_prefix);

//orig_lineNum++;

}    /* end of while(1) */

fclose(fp);

#ifdef BAR_NEG_ATOM
toWrite = (char *) malloc(2 * MAX_PREDICATE_LENGTH + 25);
barString = (char *) malloc(MAX_PREDICATE_LENGTH + 10);
barStringbar = (char *) malloc(MAX_PREDICATE_LENGTH + 10);


// printf("before adding neg and bar\n");

/*
* Write bar and neg atoms 
*/

for (barnode = root_baratom; barnode != NULL; barnode = barnode->next) {
concat(":- ", barnode->val, barString);
concat(barString, ",", barString);

for (i = 0; isPredicateSymbol(barnode->val[i]); i++)
barStringbar[i] = barnode->val[i];

k = i;
barStringbar[k++] = 'b';
barStringbar[k++] = 'a';
barStringbar[k++] = 'r';

if (barnode->val[i] == '(') {
paran_count = 1;
while (paran_count != 0) {
barStringbar[k++] = barnode->val[i++];
if (barnode->val[i] == '(')
paran_count++;
if (barnode->val[i] == ')')
paran_count--;
}
barStringbar[k++] = ')';
}
barStringbar[k] = '\0';

concat(barString, barStringbar, toWrite);
concat(toWrite, ".\n", toWrite);

for (i = 0; toWrite[i] != '\0'; i++);
// printf("towrite %s %d\n",toWrite,i);

ret = fwrite(toWrite, i, 1, fpSolverInput);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);

}
// printf("wrote %s %d\n",toWrite,i);

concat("not ", barnode->val, barString);
concat(barString, ".\n", barString);
concat(barStringbar, " :- ", toWrite);
concat(toWrite, barString, toWrite);

for (i = 0; toWrite[i] != '\0'; i++);

ret = fwrite(toWrite, i, 1, fpSolverInput);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}
// printf("wrote %s %d\n",toWrite,i);

}

for (negnode = root_negatom; negnode != NULL; negnode = negnode->next) {
concat(":- ", negnode->val, barString);
concat(barString, ",", barString);

for (i = 0; isPredicateSymbol(negnode->val[i]); i++)
barStringbar[i] = negnode->val[i];

k = i;
barStringbar[k++] = 'n';
barStringbar[k++] = 'e';
barStringbar[k++] = 'g';

if (negnode->val[i] == '(') {
paran_count = 1;
while (paran_count != 0) {
barStringbar[k++] = negnode->val[i++];
if (negnode->val[i] == '(')
paran_count++;
if (negnode->val[i] == ')')
paran_count--;
}
barStringbar[k++] = ')';
}
barStringbar[k] = '\0';

concat(barString, barStringbar, toWrite);
concat(toWrite, ".\n", toWrite);

for (i = 0; toWrite[i] != '\0'; i++);

ret = fwrite(toWrite, i, 1, fpSolverInput);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);

}
}
#endif
// printf("wrote all rules\n");

ret = fwrite("true.\n", 1, sizeof("true.\n") - 1, fpSolverInput);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}

ret =
fwrite(":- false.\n", 1, sizeof(":- false.\n") - 1, fpSolverInput);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}
ret = fwrite("#hide true.\n", 1, sizeof("#hide true.\n") - 1, fpSolverInput);
if (ret == 0) {
	printf("write to output file failed\n");
	exit(1);
}

/* hiding new predicates */
/* need to find the arity and maybe the domain variables for that */
#if 0
for ( pred_index_var = 1; pred_index_var < pred_index; pred_index_var++ )
{
	newAtom = (char *)malloc(8+no_of_char(pred_index_var)+1);
	hideNewAtom = (char *)malloc(5+8+no_of_char(pred_index_var)+1+1);
	concat ("new_pred",itoa(pred_index_var,10),newAtom);
	concat ("hide ",newAtom,hideNewAtom);

	i = 0;
	while (hideNewAtom[i] != '\0')
		i++;

	ret = fwrite(hideNewAtom, 1, i, fpSolverInput);
	if (ret == 0) {
		printf("write to output file failed\n");
		exit(1);
	}

	ret = fwrite("(K).\n", 1, sizeof("(K).\n") - 1, fpSolverInput);
	if (ret == 0) {
		printf("write to output file failed\n");
		exit(1);
	}

}
#endif

if (fclose(fpSolverInput) != 0) {
printf("Closing output file failed\n");
}



/*
* print the output
*/

//if ( argc == 2 )
{
	fpSolverInput = fopen(".solver_input.lp","r");
	if ( fpSolverInput == NULL )
	{
		printf("opening the solver input failed\n");
		exit(1);
	}
	
	while (1)
	{
		
		if (fgets(line,LINE_MAX,fpSolverInput) == NULL)
		{
			if (!feof(fpSolverInput))
			{
				printf("reading from output file solver input failed\n");
				exit(1);
			}
			break;
		}
		
		printf("%s",line);
			
	}
	
	if (fclose(fpSolverInput) != 0) {
		printf("Closing output file failed\n");
	}
	
}


return 0;
}


void
concat(char *str1, char *str2, char *dst)
{
int             i = 0,
k = 0;
for (i = 0; str1[i] != '\0'; i++)
dst[k++] = str1[i];
for (i = 0; str2[i] != '\0'; i++)
dst[k++] = str2[i];
dst[k] = '\0';
}


void find_other_conn (NODEP root, char symbol,  struct node **connPtr, struct node  **prevPtr, int *dir)
{
	if ( root == NULL )
		return;
	
	if ( (root->val[0] != symbol) && !isPredicateSymbol(root->val[0]) )
	{
		if (root->val[0] != '-')
		{
			*connPtr = root;
			//printf("found %c\n",root->val[0]);
			other_found = 1;
		}
		else if ( !isPredicateSymbol(root->right->val[0]) )
		{
			*connPtr = root;
			//printf("found %c\n",root->val[0]);
			other_found = 1;
		}
		
	}
	
	if (!other_found)
	{
		*dir = 0;
		*prevPtr = root;
		find_other_conn (root->left, symbol, connPtr, prevPtr, dir);
	}
	
	if (!other_found)
	{
		*dir = 1;
		*prevPtr = root;
		find_other_conn (root->right, symbol, connPtr, prevPtr, dir);
	}
	 
}



void copy_tree(NODEP src, struct node **dst)
{
	NODEP temp = NULL;
	
	if (src == NULL)
		return;
	//printf("In copy_tree\n");
	
	temp = (NODEP)malloc(sizeof(struct node));
	strcpy(temp->val,src->val);
	temp->left = NULL;
	temp->right = NULL;
	*dst = temp;
	
	copy_tree(src->left,&((*dst)->left));
	copy_tree(src->right,&((*dst)->right));
	//printf("exiting copy tree\n");
}



void
applyRules(NODEP root)
{
NODEP           temp1 = NULL,
temp2 = NULL,
temp3 = NULL,
temp4 = NULL,
temp5 = NULL;
NODEP new_root = NULL;
NODEP connPtr = NULL, prevPtr = NULL;
int 		dir = 0;
int             cons_flag = 1;
int             ant_flag = 1;
char *inorderF = NULL;

#ifdef DEBUG
inorderF = (char *)malloc(LINE_MAX);
#endif

//printf("In apply rules\n");
//inorder_save(root, rule);
//printf("rule %s\n",rule);
//inorder_print(root);
/*
* check if already in rule format 
*/
/*
* check if consequent has only - and | 
*/
checkRuleForm(root->right, &cons_flag, 1);
/*
* check if antecedent has only - and & 
*/
checkRuleForm(root->left, &ant_flag, 0);
if (ant_flag && cons_flag) {
// printf("in rule form - writing into file\n");
// inorder_print(root);
format_and_write(root);
return;
}
/*
* apply rules L1-L5 and R1-R5 
*/
/*
* apply any rule that fits 
*/
/*
* check for L1 
*/
if ((!strcmp(root->left->val, "&")) &&
((!strcmp(root->left->left->val, "true")) ||
(!strcmp(root->left->right->val, "true")))) {

#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In L1 %s\n",inorderF);
#endif	
	//inorder_print(root);
/*
* form a new tree for L1 and recurse 
*/
temp1 = (NODEP) malloc(sizeof(struct node));

strcpy(temp1->val, root->val);
if (!strcmp(root->left->left->val, "true"))
temp2 = root->left->right;
else
temp2 = root->left->left;

temp3 = root->right;
temp1->left = temp2;
temp1->right = temp3;

/*
* recurse 
*/
applyRules(temp1);
}
/*
* check for L2 
*/
else if ((!strcmp(root->left->val, "&")) &&
((!strcmp(root->left->left->val, "false")) ||
(!strcmp(root->left->right->val, "false")))) {
return;
}
/*
* check for L3 
*/
else if ((root->left->val[0] == '&') &&
(((root->left->left->val[0] == '-') &&
	(root->left->left->right->val[0] == '-')) ||
((root->left->right->val[0] == '-') &&
	(root->left->right->right->val[0] == '-')))) {
#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In L3 %s\n",inorderF);
#endif
	
if (((root->left->left->val[0] == '-') &&
(root->left->left->right->val[0] == '-'))) {
	
/*
* form a new tree for L3 and recurse 
*/
temp1 = (NODEP) malloc(sizeof(struct node));
temp2 = (NODEP) malloc(sizeof(struct node));

strcpy(temp1->val, root->val);

temp2->val[0] = '|';
temp2->val[1] = '\0';
temp3 = root->left->left->right;
temp4 = root->right;
temp2->left = temp3;
temp2->right = temp4;

temp1->left = root->left->right;
temp1->right = temp2;

applyRules(temp1);
} else {
/*
* form a new tree for L3 and recurse 
*/
temp1 = (NODEP) malloc(sizeof(struct node));
temp2 = (NODEP) malloc(sizeof(struct node));

strcpy(temp1->val, root->val);

temp2->val[0] = '|';
temp2->val[1] = '\0';
temp3 = root->left->right->right;
temp4 = root->right;
temp2->left = temp3;
temp2->right = temp4;

temp1->left = root->left->left;
temp1->right = temp2;

applyRules(temp1);
}
} else if ((root->left->val[0] == '-') &&
	(root->left->right->val[0] == '-')) {
	
#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In L3 %s\n",inorderF);
#endif
	/*
	* form a new tree for L3 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp5 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp2->val[0] = '|';
	temp2->val[1] = '\0';
	temp3 = root->left->right;
	temp4 = root->right;
	temp2->left = temp3;
	temp2->right = temp4;

	strcpy(temp5->val, "true");
	temp5->left = NULL;
	temp5->right = NULL;

	temp1->left = temp5;
	temp1->right = temp2;

	applyRules(temp1);
}
/*
* check for L4 
*/
else if ((root->left->val[0] == '&') &&
	((root->left->left->val[0] == '|') ||
	(root->left->right->val[0] == '|'))) {
	
#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In L4 %s\n",inorderF);
#endif
	
	if (root->left->left->val[0] == '|') {
	/*
	* form a new trees for L4 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp2->val[0] = '&';
	temp2->val[1] = '\0';
	temp2->left = root->left->left->left;
	temp2->right = root->left->right;

	temp1->left = temp2;
	temp1->right = root->right;

	applyRules(temp1);

	
	temp2->left = root->left->left->right;
	
	applyRules(temp1);


	} else {
	/*
	* form a new trees for L4 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp2->val[0] = '&';
	temp2->val[1] = '\0';
	temp2->left = root->left->right->left;
	temp2->right = root->left->left;

	temp1->left = temp2;
	temp1->right = root->right;

	//printf("L4 - first rule\n");
	//inorder_print(temp1);
	applyRules(temp1);

	
	temp2->left = root->left->right->right;

	//printf("L4 - second rule\n");
	//inorder_print(temp1);
	applyRules(temp1);
	}
} else if (root->left->val[0] == '|') {

#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In L4 %s\n",inorderF);
#endif
	/*
	* form new trees for L4 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp1->left = root->left->left;
	temp1->right = root->right;

	applyRules(temp1);

	
	
	temp1->left = root->left->right;

	applyRules(temp1);

}
/*
* check for L5 
*/
else if ((root->left->val[0] == '&') &&
	((root->left->left->val[0] == ':') ||
	(root->left->right->val[0] == ':'))) {
#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In L5 %s\n",inorderF);
#endif	
	
	if (root->left->left->val[0] == ':') {
	/*
	* form a new trees for L5 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp3 = (NODEP) malloc(sizeof(struct node));
	temp4 = (NODEP) malloc(sizeof(struct node));
	temp5 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp2->val[0] = '&';
	temp2->val[1] = '\0';
	temp3->val[0] = '-';
	temp3->val[1] = '\0';
	temp3->left = NULL;
	temp3->right = root->left->left->left;

	temp2->left = temp3;
	temp2->right = root->left->right;

	temp1->left = temp2;
	temp1->right = root->right;

	convert_NNF(temp1);
	applyRules(temp1);
	
	temp2->left = root->left->left->right;
	temp2->right = root->left->right;

	convert_NNF(temp1);
	applyRules(temp1);
	
	temp3->val[0] = '-';
	temp3->val[1] = '\0';
	temp3->left = NULL;

	temp1->left = root->left->right;

	temp4->val[0] = '|';
	temp4->val[1] = '\0';
	temp4->right = root->right;

	temp5->val[0] = '|';
	temp5->val[1] = '\0';
	temp3->right = root->left->left->right;
	temp5->left = root->left->left->left;
	temp5->right = temp3;
	temp4->left = temp5;

	temp1->right = temp4;
	convert_NNF(temp1);
	applyRules(temp1);

	} else {
	/*
	* form new trees for L5 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp3 = (NODEP) malloc(sizeof(struct node));
	temp4 = (NODEP) malloc(sizeof(struct node));
	temp5 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp2->val[0] = '&';
	temp2->val[1] = '\0';
	temp3->val[0] = '-';
	temp3->val[1] = '\0';
	temp3->left = NULL;
	temp3->right = root->left->right->left;

	temp2->left = temp3;
	temp2->right = root->left->left;

	temp1->left = temp2;
	temp1->right = root->right;

	convert_NNF(temp1);
	applyRules(temp1);

	temp2->left = root->left->right->right;
	temp2->right = root->left->left;

	convert_NNF(temp1);
	applyRules(temp1);
	
	temp3->val[0] = '-';
	temp3->val[1] = '\0';
	temp3->left = NULL;

	temp1->left = root->left->left;

	temp4->val[0] = '|';
	temp4->val[1] = '\0';
	temp4->right = root->right;

	temp5->val[0] = '|';
	temp5->val[1] = '\0';
	temp3->right = root->left->right->right;
	temp5->left = root->left->right->left;
	temp5->right = temp3;
	temp4->left = temp5;

	temp1->right = temp4;
	convert_NNF(temp1);
	applyRules(temp1);
	}
} else if (root->left->val[0] == ':') {

#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In L5 %s\n",inorderF);
#endif	
	/*
	* form new trees for L5 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp3 = (NODEP) malloc(sizeof(struct node));
	temp4 = (NODEP) malloc(sizeof(struct node));
	temp5 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp3->val[0] = '-';
	temp3->val[1] = '\0';
	temp3->left = NULL;
	temp3->right = root->left->left;

	temp1->left = temp3;
	temp1->right = root->right;

	convert_NNF(temp1);
	applyRules(temp1);

	temp1->left = root->left->right;

	convert_NNF(temp1);
	applyRules(temp1);
	
	temp3->val[0] = '-';
	temp3->val[1] = '\0';
	temp3->left = NULL;

	strcpy(temp2->val, "true");
	temp2->left = NULL;
	temp2->right = NULL;
	temp1->left = temp2;

	temp4->val[0] = '|';
	temp4->val[1] = '\0';
	temp4->right = root->right;

	temp5->val[0] = '|';
	temp5->val[1] = '\0';
	temp3->right = root->left->right;
	temp5->left = root->left->left;
	temp5->right = temp3;
	temp4->left = temp5;

	temp1->right = temp4;
	convert_NNF(temp1);
	applyRules(temp1);

}
  
/*
* check for R1 
*/
else if ((!strcmp(root->right->val, "|")) &&
	((!strcmp(root->right->left->val, "false")) ||
	(!strcmp(root->right->right->val, "false")))) {
#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In R1 %s\n",inorderF);
#endif	
	/*
	* form a new tree for R1 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);
	if (!strcmp(root->right->left->val, "false"))
	temp2 = root->right->right;
	else
	temp2 = root->right->left;

	temp1->right = temp2;
	temp1->left = root->left;

	/*
	* recurse 
	*/
	applyRules(temp1);
}
/*
* check for R2 
*/
else if ((!strcmp(root->right->val, "|")) &&
	((!strcmp(root->right->left->val, "true")) ||
	(!strcmp(root->right->right->val, "true")))) {
	return;
}
/*
* check for R3 
*/
else if ((root->right->val[0] == '|') &&
	(((root->right->left->val[0] == '-') &&
	(root->right->left->right->val[0] == '-')) ||
	((root->right->right->val[0] == '-') &&
	(root->right->right->right->val[0] == '-')))) {
#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In R3 %s\n",inorderF);
#endif	
	
	if (((root->right->left->val[0] == '-') &&
	(root->right->left->right->val[0] == '-'))) {
	/*
	* form a new tree for R3 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp2->val[0] = '&';
	temp2->val[1] = '\0';
	temp3 = root->right->left->right;
	temp4 = root->left;
	temp2->left = temp3;
	temp2->right = temp4;

	temp1->left = temp2;
	temp1->right = root->right->right;

	applyRules(temp1);
	} else {
	/*
	* form a new tree for R3 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp2->val[0] = '&';
	temp2->val[1] = '\0';
	temp3 = root->right->right->right;
	temp4 = root->left;
	temp2->left = temp3;
	temp2->right = temp4;

	temp1->left = temp2;
	temp1->right = root->right->left;

	applyRules(temp1);
	}
} else if ((root->right->val[0] == '-') &&
	(root->right->right->val[0] == '-')) {

#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In R3 %s\n",inorderF);
#endif	
	/*
	* form a new tree for R3 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp5 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp2->val[0] = '&';
	temp2->val[1] = '\0';
	temp3 = root->right->right;
	temp4 = root->left;
	temp2->left = temp3;
	temp2->right = temp4;

	temp1->left = temp2;
	strcpy(temp5->val, "false");
	temp5->left = NULL;
	temp5->right = NULL;
	temp1->right = temp5;

	applyRules(temp1);
}
/*
* check for R4 
*/
else if ((root->right->val[0] == '|') &&
	((root->right->left->val[0] == '&') ||
	(root->right->right->val[0] == '&'))) {
#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In R4 %s\n",inorderF);
#endif	
	
	if (root->right->left->val[0] == '&') {
	/*
	* form new trees for R4 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp2->val[0] = '|';
	temp2->val[1] = '\0';
	temp2->left = root->right->left->left;
	temp2->right = root->right->right;

	temp1->right = temp2;
	temp1->left = root->left;

	applyRules(temp1);

	temp2->left = root->right->left->right;

	applyRules(temp1);


	} else {
	/*
	* form new trees for R4 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp2->val[0] = '|';
	temp2->val[1] = '\0';
	temp2->left = root->right->right->left;
	temp2->right = root->right->left;

	temp1->right = temp2;
	temp1->left = root->left;

	applyRules(temp1);

	temp2->left = root->right->right->right;

	applyRules(temp1);
	}
} else if (root->right->val[0] == '&') {

#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In R4 %s\n",inorderF);
#endif	
	/*
	* form new trees for R4 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);

	temp1->right = root->right->left;
	temp1->left = root->left;

	applyRules(temp1);

	temp1->right = root->right->right;

	applyRules(temp1);


}
/*
* check for R5 
*/
else if ((root->right->val[0] == '|') &&
	((root->right->left->val[0] == ':') ||
	(root->right->right->val[0] == ':'))) {
#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In R5 %s\n",inorderF);
#endif	
	
	if (root->right->left->val[0] == ':') {
	/*
	* form new trees for R5 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp3 = (NODEP) malloc(sizeof(struct node));
	temp4 = (NODEP) malloc(sizeof(struct node));
	temp5 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);
	/*
	* antecedent 
	*/
	temp2->val[0] = '&';
	temp2->val[1] = '\0';
	temp2->left = root->right->left->left;
	temp2->right = root->left;

	/*
	* consequent 
	*/
	temp3->val[0] = '|';
	temp3->val[1] = '\0';
	temp3->left = root->right->left->right;
	temp3->right = root->right->right;

	temp1->left = temp2;
	temp1->right = temp3;

	convert_NNF(temp1);
	applyRules(temp1);

	/*
	* antecedent 
	*/
	temp4->val[0] = '-';
	temp4->val[1] = '\0';
	temp4->left = NULL;
	temp4->right = root->right->left->right;

	temp2->left = temp4;

	/*
	* consequent 
	*/
	temp5->val[0] = '-';
	temp5->val[1] = '\0';
	temp5->left = NULL;
	temp5->right = root->right->left->left;

	temp3->left = temp5;

	convert_NNF(temp1);
	applyRules(temp1);

	} else {
	/*
	* form new trees for R5 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp3 = (NODEP) malloc(sizeof(struct node));
	temp4 = (NODEP) malloc(sizeof(struct node));
	temp5 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);
	/*
	* antecedent 
	*/
	temp2->val[0] = '&';
	temp2->val[1] = '\0';
	temp2->left = root->right->right->left;
	temp2->right = root->left;

	/*
	* consequent 
	*/
	temp3->val[0] = '|';
	temp3->val[1] = '\0';
	temp3->left = root->right->right->right;
	temp3->right = root->right->left;

	temp1->left = temp2;
	temp1->right = temp3;

	convert_NNF(temp1);
	applyRules(temp1);

	/*
	* antecedent 
	*/
	temp4->val[0] = '-';
	temp4->val[1] = '\0';
	temp4->left = NULL;
	temp4->right = root->right->right->right;

	temp2->left = temp4;

	/*
	* consequent 
	*/
	temp5->val[0] = '-';
	temp5->val[1] = '\0';
	temp5->left = NULL;
	temp5->right = root->right->right->left;

	temp3->left = temp5;

	convert_NNF(temp1);
	applyRules(temp1);
	}
} else if (root->right->val[0] == ':') {

#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In R5 %s\n",inorderF);
#endif	
	/*
	* form new trees for R5 and recurse 
	*/
	temp1 = (NODEP) malloc(sizeof(struct node));
	temp2 = (NODEP) malloc(sizeof(struct node));
	temp3 = (NODEP) malloc(sizeof(struct node));
	temp4 = (NODEP) malloc(sizeof(struct node));
	temp5 = (NODEP) malloc(sizeof(struct node));

	strcpy(temp1->val, root->val);
	/*
	* antecedent 
	*/
	temp2->val[0] = '&';
	temp2->val[1] = '\0';
	temp2->left = root->right->left;
	temp2->right = root->left;

	/*
	* consequent 
	*/
	temp1->left = temp2;
	temp1->right = root->right->right;

	convert_NNF(temp1);
	applyRules(temp1);

	/*
	* antecedent 
	*/
	temp4->val[0] = '-';
	temp4->val[1] = '\0';
	temp4->left = NULL;
	temp4->right = root->right->right;

	temp2->left = temp4;

	/*
	* consequent 
	*/
	temp5->val[0] = '-';
	temp5->val[1] = '\0';
	temp5->left = NULL;
	temp5->right = root->right->left;

	temp1->right = temp5;
	convert_NNF(temp1);
	applyRules(temp1);

}
/*
 * to apply associativity
 */ 
else
{

#ifdef DEBUG
	reading_index = 0;
	inorder_save_br(root,inorderF);	
	printf("In assoc. %s\n",inorderF);
#endif
	//printf("entering asso\n");
	
	/* copy the tree so that root is not changed */
	copy_tree(root,&new_root);
	
	//printf("printing new_root\n");
	//inorder_print(new_root);
	//printf("printing root\n");
	//inorder_print(root);
	//printf("\n");
	//while(1);
	other_found = 0;
	find_other_conn (new_root->left->left, '&', &connPtr, &prevPtr, &dir);
	
	
	if ( connPtr != NULL )
	{
		/* found another connective */
		/* exchange with root->left->right */
		temp1 = new_root->left->right;
		new_root->left->right = connPtr;
		if ( dir == 0 )
			prevPtr->left = temp1;
		else
			prevPtr->right = temp1;
		
		applyRules(new_root);
		
	}
	else
	{
		other_found = 0;
		find_other_conn (new_root->left->right, '&', &connPtr, &prevPtr, &dir);
		if ( connPtr != NULL )
		{
			/* found another connective */
			/* exchange with root->left->left */
			temp1 = new_root->left->left;
			new_root->left->left = connPtr;
			if ( dir == 0 )
				prevPtr->left = temp1;
			else
				prevPtr->right = temp1;
			
			applyRules(new_root);
				
		}

		else
		{
			if ( root->right->val[0] == '|' )
			{
				#ifdef DEBUG
				reading_index = 0;
				inorder_save_br(root,inorderF);	
				printf("In assoc. R %s\n",inorderF);
				#endif
				copy_tree(root,&new_root);
				//printf("printing new_root\n");
				//inorder_print(new_root);
				//printf("printing root\n");
				//inorder_print(root);
				//printf("\n");
				//while(1);
	
				other_found = 0;
				find_other_conn (new_root->right->left, '|', &connPtr, &prevPtr, &dir);
				if ( connPtr != NULL )
				{
					/* found another connective */
					/* exchange with root->right->right */
					temp1 = new_root->right->right;
					new_root->right->right = connPtr;
					if ( dir == 0 )
						prevPtr->left = temp1;
					else
						prevPtr->right = temp1;
		
					applyRules(new_root);
				
				}
				else
				{
					other_found = 0;
					find_other_conn (new_root->right->right, '|', &connPtr, &prevPtr, &dir);
					if ( connPtr != NULL )
					{
						/* found another connective */
						/* exchange with root->right->left */
						temp1 = new_root->right->left;
						new_root->right->left = connPtr;
						if ( dir == 0 )
							prevPtr->left = temp1;
						else
							prevPtr->right = temp1;
			
						applyRules(new_root);
			
					}
				}
	
	
			}
			

		}
	}
	//printf("exiting asso\n");
	//inorder_print(root);
	
} 
 

}

void
checkRuleForm(NODEP root, int *rule_flag, int ant_cons_flag)
{
if (root == NULL)
return;
switch (ant_cons_flag) {
case 0:
if (!isPredicateSymbol(root->val[0]) &&
(((root->val[0] != '-') && (root->val[0] != '&'))
|| ((root->val[0] == '-') &&
(!isPredicateSymbol(root->right->val[0]))))) {
*rule_flag = 0;
}
checkRuleForm(root->left, rule_flag, ant_cons_flag);
checkRuleForm(root->right, rule_flag, ant_cons_flag);
break;
case 1:
if (!isPredicateSymbol(root->val[0]) &&
(((root->val[0] != '-') && (root->val[0] != '|'))
|| ((root->val[0] == '-') &&
(!isPredicateSymbol(root->right->val[0]))))) {
*rule_flag = 0;
}
checkRuleForm(root->left, rule_flag, ant_cons_flag);
checkRuleForm(root->right, rule_flag, ant_cons_flag);
break;
}
}



void convert_specialPred ( char *in, char *out, char *body )
{
	int index = 0;
	int neg = 0, body_index = 0;
	int antecedent = 1;
	int j = 0;
	int gt = 0, lt = 0;
	int tuple_paran_count = 0;
	int removedOr = 0;
	
	while (in[index] != '\0')
	{
		removedOr = 0;
		
		neg = 0;
		switch ( in[index] )
		{
				
			case ':':
				antecedent = 0;
				out[j++] = in[index++];
				break;
				
			case 'e':
				tuple_paran_count = 0;
				/* maybe equality */
				if ( (index == 0) || !isPredicateSymbol(in[index-1]) )
				{
					if ( (in[index+1]  != 'q') || (in[index+2]  != '(') )
					{
						out[j++] = in[index++];
						break;
					}
					
					if ( (index > 0) && (in[index - 1] == '-') )
					{
						/* remove the negation since it is a special symbol */
						neg = 1;
						j--;
					}
					
					if ( antecedent == 1 )
					{
						index = index + 3;
						/* write in infix form */
						
						while ( (in[index] != ',') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							
							out[j++] = in[index++];
						}
						
						/*	
						while ( in[index] != ',' )
						{
							out[j++] = in[index++]; 
						}
						*/
						index++;
						/* if negation, then change the symbol */
						if ( neg == 1 )
						{
							out[j++] = '!';
						}
						
						out[j++] = '=';
						tuple_paran_count = 0;
						
						while ( (in[index] != ')') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							
							out[j++] = in[index++];
						}
						index++;
						
					}
					else /* if in consequent */
					{
						/* remove the connective if not an implication */
						if ( out[j-1] != ':' )
						{
							j--;
							removedOr = 1;
						}
						index = index + 3;
						/* write in infix form */
						while ( (in[index] != ',') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							
							if ( in[index] == '@' )
							{
								body[body_index++] = '-';
								index++;
							}
							else if ( in[index] == '`' )
							{
				
								body[body_index++] = ' ';
								body[body_index++] = 'm';
								body[body_index++] = 'o';
								body[body_index++] = 'd';
								body[body_index++] = ' ';
								index++;
							}
							else
							{
								body[body_index++] = in[index++]; 
							}
						}
						index++;
						
						/* if not negation, then change symbol */
						if ( neg == 0 )
						{
							body[body_index++] = '!';
						}
						
						body[body_index++] = '=';
					
						tuple_paran_count = 0;	
						while ( (in[index] != ')') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							
							if ( in[index] == '@' )
							{
								body[body_index++] = '-';
								index++;
							}
							else if ( in[index] == '`' )
							{
				
								body[body_index++] = ' ';
								body[body_index++] = 'm';
								body[body_index++] = 'o';
								body[body_index++] = 'd';
								body[body_index++] = ' ';
								index++;
							}
							else
							{
								body[body_index++] = in[index++]; 
							}
						}
						index++;
						/* If | was not removed earlier, then skip the next | */
						if (removedOr == 0 )
						{
							if (in[index] == '|')
								index++;
						}
						body[body_index++] = ',';
						body[body_index] = '\0';
						
					}
				}
				else
				{
					out[j++] = in[index++];
				}
				
				break;
				
			case 'n':
				tuple_paran_count = 0;
				/* maybe equality */
				if ( (index == 0) || !isPredicateSymbol(in[index-1]) )
				{
					if ( (in[index+1]  != 'e') || (in[index+2]  != 'q') || (in[index+3]  != '('))
					{
						out[j++] = in[index++];
						break;
					}
					
					if ( (index > 0) && (in[index - 1] == '-') )
					{
						/* remove the negation since it is a special symbol */
						neg = 1;
						j--;
					}
					
					if ( antecedent == 1 )
					{
						index = index + 4;
						/* write in infix form */
						while ( (in[index] != ',') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;

							out[j++] = in[index++]; 
						}
						index++;
						/* if not negation, then do not change the symbol */
						if ( neg == 0 )
						{
							out[j++] = '!';
						}
						
						out[j++] = '=';
						
						tuple_paran_count = 0;
						while ( (in[index] != ')') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							
							out[j++] = in[index++];
						}
						index++;
						
					}
					else /* if in consequent */
					{
						/* remove the connective if not an implication */
						if ( out[j-1] != ':' )
						{
							j--;
							removedOr = 1;
						}
						index = index + 4;
						/* write in infix form */
						while ( (in[index] != ',') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;

							if ( in[index] == '@' )
							{
								body[body_index++] = '-';
								index++;
							}
							else if ( in[index] == '`' )
							{
				
								body[body_index++] = ' ';
								body[body_index++] = 'm';
								body[body_index++] = 'o';
								body[body_index++] = 'd';
								body[body_index++] = ' ';
								index++;
							}
							else
							{
								body[body_index++] = in[index++]; 
							} 
						}
						index++;
						
						/* if negation, then do not change symbol */
						if ( neg == 1 )
						{
							body[body_index++] = '!';
						}
						
						body[body_index++] = '=';
						
						tuple_paran_count = 0;
						while ( (in[index] != ')') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							
							if ( in[index] == '@' )
							{
								body[body_index++] = '-';
								index++;
							}
							else if ( in[index] == '`' )
							{
				
								body[body_index++] = ' ';
								body[body_index++] = 'm';
								body[body_index++] = 'o';
								body[body_index++] = 'd';
								body[body_index++] = ' ';
								index++;
							}
							else
							{
								body[body_index++] = in[index++]; 
							}
						}
						index++;
						/* If | was not removed earlier, then skip the next | */
						if (removedOr == 0 )
						{
							if (in[index] == '|')
								index++;
						}
						body[body_index++] = ',';
						body[body_index] = '\0';
						
					}
				}
				else
				{
					out[j++] = in[index++];
				}
				
				break;
				
			case 'l':
				tuple_paran_count = 0;
				/* maybe less than */
				if ( (index == 0) || !isPredicateSymbol(in[index-1]) )
				{
					if ( ( (in[index+1] != 't') && (in[index+1]  != 'e') ) || (in[index+2]  != '(') )
					{
						out[j++] = in[index++];
						break;
					}
					
					if ( in[index+1] == 't' )
						lt = 1;
					else
						lt = 0;
					
					if ( (index > 0) && (in[index - 1] == '-') )
					{
						/* remove the negation since it is a special symbol */
						neg = 1;
						j--;
					}
					
					if ( antecedent == 1 )
					{
						index = index + 3;
						/* write in infix form */
						while ( (in[index] != ',') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							out[j++] = in[index++]; 
						}
						index++;
						/* if negation, then change the symbol */
						if ( neg == 1 )
						{
							if ( lt == 1 )
							{
								out[j++] = '>';
								out[j++] = '=';
							}
							else
							{
								out[j++] = '>';
							}
						}
						else
						{
							if ( lt == 1 )
							{
								out[j++] = '<';
							}
							else
							{
								out[j++] = '<';
								out[j++] = '=';
							}
						}
						
						
						tuple_paran_count = 0;
						while ( (in[index] != ')') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							
							out[j++] = in[index++];
						}
						index++;
						
					}
					else /* if in consequent */
					{
						/* remove the connective if not an implication */
						if ( out[j-1] != ':' )
						{
							j--;
							removedOr = 1;
						}
						index = index + 3;
						/* write in infix form */
						while ( (in[index] != ',') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;

							if ( in[index] == '@' )
							{
								body[body_index++] = '-';
								index++;
							}
							else if ( in[index] == '`' )
							{
				
								body[body_index++] = ' ';
								body[body_index++] = 'm';
								body[body_index++] = 'o';
								body[body_index++] = 'd';
								body[body_index++] = ' ';
								index++;
							}
							else
							{
								body[body_index++] = in[index++]; 
							} 
						}
						index++;
						
						/* if negation, then do not change symbol */
						if ( neg == 1 )
						{
							if ( lt == 1 )
							{
								body[body_index++] = '<';
							}
							else
							{
								body[body_index++] = '<';
								body[body_index++] = '=';
							}
								
						}
						else
						{
							if ( lt == 1 )
							{
								body[body_index++] = '>';
								body[body_index++] = '=';
							}
							else
							{
								body[body_index++] = '>';
							}
								
						}
					
						tuple_paran_count = 0;	
						while ( (in[index] != ')') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							
							if ( in[index] == '@' )
							{
								body[body_index++] = '-';
								index++;
							}
							else if ( in[index] == '`' )
							{
				
								body[body_index++] = ' ';
								body[body_index++] = 'm';
								body[body_index++] = 'o';
								body[body_index++] = 'd';
								body[body_index++] = ' ';
								index++;
							}
							else
							{
								body[body_index++] = in[index++]; 
							}
						}
						index++;
						/* If | was not removed earlier, then skip the next | */
						if (removedOr == 0 )
						{
							if (in[index] == '|')
								index++;
						}
						body[body_index++] = ',';
						body[body_index] = '\0';
						
					}
				}
				else
				{
					out[j++] = in[index++];
				}
				
				break;
				
			case 'g':
				tuple_paran_count = 0;
				/* maybe greater than */
				if ( (index == 0) || !isPredicateSymbol(in[index-1]) )
				{
					if ( ( (in[index+1] != 't') && (in[index+1]  != 'e') ) || (in[index+2]  != '(') )
					{
						out[j++] = in[index++];
						break;
					}
					
					if ( in[index+1] == 't' )
						gt = 1;
					else
						gt = 0;
					
					if ( (index > 0) && (in[index - 1] == '-') )
					{
						/* remove the negation since it is a special symbol */
						neg = 1;
						j--;
					}
					
					if ( antecedent == 1 )
					{
						index = index + 3;
						/* write in infix form */
						while ( (in[index] != ',') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;

							out[j++] = in[index++]; 
						}
						index++;
						/* if negation, then change the symbol */
						if ( neg == 1 )
						{
							if ( gt == 1 )
							{
								out[j++] = '<';
								out[j++] = '=';
							}
							else
							{
								out[j++] = '<';
							}
						}
						else
						{
							if ( gt == 1 )
							{
								out[j++] = '>';
							}
							else
							{
								out[j++] = '>';
								out[j++] = '=';
							}
						}
						
						
						tuple_paran_count = 0;
						while ( (in[index] != ')') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							
							out[j++] = in[index++];
						}
						index++;
						
					}
					else /* if in consequent */
					{
						/* remove the connective if not an implication */
						if ( out[j-1] != ':' )
						{
							j--;
							removedOr=1;
						}
						index = index + 3;
						/* write in infix form */
						while ( (in[index] != ',') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;

							if ( in[index] == '@' )
							{
								body[body_index++] = '-';
								index++;
							}
							else if ( in[index] == '`' )
							{
				
								body[body_index++] = ' ';
								body[body_index++] = 'm';
								body[body_index++] = 'o';
								body[body_index++] = 'd';
								body[body_index++] = ' ';
								index++;
							}
							else
							{
								body[body_index++] = in[index++]; 
							} 
						}
						index++;
						
						/* if negation, then do not change symbol */
						if ( neg == 1 )
						{
							if ( gt == 1 )
							{
								body[body_index++] = '>';
							}
							else
							{
								body[body_index++] = '>';
								body[body_index++] = '=';
							}
								
						}
						else
						{
							if ( gt == 1 )
							{
								body[body_index++] = '<';
								body[body_index++] = '=';
							}
							else
							{
								body[body_index++] = '<';
							}
								
						}
						
						tuple_paran_count = 0;
						while ( (in[index] != ')') || (tuple_paran_count != 0) )
						{
							if ( in[index] == '(' )
								tuple_paran_count++;
							if ( in[index] == ')' )
								tuple_paran_count--;
							
							if ( in[index] == '@' )
							{
								body[body_index++] = '-';
								index++;
							}
							else if ( in[index] == '`' )
							{
				
								body[body_index++] = ' ';
								body[body_index++] = 'm';
								body[body_index++] = 'o';
								body[body_index++] = 'd';
								body[body_index++] = ' ';
								index++;
							}
							else
							{
								body[body_index++] = in[index++]; 
							}
						}
						index++;
						/* If | was not removed earlier, then skip the next | */
						if (removedOr == 0 )
						{
							if (in[index] == '|')
								index++;
						}
						body[body_index++] = ',';
						body[body_index] = '\0';
						
					}
				}
				else
				{
					out[j++] = in[index++];
				}
				
				break;
				
				
				
			default:
				out[j++] = in[index++];
				break;
				
				
		}
	}
	out[j] = '\0';
	
}






void
format_and_write(NODEP root)
{
int             i = 0;
int             k = 0;
int             ret = 0;
int             index = 0, body_index = 0;
int             bar_exist = 0;
int             paran_count = 0;

char            rule[LINE_MAX] = { (char) NULL };
char            body[LINE_MAX] = { (char) NULL };
char            head[LINE_MAX] = { (char) NULL };
char            writeBuf = (char) NULL;

#ifdef BAR_NEG_ATOM
struct baratom *newnode = NULL;
struct baratom *barnode = NULL;
#endif


reading_index = 0;
inorder_save(root, head);
#ifdef DEBUG
printf("rule %s\n",head);
#endif

convert_specialPred (head,rule,body);
#ifdef DEBUG
printf("after converting special predicates to infix %s\n",rule);
#endif
while (body[body_index] != '\0')
{
	body_index++;
}


/*
* write into the solver input file 
*/
for (i=0; rule[i] != ':'; i++);

for (i = i+1; rule[i] != '\0'; i++) {
	if ( (rule[i] == '-') && (paran_count == 0) ) {
		i++;
#ifdef BAR_NEG_ATOM
		newnode = (struct baratom *) malloc(sizeof(struct baratom));
		newnode->next = NULL;
#endif

		k = 0;
		/* write it as not 1{not } in the body */
		
//body[body_index++] = 'n';
//body[body_index++] = 'o';
//body[body_index++] = 't';
//body[body_index++] = ' ';
		body[body_index++] = '{';
		body[body_index++] = 'n';
		body[body_index++] = 'o';
		body[body_index++] = 't';
		body[body_index++] = ' ';
/*
		* read the entire atom 
*/
		while (isPredicateSymbol(rule[i])) {
			if ( (rule[i] == '~') || (rule[i] == '@') )
			{
#ifdef BAR_NEG_ATOM
				newnode->val[k++] = '-';
#endif	
				body[body_index++] = '-';
			}
			else if ( rule[i] == '`' )
			{
#ifdef BAR_NEG_ATOM
				newnode->val[k++] = ' ';
				newnode->val[k++] = 'm';
				newnode->val[k++] = 'o';
				newnode->val[k++] = 'd';
				newnode->val[k++] = ' ';
#endif		
				body[body_index++] = ' ';
				body[body_index++] = 'm';
				body[body_index++] = 'o';
				body[body_index++] = 'd';
				body[body_index++] = ' ';
			}
			else
			{
				
#ifdef BAR_NEG_ATOM
				newnode->val[k++] = rule[i];
#endif	
				body[body_index++] = rule[i];
			}
			
			i++;
		}

/*
		head[index++] = 'b';
		head[index++] = 'a';
		head[index++] = 'r';
*/
// printf("added bar\n");


		if (rule[i] == '(') {
			paran_count = 1;
			while (paran_count != 0) {
				if ( rule[i] == '@' )
				{
#ifdef BAR_NEG_ATOM
					newnode->val[k++] = '-';
#endif	
					body[body_index++] = '-';
				}
				else if ( rule[i] == '`' )
				{
#ifdef BAR_NEG_ATOM
					newnode->val[k++] = ' ';
					newnode->val[k++] = 'm';
					newnode->val[k++] = 'o';
					newnode->val[k++] = 'd';
					newnode->val[k++] = ' ';
#endif		
					body[body_index++] = ' ';
					body[body_index++] = 'm';
					body[body_index++] = 'o';
					body[body_index++] = 'd';
					body[body_index++] = ' ';
				}
				else
				{
#ifdef BAR_NEG_ATOM
					newnode->val[k++] = rule[i];
#endif	
					body[body_index++] = rule[i];
				}
				i++;
				if (rule[i] == '(')
					paran_count++;
				if (rule[i] == ')')
					paran_count--;
			}
#ifdef BAR_NEG_ATOM
			newnode->val[k++] = ')';
#endif	
			body[body_index++] = ')';

			/* To get rid of '|' */
			if (rule[i+1] == '|')
				i++;
		}
		else
		{
			/* to avoid overshooting */
			if ( rule[i] != '|' )
				i--;
		}
		body[body_index++] = '}';
		body[body_index++] = '0';
		body[body_index++] = ',';
		body[body_index] = '\0';
#ifdef BAR_NEG_ATOM
		newnode->val[k] = '\0';
#endif	
// printf("added tuple\n");
#ifdef BAR_NEG_ATOM
		bar_exist = 0;
		for (barnode = root_baratom; barnode != NULL;
				   barnode = barnode->next) {
					   if (!strcmp(barnode->val, newnode->val)) {
						   bar_exist = 1;
					   }
				   }

				   if (!bar_exist) {
					   if (prev_baratom != NULL) {
						   prev_baratom->next = newnode;
						   prev_baratom = newnode;
					   } else {
						   root_baratom = newnode;
						   prev_baratom = root_baratom;
					   }
				   }
#endif

	} 
	else if ( (rule[i] == '~') || (rule[i] == '@') ){
		head[index++] = '-';
		}
		else if ( rule[i] == '`' )
		{		
			head[index++] = ' ';
			head[index++] = 'm';
			head[index++] = 'o';
			head[index++] = 'd';
			head[index++] = ' ';
		}
		else {
			if ( rule[i] == '(' )
				paran_count++;
			if ( rule[i] == ')' )
				paran_count--;
		head[index++] = rule[i];
		}
}
/* to handle cases like p | -q */
if (head[index-1] == '|')
	head[index-1] = '\0';
else	
	head[index] = '\0';

index = body_index;

for (i = 0; rule[i] != ':'; i++) {
if ((rule[i] == '-') && (paran_count == 0)) {
body[index++] = 'n';
body[index++] = 'o';
body[index++] = 't';
body[index++] = ' ';
} else if (rule[i] == '&') {
body[index++] = ',';
} else if ( (rule[i] == '~') || (rule[i] == '@') ){
body[index++] = '-';
}
else if ( rule[i] == '`' )
{			
	body[index++] = ' ';
	body[index++] = 'm';
	body[index++] = 'o';
	body[index++] = 'd';
	body[index++] = ' ';
}
else {
	if ( rule[i] == '(' )
		paran_count++;
	if ( rule[i] == ')' )
		paran_count--;
body[index++] = rule[i];
}
}
body[index] = '\0';
body_index = index;

index = 0;


// printf("added head %s\n",head);

for (i = 0; head[i] != '\0'; i++) {
writeBuf = head[i];
ret = fwrite(&writeBuf, 1, 1, fpSolverInput);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}
}

writeBuf = ' ';
ret = fwrite(&writeBuf, 1, 1, fpSolverInput);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}

writeBuf = ':';
ret = fwrite(&writeBuf, 1, 1, fpSolverInput);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}

writeBuf = '-';
ret = fwrite(&writeBuf, 1, 1, fpSolverInput);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}

writeBuf = ' ';
ret = fwrite(&writeBuf, 1, 1, fpSolverInput);
if (ret == 0) {
printf("write to output file failed\n");
exit(1);
}

for (i = 0; body[i] != '\0'; i++) {
	writeBuf = body[i];
	ret = fwrite(&writeBuf, 1, 1, fpSolverInput);
	if (ret == 0) {
	printf("write to output file failed\n");
	exit(1);
	}
}

writeBuf = '.';
ret = fwrite(&writeBuf, 1, 1, fpSolverInput);
if (ret == 0) {
	printf("write to output file failed\n");
	exit(1);
}

writeBuf = '\n';
ret = fwrite(&writeBuf, 1, 1, fpSolverInput);
if (ret == 0) {
	printf("write to output file failed\n");
	exit(1);
}

}
