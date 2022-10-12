%{
/****************************************************************************
expr.y
YACC file
Date: 2022/10/11
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#ifndef YYSTYPE
#define YYSTYPE double
#endif
int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);
%}

%token ADD SUB MUL DIV LKH RKH
%token NUMBER
%left ADD SUB
%left MUL DIV
%right UMINUS

%%


lines	:	lines expr ';'	{ printf("%f\n", $2); }
		|	lines ';'
		|
		;

expr	:	expr ADD expr	{ $$ = $1 + $3; }
		|	expr SUB expr	{ $$ = $1 - $3; }
		|	expr MUL expr	{ $$ = $1 * $3; }
		|	expr DIV expr	{ $$ = $1 / $3; }
		|	LKH expr RKH	{ $$ = $2; }
		|	SUB expr %prec UMINUS	{ $$ = -$2; }
		|	NUMBER
		;

		
%%

//programs section

int yylex()
{	
	//place your token retrieving code here
	char c;
	c=getchar();
	while(c==' '||c=='\t'||c=='\n')
		c=getchar();
		
	if(c>='0'&&c<='9')
	{ 
		yylval=c-'0';
		c=getchar();
		while(c==' '||c=='\t'||c=='\n')
			c=getchar();
		while(c>='0'&&c<='9')
		{
			yylval = yylval*10+c-'0';
			c=getchar();
			while(c==' '||c=='\t'||c=='\n')
				c=getchar();
		}
		ungetc(c,stdin);
		return NUMBER;
	}
	else
	{
		switch(c)
		{ 
			case '+':
				return ADD;
			case '-':
				return SUB;
			case '*':
				return MUL;
			case '/':
				return DIV;
			case '(':
				return LKH;
			case ')':
				return RKH;
		}
	}
	return c;
}

int main(void)
{
	yyin = stdin;
	do{
		yyparse();
	}while(!feof(yyin));
	return 0;
}
void yyerror(const char* s){
	fprintf(stderr,"Parse error: %s\n",s);
	exit(1);
}
