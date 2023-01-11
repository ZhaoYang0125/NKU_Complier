%code top{
    #include <iostream>
    #include <assert.h>
    #include <stack>
    #include "parser.h"
    extern Ast ast;
    int yylex();
    int yyerror( char const * );
    Type* declType;
    int paramCount=0;
    ArrayType* arrayType;
    std::stack<StmtNode*> WhileStmts;
}

%code requires {
    #include "Ast.h"
    #include "SymbolTable.h"
    #include "Type.h"
}

%union {
    int itype;
    char* strtype;
    StmtNode* stmttype;
    ExprNode* exprtype;
    Type* type;
    ListNode* listtype;
}

%start Program
%token <strtype> ID 
%token <itype> INTEGER
%token IF ELSE WHILE
%token BREAK CONTINUE
%token INT VOID
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA
%token ADD SUB MUL DIV MOD OR AND NOT LESS GREATER LESSEQUAL GREATEREQUAL EQUAL NOTEQUAL ASSIGN
%token RETURN
%token CONST
%token LBRACKET RBRACKET

%nterm <stmttype> Stmts Stmt AssignStmt BlockStmt IfStmt WhileStmt ReturnStmt DeclStmt VarDeclStmt VarDef ConstDeclStmt ConstDef FuncDef
%nterm <stmttype> ExprStmt BlankStmt VarDefList ConstDefList
%nterm <stmttype> BreakStmt ContinueStmt
%nterm <stmttype> FuncFParam FuncFParams
%nterm <exprtype> FuncRParams
%nterm <exprtype> Exp AddExp MulExp Cond LOrExp PrimaryExp UnaryExp LVal RelExp LAndExp EqExp
%nterm <exprtype> ArrayIndices FuncArrayIndices
%nterm <type> Type

%precedence THEN
%precedence ELSE
%%
Program
    : Stmts {
        ast.setRoot($1);
    }
    ;
Stmts
    : Stmt {$$=$1;}
    | Stmts Stmt{
        $$ = new SeqNode($1, $2);
    }
    ;
Stmt
    : AssignStmt {$$=$1;}
    | BlockStmt {$$=$1;}
    | IfStmt {$$=$1;}
    | WhileStmt {$$=$1;}
    | BreakStmt {$$=$1;}
    | ContinueStmt {$$=$1;}
    | ReturnStmt {$$=$1;}
    | DeclStmt {$$=$1;}
    | FuncDef {$$=$1;}
    | ExprStmt {$$=$1;}
    | BlankStmt {$$=$1;}
    ;
LVal
    : ID {
        SymbolEntry *se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            assert(se != nullptr);
        }
        $$ = new Id(se);
        delete []$1;
    }
    | ID ArrayIndices{
        SymbolEntry* se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            assert(se != nullptr);
        }
        $$ = new Id(se, $2);
        delete []$1;
    }
    ;
AssignStmt
    :
    LVal ASSIGN Exp SEMICOLON {
        $$ = new AssignStmt($1, $3);
    }
    ;
BlockStmt
    :   LBRACE 
        {identifiers = new SymbolTable(identifiers);} 
        Stmts RBRACE 
        {
            $$ = new CompoundStmt($3);
            SymbolTable *top = identifiers;
            identifiers = identifiers->getPrev();
            delete top;
        }
    |LBRACE RBRACE{
        $$=new CompoundStmt(nullptr);
    }
    ;
IfStmt
    : IF LPAREN Cond RPAREN Stmt %prec THEN {
        $$ = new IfStmt($3, $5);
    }
    | IF LPAREN Cond RPAREN Stmt ELSE Stmt {
        $$ = new IfElseStmt($3, $5, $7);
    }
    ;
ReturnStmt
    :
    RETURN Exp SEMICOLON{
        $$ = new ReturnStmt($2);
    }
    | 
    RETURN SEMICOLON{
        ExprNode* retValue=nullptr;
        $$=new ReturnStmt(retValue);
    }
    ;
WhileStmt
    :
    WHILE LPAREN Cond RPAREN{
        StmtNode* st=new WhileStmt($3,nullptr);
        WhileStmts.push(st);
    } Stmt{
        ((WhileStmt*)(WhileStmts.top()))->setStmt($6);
        $$ = WhileStmts.top();
        WhileStmts.pop();
    }
    ;
BreakStmt
    : 
    BREAK SEMICOLON{
        $$ = new BreakStmt(WhileStmts.top());
    }
    ;
ContinueStmt
    : 
    CONTINUE SEMICOLON{
        $$ = new ContinueStmt(WhileStmts.top());
    }
    ;
Exp
    :
    AddExp {$$ = $1;}
    ;
Cond
    :
    LOrExp {$$ = $1;}
    ;
PrimaryExp
    :
    LPAREN Exp RPAREN {
        $$ = $2;
    }
    |
    LVal {
        $$ = $1;
    }
    | INTEGER {
        SymbolEntry *se = new ConstantSymbolEntry(TypeSystem::intType, $1);
        $$ = new Constant(se);
    }
    ;
ExprStmt
    : Exp SEMICOLON {
        $$ = new ExprStmt($1);
    }
    ;
BlankStmt
    : SEMICOLON {
        $$ = new BlankStmt();
    }
    ;
UnaryExp
    :
    PrimaryExp {
        $$ = $1;
    }
    | 
    ID LPAREN FuncRParams RPAREN {//函数调用
        SymbolEntry* se;
        se = identifiers->lookup($1);
        if (se == nullptr){
            fprintf(stderr, "function \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            assert(se!=nullptr);
        }
        $$ = new CallExpr(se, $3);
    }
    |
    ID LPAREN RPAREN {
        SymbolEntry* se;
        se = identifiers->lookup($1);
        if (se == nullptr){
            fprintf(stderr, "function \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            assert(se!=nullptr);
        }
        $$ = new CallExpr(se);
    }
    |
    ADD UnaryExp {
        $$ = $2;
    }
    |
    SUB UnaryExp {
        //std::cout<<"sub"<<std::endl;
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new UnaryExpr(se, UnaryExpr::SUB, $2);
    }
    |
    NOT UnaryExp {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new UnaryExpr(se, UnaryExpr::NOT, $2);
    }
    ;
MulExp
    :
    UnaryExp {$$ = $1;}
    |
    MulExp MUL UnaryExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::MUL, $1, $3);
    }
    |
    MulExp DIV UnaryExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::DIV, $1, $3);
    }
    |
    MulExp MOD UnaryExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::MOD, $1, $3);
    }
AddExp
    :
    MulExp {$$ = $1;}
    |
    AddExp ADD MulExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::ADD, $1, $3);
    }
    |
    AddExp SUB MulExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::SUB, $1, $3);
    }
    ;
RelExp
    :
    AddExp {$$ = $1;}
    |
    RelExp LESS AddExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::LESS, $1, $3);
    }
    |
    RelExp GREATER AddExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::GREATER, $1, $3);
    }
    |
    RelExp GREATEREQUAL AddExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::GREATEREQUAL, $1, $3);
    }
    |
    RelExp LESSEQUAL AddExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::LESSEQUAL, $1, $3);
    }
    ;
LAndExp
    :
    EqExp {$$ = $1;}
    |
    LAndExp AND EqExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::AND, $1, $3);
    }
    ;
EqExp
    : 
    RelExp {$$ = $1;}
    | 
    EqExp EQUAL RelExp {
        SymbolEntry* se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::EQUAL, $1, $3);
    }
    | 
    EqExp NOTEQUAL RelExp {
        SymbolEntry* se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::NOTEQUAL, $1, $3);
    }
    ;
LOrExp
    :
    LAndExp {$$ = $1;}
    |
    LOrExp OR LAndExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::OR, $1, $3);
    }
    ;
Type
    : INT {
        $$ = TypeSystem::intType;
        declType = TypeSystem::intType;
    }
    | VOID {
        $$ = TypeSystem::voidType;
        declType = TypeSystem::voidType;
    }
    ;
FuncRParams 
    : Exp { $$ = $1; }
    | FuncRParams COMMA Exp {
        $1->setNext($3);
        $$ = $1;
    }
    ;
ArrayIndices
    : LBRACKET Exp RBRACKET {
        $$ = $2;
    }
    | ArrayIndices LBRACKET Exp RBRACKET {
        $$ = $1;
        $1->setNext($3);    // 多维
    }
    ;
DeclStmt
    : VarDeclStmt { $$ = $1; }
    | ConstDeclStmt { $$ = $1; }
    ;
VarDef
    :
    ID{
        std::vector<Id*> idlist;
        std::vector<AssignStmt*> assignlist;
        IdList *tem = new IdList(idlist, assignlist);//标识符列表
        SymbolEntry* se;
        se = new IdentifierSymbolEntry(declType, $1, identifiers->getLevel());
        bool a=false;
        if(!identifiers->lookup_cur_block($1)){
            identifiers->install($1, se);
            a=false;    
        }
        else{
            a=true;
            fprintf(stderr,"identifier %s is redefined\n",(char*)$1);
            delete [](char*)$1;
            assert( !a);
        }
        tem->idlist.push_back(new Id(se));
        $$=(StmtNode*)tem;
        delete []$1;
    }
    |
    ID ASSIGN Exp {
        std::vector<Id*> idlist;
        std::vector<AssignStmt*> assignlist;
        IdList *tem = new IdList(idlist, assignlist);//标识符列表
        SymbolEntry *se;
        se = new IdentifierSymbolEntry(declType, $1, identifiers->getLevel());
        ((IdentifierSymbolEntry*)se)->setValue($3->getValue());
        bool a=false;
        if(!identifiers->lookup_cur_block($1)){
            identifiers->install($1, se);
            a=false;    
        }
        else{
            a=true;
            fprintf(stderr,"identifier %s is redefined\n",(char*)$1);
            delete [](char*)$1;
            assert( !a);
        }
        //$$ = new DeclStmt(new Id(se));
        tem->idlist.push_back(new Id(se));
        tem->assignlist.push_back(new AssignStmt(new Id(se),$3));
        $$=(StmtNode*)tem;
        //$$ = new Id(se);
        delete []$1;
    }
    |
    ID ArrayIndices {
        std::vector<int> vec;   //分别存放维度值
        ExprNode* temp = $2;
        // 保存数组维度，从高维到低维
        while(temp){
            vec.push_back(temp->getValue());
            temp = (ExprNode*)(temp->getNext());
        }

        Type *type = declType;
        Type* temp1;
        while(!vec.empty()){
        //嵌套数组类型
            temp1 = new ArrayType(type, vec.back());
            //考虑多维数组 每个元素是数组指针
            //如果元素是数组 type设置为数组维度
            if(type->isArray())
                ((ArrayType*)type)->setArrayType(temp1);
            type = temp1;
            vec.pop_back();
        }
        // 保存最低维数据类型
        arrayType = (ArrayType*)type;
        SymbolEntry* se;
        se = new IdentifierSymbolEntry(type, $1, identifiers->getLevel());
 
        ((IdentifierSymbolEntry*)se)->setAllZero(); //  初始化为0
        int *p = new int[type->getSize()];  // 开辟数组空间
        ((IdentifierSymbolEntry*)se)->setArrayValue(p);

        std::vector<Id*> idlist;
        std::vector<AssignStmt*> assignlist;
        IdList *tem = new IdList(idlist, assignlist); // 标识符列表
        if(!identifiers->lookup($1)){
            identifiers->install($1, se);
        }
        else{
            fprintf(stderr,"identifier %s is redefined\n",(char*)$1);
            delete [](char*)$1;
        }
        tem->idlist.push_back(new Id(se));
        /* 插入一条nullptr，让idlist和assignlist对齐，暂时行得通 */
        tem->assignlist.push_back(nullptr);
 
        $$=(StmtNode*)tem;
        delete []$1;
    }
    ;
VarDefList
    :
    VarDefList COMMA VarDef {
        IdList *ids=(IdList*)$1;
        IdList *id=(IdList*)$3;
        ids->idlist.insert(ids->idlist.end(),id->idlist.begin(),id->idlist.end());
        ids->assignlist.insert(ids->assignlist.end(),id->assignlist.begin(),id->assignlist.end());
        $1->setNext($3);
        //std::cout<<"idlist"<<std::endl;
        $$ = (StmtNode*)ids;
    } 
    |
    VarDef { $$ = $1;}
    ;
VarDeclStmt
    :
    Type VarDefList SEMICOLON {
        $$ = new DeclStmt((IdList*)$2);
        //std::cout<<"declstmt"<<std::endl;
    }
    ;
ConstDef
    :
     ID ASSIGN Exp {
         std::vector<Id*> idlist;
         std::vector<AssignStmt*> assignlist;
        IdList *tem = new IdList(idlist, assignlist);//标识符列表
        IdentifierSymbolEntry *se;
        se = new IdentifierSymbolEntry(declType, $1, identifiers->getLevel());
        se->setConst();
        ((IdentifierSymbolEntry*)se)->setValue($3->getValue());
        identifiers->install($1, se);
        tem->idlist.push_back(new Id(se));
        tem->assignlist.push_back(new AssignStmt(new Id(se),$3));
        $$=(StmtNode*)tem;
        //$$ = new DeclStmt(new Id(se));
     }
     ;
ConstDefList
    :
    ConstDef { $$ = $1;}
    |
    ConstDefList COMMA ConstDef {
        IdList *ids=(IdList*)$1;
        IdList *id=(IdList*)$3;
        ids->idlist.insert(ids->idlist.end(),id->idlist.begin(),id->idlist.end());
        ids->assignlist.insert(ids->assignlist.end(),id->assignlist.begin(),id->assignlist.end());
        $1->setNext($3);
        //std::cout<<"idlist"<<std::endl;
        $$ = (StmtNode*)ids;
    } 
    ;
ConstDeclStmt
    :
    CONST Type ConstDefList SEMICOLON{
        declType = $2;
        //$$ = $3;
        $$ = new DeclStmt((IdList*)$3);
    }
    ;
FuncFParams
    : 
    FuncFParams COMMA FuncFParam {
        IdList *ids=(IdList*)$1;
        IdList *id=(IdList*)$3;
        ids->idlist.insert(ids->idlist.end(),id->idlist.begin(),id->idlist.end());
        ids->assignlist.insert(ids->assignlist.end(),id->assignlist.begin(),id->assignlist.end());
        $1->setNext($3);
        //std::cout<<"idlist"<<std::endl;
        $$ = (StmtNode*)ids;
    }
    | 
    FuncFParam {
        $$ = $1;
    }
    ;
FuncFParam
    : 
    Type ID {
        std::vector<Id*> idlist;
        std::vector<AssignStmt*> assignlist;
        IdList *tem = new IdList(idlist, assignlist);//标识符列表
        SymbolEntry* se;
        se = new IdentifierSymbolEntry($1, $2, identifiers->getLevel(),paramCount++);
        identifiers->install($2, se);
        tem->idlist.push_back(new Id(se));
        $$=(StmtNode*)tem;
        delete []$2;
    }
    | Type ID FuncArrayIndices {
        std::stack<ExprNode*> vec;   //分别存放维度值
        ExprNode* temp = $3;
        // 保存数组维度，从高维到低维
        while(temp){
            vec.push(temp);
            temp = (ExprNode*)(temp->getNext());
        }

        Type *type = TypeSystem::intType;
        Type* temp1;
        while(!vec.empty()){
        //嵌套数组类型
            temp1 = new ArrayType(type, vec.top()->getValue());
            //考虑多维数组 每个元素是数组指针
            //如果元素是数组 type设置为数组维度
            if(type->isArray())
                ((ArrayType*)type)->setArrayType(temp1);
            type = temp1;
            vec.pop();
        }
        SymbolEntry* se;
        se = new IdentifierSymbolEntry(type, $2, identifiers->getLevel(),paramCount++);
        //((IdentifierSymbolEntry*)se)->setAddr(new Operand(se));
        std::vector<Id*> idlist;
        std::vector<AssignStmt*> assignlist;
        IdList *tem = new IdList(idlist, assignlist); // 标识符列表

        identifiers->install($2, se);

        tem->idlist.push_back(new Id(se));
        /* 插入一条nullptr，让idlist和assignlist对齐，暂时行得通 */
        tem->assignlist.push_back(nullptr);
 
        $$=(StmtNode*)tem;
        delete []$2;
    }
    ;
 // 输入参数为数组 arr[][1]
FuncArrayIndices 
    : LBRACKET RBRACKET {
        $$ = new ExprNode(nullptr);
    }
    | FuncArrayIndices LBRACKET Exp RBRACKET {
        $$ = $1;
        $$->setNext($3);
    }
    ;
FuncDef
    :
    Type ID LPAREN {
        Type *funcType;
        funcType = new FunctionType($1,{});
        SymbolEntry *se = new IdentifierSymbolEntry(funcType, $2, identifiers->getLevel());
        bool a=false;
        if(!identifiers->lookup($2)){
            identifiers->install($2, se);
            a=false;    
        }
        else{
            a=true;
            fprintf(stderr,"function %s is redefined\n",(char*)$2);
            delete [](char*)$2;
            assert(!a);
        }
        identifiers = new SymbolTable(identifiers);
    }
    RPAREN
    BlockStmt
    {
        SymbolEntry *se;
        se = identifiers->lookup($2);
        assert(se != nullptr);
        $$ = new FunctionDef(se, $6);
        SymbolTable *top = identifiers;
        identifiers = identifiers->getPrev();
        delete top;
        delete []$2;
    }
    |
    Type ID LPAREN 
    {
        Type *funcType = new FunctionType($1,{});
        SymbolEntry *se = new IdentifierSymbolEntry(funcType, $2, identifiers->getLevel());
        //std::cout<<se->toStr()<<std::endl;
        bool a=false;
        if(!identifiers->lookup($2)){
            identifiers->install($2, se);
            a=false;    
        }
        else{
            a=true;
            fprintf(stderr,"function %s is redefined\n",(char*)$2);
            delete [](char*)$2;
            assert(!a);
        }
        paramCount=0;
        identifiers = new SymbolTable(identifiers);
    }
    FuncFParams {
        IdList* params=(IdList*)$5;
        std::vector<Type*> paramsType;
        for(unsigned int i=0;i<params->idlist.size();i++){
            Type* t=params->idlist[i]->getType();
            paramsType.push_back(t);
        }
        SymbolEntry *se = identifiers->lookup($2);
        assert(se != nullptr);
        ((FunctionType*)(se->getType()))->paramsType=paramsType;
    }
    RPAREN
    BlockStmt   
    {
        SymbolEntry *se;
        se = identifiers->lookup($2);
        assert(se != nullptr);
        $$ = new FunctionDef(se, (IdList*)$5 ,$8);
        //std::cout<<"stmt"<<std::endl;
        SymbolTable *top = identifiers;
        identifiers = identifiers->getPrev();
        delete top;
        delete []$2;
    }
    ;
%%

int yyerror(char const* message)
{
    std::cerr<<message<<std::endl;
    return -1;
}
