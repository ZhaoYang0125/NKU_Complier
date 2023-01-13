#include "Ast.h"
#include "SymbolTable.h"
#include "Unit.h"
#include "Instruction.h"
#include "IRBuilder.h"
#include <string>
#include <queue>
#include "Type.h"

extern Unit unit;
extern MachineUnit mUnit;
extern FILE *yyout;
int Node::counter = 0;
IRBuilder *Node::builder = nullptr;
bool isreturn=false;
std::queue<bool>isreturn_queue;

Node::Node()
{
    seq = counter++;
    next = nullptr;
}

void Node::setNext(Node *next)
{
    Node *p = this;
    while (p->getNext())
    {
        p = p->getNext();
    }
    if (p == this)
        p->next = next;
    else
        p->setNext(next);
}

void Node::backPatch(std::vector<Instruction *> &list, BasicBlock *bb)
{
    for (auto &inst : list)
    {
        if (inst->isCond())
            dynamic_cast<CondBrInstruction *>(inst)->setTrueBranch(bb);
        else if (inst->isUncond())
            dynamic_cast<UncondBrInstruction *>(inst)->setBranch(bb);
    }
}
//分支为假的回填函数 /*retodo*/
void Node::backPatchFalse(std::vector<Instruction *> &list, BasicBlock *bb)
{
    for (auto &inst : list)
    {
        if (inst->isCond())
            dynamic_cast<CondBrInstruction *>(inst)->setFalseBranch(bb);
        else if (inst->isUncond())
            dynamic_cast<UncondBrInstruction *>(inst)->setBranch(bb);
    }
}

std::vector<Instruction *> Node::merge(std::vector<Instruction *> &list1, std::vector<Instruction *> &list2)
{
    std::vector<Instruction *> res(list1);
    res.insert(res.end(), list2.begin(), list2.end());
    return res;
}

void Ast::genCode(Unit *unit)
{
    //直接把四个函数都声明了，免得后面出错
    //fprintf(yyout, "declare i32 @getint()\ndeclare i32 @getch()\n");
    //fprintf(yyout,"declare void @putint(i32)\ndeclare void @putch(i32)\n");
    IRBuilder *builder = new IRBuilder(unit);
    Node::setIRBuilder(builder);
    root->genCode();
}

void FunctionDef::genCode()
{
    Unit *unit = builder->getUnit();
    Function *func = new Function(unit, se);
    BasicBlock *entry = func->getEntry();
    // set the insert point to the entry basicblock of this function.
    builder->setInsertBB(entry);

    if(ids!=nullptr){
        for(unsigned long int j=0;j<ids->idlist.size();j++){
            // if(j==4)
            //     break;
            IdentifierSymbolEntry *se = dynamic_cast<IdentifierSymbolEntry *>(ids->idlist[j]->getSymPtr());
            //std::cout<<se->getScope()<<std::endl;
            Function *func = builder->getInsertBB()->getParent();
            BasicBlock *entry = func->getEntry();
            Operand *addr;
            SymbolEntry *addr_se;
            Type *type;
            type = new PointerType(se->getType());
            addr_se = new TemporarySymbolEntry(type, SymbolTable::getLabel(),j);//临时符号表
            addr = new Operand(addr_se);
            Instruction *alloca = new AllocaInstruction(addr, se); // alloca指令
            entry->insertFront(alloca);               // allocate instructions should be inserted into the begin of the entry block.
            
            Operand *addr2 = new Operand(ids->idlist[j]->getSymPtr());

            StoreInstruction *store = new StoreInstruction(addr, addr2,builder->getInsertBB());
            se->setAddr(addr);                        // set the addr operand in symbol entry so that we can use it in subsequent code generation.
            
            entry -> insertBack(store);
            func->params.push_back(addr2); 
        }
    }
    stmt->genCode();

    /**
     * Construct control flow graph. You need do set successors and predecessors for each basic block.
     * Todo
     */
    for (auto block = func->begin(); block != func->end(); block++)
    {
        //获取该块的最后一条指令
        Instruction* i = (*block)->begin();
        Instruction* last = (*block)->rbegin();
        //从块中删除条件型语句
        while (i != last) {
            if (i->isCond() || i->isUncond()) {
                (*block)->remove(i);
            }
            i = i->getNext();
        }
        
        if (last->isCond()) {
            BasicBlock *truebranch, *falsebranch;
            truebranch = dynamic_cast<CondBrInstruction*>(last)->getTrueBranch();
            falsebranch = dynamic_cast<CondBrInstruction*>(last)->getFalseBranch();
            
            (*block)->addSucc(truebranch);
            (*block)->addSucc(falsebranch);
            truebranch->addPred(*block);
            falsebranch->addPred(*block);
        } 
        else if (last->isUncond())  //无条件跳转指令可获取跳转的目标块
        {
            BasicBlock* dst =
                dynamic_cast<UncondBrInstruction*>(last)->getBranch();
            (*block)->addSucc(dst);
            //如果无条件跳转的目标块为空 那么就插入return
            dst->addPred(*block);
            if (dst->empty()) {
                if (((FunctionType*)(se->getType()))->getRetType() == TypeSystem::intType)
                    new RetInstruction(new Operand(new ConstantSymbolEntry( TypeSystem::intType, 0)), dst);
                else if (((FunctionType*)(se->getType()))->getRetType() == TypeSystem::voidType)
                    new RetInstruction(nullptr, dst);
            }
        }
    }
            SymbolEntry *se = this->getSymbolEntry();
            Type *ret = ((FunctionType *)(se->getType()))->getRetType();
            if(!isreturn_queue.front() && ret == TypeSystem::voidType){//没有return且函数为空，加上return语句
                    BasicBlock* ret_bb = builder->getInsertBB();
                    Operand* src=nullptr;
                    new RetInstruction(src,ret_bb);
                }
            isreturn_queue.pop();
}

void BinaryExpr::genCode()
{
    BasicBlock *bb = builder->getInsertBB();
    Function *func = bb->getParent();
    if (op == AND)
    {
        BasicBlock *trueBB = new BasicBlock(func); // if the result of lhs is true, jump to the trueBB.
        // trueBB->addPred(bb);//放置这个基本块，设置前驱后继
        // bb->addSucc(trueBB);
        //判断如果是int型则转化为bool
        
        if(!expr1->getSymPtr()->isVariable()&& expr1->getOperand()->getType()->isInt()){
            expr1->int2Bool();
        }
        if(!expr2->getSymPtr()->isVariable()&& expr2->getOperand()->getType()->isInt()){
            expr2->int2Bool(); 
        }
        //std::cout<<"isbool"<<std::endl;
        expr1->genCode();
        backPatch(expr1->trueList(), trueBB);
        builder->setInsertBB(trueBB); // set the insert point to the trueBB so that intructions generated by expr2 will be inserted into it.
        expr2->genCode();
        true_list = expr2->trueList();
        false_list = merge(expr1->falseList(), expr2->falseList());
    }
    else if (op == OR)
    {
        // Todo 
        BasicBlock* falseBB = new BasicBlock(func);
        // falseBB->addPred(bb);
        // bb->addSucc(falseBB);
        //判断如果是int型则转化为bool
        if(expr1->getOperand()->getType()->isInt()){
            expr1->int2Bool();
        }
        if(expr2->getOperand()->getType()->isInt()){
            expr2->int2Bool(); 
        }
        expr1->genCode();
        backPatchFalse(expr1->falseList(), falseBB);
        builder->setInsertBB(falseBB);
        expr2->genCode();
        false_list = expr2->falseList();
        true_list = merge(expr1->trueList(), expr2->trueList());
    }
    /*
    enum {ADD, SUB, MUL, DIV, MOD, 
            AND, OR,
             LESS, GREATER, LESSEQUAL, GREATEREQUAL, EQUAL, NOTEQUAL};
    */
    else if (op >= ADD && op <= MOD)
    {
        expr1->genCode();
        expr2->genCode();
        Operand *src1 = expr1->getOperand();
        Operand *src2 = expr2->getOperand();
        int opcode;
        switch (op)
        {
        case ADD:
            opcode = BinaryInstruction::ADD;
            break;
        case SUB:
            opcode = BinaryInstruction::SUB;
            break;
        case MUL:
            opcode = BinaryInstruction::MUL;
            break;
        case DIV:
            opcode = BinaryInstruction::DIV;
            break;
        case MOD:
            opcode = BinaryInstruction::MOD;
            break;
        default:
            break;
        }
        new BinaryInstruction(opcode, dst, src1, src2, bb);
    }
    else if (op >= LESS && op <= NOTEQUAL)
    {
        // Todo
        expr1->genCode();
        expr2->genCode();
        Operand *src1 = expr1->getOperand();
        Operand *src2 = expr2->getOperand();
        int cmpcode;
        switch (op)
        {
        case LESS:
            cmpcode = CmpInstruction::L;
            break;
        case LESSEQUAL:
            cmpcode = CmpInstruction::LE;
            break;
        case GREATER:
            cmpcode = CmpInstruction::G;
            break;
        case GREATEREQUAL:
            cmpcode = CmpInstruction::GE;
            break;
        case EQUAL:
            cmpcode = CmpInstruction::E;
            break;
        case NOTEQUAL:
            cmpcode = CmpInstruction::NE;
            break;
        default:
            break;
        }
        new CmpInstruction(cmpcode, dst, src1, src2, bb);

        // need modify
        //正确错误列表合并
        //true_list = merge(expr1->trueList(), expr2->trueList());
        //false_list = merge(expr1->falseList(), expr2->falseList());
        //条件跳转指令
        BasicBlock *truebb = new BasicBlock(func);
        BasicBlock *falsebb = new BasicBlock(func);
        BasicBlock *tempbb = new BasicBlock(func);
        Instruction* temp = new CondBrInstruction(truebb,tempbb,dst,bb);
        this->trueList().push_back(temp);
        temp=new UncondBrInstruction(falsebb, tempbb);
        this->falseList().push_back(temp);
    }
}

void Constant::genCode()
{
    // we don't need to generate code.
}

void Id::genCode()
{
    //std::cout<<"id"<<std::endl;
    BasicBlock *bb = builder->getInsertBB();
    Operand *addr = dynamic_cast<IdentifierSymbolEntry *>(symbolEntry)->getAddr();
    if (type->isInt())
        new LoadInstruction(dst, addr, bb);
    // 主要思想是多维的 先把上一维度的地址找到 然后根据下标找下一个维度
    else if(type->isArray())
    {
        //遍历维度
        if (arrIdx) 
        {
            //获取当前类型和元素类型
            Type* type = ((ArrayType*)(this->type))->getElementType();
            Type* type1 = this->type;
            
            Operand* tempSrc = addr;//中间目标地址
            Operand* tempDst = dst;//中间目标值
            
            ExprNode* idx = arrIdx;
            //标识GepInstruction的paramFirst
            //主要是用于区分函数参数a[][3]的情况
            bool flag = false;
            
            bool pointer = false;
            bool firstFlag = true;
            
            while (true) 
            {
               //针对参数是数组的情况  a[][3]
               //把基址加载到tempSrc
                if (((ArrayType*)type1)->getLength() == -1) 
                {
                    Operand* dst1 = new Operand(new TemporarySymbolEntry(
                        new PointerType(type), SymbolTable::getLabel()));
                    tempSrc = dst1;//中间变量
                    new LoadInstruction(dst1, addr, bb);
                    
                    flag = true;
                    firstFlag = false;
                }
                //如果维度遍历结束 将对应数组值传递到dst 然后退出
                if (!idx) {
                    Operand* dst1 = new Operand(new TemporarySymbolEntry(
                        new PointerType(type), SymbolTable::getLabel()));
                    Operand* idx = new Operand(
                        new ConstantSymbolEntry(TypeSystem::intType, 0));
                    new GepInstruction(dst1, tempSrc, idx, bb);
                    tempDst = dst1;
                    pointer = true;
                    break;
                }
                //生成维度
                idx->genCode();
                //用于维度寻址 将tempSrc[idx]的值加载到tempDst
                auto gep = new GepInstruction(tempDst, tempSrc,
                                              idx->getOperand(), bb, flag);
                //如果当前不是a[][3]这种情况
                //并且是第一个维度寻址
                if (!flag && firstFlag) 
                {
                    gep->setFirst();
                    firstFlag = false;
                }
                //flag每个参数都要重置
                if (flag)
                    flag = false;
                //维度要全部换成整数的维度    
                if (type == TypeSystem::intType)
                    break;
                type = ((ArrayType*)type)->getElementType();
                type1 = ((ArrayType*)type1)->getElementType();
                tempSrc = tempDst;
                tempDst = new Operand(new TemporarySymbolEntry(
                    new PointerType(type), SymbolTable::getLabel()));
                idx = (ExprNode*)(idx->getNext());
            }
            dst = tempDst;
            
            // 如果此ID是右值 需要再次load
            if (!left && !pointer) 
            {
                Operand* dst1 = new Operand(new TemporarySymbolEntry(
                    TypeSystem::intType, SymbolTable::getLabel()));
                new LoadInstruction(dst1, dst, bb);
                dst = dst1;
            }
            
        } 
        //针对声明数组的情况 和上面类似
        else 
        {
            if (((ArrayType*)(this->type))->getLength() == -1) 
            {
                Operand* dst1 = new Operand(new TemporarySymbolEntry(
                    new PointerType(
                        ((ArrayType*)(this->type))->getElementType()),
                    SymbolTable::getLabel()));
                new LoadInstruction(dst1, addr, bb);
                dst = dst1;
            } 
            else 
            {
                Operand* idx = new Operand(
                    new ConstantSymbolEntry(TypeSystem::intType, 0));
                auto gep = new GepInstruction(dst, addr, idx, bb);
                gep->setFirst();
            }
        }
    }
}

void IdList::genCode(){

}

Int2BoolExpr::Int2BoolExpr(ExprNode* expr) : ExprNode(nullptr), expr(expr) 
{
    symbolEntry = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
    dst = new Operand(symbolEntry);
}

void Int2BoolExpr::genCode(){
    expr->genCode();
    BasicBlock* bb = builder->getInsertBB();
    Function* func = bb->getParent();
    Instruction* cur_inst;
    BasicBlock* tempbb = new BasicBlock(func);
    BasicBlock* trueBB = new BasicBlock(func);
    BasicBlock* falseBB = new BasicBlock(func);

    Operand* src=new Operand(new ConstantSymbolEntry(TypeSystem::intType, 0));
    new CmpInstruction(CmpInstruction::NE, this->dst, this->expr->getOperand(),src , bb);
    cur_inst=new CondBrInstruction(trueBB, tempbb, this->dst, bb);
    this->trueList().push_back(cur_inst);
    cur_inst=new UncondBrInstruction(falseBB, tempbb);
    this->falseList().push_back(cur_inst);
}

IfStmt::IfStmt(ExprNode *cond, StmtNode *thenStmt) : cond(cond), thenStmt(thenStmt){
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

void IfStmt::genCode()
{
    Function *func;
    BasicBlock *then_bb, *end_bb;

    func = builder->getInsertBB()->getParent();
    then_bb = new BasicBlock(func);
    end_bb = new BasicBlock(func);
    //printf("label: %d, label: %d\n", then_bb->getNo(), end_bb->getNo());

    // then_bb -> addPred(builder->getInsertBB());//设置其前驱
    // builder -> getInsertBB() -> addSucc(then_bb);//设置后继
    // end_bb -> addPred(then_bb);
    // then_bb -> addSucc(end_bb);//

    Type* t = cond->getSymPtr()->getType();
    //std::cout<<"unary"<<std::endl;
    if(t->isInt() && ((IntType*) t)->getSize() == 32){
        cond->int2Bool();
    }
    
    cond->genCode();
    backPatch(cond->trueList(), then_bb);
    backPatchFalse(cond->falseList(), end_bb);

    builder->setInsertBB(then_bb);
    thenStmt->genCode();
    then_bb = builder->getInsertBB();
    new UncondBrInstruction(end_bb, then_bb);

    builder->setInsertBB(end_bb);
}

/*if then else end*/
void IfElseStmt::genCode()
{
    // Todo
    Function *func;
    BasicBlock *then_bb,*end_bb,*else_bb;
    //bulider用于传递继承属性
    func=builder->getInsertBB()->getParent();//找到当前要插入的基本块的父节点
    then_bb = new BasicBlock(func);
    else_bb = new BasicBlock(func);
    end_bb = new BasicBlock(func);

    //设置前驱后继
    // then_bb -> addPred(builder -> getInsertBB());
    // builder -> getInsertBB() -> addSucc(then_bb);

    // else_bb -> addPred(builder -> getInsertBB());
    // builder -> getInsertBB() -> addSucc(else_bb);

    // end_bb -> addPred(then_bb);
    // then_bb -> addSucc(end_bb);
    // end_bb -> addPred(else_bb);
    // else_bb -> addSucc(end_bb);

    cond->genCode();

    backPatch(cond->trueList(),then_bb);
    backPatchFalse(cond->falseList(),else_bb);

    //thenStmt
    builder->setInsertBB(then_bb);
    thenStmt->genCode();
    then_bb = builder->getInsertBB();
    new UncondBrInstruction(end_bb, then_bb);

    builder->setInsertBB(else_bb);
    elseStmt->genCode();
    else_bb=builder->getInsertBB();
    new UncondBrInstruction(end_bb,else_bb);

    builder->setInsertBB(end_bb);
}
//函数体
void CompoundStmt::genCode()
{
    // Todo
    if(stmt!=nullptr){
        stmt->genCode();
    }
}

void SeqNode::genCode()
{
    // Todo
    if(stmt1 != nullptr)
        stmt1->genCode();
    if(stmt2 != nullptr)
        stmt2->genCode();
}

void DeclStmt::genCode()
{//类型不兼容，必须用unsigned long
    for(unsigned long int j=0;j<ids->idlist.size();j++){
        IdentifierSymbolEntry *se = dynamic_cast<IdentifierSymbolEntry *>(ids->idlist[j]->getSymPtr());
        if (se->isGlobal())
        {   //设置se
            Operand *addr;
            SymbolEntry *addr_se;
            addr_se = new IdentifierSymbolEntry(*se);
            addr_se->setType(new PointerType(se->getType()));
            addr = new Operand(addr_se);
            se->setAddr(addr);
            mUnit.InsertGlobal(se);
            //判断变量是否直接赋值
            Operand *src;
            if(ids->assignlist.size()>0&&ids->assignlist[j]){//如果有预先赋值
                //std::cout<<"i"<<std::endl;
                //获取赋的值
                AssignStmt* assign=ids->assignlist[j];
                assign -> genCode();
                src = assign-> expr -> getOperand();
            }
            else{//没有赋初值则置零
                SymbolEntry *se = new ConstantSymbolEntry(TypeSystem::intType, 0);
                Constant* digit = new Constant(se);
                src = digit -> getOperand();
            }
            GlobalInstruction *g=new GlobalInstruction(addr,src,se);
            g->output();
        }
        else if (se->isLocal())
        {
            Function *func = builder->getInsertBB()->getParent();
            BasicBlock *entry = func->getEntry();
            Instruction *alloca;
            Operand *addr;
            SymbolEntry *addr_se;
            Type *type;
            type = new PointerType(se->getType());
            addr_se = new TemporarySymbolEntry(type, SymbolTable::getLabel());//临时符号表
            addr = new Operand(addr_se);
            alloca = new AllocaInstruction(addr, se); // alloca指令
            entry->insertFront(alloca);               // allocate instructions should be inserted into the begin of the entry block.
            se->setAddr(addr);                        // set the addr operand in symbol entry so that we can use it in subsequent code generation.
            //如果赋了初值
            if(ids->assignlist.size()>0&&ids->assignlist[j]!=nullptr){
                Operand *addr = dynamic_cast<IdentifierSymbolEntry*>(ids -> assignlist[j] -> lval ->getSymPtr())->getAddr();
                se->setAddr(addr); 
                ids -> assignlist[j] -> genCode();
            }
        }
    }
}

void ReturnStmt::genCode()
{
    // Todo
    BasicBlock* ret_bb = builder->getInsertBB();
    Operand* src=nullptr;
    if(retValue){
        retValue->genCode();
        src=retValue->getOperand();
    }
    new RetInstruction(src,ret_bb);
}

void AssignStmt::genCode()
{
    BasicBlock *bb = builder->getInsertBB();
    expr->genCode();
    Operand* addr = nullptr;
    if (lval->getType()->isInt())
        addr = dynamic_cast<IdentifierSymbolEntry *>(lval->getSymPtr())->getAddr();
    else if (lval->getType()->isArray()) {
        ((Id*)lval)->setLeft();
        lval->genCode();
        addr = lval->getOperand();
    }
    Operand *src = expr->getOperand();
    /***
     * We haven't implemented array yet, the lval can only be ID. So we just store the result of the `expr` to the addr of the id.
     * If you want to implement array, you have to caculate the address first and then store the result into it.
     */
    new StoreInstruction(addr, src, bb);
}

// self define
void ExprStmt::genCode()
{
    // Todo
    expr->genCode();
}
void CallExpr::genCode()
{
    // Todo
    std::vector<Operand*> operands;
    ExprNode* temp = param;
    while (temp) {
        temp->genCode();
        operands.push_back(temp->getOperand());
        temp = ((ExprNode*)temp->getNext());
    }
    BasicBlock* bb = builder->getInsertBB();
    Type* retType = ((FunctionType*)symbolEntry->getType())->getRetType();
    if(!retType->isVoid())
    {
        Type *type2 = new IntType(32);  // temp register for function retValue
        SymbolEntry *addr_se2 = new TemporarySymbolEntry(type2, SymbolTable::getLabel());
        dst = new Operand(addr_se2);
    }
    else{
        dst=nullptr;
    }
    new CallInstruction(dst, symbolEntry, operands, bb);
}

void UnaryExpr::genCode()
{
    // Todo
    BasicBlock* bb = builder->getInsertBB();
    if (op == NOT) 
    {
        expr->genCode();
        Operand* src = expr->getOperand();
        if (!expr->getOperand()->getType()->isBool()) 
        {//int->bool 与零作比较
            Operand* temp = new Operand(new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel()));
            new CmpInstruction(CmpInstruction::NE, temp, src, new Operand(new ConstantSymbolEntry(TypeSystem::intType, 0)), bb);
            src = temp;
        }
        new XorInstruction(dst, src, bb);
    } 
    else if (op == SUB || op == ADD) 
    {
        expr->genCode();
        Operand* src2 = expr->getOperand();//获得表达式源操作数

        // to complete
        if (src2->getType() == TypeSystem::boolType) 
        {
            Operand* temp =new Operand(new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel()));
            new ZextInstruction(temp, expr->dst,bb); //bool型扩展为整形
            expr->dst = temp;   
            src2 = temp; 
        }
        int opcode;
        switch (op)
        {
        case ADD:
            opcode = BinaryInstruction::ADD;
            break;
        case SUB:
            opcode = BinaryInstruction::SUB;
            break;
        default:
            break;
        }
        Operand* src1 = new Operand(new ConstantSymbolEntry(TypeSystem::intType, 0));
        //if(src1)
        new BinaryInstruction(opcode, dst, src1, src2, bb);
    }
}

void BlankStmt::genCode()
{
    // Todo
    // nothing
}
void WhileStmt::genCode()
{
    // Todo    Function* func;
    Function *func;
    BasicBlock *loop_bb, *end_bb , *cond_bb;

    func = builder -> getInsertBB() -> getParent();
    loop_bb = new BasicBlock(func);
    end_bb = new BasicBlock(func);
    cond_bb = new BasicBlock(func);

    this->cond_bb = cond_bb;
    this->end_bb = end_bb;

    new UncondBrInstruction(cond_bb, builder -> getInsertBB());
    //设置前驱后继
    // cond_bb -> addPred(builder -> getInsertBB());
    // builder -> getInsertBB() -> addSucc(cond_bb);
    // loop_bb -> addPred(cond_bb);
    // cond_bb -> addSucc(loop_bb);
    // end_bb -> addPred(loop_bb);
    // loop_bb -> addSucc(end_bb);

    // end_bb -> addPred(cond_bb);
    // cond_bb -> addSucc(end_bb);

    builder->setInsertBB(cond_bb);
    cond -> genCode();

    backPatch(cond -> trueList(), loop_bb);
    backPatchFalse(cond -> falseList(), end_bb);

    builder -> setInsertBB(loop_bb);
    Stmt -> genCode();
    loop_bb = builder -> getInsertBB();
    new UncondBrInstruction(cond_bb, loop_bb);
    builder->setInsertBB(end_bb);
}

void BreakStmt::genCode(){
    Function* func = builder->getInsertBB()->getParent();
    BasicBlock* bb = builder->getInsertBB();
    BasicBlock* end_bb = ((WhileStmt*)this->stmt)->end_bb;
    new UncondBrInstruction(end_bb, bb);
    BasicBlock* next_bb = new BasicBlock(func);
    builder->setInsertBB(next_bb);
}

void ContinueStmt::genCode(){
    Function* func = builder->getInsertBB()->getParent();
    BasicBlock* bb = builder->getInsertBB();
    BasicBlock* cond_bb = ((WhileStmt*)this->stmt)->cond_bb;
    new UncondBrInstruction(cond_bb, bb);
    BasicBlock* next_bb = new BasicBlock(func);
    builder->setInsertBB(next_bb);
}

void BreakStmt::typeCheck(Type* retType){}
void ContinueStmt::typeCheck(Type* retType){}

void ExprStmt::typeCheck(Type* retType)
{
    // Todo 
    // nothing
}
void WhileStmt::typeCheck(Type* retType)
{
    // Todo
    if(Stmt)
        return Stmt->typeCheck(retType);
}
void CallExpr::typeCheck(Type* retType)
{
    // Todo
    // nothing
    //std::cout<<"expr1->getType()->toStr()"<<std::endl;
    //this->type=((FunctionType*)symbolEntry->getType())->getRetType();
}
void UnaryExpr::typeCheck(Type* retType)
{
    // Todo
    // nothing
    //std::cout<<"expr1->getType()->toStr()"<<std::endl;
}
void BlankStmt::typeCheck(Type* retType)
{
    // Todo
    // nothing
}

void Ast::typeCheck(Type* retType)
{
    if (root != nullptr)
        root->typeCheck();
}

void FunctionDef::typeCheck(Type* retType)
{
    // Todo
    isreturn=false;
    isreturn_queue.push(false);
    
    SymbolEntry *se = this->getSymbolEntry();
    Type *ret = ((FunctionType *)(se->getType()))->getRetType();
    if (stmt == nullptr&&ret != TypeSystem::voidType)//函数内容为空
    {
        fprintf(stderr, "返回值不为空的函数\'%s\'缺少return语句。\n",se->toStr().c_str());
        exit(EXIT_FAILURE);
    }
    else{
        stmt->typeCheck(ret);
        if(isreturn)
            isreturn_queue.back()=true;
        if(!isreturn && ret != TypeSystem::voidType){//函数没有return语句
            fprintf(stderr, "返回值不为空的函数\'%s\'缺少return语句。\n",se->toStr().c_str());
            exit(EXIT_FAILURE);
        }
    }
}

void BinaryExpr::typeCheck(Type* retType)
{
    // Todo
    expr1->typeCheck(retType);
    expr2->typeCheck(retType);
    Type *type1 = expr1->getSymPtr()->getType();
    // Type *type2 = expr2->getSymPtr()->getType();

    // if (!type1->equal(type2))
    // {
    //     fprintf(stderr, "类型为 %s 的变量 %s 和类型为 %s 的变量 %s不匹配。\n",
    //             type1->toStr().c_str(), expr1->getSymPtr()->toStr().c_str(),
    //             type2->toStr().c_str(), expr2->getSymPtr()->toStr().c_str());
    //     exit(EXIT_FAILURE);
    // }
    if(expr1->getType()){
        if(expr1->getType()->isVoid()){
            fprintf(stderr, "类型为空的表达式 %s 不能进行运算。\n",
                    expr1->getSymPtr()->toStr().c_str());
            exit(EXIT_FAILURE);
        }
    }
    if(expr2->getType()){
        if(expr2->getType()->isVoid()){
            fprintf(stderr, "类型为空的表达式 %s 不能进行运算。\n",
                    expr2->getSymPtr()->toStr().c_str());
            exit(EXIT_FAILURE);
        }
    }
    symbolEntry->setType(type1);
}

void Constant::typeCheck(Type* retType)
{
    // Todo
    // nothing need to do
}

void Id::typeCheck(Type* retType)
{
    // Todo
    // nothing can do
}

void IdList::typeCheck(Type* retType){}

void IfStmt::typeCheck(Type* retType)
{
    // Todo
    cond->typeCheck(retType);
    if (thenStmt)
        thenStmt->typeCheck(retType);
}

void IfElseStmt::typeCheck(Type* retType)
{
    // Todo
    if (thenStmt)
        thenStmt->typeCheck(retType);
    if (elseStmt)
        elseStmt->typeCheck(retType);
}

void CompoundStmt::typeCheck(Type* retType)
{
    // Todo
    if (stmt)
        stmt->typeCheck(retType);
}

void SeqNode::typeCheck(Type* retType)
{
    // Todo
    if (stmt1)
        stmt1->typeCheck(retType);
    if (stmt2)
        stmt2->typeCheck(retType);
}

void DeclStmt::typeCheck(Type* retType)
{
    // Todo
    // nothing to do
    for(unsigned long int j = 0; j < ids->idlist.size(); j++)
    {
       //如果赋了初值
        if(ids->assignlist.size() > 0 && ids->assignlist[j] != nullptr){
            ids -> assignlist[j] -> typeCheck();
        }
        //存疑
    }
}

void ReturnStmt::typeCheck(Type* retType)
{
   // Todo
    if (!retValue && !retType->isVoid()) {
        fprintf(stderr,"\'%s\'型函数缺少返回值。\n",retType->toStr().c_str());
        exit(EXIT_FAILURE);
    }
    if (retValue && retType->isVoid()) {
        fprintf(stderr,"void型函数不应有返回值。\n");
        exit(EXIT_FAILURE);
    }
    isreturn=true;//判断函数是否有return语句用
}

void AssignStmt::typeCheck(Type* retType)
{
    // Todo
    expr->typeCheck();
}

void UnaryExpr::output(int level)
{
    std::string op_str;
    switch (op)
    {
    case ADD:
        op_str = "add";
        break;
    case SUB:
        op_str = "sub";
        break;
    case NOT:
        op_str = "not";
        break;
    }
    fprintf(yyout, "%*cUnaryExpr\top: %s\n", level, ' ', op_str.c_str());
    expr->output(level + 4);
}

void BinaryExpr::output(int level)
{
    std::string op_str;
    switch (op)
    {
    case ADD:
        op_str = "add";
        break;
    case SUB:
        op_str = "sub";
        break;
    case MUL:
        op_str = "mul";
        break;
    case DIV:
        op_str = "div";
        break;
    case MOD:
        op_str = "mod";
        break;
    case AND:
        op_str = "and";
        break;
    case OR:
        op_str = "or";
        break;
    case LESS:
        op_str = "less";
        break;
    case GREATER:
        op_str = "greater";
        break;
    case LESSEQUAL:
        op_str = "less equal";
        break;
    case GREATEREQUAL:
        op_str = "greater equal";
        break;
    case EQUAL:
        op_str = "equal";
        break;
    case NOTEQUAL:
        op_str = "not equal";
        break;
    }
    fprintf(yyout, "%*cBinaryExpr\top: %s\n", level, ' ', op_str.c_str());
    expr1->output(level + 4);
    expr2->output(level + 4);
}

void Ast::output()
{
    fprintf(yyout, "program\n");
    if (root != nullptr)
        root->output(4);
}

void Constant::output(int level)
{
    std::string type, value;
    type = symbolEntry->getType()->toStr();
    value = symbolEntry->toStr();
    fprintf(yyout, "%*cIntegerLiteral\tvalue: %s\ttype: %s\n", level, ' ',
            value.c_str(), type.c_str());
}

void Id::output(int level)
{
    std::string name, type;
    int scope;
    bool inited;
    int value;
    inited = dynamic_cast<IdentifierSymbolEntry *>(symbolEntry)->inited;
    if (inited)
    {
        value = dynamic_cast<IdentifierSymbolEntry *>(symbolEntry)->getValue();
    }
    name = symbolEntry->toStr();
    type = symbolEntry->getType()->toStr();
    scope = dynamic_cast<IdentifierSymbolEntry *>(symbolEntry)->getScope();
    fprintf(yyout, "%*cId\tname: %s\tscope: %d\ttype: %s\n", level, ' ',
            name.c_str(), scope, type.c_str());

    if (inited)
    {
        fprintf(yyout, "value: %d\n", value);
    }
    /*
    else
    {
        fprintf(yyout, "value: uninitialized\n");
    }
    */
}

CallExpr::CallExpr(SymbolEntry *se, ExprNode *param /*=NULL*/) : ExprNode(se), param(param)
{ // 有参函数调用
    SymbolEntry *s = se;
    ExprNode *temp = param;
    int paramnum = 0;
    while (temp)
    {
        temp = (ExprNode *)(temp->getNext());
        paramnum++;
    }
    FunctionType* type = (FunctionType*)s->getType();
    int paramsize=type->paramsType.size();

    this->type=((FunctionType*)s->getType())->getRetType();
    // std::vector<Type *> params;
    // while (s)
    // {
    //     Type *type = s->getType();
    //     params.push_back(type);
    //     s = s->getNext();
    // }
    if (paramnum == paramsize)
    {
        // this->symbolEntry = s;
    }
    else
    {
        fprintf(stderr, "函数 \'%s\'有%d个参数，但在调用时传了%d个。\n", se->toStr().c_str(), paramsize,int(paramnum));
        exit(EXIT_FAILURE);
    }
    //params.clear();
};

void ExprStmt::output(int level)
{
    fprintf(yyout, "%*cExprStmt\n", level, ' ');
    expr->output(level + 4);
}

void BlankStmt::output(int level)
{
    fprintf(yyout, "%*cBlankStmt\n", level, ' ');
}

void CallExpr::output(int level)
{
    std::string name, type;
    int scope;
    if (symbolEntry)
    {
        name = symbolEntry->toStr();
        type = symbolEntry->getType()->toStr();
        scope = dynamic_cast<IdentifierSymbolEntry *>(symbolEntry)->getScope();

        fprintf(yyout, "%*cCallExpr\tfunction name: %s\tscope: %d\ttype: %s\n", level, ' ', name.c_str(), scope, type.c_str());
        Node *temp = param; // 形参输出
        while (temp)
        {
            temp->output(level + 4);
            temp = temp->getNext();
        }
    }
}

void CompoundStmt::output(int level)
{
    fprintf(yyout, "%*cCompoundStmt\n", level, ' ');
    stmt->output(level + 4);
}

void SeqNode::output(int level)
{
    fprintf(yyout, "%*cSequence\n", level, ' ');
    stmt1->output(level + 4);
    stmt2->output(level + 4);
}

void IdList::output(int level){
    fprintf(yyout, "%*cIdList\n", level, ' ');
    for(unsigned long int i = 0; i < idlist.size(); i++){
        idlist[i] -> output(level + 4);
    }
    for(unsigned long int j = 0; j < assignlist.size(); j++){
        assignlist[j] -> output(level + 4);
    }
}

void DeclStmt::output(int level)
{
    fprintf(yyout, "%*cDeclStmt\n", level, ' ');
    ids->output(level + 4);

    if (this->getNext())
    {
        this->getNext()->output(level);
    }
}

void IfStmt::output(int level)
{
    fprintf(yyout, "%*cIfStmt\n", level, ' ');
    cond->output(level + 4);
    thenStmt->output(level + 4);
}

void IfElseStmt::output(int level)
{
    fprintf(yyout, "%*cIfElseStmt\n", level, ' ');
    cond->output(level + 4);
    thenStmt->output(level + 4);
    elseStmt->output(level + 4);
}

void WhileStmt::output(int level)
{
    fprintf(yyout, "%*cWhileStmt\n", level, ' ');
    cond->output(level + 4);
    Stmt->output(level + 4);
}

void ReturnStmt::output(int level)
{
    fprintf(yyout, "%*cReturnStmt\n", level, ' ');
    retValue->output(level + 4);
}

void AssignStmt::output(int level)
{
    fprintf(yyout, "%*cAssignStmt\n", level, ' ');
    lval->output(level + 4);
    expr->output(level + 4);
}

void FunctionDef::output(int level)
{
    std::string name, type;
    name = se->toStr();
    type = se->getType()->toStr();
    fprintf(yyout, "%*cFunctionDefine function name: %s, type: %s\n", level, ' ',
            name.c_str(), type.c_str());
    stmt->output(level + 4);
}

void BreakStmt::output(int level)
{
    fprintf(yyout, "%*cBreakStmt\n", level, ' ');
}

void ContinueStmt::output(int level)
{
    fprintf(yyout, "%*cContinueStmt\n", level, ' ');
}

int UnaryExpr::getValue(){
    // lab7todo
    int value = 0;
    switch(op)      /* enum {ADD, SUB, NOT}; */
    {
        case ADD:
            value = expr->getValue();
            break;
        case SUB:
            value = -expr->getValue();
            break;
        case NOT:
            value = !expr->getValue();
            break;
    }
    return value;
}

int CallExpr::getValue(){
    // lab7todo
    return 0;
}

int BinaryExpr::getValue(){
    // lab7todo
    int value=0;
    int value1=expr1->getValue();
    int value2=expr2->getValue();
    /* 
    enum {ADD, SUB, MUL, DIV, MOD, AND, OR, 
    LESS, GREATER, LESSEQUAL, GREATEREQUAL, EQUAL, NOTEQUAL}; 
    */
    switch(op)
    {
        case ADD:
            value = value1 + value2;
            break;
        case SUB:
            value = value1 - value2;
            break;
        case MUL:
            value = value1 * value2;
            break;
        case DIV:
            if(value2!=0)
                value = value1 / value2;
            break;
        case MOD:
            value = value1 % value2;
            break;
        case AND:
            value = value1 && value2;
            break;
        case OR:
            value = value1 || value2;
            break;
        case LESS:
            value = value1 < value2;
            break;
        case GREATER:
            value = value1 > value2;
            break;
        case LESSEQUAL:
            value = value1 <= value2;
            break;
        case GREATEREQUAL:
            value = value1 >= value2;
            break;
        case EQUAL:
            value = value1 == value2;
            break;
        case NOTEQUAL:
            value = value1 != value2;
            break;
    }
    return value;
}

int Constant::getValue() 
{
    return ((ConstantSymbolEntry*)symbolEntry)->getValue();
}

int Id::getValue() 
{
    return ((IdentifierSymbolEntry*)symbolEntry)->getValue();
}

void InitValueListExpr::addExpr(ExprNode *expr)
{
    if (this->expr == nullptr) {
        if(childCnt != 0) { printf("childCnt != 0\n"); exit(-1); }
        childCnt++;
        this->expr = expr;
    } else {
        childCnt++;
        this->expr->setNext(expr);
    }
}

bool InitValueListExpr::isFull()
{
    ArrayType* type = (ArrayType*)(this->symbolEntry->getType());
    return childCnt == type->getLength();
}

void InitValueListExpr::fill()
{
    if (allZero) {
        return;
    }
    Type* type = ((ArrayType*)(this->getType()))->getElementType();
    if (type->isArray()) {
        while (!isFull()) {
            this->addExpr(new InitValueListExpr(new ConstantSymbolEntry(type)));
        }
        ExprNode* temp = expr;
        while (temp) {
            ((InitValueListExpr*)temp)->fill();
            temp = (ExprNode*)(temp->getNext());
        }
    }
    if (type->isInt()) {
        while (!isFull()) {
            this->addExpr(new Constant(new ConstantSymbolEntry(type, 0)));
        }
        return;
    }
}
