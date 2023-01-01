#include <algorithm>
#include "LinearScan.h"
#include "MachineCode.h"
#include "LiveVariableAnalysis.h"

LinearScan::LinearScan(MachineUnit *unit)
{
    this->unit = unit;
    for (int i = 4; i < 11; i++)
        regs.push_back(i);
}

void LinearScan::allocateRegisters()
{
    for (auto &f : unit->getFuncs())
    {
        func = f;
        bool success;
        success = false;
        while (!success)        // repeat until all vregs can be mapped
        {
            computeLiveIntervals();
            success = linearScanRegisterAllocation();
            if (success)        // all vregs can be mapped to real regs
                modifyCode();
            else                // spill vregs that can't be mapped to real regs
                genSpillCode();
        }
    }
}

void LinearScan::makeDuChains()
{
    LiveVariableAnalysis lva;
    lva.pass(func);
    du_chains.clear();
    int i = 0;
    std::map<MachineOperand, std::set<MachineOperand *>> liveVar;
    for (auto &bb : func->getBlocks())
    {
        liveVar.clear();
        for (auto &t : bb->getLiveOut())
            liveVar[*t].insert(t);
        int no;
        no = i = bb->getInsts().size() + i;
        for (auto inst = bb->getInsts().rbegin(); inst != bb->getInsts().rend(); inst++)
        {
            (*inst)->setNo(no--);
            for (auto &def : (*inst)->getDef())
            {
                if (def->isVReg())
                {
                    auto &uses = liveVar[*def];
                    du_chains[def].insert(uses.begin(), uses.end());
                    auto &kill = lva.getAllUses()[*def];
                    std::set<MachineOperand *> res;
                    set_difference(uses.begin(), uses.end(), kill.begin(), kill.end(), inserter(res, res.end()));
                    liveVar[*def] = res;
                }
            }
            for (auto &use : (*inst)->getUse())
            {
                if (use->isVReg())
                    liveVar[*use].insert(use);
            }
        }
    }
}

void LinearScan::computeLiveIntervals()
{
    makeDuChains();
    intervals.clear();
    for (auto &du_chain : du_chains)
    {
        int t = -1;
        for (auto &use : du_chain.second)
            t = std::max(t, use->getParent()->getNo());
        Interval *interval = new Interval({du_chain.first->getParent()->getNo(), t, false, 0, 0, {du_chain.first}, du_chain.second});
        intervals.push_back(interval);
    }
    for (auto& interval : intervals) {
        auto uses = interval->uses;
        auto begin = interval->start;
        auto end = interval->end;
        for (auto block : func->getBlocks()) {
            auto liveIn = block->getLiveIn();
            auto liveOut = block->getLiveOut();
            bool in = false;
            bool out = false;
            for (auto use : uses)
                if (liveIn.count(use)) {
                    in = true;
                    break;
                }
            for (auto use : uses)
                if (liveOut.count(use)) {
                    out = true;
                    break;
                }
            if (in && out) {
                begin = std::min(begin, (*(block->begin()))->getNo());
                end = std::max(end, (*(block->rbegin()))->getNo());
            } else if (!in && out) {
                for (auto i : block->getInsts())
                    if (i->getDef().size() > 0 &&
                        i->getDef()[0] == *(uses.begin())) {
                        begin = std::min(begin, i->getNo());
                        break;
                    }
                end = std::max(end, (*(block->rbegin()))->getNo());
            } else if (in && !out) {
                begin = std::min(begin, (*(block->begin()))->getNo());
                int temp = 0;
                for (auto use : uses)
                    if (use->getParent()->getParent() == block)
                        temp = std::max(temp, use->getParent()->getNo());
                end = std::max(temp, end);
            }
        }
        interval->start = begin;
        interval->end = end;
    }
    bool change;
    change = true;
    while (change)
    {
        change = false;
        std::vector<Interval *> t(intervals.begin(), intervals.end());
        for (size_t i = 0; i < t.size(); i++)
            for (size_t j = i + 1; j < t.size(); j++)
            {
                Interval *w1 = t[i];
                Interval *w2 = t[j];
                if (**w1->defs.begin() == **w2->defs.begin())
                {
                    std::set<MachineOperand *> temp;
                    set_intersection(w1->uses.begin(), w1->uses.end(), w2->uses.begin(), w2->uses.end(), inserter(temp, temp.end()));
                    if (!temp.empty())
                    {
                        change = true;
                        w1->defs.insert(w2->defs.begin(), w2->defs.end());
                        w1->uses.insert(w2->uses.begin(), w2->uses.end());
                        // w1->start = std::min(w1->start, w2->start);
                        // w1->end = std::max(w1->end, w2->end);
                        auto w1Min = std::min(w1->start, w1->end);
                        auto w1Max = std::max(w1->start, w1->end);
                        auto w2Min = std::min(w2->start, w2->end);
                        auto w2Max = std::max(w2->start, w2->end);
                        w1->start = std::min(w1Min, w2Min);
                        w1->end = std::max(w1Max, w2Max);
                        auto it = std::find(intervals.begin(), intervals.end(), w2);
                        if (it != intervals.end())
                            intervals.erase(it);
                    }
                }
            }
    }
    sort(intervals.begin(), intervals.end(), compareStart);
}

bool LinearScan::linearScanRegisterAllocation()
{
    // Todo
    /* 标志位，是否分配成功 */
    bool flag = true;
    /* 初始化 */
    actives.clear();
    regs.clear();
    for(int i=4;i<=10;i++)
        regs.push_back(i);
    
    /* 遍历intervals 列表进行寄存器分配 */
    for(auto i = intervals.begin(); i!=intervals.end(); i++)
    {
        /*
        遍历active 列表，
        看该列表中是否存在结束时间早于unhandled interval 的interval，
        若有，则回收其寄存器
        */
        expireOldIntervals(*i);
        /* 如果当前所有物理寄存器都被占用，需要进行寄存器溢出操作 */
        if(regs.empty())
        {
            spillAtInterval(*i);
            flag = false;
        }
        else
        {
            /* 分配寄存器 */
            (*i)->rreg = regs.front();
            regs.erase(regs.begin());
            /* 插入到active 列表中 */
            actives.push_back((*i));
            //对活跃区间按照结束时间升序排序
            sort(actives.begin(), actives.end(), compareEnd);
        }

    }
    return flag;
}

void LinearScan::modifyCode()
{
    for (auto &interval : intervals)
    {
        func->addSavedRegs(interval->rreg);
        for (auto def : interval->defs)
            def->setReg(interval->rreg);
        for (auto use : interval->uses)
            use->setReg(interval->rreg);
    }
}

/*
1. 为其在栈内分配空间，获取当前在栈内相对FP 的偏移；
2. 遍历其USE 指令的列表，在USE 指令前插入LoadMInstruction，将其从栈内加载到目前的虚拟寄存器中;
3. 遍历其DEF 指令的列表，在DEF 指令后插入StoreMInstruction，将其从目前的虚拟寄存器中存到栈内;
插入结束后，会迭代进行以上过程，重新活跃变量分析，进行寄存器分配，直至没有溢出情况出
现。
*/
void LinearScan::genSpillCode()
{
    for(auto &interval:intervals)
    {
        if(!interval->spill)
            continue;
        // TODO
        /* HINT:
         * The vreg should be spilled to memory.
         * 1. insert ldr inst before the use of vreg
         * 2. insert str inst after the def of vreg
         */ 

        /* 获取当前在栈内相对FP的偏移 */
        interval->disp = -func->AllocSpace(4);
        //获取偏移和FP寄存器的值
        auto off = new MachineOperand(MachineOperand::IMM, interval->disp);
        auto fp = new MachineOperand(MachineOperand::REG, 11);
        /* 遍历其USE 指令的列表 */
        for (auto use : interval->uses) {   /* 在USE 指令前插入LoadMInstruction，将其从栈内加载到目前的虚拟寄存器中; */
            auto temp = new MachineOperand(*use);
            MachineOperand* operand = nullptr;
            /* 首先判断当前数据地址是否超过寻址空间 */
            if (interval->disp > 255 || interval->disp < -255) {    /* 超出寻址空间 不能直接加载 要分两步 */
                /* 第一步加载到虚拟寄存器 ldr v1,off */
                operand = new MachineOperand(MachineOperand::VREG, SymbolTable::getLabel());
                auto inst = new LoadMInstruction(use->getParent()->getParent(), operand, off);
                /* 在USE指令前插入Load指令，加载到虚拟寄存器 */
                auto& instructions = use->getParent()->getParent()->getInsts();
                auto it = std::find(instructions.begin(), instructions.end(), use->getParent());
                instructions.insert(it, inst);
                /* 第二步, ldr r0,[fp,v1] */
                inst = new LoadMInstruction(use->getParent()->getParent(), temp, fp, new MachineOperand(*operand));
                /* 在USE指令前插入Load指令 */
                instructions.insert(it, inst);
            }
            else {
                /* 未超过寻址空间，直接从fp-off的地方加载 */
                auto inst = new LoadMInstruction(use->getParent()->getParent(), temp, fp, off);
                /* 在USE 指令前插入Load指令 */
                auto& instructions = use->getParent()->getParent()->getInsts();
                auto it = std::find(instructions.begin(), instructions.end(), use->getParent());
                instructions.insert(it, inst);
            }
        }
        /* 遍历其DEF 指令的列表，在DEF 指令后插入StoreMInstruction，将其从目前的虚拟寄存器中存到栈内; */
        for (auto def : interval->defs) {
            auto temp = new MachineOperand(*def);
            MachineOperand* operand = nullptr;
            MachineInstruction *inst = nullptr;
            /* 同样考虑寻址空间 */
            if (interval->disp > 255 || interval->disp < -255) {
                operand = new MachineOperand(MachineOperand::VREG, SymbolTable::getLabel());
                /* 第一步， 加载到虚拟寄存器 */
                inst = new LoadMInstruction(def->getParent()->getParent(), operand, off);
                auto& instructions = def->getParent()->getParent()->getInsts();
                auto it = std::find(instructions.begin(), instructions.end(), def->getParent());
                instructions.insert(++it, inst);
                /* 第二步， 在DEF 指令后插入Store指令 */
                inst = new StoreMInstruction(def->getParent()->getParent(), temp, fp, new MachineOperand(*operand));
                instructions.insert(++it, inst);
            }
            else{
                /*  直接在DEF 指令后插入Store指令 */
                inst = new StoreMInstruction(def->getParent()->getParent(), temp, fp, off);
                auto& instructions = def->getParent()->getParent()->getInsts();
                auto it = std::find(instructions.begin(), instructions.end(), def->getParent());
                instructions.insert(++it, inst);
            }
        }
    }
}

/*
遍历active 列表，
看该列表中是否存在结束时间早于unhandled interval 的interval（即与当前unhandled interval 的活跃区间不冲突），
若有，则说明此时为其分配的物理寄存器可以回收，可以用于后续的分配，需要将其在active 列表删除；
*/
void LinearScan::expireOldIntervals(Interval *interval)
{
    // Todo
    /* actives 已按照结束时间升序排列 */
    for(auto i=actives.begin(); i!=actives.end();)
    {
        if((*i)->end >= interval->start)    /* 直到结束晚于开始时间，可以返回了 */
            return;
        /* 回收寄存器 */
        regs.push_back((*i)->rreg);
        sort(regs.begin(), regs.end());
        /* 从actives删除 */
        i = actives.erase(find(actives.begin(), actives.end(), *i));

        /* 未考虑浮点寄存器的情况 */
    }
}

/*
具体为在active列表中最后一个interval 和当前unhandled interval 中选择一个interval 将其溢出到栈中，
选择策略就是看谁的活跃区间结束时间更晚，如果是unhandled interval 的结束时间更晚，
只需要置位其spill 标志位即可，如果是active 列表中的interval 结束时间更晚，需要置位
其spill 标志位，并将其占用的寄存器分配给unhandled interval，再按照unhandled interval
活跃区间结束位置，将其插入到active 列表中。
*/
void LinearScan::spillAtInterval(Interval *interval)
{
    // Todo
    /* 在active列表中最后一个interval 和当前unhandled interval 中选择一个interval 将其溢出到栈中 */
    auto activeInterval = actives.back();
    /* 选择策略就是看谁的活跃区间结束时间更晚 */
    if (activeInterval->end > interval->end) {   /* 如果是active 列表中的interval 结束时间更晚 */
        /* 需要置位其spill 标志位 */
        activeInterval->spill = true;
        /* 并将其占用的寄存器分配给unhandled interval */
        interval->rreg = activeInterval->rreg;
        //额外添加 处理寄存器
        func->addSavedRegs(interval->rreg);
        /* 将unhandled interval 插入到 active 列表中。*/
        actives.push_back(interval);
        /* 插入后再次按照结束时间对活跃区间进行排序 */
        sort(actives.begin(), actives.end(), compareEnd);
    } else {
        /* 如果是unhandled interval 的结束时间更晚，
            只需要置位其spill 标志位即可 */
        interval->spill = true;
    }
}

bool LinearScan::compareStart(Interval *a, Interval *b)
{
    return a->start < b->start;
}

bool LinearScan::compareEnd(Interval *a, Interval *b)
{
    return a->end < b->end;
}
