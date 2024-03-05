#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

struct CP_info {
    unsigned char tag = 0;
    virtual ~CP_info() {}
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
    void readCPinfo(ifstream& is);
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

// assumes a valid pointer to allocated memory which is enough; else UB
void readToBuf(ifstream& is, char* buffer, size_t n) {
    is.read(buffer, n);
    reverseArray(buffer, n);
}

ClassFile readClassFile(ifstream& is) {
    ClassFile cf;
    cf.magic = readbytes<unsigned int>(is);
    cf.minor_version = readbytes<unsigned short>(is);
    cf.major_version= readbytes<unsigned short>(is);
    cf.constant_pool_count= readbytes<unsigned short>(is);
    return cf;
}

// reads one CPinfo struct from constant pool;
// takes vector of CPinfo and input stream;
void readCPinfo(vector<CP_info*>& vec, ifstream& is) {
    unsigned char tag = readbytes<unsigned char>(is);
    size_t i = vec.size();
    switch (tag) {
        case 7: {
            C_Class* cls = new C_Class;
            cls->name_index = readbytes<unsigned short>(is);
            vec.push_back(cls);
            vec.at(i)->tag = 7;
            break;
        }
        case 9: {
            C_Fieldref* fld = new C_Fieldref;
            fld->class_index = readbytes<unsigned short>(is);
            fld->name_and_type = readbytes<unsigned short>(is);
            vec.push_back(fld);
            vec.at(i)->tag = 9;
            break;
        }
        case 10: {
            C_Methodref* mthd = new C_Methodref;
            mthd->class_index = readbytes<unsigned short>(is);
            mthd->name_and_type = readbytes<unsigned short>(is);
            vec.push_back(mthd);
            vec.at(i)->tag = 10;
            break;
        }
        case 11: {
            C_InterfaceMethodref* imthd = new C_InterfaceMethodref;
            imthd->class_index= readbytes<unsigned short>(is);
            imthd->name_and_type = readbytes<unsigned short>(is);
            vec.push_back(imthd);
            vec.at(i)->tag = 11;
            break;
        }
        case 8: {
            C_String* str = new C_String;
            str->string_index = readbytes<unsigned short>(is);
            vec.push_back(str);
            vec.at(i)->tag = 8;
            break;
        }
        case 3: {
            C_Integer* integer = new C_Integer;
            integer->bytes = readbytes<unsigned int>(is);
            vec.push_back(integer);
            vec.at(i)->tag = 3;
            break;
        }
        case 4: {
            C_Float* flt = new C_Float;
            flt->bytes = readbytes<unsigned int>(is);
            vec.push_back(flt);
            vec.at(i)->tag = 4;
            break;
        }
        case 5: {
            C_Long* lng = new C_Long;
            lng->high_bytes = readbytes<unsigned int>(is);
            lng->low_bytes = readbytes<unsigned int>(is);
            vec.push_back(lng);
            vec.at(i)->tag = 5;
            break;
        }
        case 6: {
            C_Double * dbl = new C_Double;
            dbl->high_bytes = readbytes<unsigned int>(is);
            dbl->low_bytes = readbytes<unsigned int>(is);
            vec.push_back(dbl);
            vec.at(i)->tag = 6;
            break;
        }
        case 12: {
            C_NameAndType* nat = new C_NameAndType;
            nat->name_index = readbytes<unsigned short>(is);
            nat->descriptor = readbytes<unsigned short>(is);
            vec.push_back(nat);
            vec.at(i)->tag = 12;
            break;
        }
        case 1: {
            C_Utf8* utf = new C_Utf8;
            utf->len = readbytes<unsigned short>(is);
            utf->bytes = new unsigned char[utf->len];
            readToBuf(is, (char*) utf->bytes, utf->len);
            vec.push_back(utf);
            vec.at(i)->tag = 1;
            break;
        }
        case 15: {
            C_MethodHandle* mhandle = new C_MethodHandle;
            mhandle->reference_kind = readbytes<unsigned char>(is);
            mhandle->reference_index = readbytes<unsigned short>(is);
            vec.push_back(mhandle);
            vec.at(i)->tag = 15;
            break;
        }
        case 16: {
            C_MethodType* mtype = new C_MethodType;
            mtype->descriptor_index = readbytes<unsigned short>(is);
            vec.push_back(mtype);
            vec.at(i)->tag = 16;
            break;
        }
        case 18: {
            C_InvokeDynamic* invdyn = new C_InvokeDynamic;
            invdyn->bootstrap_method_attr_index = readbytes<unsigned short>(is);
            invdyn->name_and_type = readbytes<unsigned short>(is);
            vec.push_back(invdyn);
            vec.at(i)->tag = 18;
            break;
        }
    }
}

int main() {
    ifstream is("../examples/HelloVM.class", ios::binary);
    ClassFile cf = readClassFile(is); 
    cout << cf.minor_version << " " << cf.major_version << " " << cf.constant_pool_count;
    vector<CP_info*>* vec = new vector<CP_info*>;
    readCPinfo(*vec, is);
    cout << (int)vec->at(0)->tag;
    // vector<CP_info*>* vec = new vector<CP_info*>;
    // C_Class* something = new C_Class;
    // something->name_index = 344;
    // vec->push_back(something);
    // vec->at(0)->tag = 7;
    // cout << (unsigned int)vec->at(0)->tag;
    // C_Class* som = dynamic_cast<C_Class*>(vec->at(0));
    // cout << (unsigned int)som->tag;
    // cout << (unsigned int)som->name_index;
    // delete something;
    // delete vec;
}
