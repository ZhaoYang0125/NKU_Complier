#include "MachineCode.h"
#include"Type.h"
#include<string.h>
#include<iostream>
extern FILE* yyout;

MachineOperand::MachineOperand(int tp, int val)
{
    this->type = tp;
    if(tp == MachineOperand::IMM)
        this->val = val;
    else 
        this->reg_no = val;
}

MachineOperand::MachineOperand(std::string label)
{
    this->type = MachineOperand::LABEL;
    this->label = label;
}

bool MachineOperand::operator==(const MachineOperand&a) const
{
    if (this->type != a.type)
        return false;
    if (this->type == IMM)
        return this->val == a.val;
    return this->reg_no == a.reg_no;
}

bool MachineOperand::operator<(const MachineOperand&a) const
{
    if(this->type == a.type)
    {
        if(this->type == IMM)
            return this->val < a.val;
        return this->reg_no < a.reg_no;
    }
    return this->type < a.type;

    if (this->type != a.type)
        return false;
    if (this->type == IMM)
        return this->val == a.val;
    return this->reg_no == a.reg_no;
}

void MachineOperand::PrintReg()
{
    switch (reg_no)
    {
    case 11:
        fprintf(yyout, "fp");
        break;
    case 13:
        fprintf(yyout, "sp");
        break;
    case 14:
        fprintf(yyout, "lr");
        break;
    case 15:
        fprintf(yyout, "pc");
        break;
    default:
        fprintf(yyout, "r%d", reg_no);
        break;
    }
}

void MachineOperand::output() 
{
    /* HINT：print operand
    * Example:
    * immediate num 1 -> print #1;
    * register 1 -> print r1;
    * lable addr_a -> print addr_a; */
    switch (this->type)
    {
    case IMM:
        fprintf(yyout, "#%d", this->val);
        break;
    case VREG:
        fprintf(yyout, "v%d", this->reg_no);
        break;
    case REG:
        PrintReg();
        break;
    case LABEL:
        //std::cout<<this->getLabel()<<std::endl;
        if (this->label.substr(0, 2) == ".L")
            fprintf(yyout, "%s", this->label.c_str());
        else if (this->label.substr(0, 1) == "@")//是函数的情况
                fprintf(yyout, "%s", this->label.c_str() + 1);
        else
            fprintf(yyout, "addr_%s", this->label.c_str());
    default:
        break;
    }
}

void MachineInstruction::PrintCond()
{
    // TODO
    //std::cout<<"cond"<<cond<<std::endl;
    switch (cond)
    {
        case LT:
            fprintf(yyout, "lt");
            break;
        case GT:
            fprintf(yyout, "gt");
            break;
        case EQ:
            fprintf(yyout, "eq");
            break;
        case NE:
            fprintf(yyout, "ne");
            break;
        case LE:
            fprintf(yyout, "le");
            break;
        case GE:
            fprintf(yyout, "ge");
            break;
        default:
            break;
    }
}

BinaryMInstruction::BinaryMInstruction(
    MachineBlock* p, int op, 
    MachineOperand* dst, MachineOperand* src1, MachineOperand* src2, 
    int cond)
{
    this->parent = p;
    this->type = MachineInstruction::BINARY;
    this->op = op;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    dst->setParent(this);
    src1->setParent(this);
    src2->setParent(this);
}

void BinaryMInstruction::output() 
{
    // TODO: 
    // Complete other instructions
    switch (this->op)
    {
    case BinaryMInstruction::ADD:
        fprintf(yyout, "\tadd ");
        break;
    case BinaryMInstruction::SUB:
        fprintf(yyout, "\tsub ");
        break;
    case BinaryMInstruction::AND:
        fprintf(yyout, "\tand ");
        break;
    case BinaryMInstruction::OR:
        fprintf(yyout, "\torr ");
        break;
    case BinaryMInstruction::MUL:
        fprintf(yyout, "\tmul ");
        break;
    case BinaryMInstruction::DIV:
        fprintf(yyout, "\tsdiv ");
        break;
    default:
        break;
    }
    this->PrintCond();
    this->def_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[1]->output();
    fprintf(yyout, "\n");
}

LoadMInstruction::LoadMInstruction(MachineBlock* p,
    MachineOperand* dst, MachineOperand* src1, MachineOperand* src2,
    int cond)
{
    this->parent = p;
    this->type = MachineInstruction::LOAD;
    this->op = -1;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src1);
    if (src2)
        this->use_list.push_back(src2);
    dst->setParent(this);
    src1->setParent(this);
    if (src2)
        src2->setParent(this);
}

void LoadMInstruction::output()
{
    fprintf(yyout, "\tldr ");
    this->def_list[0]->output();
    fprintf(yyout, ", ");

    // Load immediate num, eg: ldr r1, =8
    if(this->use_list[0]->isImm())
    {
        fprintf(yyout, "=%d\n", this->use_list[0]->getVal());
        return;
    }

    // Load address
    if(this->use_list[0]->isReg()||this->use_list[0]->isVReg())
        fprintf(yyout, "[");

    this->use_list[0]->output();
    if( this->use_list.size() > 1 )
    {
        fprintf(yyout, ", ");
        this->use_list[1]->output();
    }

    if(this->use_list[0]->isReg()||this->use_list[0]->isVReg())
        fprintf(yyout, "]");
    fprintf(yyout, "\n");
}

StoreMInstruction::StoreMInstruction(MachineBlock* p,
    MachineOperand* src1, MachineOperand* src2, MachineOperand* src3, 
    int cond)
{
    // TODO
    this->parent = p;
    this->type = MachineInstruction::STORE;
    this->op = -1;
    this->cond = cond;
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    if (src3)
        this->use_list.push_back(src3);
    src1->setParent(this);
    src2->setParent(this);
    if (src3)
        src3->setParent(this);
}

void StoreMInstruction::output()
{
    // TODO
    fprintf(yyout, "\tstr ");
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    // store address
    if (this->use_list[1]->isReg() || this->use_list[1]->isVReg())
        fprintf(yyout, "[");
    this->use_list[1]->output();
    if (this->use_list.size() > 2) {
        fprintf(yyout, ", ");
        this->use_list[2]->output();
    }
    if (this->use_list[1]->isReg() || this->use_list[1]->isVReg())
        fprintf(yyout, "]");
    fprintf(yyout, "\n");
}

MovMInstruction::MovMInstruction(MachineBlock* p, int op, 
    MachineOperand* dst, MachineOperand* src,
    int cond)
{
    // TODO 设置一些成员变量的值
    this->parent=p;
    this->op=op;
    this->cond=cond;
    this->type=MachineInstruction::MOV;
    this->addDef(dst);
    this->addUse(src);
    //设置两个操作数的parent
    dst->setParent(this);
    src->setParent(this);
}

void MovMInstruction::output() 
{
    // TODO
    fprintf(yyout, "\tmov");
    PrintCond();
    fprintf(yyout, " ");
    this->def_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[0]->output();
    fprintf(yyout, "\n");
}

BranchMInstruction::BranchMInstruction(MachineBlock* p, int op, 
    MachineOperand* dst, 
    int cond)
{
    // TODO
    //std::cout<<cond<<std::endl;
    this->parent = p;
    this->op = op;
    this->cond = cond;
    this->type = MachineInstruction::BRANCH;
    this->addUse(dst);
    dst->setParent(this);
}

void BranchMInstruction::output()
{
    // TODO
    switch (op) {
        case B:
            fprintf(yyout, "\tb");
            break;
        case BX:
        {
            //
            auto fp = new MachineOperand(MachineOperand::REG, 11);
            auto lr = new MachineOperand(MachineOperand::REG, 14);
            auto cur_inst = new StackMInstrcuton(this->getParent(), StackMInstrcuton::POP, this->getParent()->getParent()->getSavedRegs(), fp, lr);
            cur_inst->output();

            fprintf(yyout, "\tbx");
            break;
        }
        case BL:
            fprintf(yyout, "\tbl");
            break;
        default:
            break;
    }
    PrintCond();
    fprintf(yyout, " ");
    this->use_list[0]->output();
    fprintf(yyout, "\n");
}

CmpMInstruction::CmpMInstruction(MachineBlock* p, 
    MachineOperand* src1, MachineOperand* src2, 
    int cond)
{
    // TODO
    this->parent = p;
    this->type = MachineInstruction::CMP;
    this->op = -1;
    this->cond = cond;
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    src1->setParent(this);
    src2->setParent(this);
}

void CmpMInstruction::output()
{
    // TODO
    // Jsut for reg alloca test
    // delete it after test
    fprintf(yyout, "\tcmp ");
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[1]->output();
    fprintf(yyout, "\n");
}

StackMInstrcuton::StackMInstrcuton(MachineBlock* p, int op, 
    std::vector<MachineOperand*> regs,
    MachineOperand* src1,
    MachineOperand* src2,
    int cond)
{
    // TODO
    this->parent=p;
    this->op=op;
    this->cond=cond;
    this->type=MachineInstruction::STACK;
    if(regs.size()!=0){
        for(auto reg:regs){
            this->addUse(reg);
        }
    }
    if (src1 != nullptr) 
    {
        this->use_list.push_back(src1);
        src1->setParent(this);
    }
    if (src2 != nullptr) 
    {
        this->use_list.push_back(src2);
        src2->setParent(this);
    }
}

void StackMInstrcuton::output()
{
    // TODO
    switch (op) 
    {
        case PUSH:
            fprintf(yyout, "\tpush ");
            break;
        case POP:
            fprintf(yyout, "\tpop ");
            break;
    }
    fprintf(yyout, "{");
    this->use_list[0]->output();
    for(long unsigned int i=1;i<use_list.size();i++){
        fprintf(yyout, ", ");
        this->use_list[i]->output();
    }
    fprintf(yyout, "}\n");
}

MachineFunction::MachineFunction(MachineUnit* p, SymbolEntry* sym_ptr) 
{ 
    this->parent = p; 
    this->sym_ptr = sym_ptr; 
    this->stack_size = 0;
};

void MachineBlock::output()
{
    fprintf(yyout, ".L%d:\n", this->no);
    for(auto iter : inst_list)
        iter->output();
}
//获得整数vector代表的寄存器信息
std::vector<MachineOperand*> MachineFunction::getSavedRegs() 
{
    std::vector<MachineOperand*> regs;
    for (auto i: saved_regs) {
        MachineOperand * reg = new MachineOperand(MachineOperand::REG, i);
        regs.push_back(reg);
    }
    return regs;
}

void MachineFunction::output()
{
    char *func_name=new char[strlen(this->sym_ptr->toStr().c_str())];// = this->sym_ptr->toStr().c_str()+1;
    strcpy(func_name,this->sym_ptr->toStr().c_str()+1);
    fprintf(yyout, "\t.global %s\n", func_name);
    fprintf(yyout, "\t.type %s , %%function\n", func_name);
    fprintf(yyout, "%s:\n", func_name);
    // TODO
    /* Hint:
    *  1. Save fp
    *  2. fp = sp
    *  3. Save callee saved register
    *  4. Allocate stack space for local variable */
    
    // Traverse all the block in block_list to print assembly code.
    MachineOperand *fp = new MachineOperand(MachineOperand::REG, 11);
    MachineOperand *sp = new MachineOperand(MachineOperand::REG, 13);
    MachineOperand *lr = new MachineOperand(MachineOperand::REG, 14);
    MachineInstruction* cur_inst;
    cur_inst=new StackMInstrcuton(nullptr,StackMInstrcuton::PUSH,this->getSavedRegs(),fp,lr);
    cur_inst->output();
    cur_inst=new MovMInstruction(nullptr, MovMInstruction::MOV, fp, sp);
    cur_inst->output();
    cur_inst=new BinaryMInstruction(nullptr, BinaryMInstruction::SUB, sp, sp, new MachineOperand(MachineOperand::IMM, AllocSpace(0)));
    cur_inst->output();

    // int offset=AllocSpace(0);
    // if(offset!=0){
    //     MachineOperand* size = new MachineOperand(MachineOperand::IMM, offset);
    //     if (offset < -255 || offset > 255) {//偏移量太大
    //         MachineOperand* r4 = new MachineOperand(MachineOperand::REG, 4);
    //         cur_inst=new LoadMInstruction(nullptr, r4, size);
    //         cur_inst->output();
    //         cur_inst=new BinaryMInstruction(nullptr, BinaryMInstruction::SUB, sp, sp,r4);
    //         cur_inst->output();
    //     } else {
    //         cur_inst=new BinaryMInstruction(nullptr, BinaryMInstruction::SUB, sp, sp, size);
    //         cur_inst->output();
    //     }
    // }
    for(auto iter : block_list)
        iter->output();
    fprintf(yyout, "\n");
}

void MachineUnit::PrintGlobalDecl()
{
    // TODO:
    // You need to print global variable/const declarition code;
    if(!global_list.empty()){
        fprintf(yyout,"\t.data\n");
    }
    std::vector<IdentifierSymbolEntry*> Global_list;
    std::vector<IdentifierSymbolEntry*> Global_array_list;
    for(auto global:global_list){
        IdentifierSymbolEntry* se = (IdentifierSymbolEntry*)global;
        if(se->getConst()){//constId
            Global_list.push_back(se);
        }
        else if(se->isAllZero()){
            //std::cout<<"allzero"<<std::endl;
            Global_array_list.push_back(se);
        }
        else{//变量
            fprintf(yyout, ".global %s\n", se->toStr().c_str());
            fprintf(yyout, "\t.size %s, %d\n", se->toStr().c_str(), se->getType()->size/8);
            fprintf(yyout, "%s:\n", se->toStr().c_str());
            if(!se->getType()->isArray())
            {
                fprintf(yyout, "\t.word %d\n", se->getValue());
            }
            else
            {
                int n = se->getType()->getSize() / 32;
                Type* arrTy = ((ArrayType*)(se->getType()))->getElementType();

                while (!arrTy->isInt()) {
                    arrTy = ((ArrayType*)(arrTy))->getElementType();
                } 

                int* p = se->getArrayValue();
                for (int i = 0; i < n; i++) {
                    fprintf(yyout, "\t.word %d\n", (int)p[i]);
                }
            }
        }
    }
    if(Global_list.empty()==false){
        fprintf(yyout, ".section .rodata\n");
        for(auto con:Global_list){
            IdentifierSymbolEntry* se=con;
            fprintf(yyout, ".global %s\n", se->toStr().c_str());
            fprintf(yyout, "\t.size %s, %d\n", se->toStr().c_str(), se->getType()->size/ 8);
            fprintf(yyout, "%s:\n", se->toStr().c_str());
            if(!se->getType()->isArray())
            {
                fprintf(yyout, "\t.word %d\n", se->getValue());
            }
            else
            {
                int n = se->getType()->getSize() / 32;
                Type* arrTy = ((ArrayType*)(se->getType()))->getElementType();

                while (!arrTy->isInt()) {
                    arrTy = ((ArrayType*)(arrTy))->getElementType();
                } 

                int* p = se->getArrayValue();
                for (int i = 0; i < n; i++) {
                    fprintf(yyout, "\t.word %d\n", (int)p[i]);
                }
            }
        }
    }
    if(Global_array_list.empty()==false){
        for (auto a : Global_array_list) {
            IdentifierSymbolEntry* se = a;
            if (se->getType()->isArray()) {
                fprintf(yyout, "\t.comm %s, %d, 4\n", se->toStr().c_str(),se->getType()->getSize() / 8);
            }
        }
    }
}

void MachineUnit::PrintGlobal(){
    for (auto s : global_list) {
        IdentifierSymbolEntry* se = (IdentifierSymbolEntry*)s;
        fprintf(yyout, "addr_%s:\n", se->toStr().c_str());
        fprintf(yyout, "\t.word %s\n", se->toStr().c_str());
    }
}

void MachineUnit::output()
{
    // TODO
    /* Hint:
    * 1. You need to print global variable/const declarition code;
    * 2. Traverse all the function in func_list to print assembly code;
    * 3. Don't forget print bridge label at the end of assembly code!! */
    fprintf(yyout, "\t.arch armv8-a\n");
    fprintf(yyout, "\t.arch_extension crc\n");
    fprintf(yyout, "\t.arm\n");
    PrintGlobalDecl();
    fprintf(yyout, "\t.text\n");
    for(auto iter : func_list)
        iter->output();
    PrintGlobal();
}
