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
    void backPatch(std::vector<Instruction*> &list, BasicBlock*bb);//正确分支回填
    void backPatchFalse(std::vector<Instruction *> &list, BasicBlock *bb);//错误分支回填
    std::vector<Instruction *> merge(std::vector<Instruction *> &list1, std::vector<Instruction *> &list2);

public:
    Node();
    int getSeq() const {return seq;};
    Node* getNext(){return next;};
    void setNext(Node* next);
    static void setIRBuilder(IRBuilder*ib) {builder = ib;};
    virtual void output(int level) = 0;
    virtual void typeCheck(Type* retType=nullptr) = 0;
    virtual void genCode() = 0;
    std::vector<Instruction*>& trueList() {return true_list;}
    std::vector<Instruction*>& falseList() {return false_list;}
};

class ExprNode : public Node
{
protected:
    SymbolEntry *symbolEntry;
    Type* type;
public:
    Operand *dst;   // The result of the subtree is stored into dst.
    ExprNode(SymbolEntry *symbolEntry) : symbolEntry(symbolEntry){dst=new Operand(symbolEntry);};
    Operand* getOperand() {return dst;};
    SymbolEntry* getSymPtr() {return symbolEntry;};
    Type* getType() { return type; };
    virtual int getValue() { return -1; };
    // int类型转化为bool型 /*retodo*/
    void int2Bool(){
        symbolEntry = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        dst = new Operand(symbolEntry);
    }
    void genCode() { return; };
    void output(int level){ return; };
    virtual void typeCheck(Type* retType = nullptr) { return ; };

};

class UnaryExpr : public ExprNode
{
private:
    int op;
    ExprNode* expr;
public:
    enum {ADD, SUB, NOT};
    UnaryExpr(SymbolEntry *se, int op, ExprNode* expr) : ExprNode(se), op(op), expr(expr){};
    int getValue();
    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class CallExpr : public ExprNode 
{
private:
    ExprNode* param;
public:
    CallExpr(SymbolEntry* se, ExprNode* param=NULL);
    int getValue();
    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class Int2BoolExpr:public ExprNode
{
private:
    ExprNode* expr;
public:
    Int2BoolExpr(ExprNode* expr);
    int getValue(){return expr->getValue();};
    void output(int level){};
    void typeCheck(Type* retType=nullptr){};
    void genCode();
};

class BinaryExpr : public ExprNode
{
private:
    int op;
    ExprNode *expr1, *expr2;
public:
    enum {ADD, SUB, MUL, DIV, MOD, AND, OR, LESS, GREATER, LESSEQUAL, GREATEREQUAL, EQUAL, NOTEQUAL};
    BinaryExpr(SymbolEntry *se, int op, ExprNode*expr1, ExprNode*expr2) : ExprNode(se), op(op), expr1(expr1), expr2(expr2){
        if (op >= BinaryExpr::AND && op <= BinaryExpr::NOTEQUAL) 
        {//判断是否为整数
            this->type = TypeSystem::boolType;
            if (op == BinaryExpr::AND || op == BinaryExpr::OR) {
                if (expr1->getType()->isInt() && expr1->getType()->getSize() == 32){
                    Int2BoolExpr* temp = new Int2BoolExpr(expr1);
                    this->expr1 = temp;
                }
                if (expr2->getType()->isInt() && expr2->getType()->getSize() == 32){
                    Int2BoolExpr* temp = new Int2BoolExpr(expr2);
                    this->expr2 = temp;
                }
            }
        } 
        else{
            type = TypeSystem::intType;
        }
    }; // new dst
    int getValue();
    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class Constant : public ExprNode
{
public:
    Constant(SymbolEntry *se) : ExprNode(se){dst = new Operand(se);};
    int getValue();
    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class Id : public ExprNode
{
private:
    ExprNode* arrIdx;
    bool left = false;
public:
    Id(SymbolEntry *se, ExprNode* arrIdx = nullptr) : ExprNode(se), arrIdx(arrIdx)
    {
        type = se->getType();
        if(se->getType()->isArray())
        {
            SymbolEntry* temp = new TemporarySymbolEntry(
                new PointerType(((ArrayType*)se->getType())->getElementType()), SymbolTable::getLabel());
            dst = new Operand(temp);
        }
        else
        {
            SymbolEntry *temp = new TemporarySymbolEntry(se->getType(), SymbolTable::getLabel());
            dst = new Operand(temp);
        }
    };
    int getValue();
    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
    // 数组相关
    ExprNode* getArrIdx() { return arrIdx; };
    bool isLeft() const { return left; };
    void setLeft() { left = true; }
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
    void typeCheck(Type* retType=nullptr);
    //void typeCheck();
    void genCode();
};

class BlankStmt : public StmtNode {
public:
    BlankStmt(){};

    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class CompoundStmt : public StmtNode
{
private:
    StmtNode *stmt;
public:
    CompoundStmt(StmtNode *stmt) : stmt(stmt) {};

    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class SeqNode : public StmtNode
{
private:
    StmtNode *stmt1, *stmt2;
public:
    SeqNode(StmtNode *stmt1, StmtNode *stmt2) : stmt1(stmt1), stmt2(stmt2){};

    void output(int level);
    void typeCheck(Type* retType=nullptr);
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
    void typeCheck(Type* retType=nullptr);
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
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class DeclStmt : public StmtNode
{
private:
    IdList *ids;
public:
    DeclStmt(IdList *ids) : ids(ids){};
    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class IfStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;
public:
    IfStmt(ExprNode *cond, StmtNode *thenStmt);

    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class IfElseStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;
    StmtNode *elseStmt;
public:
    IfElseStmt(ExprNode *cond, StmtNode *thenStmt, StmtNode *elseStmt) : cond(cond), thenStmt(thenStmt), elseStmt(elseStmt) {
        Type* t = cond->getSymPtr()->getType();
        if (t->isInt() && ((IntType*) t)->getSize() == 32) 
        {
            Int2BoolExpr* temp = new Int2BoolExpr(cond);
            this->cond = temp;
        }
        if(t->isFunc()&&((FunctionType*)t)->getRetType()->isInt()){
            Int2BoolExpr* temp = new Int2BoolExpr(cond);
            this->cond = temp;
        }
    };

    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class WhileStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *Stmt;
public:
    BasicBlock* cond_bb=nullptr;
    BasicBlock* end_bb=nullptr;
    WhileStmt(ExprNode *cond, StmtNode *Stmt) : cond(cond), Stmt(Stmt) {
        Type* t = cond->getSymPtr()->getType();
        if (t->isInt() && ((IntType*) t)->getSize() == 32) 
        {
            Int2BoolExpr* temp = new Int2BoolExpr(cond);
            this->cond = temp;
        }
        if(t->isFunc()&&((FunctionType*)t)->getRetType()->isInt()){
            Int2BoolExpr* temp = new Int2BoolExpr(cond);
            this->cond = temp;
        }
    };

    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
    void setStmt(StmtNode *Stmt){this->Stmt=Stmt;};
};


class ReturnStmt : public StmtNode
{
private:
    ExprNode *retValue;
public:
    ReturnStmt(ExprNode*retValue) : retValue(retValue) {};

    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class BreakStmt : public StmtNode
{
private:
    StmtNode* stmt;
public:
    BreakStmt(StmtNode* stmt):stmt(stmt) {};
    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class ContinueStmt : public StmtNode
{
private:
    StmtNode* stmt;
public:
    ContinueStmt(StmtNode* stmt):stmt(stmt) {};
    void output(int level);
    void typeCheck(Type* retType=nullptr);
    void genCode();
};

class FunctionDef : public StmtNode
{
private:
    SymbolEntry *se;
    IdList* ids;//参数列表
    StmtNode *stmt;
public:
    FunctionDef(SymbolEntry *se, StmtNode *stmt) : se(se), stmt(stmt){ids=nullptr;};
    FunctionDef(SymbolEntry *se, IdList *ids, StmtNode *stmt) : se(se), ids(ids), stmt(stmt){};

    void output(int level);
    void typeCheck(Type* retType=nullptr);
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
    void typeCheck(Type* retType=nullptr);
    void genCode(Unit *unit);
};

class InitValueListExpr : public ExprNode {
private:
    ExprNode* expr;
    int childCnt;
    bool allZero;

   public:
    InitValueListExpr(SymbolEntry* se, ExprNode* expr = nullptr)
        : ExprNode(se), expr(expr) {
        type = se->getType();
        childCnt = 0;
        allZero = false;
    };
    void output(int level){};
    ExprNode* getExpr() const { return expr; };
    void addExpr(ExprNode* expr);
    bool isEmpty() { return childCnt == 0; };
    bool isFull();
    void typeCheck(Type* retType = nullptr) {};
    void genCode() {};
    void fill();
    void setAllZero() { allZero = true; }
};
#endif
