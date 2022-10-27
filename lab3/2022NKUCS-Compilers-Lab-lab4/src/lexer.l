%option noyywrap
%{
    /*
    * You will need to comment this line in lab5.
    */
    #define ONLY_FOR_LEX
    
    #ifdef ONLY_FOR_LEX
    #else
    #define YYSTYPE void *//确定变量类型为空指针
    #include "parser.h"
    #endif

    #define YY_NO_UNPUT
    #define YY_NO_INPUT
    #include <string>

    #ifdef ONLY_FOR_LEX
    #include <ostream>
    #include <fstream>
    #include "symTable.h"
    using namespace std;
    extern FILE *yyin; 
    extern FILE *yyout;
    int offset=0;
    int level=0;

    symTable* symtable=new symTable();
    char pointer_to_scope[13]="";

    void DEBUG_FOR_LAB4(std::string s){
        std::string DEBUG_INFO = "[DEBUG LAB4]: \t" + s+"\t" +to_string(yylineno) +"\t"+to_string(offset)+"\t"+pointer_to_scope+"\n";
        fputs(DEBUG_INFO.c_str(), yyout);
    }
    #endif
%}

DECIMIAL ([1-9][0-9]*|0)
OCTAL [0]([1-7][0-7]*|0)
HEXADECIMIAL  ((0x)[[:xdigit:]]*)
ID [[:alpha:]_][[:alpha:][:digit:]_]*
EOL (\r\n|\n|\r)
WHITE [\t ]
LINECOMMENT (\/\/[^\n]*)
CHARACTER .

commentbegin "/*"
commentelement .|\n
commentend "*/"
%x BLOCKCOMMENT

%%

{commentbegin} {BEGIN BLOCKCOMMENT;}
<BLOCKCOMMENT>{commentelement} {
    if(*yytext=='\n'){
        yylineno++;
        offset++;
    }
    else{
        offset=0;
    }
}
<BLOCKCOMMENT>{commentend} {BEGIN INITIAL;}

{LINECOMMENT}
{DECIMIAL} {
    #ifdef ONLY_FOR_LEX
        char* d = new char[strlen(yytext)+1];    
        strcpy(d,(const char*)yytext);
        string d1=string(d);
        DEBUG_FOR_LAB4("DECIMIAL\t" + d1);
        offset+=strlen(yytext);
        delete d;
    #else
        return d;
    #endif
}
{OCTAL} {
        #ifdef ONLY_FOR_LEX
            int len=strlen(yytext);
            offset+=len;
            int n=strtol(yytext,NULL,8);
            string o=to_string(n);
            DEBUG_FOR_LAB4("OCTAL \t" + o);
        #else
            return OCTAL;
        #endif
}
{HEXADECIMIAL} {
        #ifdef ONLY_FOR_LEX
            int n=strtol(yytext,NULL,16);
            string h=to_string(n);
            DEBUG_FOR_LAB4("HEXADECIMIAL\t" + h);
        #else
            return HEXADECIMIAL;
        #endif
}
"int" {
    /*
    * Questions: 
    *   Q1: Why we need to return INT in further labs?
    *   Q2: What is "INT" actually?
    */
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("INT   \tint");
        offset+=3;
    #else
        return INT;
    #endif
}
"void" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("VOID  \tvoid");
        offset+=4;
    #else
        return VOID;
    #endif 
}
"if" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("IF    \tif");
        offset+=2;
    #else
        return IF;
    #endif
};
"else" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("ELSE  \telse");
        offset+=4;
    #else
        return ELSE;
    #endif
};
"return" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RETURN\treturn");
        offset+=6;
    #else
        return RETURN;
    #endif
}
"main" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("MAIN  \tmain");
        offset+=4;
    #else
        return main;
    #endif
}
"while" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("WHILE \twhile");
        offset+=5;
    #else
        return WHILE;
    #endif
}
"float" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("FLOAT \tfloat");
        offset+=5;
    #else
        return FLOAT;
    #endif
}
"double" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("DOUBLE\tdouble");
        offset+=6;
    #else
        return DOUBLE;
    #endif
}
"break" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("BREAK \tbreak");
        offset+=5;
    #else
        return BREAK;
    #endif
}
"continue" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("CONTINUE\tcontinue");
        offset+=6;
    #else
        return CONTINUE;
    #endif
}
{ID} {
    #ifdef ONLY_FOR_LEX
        char* id = new char[strlen(yytext)+1];
        strcpy(id,yytext);
        string id1=string(id);
        if(symtable->lookUp(id1)==nullptr){
            symEntry* entry=new symEntry(0, yylineno, offset);
            if(symtable->installID(id1, entry)){
                printf("true\t");
                printf("%s\n",id);
                sprintf(pointer_to_scope,"%p",entry);//                
            }
            else{
                printf("false\n");
                printf("%s\n",id);
            }
        }
        else{
            printf("exist\n");
            sprintf(pointer_to_scope,"%p",symtable->lookUp(id1));//
        }
        DEBUG_FOR_LAB4("ID    \t"+id1);
        strcpy(pointer_to_scope,"");
        offset+=strlen(yytext);
    #else
        return ID;
    #endif
}

"=" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("ASSIGN\t=");
        offset++;
    #else
        return ASSIGN;
    #endif
}
"<" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LESS  \t<");
        offset++;
    #else
        return LT;
    #endif
}
">" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("GREATER\t>");
        offset++;
    #else
        return GT;
    #endif
}
"<=" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LEQUAL\t<=");
        offset+=2;
    #else
        return LE;
    #endif
}
">=" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("GEQUAL\t<=");
        offset+=2;
    #else
        return GE;
    #endif
}
"==" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("EQUAL \t<=");
        offset+=2;
    #else
        return EQ;
    #endif
}
"!=" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("NEQUAL\t<=");
        offset+=2;
    #else
        return NE;
    #endif
}
"+" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("ADD   \t+");
        offset++;
    #else
        return ADD;
    #endif
}
"-" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("SUB   \t-");
        offset++;
    #else
        return SUB;
    #endif
}
"*" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("MULT  \t*");
        offset++;
    #else
        return MULT;
    #endif
}
"/" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("DEV   \t/");
        offset++;
    #else
        return DEV;
    #endif
}
"%" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("MOD   \t%");
        offset++;
    #else
        return MOD;
    #endif
}
"&&" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("AND   \t&&");
        offset+=2;
    #else
        return AND;
    #endif
}
"||" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("OR    \t||");
        offset+=2;
    #else
        return OR;
    #endif
}
"!" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("NOT   \t!");
        offset+=1;
    #else
        return NOT;
    #endif
}
";" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("SEMICOLON\t;");
        offset++;
    #else
        return SEMICOLON;
    #endif
}
"(" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LPAREN\t(");
        offset++;
    #else
        return LPAREN;
    #endif
}
")" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RPAREN\t)");
        offset++;
    #else
    return RPAREN;
    #endif
}
"[" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LBRACK\t[");
        offset++;
    #else
        return LBRACK;
    #endif
}
"]" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RBRACK\t]");
        offset++;
    #else
        return RBRACK;
    #endif
}
"{" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("LBRACE\t{");
        offset++;
        symtable = new symTable(symtable);
        level++;
    #else
        return LBRACE;
    #endif
}
"}" {
    #ifdef ONLY_FOR_LEX
        DEBUG_FOR_LAB4("RBRACE\t}");
        offset++;
        symtable = symtable->getPrev();
        level--;
    #else
        return RBRACE;
    #endif
}

{EOL} yylineno++;offset=0;
{WHITE} offset++;
%%

#ifdef ONLY_FOR_LEX
int main(int argc, char **argv){
    //printf("          token\tlexeme\tlineno\toffset\tpointer");
    if(argc != 5){
        fprintf(stderr, "Argument Not Enough");
        exit(EXIT_FAILURE);
    }

    if(!(yyin = fopen(argv[1], "r"))){
        fprintf(stderr, "No such file or directory: %s", argv[1]);
        exit(EXIT_FAILURE);
    }

    if(!(yyout = fopen(argv[3], "w"))){
        fprintf(stderr, "No such file or directory: %s", argv[3]);
        exit(EXIT_FAILURE);
    }
    yylex();
    return 0;
}
#endif
