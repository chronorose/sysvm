#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

struct CP_info {
    unsigned char tag;
    unsigned char info;
};

struct C_Class: public CP_info {
    unsigned char tag = 7;
    unsigned short name_index;  
};

struct C_Fieldref: public CP_info {
    unsigned char tag = 9;
    unsigned short class_index;
    unsigned short name_and_type;
};

struct C_Methodref: public CP_info {
    unsigned char tag = 10;
    unsigned short class_index;
    unsigned short name_and_type;
};

struct C_InterfaceMethodref: public CP_info {
    unsigned char tag = 11;  
    unsigned short class_index;
    unsigned short name_and_type;
};

struct C_String: public CP_info {
    unsigned char tag = 8;
    unsigned short string_index;
};

struct C_Integer: public CP_info {
    unsigned char tag = 3;
    unsigned int bytes;  
};

struct C_Float: public CP_info {
    unsigned char tag = 4;
    unsigned int bytes;
};

struct C_Long: public CP_info {
    unsigned char tag = 5;
    unsigned int high_bytes;
    unsigned int low_bytes;
};

struct C_Double: public CP_info {
    unsigned char tag = 6;
    unsigned int high_bytes;
    unsigned int low_bytes;
};

struct C_NameAndType: public CP_info {
    unsigned char tag = 12;
    unsigned short name_index;
    unsigned short descriptor;
};

struct C_Utf8: public CP_info {
    unsigned char tag = 1;
    unsigned short len;
    unsigned char* bytes;
};

struct C_MethodHandle: public CP_info {
    unsigned char tag = 15;
    unsigned char reference_kind;
    unsigned short reference_index;  
};

struct C_MethodType: public CP_info {
    unsigned char tag = 16;
    unsigned short descriptor_index;
};

struct C_InvokeDynamic: public CP_info {
    unsigned char tag = 18;
    unsigned short bootstrap_method_attr_index;  
    unsigned short name_and_type;
};

class ClassFile {
    public:
    unsigned int magic;
    unsigned short minor_version;
    unsigned short major_version;  
    unsigned short constant_pool_count;
    vector<CP_info*>* constant_pool = nullptr;
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
