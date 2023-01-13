#include "SymbolTable.h"
#include"Type.h"
#include <iostream>
#include <sstream>

SymbolEntry::SymbolEntry(Type *type, int kind) 
{
    this->type = type;
    this->kind = kind;
    this->next=nullptr;
}

bool SymbolEntry::setNext(SymbolEntry* se)
{
    SymbolEntry* s = this;
    while (s->getNext()) 
    {
        s = s->getNext();
    }
    if (s == this)
    {
        this->next = se;
    }
    else 
    {
        s->setNext(se);
    }
    return true;
}

ConstantSymbolEntry::ConstantSymbolEntry(Type *type, int value) : SymbolEntry(type, SymbolEntry::CONSTANT)
{
    this->value = value;
}

std::string ConstantSymbolEntry::toStr()
{
    std::ostringstream buffer;
    buffer << value;
    return buffer.str();
}

IdentifierSymbolEntry::IdentifierSymbolEntry(Type *type, std::string name, int scope,int paramNo) : SymbolEntry(type, SymbolEntry::VARIABLE),
 name(name),paramNo(paramNo)
{
    this->scope = scope;
    addr = nullptr;
    this->constant=false;
    this->inited=false;
}

bool IdentifierSymbolEntry::setValue(int value)
{
    this->value=value;
    return true;
}

int IdentifierSymbolEntry::getValue()
{
    return this->value;
}

std::string IdentifierSymbolEntry::toStr()
{
    if(type->isFunc())
        return '@'+name;
    return name;
}

//
void IdentifierSymbolEntry::setConst()
{
    this->constant=true;
}


TemporarySymbolEntry::TemporarySymbolEntry(Type *type, int label) : SymbolEntry(type, SymbolEntry::TEMPORARY)
{
    this->label = label;
}

TemporarySymbolEntry::TemporarySymbolEntry(Type *type, int label,int no) : SymbolEntry(type, SymbolEntry::TEMPORARY)
{
    this->label = label;
    this->para_No=no;
}

std::string TemporarySymbolEntry::toStr()
{
    std::ostringstream buffer;
    buffer << "%t" << label;
    return buffer.str();
}

SymbolTable::SymbolTable()
{
    prev = nullptr;
    level = 0;
}

SymbolTable::SymbolTable(SymbolTable *prev)
{
    this->prev = prev;
    this->level = prev->level + 1;
}

/*
    Description: lookup the symbol entry of an identifier in the symbol table
    Parameters: 
        name: identifier name
    Return: pointer to the symbol entry of the identifier

    hint:
    1. The symbol table is a stack. The top of the stack contains symbol entries in the current scope.
    2. Search the entry in the current symbol table at first.
    3. If it's not in the current table, search it in previous ones(along the 'prev' link).
    4. If you find the entry, return it.
    5. If you can't find it in all symbol tables, return nullptr.
*/
SymbolEntry* SymbolTable::lookup(std::string name)
{
    SymbolTable* table = this;
    while(table!=nullptr)
    {
        if(table->symbolTable.find(name)!=table->symbolTable.end())
        {
            return table->symbolTable[name];
        }
        else
        {
            table=table->prev;
            //return nullptr;
        }
    }
    return nullptr;
}

SymbolEntry* SymbolTable::lookup_cur_block(std::string name)
{
    SymbolTable* table = this;
    while(table!=nullptr)
    {
        if(table->symbolTable.find(name)!=table->symbolTable.end())
        {
            return table->symbolTable[name];
        }
        else
        {
            //table=table->prev;
            return nullptr;
        }
    }
    return nullptr;
}

// install the entry into current symbol table.
void SymbolTable::install(std::string name, SymbolEntry* entry)
{
    symbolTable[name] = entry;
}

int SymbolTable::counter = 0;
static SymbolTable t;
SymbolTable *identifiers = &t;
SymbolTable *globals = &t;
