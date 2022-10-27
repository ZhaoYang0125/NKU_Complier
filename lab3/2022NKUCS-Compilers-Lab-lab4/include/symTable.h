#include<iostream>
#include<map>
#include<string>

class symEntry{
public:
    int value;
    int lineno;
    int offset;
    symEntry(int value, int lineno, int offset){
        this->value = value;
        this->lineno = lineno;
        this->offset = offset;
    }
};

class symTable{
private:
    std::map<std::string, symEntry*> table;
    symTable* prev;
    int level;

public:
    symTable();
    symTable(symTable* prev);
    bool installID(std::string name, symEntry* entry);
    symEntry* lookUp(std::string name);
    bool setEntryVal(std::string name, int value);
    symTable* getPrev();
};


symTable::symTable() {
    prev = nullptr;
    level = 0;
}

symTable::symTable(symTable* prev){
    this->prev=prev;
    this->level=prev->level+1;
}

symEntry* symTable::lookUp(std::string name){
    symTable* p=this;
    while(p!=nullptr){
        if(p->table.find(name)!=p->table.end()){
            return p->table[name];
        }
        p=p->prev;
    }
    return nullptr;
}

bool symTable::installID(std::string name, symEntry* entry){
    if(this->table.find(name)!=this->table.end()){
        return false;
    }
    table[name]=entry;
    return true;
}

bool symTable::setEntryVal(std::string name, int value){
    symEntry* entry = this->lookUp(name);
    if(entry==nullptr){
        return false;
    }
    entry->value=value;
    return true;
}

symTable* symTable::getPrev(){
    return prev;
}