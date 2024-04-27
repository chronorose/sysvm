#include "value.hpp"
#include <unordered_map>

using namespace std;

// so the idea is for me to have a pretty stupid allocator that will just stupidly allocate everything with a pointer using C++ new and default allocator.
// the only actual purpose of the allocator is to have it in one place to track it and free it with GC.
// the method of gcing i will be using is left uncertain for now but i assume that i will be using few of them and use some kind of conditional compilation

// stands for in-memory or something or maybe im class or imaginary or intermediate or image media or instagram media IDK
// fat pointer. i would even say obese.
struct Class {
    int class_ref;
    unordered_map<int, Value*> fields;
};

struct IMClass {
    long header;
    Class* cls;
};

struct Allocator {
    IMClass* heap;  
};
