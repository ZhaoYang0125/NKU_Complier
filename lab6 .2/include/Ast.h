#ifndef __AST_H__
#define __AST_H__

#include <fstream>
#include "Operand.h"
#include "Type.h"

class SymbolEntry;
class Unit;
class Function;
class BasicBlock;
class Instruction;
class IRBuilder;

class Node
{
private:
    static int counter;
    int seq;
    Node* next;
protected:
    std::vector<Instruction*> true_list;
    std::vector<Instruction*> false_list;
    static IRBuilder *builder;
    void backPatch(std::vector<Instruction*> &list, BasicBlock*bb);
    void backPatchFalse(std::vector<Instruction *> &list, BasicBlock *bb);//什么功能？
    std::vector<Instruction *> merge(std::vector<Instruction *> &list1, std::vector<Instruction *> &list2);

public:
    Node();
    int getSeq() const {return seq;};
    Node* getNext(){return next;};
    void setNext(Node* next);
    static void setIRBuilder(IRBuilder*ib) {builder = ib;};
    virtual void output(int level) = 0;
    virtual void typeCheck() = 0;
    virtual void genCode() = 0;
    std::vector<Instruction*>& trueList() {return true_list;}
    std::vector<Instruction*>& falseList() {return false_list;}
};

class ExprNode : public Node
{
protected:
    SymbolEntry *symbolEntry;
    
public:
    Operand *dst;   // The result of the subtree is stored into dst.
    ExprNode(SymbolEntry *symbolEntry) : symbolEntry(symbolEntry){dst=new Operand(symbolEntry);};
    Operand* getOperand() {return dst;};
    SymbolEntry* getSymPtr() {return symbolEntry;};
    // 
    void int2Bool(){
        symbolEntry = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        dst = new Operand(symbolEntry);
    }
};

class UnaryExpr : public ExprNode
{
private:
    int op;
    ExprNode* expr;
public:
    enum {ADD, SUB, NOT};
    UnaryExpr(SymbolEntry *se, int op, ExprNode* expr) : ExprNode(se), op(op), expr(expr){};

    void output(int level);
    void typeCheck();
    void genCode();
};

class CallExpr : public ExprNode 
{
private:
    ExprNode* param;
public:
    CallExpr(SymbolEntry* se, ExprNode* param=NULL);
    
    void output(int level);
    void typeCheck();
    void genCode();
};

class BinaryExpr : public ExprNode
{
private:
    int op;
    ExprNode *expr1, *expr2;
public:
    enum {ADD, SUB, MUL, DIV, MOD, AND, OR, LESS, GREATER, LESSEQUAL, GREATEREQUAL, EQUAL, NOTEQUAL};
    BinaryExpr(SymbolEntry *se, int op, ExprNode*expr1, ExprNode*expr2) : ExprNode(se), op(op), expr1(expr1), expr2(expr2){}; // new dst

    void output(int level);
    void typeCheck();
    void genCode();
};

class Constant : public ExprNode
{
public:
    Constant(SymbolEntry *se) : ExprNode(se){dst = new Operand(se);};

    void output(int level);
    void typeCheck();
    void genCode();
};

class Id : public ExprNode
{
public:
    Id(SymbolEntry *se) : ExprNode(se){SymbolEntry *temp = new TemporarySymbolEntry(se->getType(), SymbolTable::getLabel()); dst = new Operand(temp);};

    void output(int level);
    void typeCheck();
    void genCode();
};

class StmtNode : public Node
{
public:
    Type *type = nullptr;
};

class ExprStmt : public StmtNode {
private:
    ExprNode* expr;

public:
    ExprStmt(ExprNode* expr) : expr(expr){};

    void output(int level);
    void typeCheck();
    void genCode();
};

class BlankStmt : public StmtNode {
public:
    BlankStmt(){};

    void output(int level);
    void typeCheck();
    void genCode();
};

class CompoundStmt : public StmtNode
{
private:
    StmtNode *stmt;
public:
    CompoundStmt(StmtNode *stmt) : stmt(stmt) {};

    void output(int level);
    void typeCheck();
    void genCode();
};

class SeqNode : public StmtNode
{
private:
    StmtNode *stmt1, *stmt2;
public:
    SeqNode(StmtNode *stmt1, StmtNode *stmt2) : stmt1(stmt1), stmt2(stmt2){};

    void output(int level);
    void typeCheck();
    void genCode();
};

class AssignStmt : public StmtNode
{
private:
public:
    ExprNode *lval;
    ExprNode *expr;
    AssignStmt(ExprNode *lval, ExprNode *expr) : lval(lval), expr(expr) {};

    void output(int level);
    void typeCheck();
    void genCode();
};

class ListNode : public Node//序列型变量类
{};

//标识符列表
class IdList:public ListNode
{
//private:
public:
    std::vector<Id*> idlist;//标识符串
    std::vector<AssignStmt*> assignlist;//声明时直接赋值
    IdList(std::vector<Id*>idlist,std::vector<AssignStmt*>ass):idlist(idlist),assignlist(ass){};
    void output(int level);
    void typeCheck();
    void genCode();
};

class DeclStmt : public StmtNode
{
private:
    IdList *ids;
public:
    DeclStmt(IdList *ids) : ids(ids){};
    void output(int level);
    void typeCheck();
    void genCode();
};

class IfStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;
public:
    IfStmt(ExprNode *cond, StmtNode *thenStmt) : cond(cond), thenStmt(thenStmt){};

    void output(int level);
    void typeCheck();
    void genCode();
};

class IfElseStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;
    StmtNode *elseStmt;
public:
    IfElseStmt(ExprNode *cond, StmtNode *thenStmt, StmtNode *elseStmt) : cond(cond), thenStmt(thenStmt), elseStmt(elseStmt) {};

    void output(int level);
    void typeCheck();
    void genCode();
};

class WhileStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *Stmt;
public:
    WhileStmt(ExprNode *cond, StmtNode *Stmt) : cond(cond), Stmt(Stmt) {};

    void output(int level);
    void typeCheck();
    void genCode();
};


class ReturnStmt : public StmtNode
{
private:
    ExprNode *retValue;
public:
    ReturnStmt(ExprNode*retValue) : retValue(retValue) {};

    void output(int level);
    void typeCheck();
    void genCode();
};

class BreakStmt : public StmtNode
{
public:
    BreakStmt() {};
    void output(int level);
    void typeCheck();
    void genCode();
};

class ContinueStmt : public StmtNode
{
public:
    ContinueStmt() {};
    void output(int level);
    void typeCheck();
    void genCode();
};

class FunctionDef : public StmtNode
{
private:
    SymbolEntry *se;
    StmtNode *stmt;
public:
    FunctionDef(SymbolEntry *se, StmtNode *stmt) : se(se), stmt(stmt){};

    void output(int level);
    void typeCheck();
    void genCode();

    //
    SymbolEntry *getSymbolEntry() { return this->se; };
};

class Ast
{
private:
    Node* root;
public:
    Ast() {root = nullptr;}
    void setRoot(Node*n) {root = n;}

    void output();
    void typeCheck();
    void genCode(Unit *unit);
};


#endif
