#include "opcodes.hpp"
#include "value.hpp"
#include "vm.hpp"

Frame::Frame(Method* method): pc(0) {
    this->method = method;
}

Frame::Frame(Method* method, vector<Value*> args): Frame(method) {
    locals = args;
}

void VM::execute_vm() {
    Method* main = mainclass->main;
    Frame* main_frame = new Frame(main);
    call_stack.push_back(main_frame);
    main_frame->execute_frame(*this);
}

void Frame::execute_frame(VM& vm) {
    int ret = 1;
    while (ret && pc < method->code.code.size()) {
        ret = execute_command(vm);
    }
    vm.call_stack.pop_back();
    delete this;
}

unsigned char Frame::getCode() {
    return method->code.code[pc];
}

Method* Frame::getMethod(VM& vm, unsigned short ref) {
    auto* mr = dynamic_cast<C_Methodref*>(vm.mainclass->constant_pool[ref - 1]);
    auto* nt = dynamic_cast<C_NameAndType*>(vm.mainclass->constant_pool[mr->name_and_type - 1]);
    for (size_t i = 0; i < vm.mainclass->methods.size(); i++) {
        if (vm.mainclass->methods.at(i)->name_index == nt->name_index) return vm.mainclass->methods.at(i);
    }
    return nullptr;
}

Value* copyStack(Value* val) {
    switch (val->type) {
        case ValueType::Int: {
            Int* new_int = new Int(to_int(val)->value);    
            return new_int;
        }
        default:
            cout << "Not yet implemented " << (int)val->type << endl;
            exit(1);
    }
}

int Frame::execute_command(VM& vm) {
    unsigned char current_command = getCode();
    #ifdef DEBUG
    cout << "pc: " << pc << " | command: " << int(current_command) << endl;
    #endif
    switch (current_command) {
        case OpCode::nop:
            break;
        case OpCode::iconst_m1:
            stack.push_back(new Int(-1));
            stack[stack.size() - 1]->type = ValueType::Int;
            break;
        case OpCode::iconst_0:
            stack.push_back(new Int(0));
            stack[stack.size() - 1]->type = ValueType::Int;
            break;
        case OpCode::iconst_1:
            stack.push_back(new Int(1));
            stack[stack.size() - 1]->type = ValueType::Int;
            break;
        case OpCode::iconst_2:
            stack.push_back(new Int(2));
            stack[stack.size() - 1]->type = ValueType::Int;
            break;
        case OpCode::iconst_3:
            stack.push_back(new Int(3));
            stack[stack.size() - 1]->type = ValueType::Int;
            break;
        case OpCode::iconst_4:
            stack.push_back(new Int(4));
            stack[stack.size() - 1]->type = ValueType::Int;
            break;
        case OpCode::iconst_5:
            stack.push_back(new Int(5));
            stack[stack.size() - 1]->type = ValueType::Int;
            break;
        case OpCode::bipush: {
            pc++;       
            unsigned char to_push = getCode();
            stack.push_back(new Int(to_push));
            stack[stack.size() - 1]->type = ValueType::Int;
            break;
        }
        case OpCode::pop: {
            Int* res = to_int(stack[stack.size() - 1]);

            #ifdef DEBUG
            cout << "Result: " << res->value << endl;
            #endif

            #ifdef MILESTONE1
            exit(res->value);
            #endif

            delete stack[stack.size() - 1];
            stack.pop_back();
            break;
        }
        case OpCode::iload_0: {
            Int* new_int = new Int(to_int(locals[0])->value);
            stack.push_back(new_int);
            break;
        }
        case OpCode::iload_1: {
            Int* new_int = new Int(to_int(locals[1])->value);
            stack.push_back(new_int);
            break;
        }
        case OpCode::istore_0: {
            Int* store = to_int(stack[stack.size() - 1]);
            stack.pop_back();
            if (locals.size() != 0) {
                delete locals[0]; 
            }
            locals[0] = store;
            break;
        }
        case OpCode::istore_1: {
            if (locals.size() < 2) {
                locals.resize(2, nullptr);
            } else {
                delete locals[1];
            }
            locals[1] = to_int(stack[stack.size() - 1]);
            stack.pop_back();
            break;
        }
        case OpCode::iinc: {
            ++pc;
            unsigned char index = getCode();       
            ++pc;
            unsigned char constant = getCode();
            Int* loc = to_int(locals[index]);
            loc->value += constant;
            break;
        }
        case OpCode::isub: {
            Int* sub_arg2 = to_int(stack[stack.size() - 1]);
            Int* sub_arg1 = to_int(stack[stack.size() - 2]);
            stack.pop_back();
            stack.pop_back();
            Int* sub_res = new Int(sub_arg1->value - sub_arg2->value);
            delete sub_arg1;
            delete sub_arg2;
            stack.push_back(sub_res);
            stack[stack.size() - 1]->type = ValueType::Int;
            break;
        }
        case OpCode::imul: {
            Int* sub_arg2 = to_int(stack[stack.size() - 1]);
            Int* sub_arg1 = to_int(stack[stack.size() - 2]);
            stack.pop_back();
            stack.pop_back();
            Int* sub_res = new Int(sub_arg1->value * sub_arg2->value);
            delete sub_arg1;
            delete sub_arg2;
            stack.push_back(sub_res);
            stack[stack.size() - 1]->type = ValueType::Int;
            break;
        }
        case OpCode::if_icmpgt: {
            ++pc;
            unsigned char indexbyte1 = getCode();
            ++pc;
            unsigned char indexbyte2 = getCode();
            unsigned short new_pc = (indexbyte1 << 8) + indexbyte2;
            if (stack.size() < 2) {
                cout << "in icmpgt less than 2 things on stack";
                cout << stack.size() << endl;
                exit (1);
            }
            Int* arg2 = to_int(stack[stack.size() - 1]);
            Int* arg1 = to_int(stack[stack.size() - 2]);
            if (arg1->value > arg2->value) {
                // cout << "old pc: " << pc << " | new pc: " << new_pc << endl;
                pc += new_pc - 3;
            }
            stack.pop_back();
            stack.pop_back();
            delete arg1;
            delete arg2;
            break;
        }
        case OpCode::op_return:
            if (stack.size() > 0) {
                cout << "stack is not empty before the return";
                exit(1);
            }
            return 0;
            break;
        case OpCode::ireturn: {
            Int* ret = to_int(stack[stack.size() - 1]);
            Int* new_ret = new Int(ret->value);
            vm.call_stack[vm.call_stack.size() - 2]->stack.push_back(new_ret);
            delete ret;
            stack.pop_back();
            return 0;
            break;
        }
        case invoke_static:
            ++pc;
            unsigned char arg1 = getCode();
            ++pc;
            unsigned char arg2 = getCode();
            unsigned short ref = (arg1 << 8) + arg2;
            Method* new_m = getMethod(vm, ref);
            if (new_m == nullptr) {
                cout << "method wasn't found" << endl;
                exit(1);
            }
            Frame* new_frame = new Frame(new_m);
            for (size_t i = 0; i < new_m->args.size(); i++) {
                auto arg = copyStack(stack[stack.size() - 1]);
                delete stack[stack.size() - 1];
                stack.pop_back();
                new_frame->locals.push_back(arg);
            }
            vm.call_stack.push_back(new_frame);
            new_frame->execute_frame(vm);
            break;
    }
    ++pc;
    return 1;
}
