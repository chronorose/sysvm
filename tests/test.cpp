#include "catch_amalgamated.hpp"
#include "../vm/vm.hpp"
#include "../parser/class_file.hpp"
#include "fstream"

ClassFile* openCF(char* path) {
    ifstream is(path);
    ClassFile* cf = new ClassFile(is);
    return cf;
} 

TEST_CASE("parser check", "[!benchmark]") {
    ClassFile* cfempty = openCF((char*)"../examples/HelloVM.class");
    VM vmempty(cfempty);
    BENCHMARK("classfile empty") {
        return vmempty.execute_vm();
    };
    ClassFile* cfif = openCF((char*)"../examples/If.class");
    VM vmif(cfif);
    BENCHMARK("classfile if") {
        return vmif.execute_vm();
    };
    ClassFile* cffact = openCF((char*)"../examples/Factorial.class");
    VM vmfact(cffact);
    BENCHMARK("classfile factorial") {
        return vmfact.execute_vm();
    };
}
