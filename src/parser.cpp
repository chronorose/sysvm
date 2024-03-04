#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

class ClassFile {
    public:
    unsigned int magic;
    unsigned short minor_version;
    unsigned short major_version;  
    unsigned short constant_pool_count;
};

class CP_info {
    public:
    unsigned char tag;
    unsigned char info;  
};

template<typename T> void swap(T* a, T* b) {
    T* tmp = a;
    a = b;
    b = tmp;
}

template<typename T> void reverseArray(T* array, size_t n) {
    size_t i = 0, j = n - 1;
    while (i < j) {
        swap<T>(array[i++], array[j--]);
    }
}

template<typename T> T readbytes(ifstream& is) {
    size_t size = sizeof(T);
    char* buf = new char[size];
    is.read(buf, size);
    reverseArray(buf, size);
    T ret = *(T*) buf;
    delete[] buf;
    return ret;
}

ClassFile readClassFile(ifstream& is) {
    ClassFile cf;
    cf.magic = readbytes<unsigned int>(is);
    cf.minor_version = readbytes<unsigned short>(is);
    cf.major_version= readbytes<unsigned short>(is);
    cf.constant_pool_count= readbytes<unsigned short>(is);
    return cf;
}

int main() {
    ifstream is("examples/HelloVM.class", ios::binary);
    ClassFile cf = readClassFile(is); 
    cout << cf.minor_version << " " << cf.major_version << " " << cf.constant_pool_count;
}
