#pragma once

#include "../parser/class_file.hpp"
#include "../parser/tables.hpp"
#include "value.hpp"

struct Frame {
    Method method;
    size_t pc;
    vector<Value*> locals;
    vector<Value*> stack;
    Frame(Method method);
    Frame(Method method, vector<Value*> args);
};

struct VM {
    ClassFile* mainclass;
    vector<Frame> call_stack;
    VM(ClassFile* cf): mainclass(cf) {}
    void execute() {
        Method* Main = mainclass->main;
        for (size_t i = 0; i < Main->code.code.size(); i++) {
            cout << (int)Main->code.code[i] << " ";
        }
        cout << endl;
    }
};
