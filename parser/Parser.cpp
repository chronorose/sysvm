#include "Common.hpp"
#include "ClassFile.hpp"

int main(int argc, char* argv[]) {
    std::ifstream is(argv[1]);
    ClassFile cf(is);
}
