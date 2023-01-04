#ifndef __TYPE_H__
#define __TYPE_H__
#include <vector>
#include <string>

class Type
{
private:
    int kind;
protected:
    enum {INT, VOID, FUNC, PTR, BOOL, ARRAY};
public:
    int size;
    Type(int kind,int size) : kind(kind) ,size(size){};
    virtual ~Type() {};
    virtual std::string toStr() = 0;
    bool isInt() const {return kind == INT;};
    bool isVoid() const {return kind == VOID;};
    bool isFunc() const {return kind == FUNC;};
    bool isBool() const {return kind == BOOL;};
    bool isPtr() const {return kind == PTR;};
    bool isArray() const {return kind == ARRAY;};
    bool equal(Type *t) { return this->kind == t->kind;}
    int getSize(){ return size;};
};

class IntType : public Type
{
private:
    //int size;
    bool constant;
public:
    IntType(int size, bool constant = false) : Type(Type::INT,size), constant(constant){};
    std::string toStr();
    bool isConstant(){ return constant;}
};

class VoidType : public Type
{
public:
    VoidType() : Type(Type::VOID,0){};
    std::string toStr();
};

class FunctionType : public Type
{
private:
    Type *returnType;
public:
    std::vector<Type*> paramsType;
    FunctionType(Type* returnType, std::vector<Type*> paramsType) : 
    Type(Type::FUNC,0), returnType(returnType), paramsType(paramsType){};
    Type* getRetType() {return returnType;};
    std::string toStr();
};

class PointerType : public Type
{
private:
    Type *valueType;
public:
    PointerType(Type* valueType) : Type(Type::PTR,0) {this->valueType = valueType;};
    std::string toStr();
    Type* getType() const { return valueType; };
};

class ArrayType : public Type {
private:
    Type* elementType;
    Type* arrayType = nullptr;
    int length;
    bool constant;

public:
    ArrayType(Type* elementType, int length, bool constant = false)
        : Type(Type::ARRAY, ((IntType*)elementType)->getSize()), 
        elementType(elementType), 
        length(length), 
        constant(constant) 
    {
        size = ((IntType*)elementType)->getSize() * length;
    };
    std::string toStr();
    int getLength() const { return length; };
    Type* getElementType() const { return elementType; };
    void setArrayType(Type* arrayType) { this->arrayType = arrayType; };
    bool isConst() const { return constant; };
    Type* getArrayType() const { return arrayType; };
};

class TypeSystem
{
private:
    static IntType commonInt;
    static IntType commonBool;
    static VoidType commonVoid;
public:
    static Type *intType;
    static Type *voidType;
    static Type *boolType;
};

#endif
