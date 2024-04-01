#include <iostream>
#include <fstream>
#include "../parser/class_file.hpp"
#include "vm.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "yeah i need classfile to work with bruh" << endl;
        exit(1);
    }
    ifstream is(argv[1]);
    ClassFile* cf = new ClassFile(is);
    VM vm(cf);
    vm.execute_vm();
    delete cf;
}
