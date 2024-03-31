#include "vm.hpp"

Frame::Frame(Method method): pc(0) {
    this->method = method;
}

Frame::Frame(Method method, vector<Value*> args): Frame(method) {
    locals = args;
}
