#include "Common.hpp"
#include "ClassFile.hpp"
#include "Tables.hpp"

using namespace std;

// TODO: watch out for double and long structs in CP

// assumes a valid pointer to allocated memory which is enough; else UB
void readToBuf(ifstream& is, char* buffer, size_t n) {
    is.read(buffer, n);
}

CP_info* ClassFile::getCP(const unsigned short index) const {
    return constant_pool->at(index - 1);
}

// gets you utf8 on the index in constant pool you specified;
// or gets you killed in the process;
// there's so much which could go wrong here, it's insane.
C_Utf8* ClassFile::getUtf8(const unsigned short index) const {
    // C_Utf8* ret = dynamic_cast<C_Utf8*>(this->constant_pool->at(index - 1));
    return CPi_cast<C_Utf8*>(getCP(index - 1));
}

// reads one CPinfo struct from constant pool;
// takes vector of CPinfo and input stream;
void ClassFile::readCPinfo(ifstream& is) {
    unsigned char tag = readbytes<unsigned char>(is);
    size_t i = constant_pool->size();
    switch (tag) {
        case 7: {
            C_Class* cls = new C_Class;
            cls->name_index = readbytes<unsigned short>(is);
            constant_pool->push_back(cls);
            constant_pool->at(i)->tag = 7;
            break;
        }
        case 9: {
            C_Fieldref* fld = new C_Fieldref;
            fld->class_index = readbytes<unsigned short>(is);
            fld->name_and_type = readbytes<unsigned short>(is);
            constant_pool->push_back(fld);
            constant_pool->at(i)->tag = 9;
            break;
        }
        case 10: {
            C_Methodref* mthd = new C_Methodref;
            mthd->class_index = readbytes<unsigned short>(is);
            mthd->name_and_type = readbytes<unsigned short>(is);
            constant_pool->push_back(mthd);
            constant_pool->at(i)->tag = 10;
            break;
        }
        case 11: {
            C_InterfaceMethodref* imthd = new C_InterfaceMethodref;
            imthd->class_index= readbytes<unsigned short>(is);
            imthd->name_and_type = readbytes<unsigned short>(is);
            constant_pool->push_back(imthd);
            constant_pool->at(i)->tag = 11;
            break;
        }
        case 8: {
            C_String* str = new C_String;
            str->string_index = readbytes<unsigned short>(is);
            constant_pool->push_back(str);
            constant_pool->at(i)->tag = 8;
            break;
        }
        case 3: {
            C_Integer* integer = new C_Integer;
            integer->bytes = readbytes<unsigned int>(is);
            constant_pool->push_back(integer);
            constant_pool->at(i)->tag = 3;
            break;
        }
        case 4: {
            C_Float* flt = new C_Float;
            flt->bytes = readbytes<unsigned int>(is);
            constant_pool->push_back(flt);
            constant_pool->at(i)->tag = 4;
            break;
        }
        case 5: {
            C_Long* lng = new C_Long;
            lng->high_bytes = readbytes<unsigned int>(is);
            lng->low_bytes = readbytes<unsigned int>(is);
            constant_pool->push_back(lng);
            constant_pool->at(i)->tag = 5;
            break;
        }
        case 6: {
            C_Double * dbl = new C_Double;
            dbl->high_bytes = readbytes<unsigned int>(is);
            dbl->low_bytes = readbytes<unsigned int>(is);
            constant_pool->push_back(dbl);
            constant_pool->at(i)->tag = 6;
            break;
        }
        case 12: {
            C_NameAndType* nat = new C_NameAndType;
            nat->name_index = readbytes<unsigned short>(is);
            nat->descriptor = readbytes<unsigned short>(is);
            constant_pool->push_back(nat);
            constant_pool->at(i)->tag = 12;
            break;
        }
        case 1: {
            C_Utf8* utf = new C_Utf8;
            unsigned short len = readbytes<unsigned short>(is);
            char* bytes = new char[len + 1];
            readToBuf(is, bytes, len);
            bytes[len] = '\0';
            utf->bytes = new string;
            *utf->bytes = bytes;
            constant_pool->push_back(utf);
            constant_pool->at(i)->tag = 1;
            delete[] bytes;
            break;
        }
        case 15: {
            C_MethodHandle* mhandle = new C_MethodHandle;
            mhandle->reference_kind = readbytes<unsigned char>(is);
            mhandle->reference_index = readbytes<unsigned short>(is);
            constant_pool->push_back(mhandle);
            constant_pool->at(i)->tag = 15;
            break;
        }
        case 16: {
            C_MethodType* mtype = new C_MethodType;
            mtype->descriptor_index = readbytes<unsigned short>(is);
            constant_pool->push_back(mtype);
            constant_pool->at(i)->tag = 16;
            break;
        }
        case 18: {
            C_InvokeDynamic* invdyn = new C_InvokeDynamic;
            invdyn->bootstrap_method_attr_index = readbytes<unsigned short>(is);
            invdyn->name_and_type = readbytes<unsigned short>(is);
            constant_pool->push_back(invdyn);
            constant_pool->at(i)->tag = 18;
            break;
        }
    }
}

ClassFile::ClassFile(ifstream& is) {
    magic = readbytes<unsigned int>(is);
    minor_version = readbytes<unsigned short>(is);
    major_version= readbytes<unsigned short>(is);
    constant_pool_count= readbytes<unsigned short>(is);
    constant_pool = new vector<CP_info*>;
    for (size_t i = 0; i < constant_pool_count - 1; i++) {
        readCPinfo(is);
    }
    access_flags = readbytes<unsigned short>(is);
    this_class = readbytes<unsigned short>(is);
    super_class = readbytes<unsigned short>(is);
    interfaces_count = readbytes<unsigned short>(is);
    interfaces = new unsigned short[interfaces_count];
    for (size_t i = 0; i < interfaces_count; i++) {
        interfaces[i] = readbytes<unsigned short>(is);
    }
    fields_count = readbytes<unsigned short>(is);
    fields = new vector<Field*>;
    for (size_t i = 0; i < fields_count; i++) {
        Field* fld = new Field(is);
        fields->push_back(fld);
    }   
    methods_count = readbytes<unsigned short>(is);
    methods = new vector<Method*>;
    for (size_t i = 0; i < methods_count; i++) {
        Method* mthd = new Method(is);
        methods->push_back(mthd);
    }
    attributes_count = readbytes<unsigned short>(is);
    attributes = new vector<Attribute*>;
    for (size_t i = 0; i < attributes_count; i++) {
        Attribute* attr = new Attribute(is);
        attributes->push_back(attr);
    }
    // C_Class* cls = dynamic_cast<C_Class*>(constant_pool->at(this_class - 1));
    // C_Utf8* utf = dynamic_cast<C_Utf8*>(constant_pool->at(cls->name_index - 1));
    // cout << *utf->bytes << endl;
    // cout << methods_count << endl;
    Method* mthd = methods->at(1);
    // cout << mthd->attributes_count;
    C_Utf8* utf1 = dynamic_cast<C_Utf8*>(constant_pool->at(mthd->attributes->at(0)->name_index - 1));
    cout << *utf1->bytes;
    // C_Utf8* utf1 = dynamic_cast<C_Utf8*>(constant_pool->at(mthd->attributes));
    // cout << *utf1->bytes << endl;
}

ClassFile::~ClassFile() {
    for (size_t i = 0; i < constant_pool->size(); i++) {
        if (constant_pool->at(i)->tag == 1) {
            C_Utf8* utf = dynamic_cast<C_Utf8*>(constant_pool->at(i));
            delete utf->bytes;
            delete utf;
        } else {
            delete constant_pool->at(i);
        }
    }
    delete constant_pool;
    delete[] interfaces;
    for (size_t i = 0; i < fields->size(); i++) {
        delete fields->at(i);
    }
    delete fields;
    for (size_t i = 0; i < methods->size(); i++) {
        delete methods->at(i);
    }
    delete methods;
    for (size_t i = 0; i < attributes->size(); i++) {
        delete attributes->at(i);
    }
    delete attributes;
}

