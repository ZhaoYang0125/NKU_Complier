#include "Instruction.h"
#include "BasicBlock.h"
#include <iostream>
#include "Function.h"
#include "Type.h"
extern FILE* yyout;

Instruction::Instruction(unsigned instType, BasicBlock *insert_bb)
{
    prev = next = this;
    opcode = -1;
    this->instType = instType;
    if (insert_bb != nullptr)
    {
        insert_bb->insertBack(this);
        parent = insert_bb;
    }
}

Instruction::~Instruction()
{
    parent->remove(this);
}

BasicBlock *Instruction::getParent()
{
    return parent;
}

void Instruction::setParent(BasicBlock *bb)
{
    parent = bb;
}

void Instruction::setNext(Instruction *inst)
{
    next = inst;
}

void Instruction::setPrev(Instruction *inst)
{
    prev = inst;
}

Instruction *Instruction::getNext()
{
    return next;
}

Instruction *Instruction::getPrev()
{
    return prev;
}

BinaryInstruction::BinaryInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb) : Instruction(BINARY, insert_bb)
{
    this->opcode = opcode;
    operands.push_back(dst);
    operands.push_back(src1);
    operands.push_back(src2);
    dst->setDef(this);
    src1->addUse(this);
    src2->addUse(this);
}

BinaryInstruction::~BinaryInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void BinaryInstruction::output() const
{
    std::string s1, s2, s3, op, type;
    s1 = operands[0]->toStr();
    s2 = operands[1]->toStr();
    s3 = operands[2]->toStr();
    type = operands[0]->getType()->toStr();
    switch (opcode)
    {
        case ADD:
            op = "add";
            break;
        case SUB:
            op = "sub";
            break;
        case MUL:
            op = "mul";
            break;
        case DIV:
            op = "sdiv";
            break;
        case MOD:
            op = "srem";
            break;
        default:
            break;
    }
    fprintf(yyout, "  %s = %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
}

CmpInstruction::CmpInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb): Instruction(CMP, insert_bb){
    this->opcode = opcode;
    operands.push_back(dst);
    operands.push_back(src1);
    operands.push_back(src2);
    dst->setDef(this);
    src1->addUse(this);
    src2->addUse(this);
}

CmpInstruction::~CmpInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void CmpInstruction::output() const
{
    std::string s1, s2, s3, op, type;
    s1 = operands[0]->toStr();
    s2 = operands[1]->toStr();
    s3 = operands[2]->toStr();
    type = operands[1]->getType()->toStr();
    switch (opcode)
    {
    case E:
        op = "eq";
        break;
    case NE:
        op = "ne";
        break;
    case L:
        op = "slt";
        break;
    case LE:
        op = "sle";
        break;
    case G:
        op = "sgt";
        break;
    case GE:
        op = "sge";
        break;
    default:
        op = "";
        break;
    }

    fprintf(yyout, "  %s = icmp %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
}

UncondBrInstruction::UncondBrInstruction(BasicBlock *to, BasicBlock *insert_bb) : Instruction(UNCOND, insert_bb)
{
    branch = to;
}

void UncondBrInstruction::output() const
{
    if(!branch->empty())
        fprintf(yyout, "  br label %%B%d\n", branch->getNo());
}

void UncondBrInstruction::setBranch(BasicBlock *bb)
{
    branch = bb;
}

BasicBlock *UncondBrInstruction::getBranch()
{
    return branch;
}

CondBrInstruction::CondBrInstruction(BasicBlock*true_branch, BasicBlock*false_branch, Operand *cond, BasicBlock *insert_bb) : Instruction(COND, insert_bb){
    this->true_branch = true_branch;
    this->false_branch = false_branch;
    cond->addUse(this);
    operands.push_back(cond);
}

CondBrInstruction::~CondBrInstruction()
{
    operands[0]->removeUse(this);
}

void CondBrInstruction::output() const
{
    std::string cond, type;
    cond = operands[0]->toStr();
    type = operands[0]->getType()->toStr();
    int true_label = true_branch->getNo();
    int false_label = false_branch->getNo();
    fprintf(yyout, "  br %s %s, label %%B%d, label %%B%d\n", type.c_str(), cond.c_str(), true_label, false_label);
}

void CondBrInstruction::setFalseBranch(BasicBlock *bb)
{
    false_branch = bb;
}

BasicBlock *CondBrInstruction::getFalseBranch()
{
    return false_branch;
}

void CondBrInstruction::setTrueBranch(BasicBlock *bb)
{
    true_branch = bb;
}

BasicBlock *CondBrInstruction::getTrueBranch()
{
    return true_branch;
}

RetInstruction::RetInstruction(Operand *src, BasicBlock *insert_bb) : Instruction(RET, insert_bb)
{
    if(src != nullptr)
    {
        operands.push_back(src);
        src->addUse(this);
    }
}

RetInstruction::~RetInstruction()
{
    if(!operands.empty())
        operands[0]->removeUse(this);
}

void RetInstruction::output() const
{
    if(operands.empty())
    {
        fprintf(yyout, "  ret void\n");
    }
    else
    {
        std::string ret, type;
        ret = operands[0]->toStr();
        type = operands[0]->getType()->toStr();
        fprintf(yyout, "  ret %s %s\n", type.c_str(), ret.c_str());
    }
}

AllocaInstruction::AllocaInstruction(Operand *dst, SymbolEntry *se, BasicBlock *insert_bb) : Instruction(ALLOCA, insert_bb)
{
    operands.push_back(dst);
    dst->setDef(this);
    this->se = se;
}

AllocaInstruction::~AllocaInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
}

void AllocaInstruction::output() const
{
    std::string dst, type;
    dst = operands[0]->toStr();
    type = se->getType()->toStr();
    fprintf(yyout, "  %s = alloca %s, align 4\n", dst.c_str(), type.c_str());
}

GlobalInstruction::GlobalInstruction(Operand *dst, Operand *src,SymbolEntry *se, BasicBlock *insert_bb): Instruction(GLOBAL, insert_bb){
    operands.push_back(dst);
    dst->setDef(this);
    this->se = se;
    this->src=src;
}

void GlobalInstruction::output() const{
    // std::string dst, type;
    // dst = operands[0]->toStr();
    // type = se->getType()->toStr();
    // fprintf(yyout, "%s = global %s %s, align 4\n", dst.c_str(),  type.c_str(), src -> toStr().c_str());
}

LoadInstruction::LoadInstruction(Operand *dst, Operand *src_addr, BasicBlock *insert_bb) : Instruction(LOAD, insert_bb)
{
    operands.push_back(dst);
    operands.push_back(src_addr);
    dst->setDef(this);
    src_addr->addUse(this);
}

LoadInstruction::~LoadInstruction()
{
    operands[0]->setDef(nullptr);
    if(operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
}

void LoadInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string src_type;
    std::string dst_type;
    dst_type = operands[0]->getType()->toStr();
    src_type = operands[1]->getType()->toStr();
    fprintf(yyout, "  %s = load %s, %s %s, align 4\n", dst.c_str(), dst_type.c_str(), src_type.c_str(), src.c_str());
}

StoreInstruction::StoreInstruction(Operand *dst_addr, Operand *src, BasicBlock *insert_bb) : Instruction(STORE, insert_bb)
{
    operands.push_back(dst_addr);
    operands.push_back(src);
    dst_addr->addUse(this);
    src->addUse(this);
}

StoreInstruction::~StoreInstruction()
{
    operands[0]->removeUse(this);
    operands[1]->removeUse(this);
}

void StoreInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string dst_type = operands[0]->getType()->toStr();
    std::string src_type = operands[1]->getType()->toStr();

    fprintf(yyout, "  store %s %s, %s %s, align 4\n", src_type.c_str(), src.c_str(), dst_type.c_str(), dst.c_str());
}

// add 
CallInstruction::CallInstruction(Operand* dst, SymbolEntry* func, 
                    std::vector<Operand*> params, 
                    BasicBlock* insert_bb)
                : Instruction(CALL, insert_bb), func(func), dst(dst) 
{
    operands.push_back(dst);
    if (dst)
    {
        dst->setDef(this);
    }
    for (auto param : params) 
    {
        operands.push_back(param);
        param->addUse(this);
    }
    
}

void CallInstruction::output() const
{
    FunctionType* type = (FunctionType*)(func->getType());
    fprintf(yyout, "  ");
    if (type->getRetType()!=TypeSystem::voidType)//返回值不为空
        fprintf(yyout, "%s = ", operands[0]->toStr().c_str());
    fprintf(yyout, "call %s %s(", type->getRetType()->toStr().c_str(),
            func->toStr().c_str());
    for (long unsigned int i = 1; i < operands.size(); i++) 
    {
        if (i != 1)
            fprintf(yyout, ", ");
        fprintf(yyout, "%s %s", operands[i]->getType()->toStr().c_str(),
                operands[i]->toStr().c_str());
    }
    fprintf(yyout, ")\n");
}

CallInstruction::~CallInstruction()
{
    if (operands[0]) 
    {
        operands[0]->setDef(nullptr);
        if (operands[0]->usersNum() == 0)
            delete operands[0];
    }
    for (long unsigned int i = 1; i < operands.size(); i++)
        operands[i]->removeUse(this);
}

ZextInstruction::ZextInstruction(Operand* dst,Operand* src,BasicBlock *insert_bb):Instruction(ZEXT,insert_bb){
    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->addUse(this);
}

void ZextInstruction::output() const{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    fprintf(yyout,"  %s = zext i1 %s to i32\n",dst.c_str(),src.c_str());
}

XorInstruction::XorInstruction(Operand* dst,Operand* src,BasicBlock *insert_bb):Instruction(XOR,insert_bb){
    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->addUse(this);
}

void XorInstruction::output() const{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    fprintf(yyout,"  %s = xor i1 %s, true\n",dst.c_str(),src.c_str());
}

MachineOperand* Instruction::genMachineOperand(Operand* ope)
{
    auto se = ope->getEntry();
    MachineOperand* mope = nullptr;
    if(se->isConstant())
        mope = new MachineOperand(MachineOperand::IMM, dynamic_cast<ConstantSymbolEntry*>(se)->getValue());
    else if(se->isTemporary())
        mope = new MachineOperand(MachineOperand::VREG, dynamic_cast<TemporarySymbolEntry*>(se)->getLabel());
    else if(se->isVariable())
    {
        auto id_se = dynamic_cast<IdentifierSymbolEntry*>(se);
        if(id_se->isGlobal())
            mope = new MachineOperand(id_se->toStr().c_str());
        else if(id_se->isParam()){
            if (id_se -> getParamNo() < 4){
                mope = new MachineOperand(MachineOperand::REG, id_se -> getParamNo());
            }
            else{
                mope = new MachineOperand(MachineOperand::REG, 3);
            }
        }
        else
            exit(0);
    }
    return mope;
}

MachineOperand* Instruction::genMachineReg(int reg) 
{
    return new MachineOperand(MachineOperand::REG, reg);
}

MachineOperand* Instruction::genMachineVReg() 
{
    return new MachineOperand(MachineOperand::VREG, SymbolTable::getLabel());
}

MachineOperand* Instruction::genMachineImm(int val) 
{
    return new MachineOperand(MachineOperand::IMM, val);
}

MachineOperand* Instruction::genMachineLabel(int block_no)
{
    std::ostringstream buf;
    buf << ".L" << block_no;
    std::string label = buf.str();
    return new MachineOperand(label);
}

void AllocaInstruction::genMachineCode(AsmBuilder* builder)
{
    /* HINT:
    * Allocate stack space for local variabel
    * Store frame offset in symbol entry */
    auto cur_func = builder->getFunction();
    int size = se->getType()->getSize() / 8;//位数转为字节数
    //默认4字节
    if (size <= 0)
        size = 4;
    int offset = cur_func->AllocSpace(size);
    dynamic_cast<TemporarySymbolEntry*>(operands[0]->getEntry())->setOffset(-offset);
}

void LoadInstruction::genMachineCode(AsmBuilder* builder)
{
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;
    // Load global operand
    if(operands[1]->getEntry()->isVariable()
    && dynamic_cast<IdentifierSymbolEntry*>(operands[1]->getEntry())->isGlobal())
    {
        auto dst = genMachineOperand(operands[0]);
        auto internal_reg1 = genMachineVReg();
        auto internal_reg2 = new MachineOperand(*internal_reg1);
        auto src = genMachineOperand(operands[1]);
        // example: load r0, addr_a
        cur_inst = new LoadMInstruction(cur_block, internal_reg1, src);
        cur_block->InsertInst(cur_inst);
        // example: load r1, [r0]
        cur_inst = new LoadMInstruction(cur_block, dst, internal_reg2);
        cur_block->InsertInst(cur_inst);
    }
    // Load local operand
    else if(operands[1]->getEntry()->isTemporary()
    && operands[1]->getDef()
    && operands[1]->getDef()->isAlloc())
    {
        // example: load r1, [r0, #4]
        auto dst = genMachineOperand(operands[0]);
        auto src1 = genMachineReg(11);
        auto src2 = genMachineImm(dynamic_cast<TemporarySymbolEntry*>(operands[1]->getEntry())->getOffset());
        cur_inst = new LoadMInstruction(cur_block, dst, src1, src2);
        cur_block->InsertInst(cur_inst);
    }
    // Load operand from temporary variable
    else
    {
        // example: load r1, [r0]
        auto dst = genMachineOperand(operands[0]);
        auto src = genMachineOperand(operands[1]);
        cur_inst = new LoadMInstruction(cur_block, dst, src);
        cur_block->InsertInst(cur_inst);
    }
}

void StoreInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst = nullptr;
    MachineOperand* dst = genMachineOperand(operands[0]);
    MachineOperand* src = genMachineOperand(operands[1]);

    // store immediate
    if (operands[1]->getEntry()->isConstant()) 
    {
        auto dst1 = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, dst1, src);
        cur_block->InsertInst(cur_inst);
        src = new MachineOperand(*dst1);
    }

    // store global operand
    if(operands[0]->getEntry()->isVariable()
    && dynamic_cast<IdentifierSymbolEntry*>(operands[0]->getEntry())->isGlobal())
    {
        MachineOperand* internal_reg1 = genMachineVReg();
        // store r1, [r0]
        cur_inst = new LoadMInstruction(cur_block, internal_reg1, dst);
        cur_block->InsertInst(cur_inst);
        cur_inst = new StoreMInstruction(cur_block, src, internal_reg1);
        cur_block->InsertInst(cur_inst);
    }
    // store local operand
    else if(operands[0]->getEntry()->isTemporary()
    && operands[0]->getDef()
    && operands[0]->getDef()->isAlloc())
    {
        // example: store r1, [r0, #4]
        //auto dst = genMachineOperand(operands[0]);
        auto src1 = genMachineReg(11);
        int offset = dynamic_cast<TemporarySymbolEntry*>(operands[0]->getEntry()) ->getOffset();
        auto src2 = genMachineImm(offset);
        // if(abs(offset) > 255)   /* 超出寻址范围 */
        // {
        //     /* 先加载到虚拟寄存器 再加载到对应寄存器 */
        //     auto operand = genMachineVReg();
        //     cur_block->InsertInst((new LoadMInstruction(cur_block, operand, src2)));
        //     src2 = operand;
        // }
        //std::cout<<"???"<<std::endl;
        cur_inst = new StoreMInstruction(cur_block, src, src1, src2);
        cur_block->InsertInst(cur_inst);
    }
    // store to pointer
    else if(operands[0]->getType()->isPtr())
    {
        cur_inst = new StoreMInstruction(cur_block, src, dst);
        cur_block->InsertInst(cur_inst);
    }
}

void BinaryInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO:
    // complete other instructions
    auto cur_block = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src1 = genMachineOperand(operands[1]);
    auto src2 = genMachineOperand(operands[2]);
    /* HINT:
    * The source operands of ADD instruction in ir code both can be immediate num.
    * However, it's not allowed in assembly code.
    * So you need to insert LOAD/MOV instrucrion to load immediate num into register.
    * As to other instructions, such as MUL, CMP, you need to deal with this situation, too.*/
    MachineInstruction* cur_inst = nullptr;
    if(src1->isImm())
    {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src1);
        cur_block->InsertInst(cur_inst);
        src1 = new MachineOperand(*internal_reg);
    }
    /*
    合法立即数：
        如果一个立即数小于 0xFF（255）那么直接用前 7～0 位表示即可，此时不用移位，11～8 位的 Rotate_imm 等于 0。
        如果前八位 immed_8 的数值大于 255，那么就看这个数是否能有 immed_8 中的某个数移位 2*Rotate_imm 位形成的。如果能，那么就是合法立即数；否则非法。
    加载不合法立即数：
    只要利用LDR伪指令就可以了，加载任意32位立即数,例如:ldr r1, =12345678
    */
    if (src2->isImm()) {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src2);
        cur_block->InsertInst(cur_inst);
        src2 = new MachineOperand(*internal_reg);
    }
    switch (opcode)     /* enum { SUB, ADD, AND, OR, MUL, DIV, MOD };*/
    {
    case ADD:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD, dst, src1, src2);
        break;    
    case SUB:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::SUB, dst, src1, src2);
        break; 
    case AND:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::AND, dst, src1, src2);
        break; 
    case OR:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::OR, dst, src1, src2);
        break; 
    case MUL:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, dst, src1, src2);
        break;
    case DIV:
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::DIV, dst, src1, src2);
        break;
    case MOD:   /* 不能直接取模，需要经过运算 */
    {
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::DIV, dst, src1, src2);//dst = src1 div src2,
        MachineOperand* tmp = new MachineOperand(*dst);
        src1 = new MachineOperand(*src1);
        src2 = new MachineOperand(*src2);
        cur_block->InsertInst(cur_inst);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, tmp, dst, src2);//tmp = dst mul src2
        cur_block->InsertInst(cur_inst);
        dst = new MachineOperand(*tmp);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::SUB, dst, src1, tmp);//dst = src1 sub tmp
        break;
    }
    default:
        break;
    }
    cur_block->InsertInst(cur_inst);
}

void CmpInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
//    MachineOperand* dst = genMachineOperand(operands[0]);
    MachineOperand* src1 = genMachineOperand(operands[1]);
    MachineOperand* src2 = genMachineOperand(operands[2]);
    MachineInstruction* cur_inst = nullptr;
    // load 立即数
    if(src1->isImm())
    {
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src1);
        cur_block->InsertInst(cur_inst);
        src1 = new MachineOperand(*internal_reg);
    }
    if (src2->isImm() &&
        ((ConstantSymbolEntry*)(operands[2]->getEntry()))->getValue() > 255) {
 
        auto internal_reg = genMachineVReg();
        cur_inst = new LoadMInstruction(cur_block, internal_reg, src2);
        cur_block->InsertInst(cur_inst);
        src2 = new MachineOperand(*internal_reg);
    }

    cur_inst = new CmpMInstruction(cur_block, src1, src2, opcode);
    cur_block->InsertInst(cur_inst);
    builder->setCmpOpcode(opcode);  /* 在builder中记录cmp指令的条件码，用于条件分支指令 */

    switch(opcode)      /*  enum {E, NE, L, GE, G, LE}; */
    {
    case E:
    {
        auto dst = genMachineOperand(operands[0]);
        auto trueOperand = genMachineImm(1);
        auto falseOperand = genMachineImm(0);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, trueOperand, E);
        cur_block->InsertInst(cur_inst);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, falseOperand, NE);
        cur_block->InsertInst(cur_inst);
        break; 
    }
    case NE:
    {
        auto dst = genMachineOperand(operands[0]);
        auto trueOperand = genMachineImm(1);
        auto falseOperand = genMachineImm(0);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, trueOperand, NE);
        cur_block->InsertInst(cur_inst);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, falseOperand, E);
        cur_block->InsertInst(cur_inst);
        break;
    }
    case L:
    {
        auto dst = genMachineOperand(operands[0]);
        auto trueOperand = genMachineImm(1);
        auto falseOperand = genMachineImm(0);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, trueOperand, L);
        cur_block->InsertInst(cur_inst);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, falseOperand, GE);
        cur_block->InsertInst(cur_inst);
        break;
    }
    case GE:
    {
        auto dst = genMachineOperand(operands[0]);
        auto trueOperand = genMachineImm(1);
        auto falseOperand = genMachineImm(0);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, trueOperand, GE);
        cur_block->InsertInst(cur_inst);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, falseOperand, L);
        cur_block->InsertInst(cur_inst);
        break;
    }
    case G:
    {
        auto dst = genMachineOperand(operands[0]);
        auto trueOperand = genMachineImm(1);
        auto falseOperand = genMachineImm(0);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, trueOperand, G);
        cur_block->InsertInst(cur_inst);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, falseOperand, LE);
        cur_block->InsertInst(cur_inst);
        break;
    }
    case LE:
    {
        auto dst = genMachineOperand(operands[0]);
        auto trueOperand = genMachineImm(1);
        auto falseOperand = genMachineImm(0);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, trueOperand, LE);
        cur_block->InsertInst(cur_inst);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, falseOperand, G);
        cur_block->InsertInst(cur_inst);
        break;
    }
    default:
        break;
    }
}

void UncondBrInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    //设置跳转到的目标分支
    MachineOperand* dst = genMachineLabel(branch->getNo());
    auto cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B, dst);
    cur_block->InsertInst(cur_inst);
}

void CondBrInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    MachineOperand* dst = genMachineLabel(true_branch->getNo());
    //std::cout<<builder->getCmpOpcode()<<std::endl;
    auto cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B, dst, builder->getCmpOpcode());
    cur_block->InsertInst(cur_inst);
    
    dst = genMachineLabel(false_branch->getNo());
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::B, dst);
    cur_block->InsertInst(cur_inst);
}

void RetInstruction::genMachineCode(AsmBuilder* builder)
{
    // TODO
    auto cur_block = builder->getBlock();
    //mov保存返回值到r0
    if(!operands.empty()){//有返回值
        auto dst = new MachineOperand(MachineOperand::REG,0);
        auto src = genMachineOperand(operands[0]);
        auto cur_inst =new MovMInstruction(cur_block, MovMInstruction::MOV, dst, src);
        cur_block->InsertInst(cur_inst);
    }
    //调整栈帧 保存寄存器sp
    auto sp = new MachineOperand(MachineOperand::REG, 13);
    auto funcSize =new MachineOperand(MachineOperand::IMM, builder->getFunction()->AllocSpace(0));//开辟函数大小空间
    cur_block->InsertInst(new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,sp, sp, funcSize));
    //bx指令，跳转lr寄存器
    auto lr = new MachineOperand(MachineOperand::REG, 14);
    cur_block->InsertInst(new BranchMInstruction(cur_block, BranchMInstruction::BX, lr));
}

void CallInstruction::genMachineCode(AsmBuilder* builder){
    // TODO
    auto cur_block = builder->getBlock();
    MachineOperand* operand;  
    MachineOperand* dst1;
    MachineInstruction* cur_inst;
    // 在进行函数调用时，对于含参函数，需要使用 R0-R3 寄存器传递参数，
    int paramCount=int(operands.size()-1);//参数数目
    //std::cout<<paramCount<<std::endl;
    if(paramCount<=4&&paramCount>0){
        for(int i=0;i<paramCount;i++){
            dst1=genMachineReg(i);
            operand=genMachineOperand(operands[i+1]);
            //判断是否是立即数
            if (operand->isImm() && operand->getVal()<255){
                cur_inst=new LoadMInstruction(cur_block, dst1, operand);
            } 
            else{
                cur_inst=new MovMInstruction(cur_block, MovMInstruction::MOV, dst1, operand);
            }
            cur_block->InsertInst(cur_inst);
        }
    }
    // 如果参数个数大于四个还需要生成 PUSH 指令来传递参数；
    else if(paramCount>4){
        for(int i=0;i<4;i++){
            dst1=genMachineReg(i);
            operand=genMachineOperand(operands[i+1]);
            //判断是否是立即数
            if (operand->isImm()){
                cur_inst=new LoadMInstruction(cur_block, dst1, operand);
            } 
            else{
                cur_inst=new MovMInstruction(cur_block, MovMInstruction::MOV, dst1, operand);
            }
            cur_block->InsertInst(cur_inst);
        }
        for(int j=4;j<paramCount;j++){
            operand = genMachineOperand(operands[j+1]);
            if(operand->isImm()){
                cur_inst = new LoadMInstruction(cur_block, genMachineVReg(), operand);
                cur_block->InsertInst(cur_inst);
                operand = genMachineVReg();
            }
            std::vector<MachineOperand*> temp;//
            cur_inst=new StackMInstrcuton(cur_block,StackMInstrcuton::PUSH,temp,operand);
            cur_block->InsertInst(cur_inst);
        }
    }
    // 之后生成跳转指令来进入 Callee 函数；
    cur_inst = new BranchMInstruction(cur_block, BranchMInstruction::BL, new MachineOperand(func->toStr().c_str()));
    cur_block->InsertInst(cur_inst);
    // 如果之前通过压栈的方式传递了参数，需要恢复 SP 寄存器；
    if (paramCount > 4) {
        MachineOperand *sp = new MachineOperand(MachineOperand::REG, 13);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,sp, sp, genMachineImm((operands.size() - 5) * 4));
        cur_block->InsertInst(cur_inst);
    }
    // 最后，如果函数执行结果被用到，还需要保存 R0 寄存器中的返回值。
    if (dst) {
        MachineOperand* r0=new MachineOperand(MachineOperand::REG, 0);
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, genMachineOperand(dst), r0);
        cur_block->InsertInst(cur_inst);
    }
}

void ZextInstruction::genMachineCode(AsmBuilder* builder){
    // TODO 补零指令，实际上用赋值的mov指令即可
    auto cur_block = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src = genMachineOperand(operands[1]);
    auto cur_inst =new MovMInstruction(cur_block, MovMInstruction::MOV, dst, src);
    cur_block->InsertInst(cur_inst);
}

void XorInstruction::genMachineCode(AsmBuilder* builder){
    // TODO 异或运算，主要是在非运算时涉及到的int->bool 
    // 与0作比较，相等为1,不等为0（直接进行了非运算）
    auto cur_block = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src = genMachineImm(1);
    auto cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, src, MachineInstruction::EQ);
    cur_block->InsertInst(cur_inst);
    src=genMachineImm(0);
    cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, dst, src, MachineInstruction::NE);
    cur_block->InsertInst(cur_inst);
}

void GlobalInstruction::genMachineCode(AsmBuilder* builder){
    // TODO
}

//用于数组寻址  
GepInstruction::GepInstruction(Operand* dst,
                               Operand* arr,
                               Operand* idx,
                               BasicBlock* insert_bb,
                               bool paramFirst)
    : Instruction(GEP, insert_bb), paramFirst(paramFirst) 
{
    operands.push_back(dst);
    operands.push_back(arr);
    operands.push_back(idx);
    dst->setDef(this);
    arr->addUse(this);
    idx->addUse(this);
    //最终代码生成部分
    first = false;
    init = nullptr;
    last = false;
}

void GepInstruction::output() const {
    Operand* dst = operands[0];
    Operand* arr = operands[1];
    Operand* idx = operands[2];
    std::string arrType = arr->getType()->toStr();
    //是否为函数参数 
    if (paramFirst)
        fprintf(yyout, "  %s = getelementptr inbounds %s, %s %s, i32 %s\n",
                dst->toStr().c_str(),
                arrType.substr(0, arrType.size() - 1).c_str(), 
                arrType.c_str(),
                arr->toStr().c_str(), 
                idx->toStr().c_str());
    else
        fprintf(
            yyout, "  %s = getelementptr inbounds %s, %s %s, i32 0, i32 %s\n",
            dst->toStr().c_str(), 
            arrType.substr(0, arrType.size() - 1).c_str(),
            arrType.c_str(), 
            arr->toStr().c_str(), 
            idx->toStr().c_str());
}

GepInstruction::~GepInstruction() {
    operands[0]->setDef(nullptr);
    if (operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void GepInstruction::genMachineCode(AsmBuilder* builder) {
    auto cur_block = builder->getBlock();
    MachineInstruction* cur_inst;
    auto dst = genMachineOperand(operands[0]);
    auto idx = genMachineOperand(operands[2]);
    
    if(init){
        if(last){//the last dimension
            auto base = genMachineOperand(init);
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::ADD, dst, base, genMachineImm(4));
            cur_block->InsertInst(cur_inst);
        }
        return;
    }
    MachineOperand* base = nullptr;
    int size;
    auto idx1 = genMachineVReg();
    if (idx->isImm()) {
        if (idx->getVal() < 255) {
            cur_inst =
                new MovMInstruction(cur_block, MovMInstruction::MOV, idx1, idx);
        } else {
            cur_inst = new LoadMInstruction(cur_block, idx1, idx);
        }
        idx = new MachineOperand(*idx1);
        cur_block->InsertInst(cur_inst);
    }
    if (paramFirst) {//if the arr is treated as params, need to get it's ptr type.
        size =
            ((PointerType*)(operands[1]->getType()))->getType()->getSize() / 8;
    } else {
        if (first) {// idx is located in the first param of array.
            base = genMachineVReg();
            // z.B. arr[a]...
            if (operands[1]->getEntry()->isVariable() &&
                ((IdentifierSymbolEntry*)(operands[1]->getEntry()))
                    ->isGlobal()) {
                auto src = genMachineOperand(operands[1]);
                cur_inst = new LoadMInstruction(cur_block, base, src);
            } else {//z.B. arr[3]...
                int offset = ((TemporarySymbolEntry*)(operands[1]->getEntry()))
                                 ->getOffset();
                if (abs(offset) < 255) {
                    cur_inst =
                        new MovMInstruction(cur_block, MovMInstruction::MOV,
                                            base, genMachineImm(offset));
                } else {
                    cur_inst = new LoadMInstruction(cur_block, base,
                                                    genMachineImm(offset));
                }
            }
            cur_block->InsertInst(cur_inst);
        }
        ArrayType* type =
            (ArrayType*)(((PointerType*)(operands[1]->getType()))->getType());
        size = type->getElementType()->getSize() / 8;//get element type
    }
    auto size1 = genMachineVReg();
    if (abs(size) < 255) {
        cur_inst = new MovMInstruction(cur_block, MovMInstruction::MOV, size1,
                                       genMachineImm(size));
    } else {
        cur_inst = new LoadMInstruction(cur_block, size1, genMachineImm(size));
    }
    cur_block->InsertInst(cur_inst);
    auto off = genMachineVReg();
    cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::MUL, off,
                                      idx, size1);
    off = new MachineOperand(*off);
    cur_block->InsertInst(cur_inst);
    if (paramFirst || !first) {//it is param or not the first dimension.
        auto arr = genMachineOperand(operands[1]);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,
                                          dst, arr, off);
        cur_block->InsertInst(cur_inst);
    } else {//it is in the func body and the 1st dimension.
        auto addr = genMachineVReg();
        auto base1 = new MachineOperand(*base);
        cur_inst = new BinaryMInstruction(cur_block, BinaryMInstruction::ADD,
                                          addr, base1, off);
        cur_block->InsertInst(cur_inst);
        addr = new MachineOperand(*addr);
        //z.B. arr[a]
        if (operands[1]->getEntry()->isVariable() &&
            ((IdentifierSymbolEntry*)(operands[1]->getEntry()))->isGlobal()) {
            cur_inst =
                new MovMInstruction(cur_block, MovMInstruction::MOV, dst, addr);//dst=addr
        } else {//z.B. arr[4]
            auto fp = genMachineReg(11);
            cur_inst = new BinaryMInstruction(
                cur_block, BinaryMInstruction::ADD, dst, fp, addr);//dst=fp+addr
        }
        cur_block->InsertInst(cur_inst);
    }
}
