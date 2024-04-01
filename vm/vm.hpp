#pragma once

#include "../parser/class_file.hpp"
#include "../parser/tables.hpp"
#include "value.hpp"

struct VM;

struct Frame {
    Method* method;
    size_t pc;
    vector<Value*> locals;
    vector<Value*> stack;
    Frame(Method* method);
    Frame(Method* method, vector<Value*> args);
    void execute_frame(VM& vm);
    int execute_command(VM& vm);
    unsigned char getCode();
    Method* getMethod(VM& vm, unsigned short ref);
    ~Frame() {
        for (size_t i = 0; i < locals.size(); i++) {
            delete locals[i];
        }
        for (size_t i = 0; i < stack.size(); i++) {
            delete stack[i];
        }
    }
};

struct VM {
    ClassFile* mainclass;
    vector<Frame*> call_stack;
    VM(ClassFile* cf): mainclass(cf) {}
    void execute_vm();
};
