#include "common.hpp"
#include "class_file.hpp"
#include "tables.hpp"

using namespace std;

// TODO: watch out for double and long structs in CP

void readToBuf(ifstream& is, char* buffer, size_t n) {
    is.read(buffer, n);
}

CP_info* ClassFile::getCP(const unsigned short index) const {
    return constant_pool.at(index - 1);
}

C_Utf8* ClassFile::getUtf8(const unsigned short index) const {
    return CPi_cast<C_Utf8*>(getCP(index));
}

// reads one CPinfo struct from constant pool;
// takes vector of CPinfo and input stream;
void ClassFile::readCPinfo(ifstream& is) {
    unsigned char tag = readbytes<unsigned char>(is);
    size_t i = constant_pool.size();
    switch (tag) {
        case 7: {
            C_Class* cls = new C_Class;
            cls->name_index = readbytes<unsigned short>(is);
            constant_pool.push_back(cls);
            break;
        }
        case 9: {
            C_Fieldref* fld = new C_Fieldref;
            fld->class_index = readbytes<unsigned short>(is);
            fld->name_and_type = readbytes<unsigned short>(is);
            constant_pool.push_back(fld);
            break;
        }
        case 10: {
            C_Methodref* mthd = new C_Methodref;
            mthd->class_index = readbytes<unsigned short>(is);
            mthd->name_and_type = readbytes<unsigned short>(is);
            constant_pool.push_back(mthd);
            break;
        }
        case 11: {
            C_InterfaceMethodref* imthd = new C_InterfaceMethodref;
            imthd->class_index= readbytes<unsigned short>(is);
            imthd->name_and_type = readbytes<unsigned short>(is);
            constant_pool.push_back(imthd);
            break;
        }
        case 8: {
            C_String* str = new C_String;
            str->string_index = readbytes<unsigned short>(is);
            constant_pool.push_back(str);
            break;
        }
        case 3: {
            C_Integer* integer = new C_Integer;
            integer->bytes = readbytes<unsigned int>(is);
            constant_pool.push_back(integer);
            break;
        }
        case 4: {
            C_Float* flt = new C_Float;
            flt->bytes = readbytes<unsigned int>(is);
            constant_pool.push_back(flt);
            break;
        }
        case 5: {
            C_Long* lng = new C_Long;
            lng->high_bytes = readbytes<unsigned int>(is);
            lng->low_bytes = readbytes<unsigned int>(is);
            constant_pool.push_back(lng);
            break;
        }
        case 6: {
            C_Double * dbl = new C_Double;
            dbl->high_bytes = readbytes<unsigned int>(is);
            dbl->low_bytes = readbytes<unsigned int>(is);
            constant_pool.push_back(dbl);
            break;
        }
        case 12: {
            C_NameAndType* nat = new C_NameAndType;
            nat->name_index = readbytes<unsigned short>(is);
            nat->descriptor = readbytes<unsigned short>(is);
            constant_pool.push_back(nat);
            break;
        }
        case 1: {
            C_Utf8* utf = new C_Utf8;
            unsigned short len = readbytes<unsigned short>(is);
            char* bytes = new char[len + 1];
            readToBuf(is, bytes, len);
            bytes[len] = '\0';
            utf->bytes = bytes;
            constant_pool.push_back(utf);
            delete[] bytes;
            break;
        }
        case 15: {
            C_MethodHandle* mhandle = new C_MethodHandle;
            mhandle->reference_kind = readbytes<unsigned char>(is);
            mhandle->reference_index = readbytes<unsigned short>(is);
            constant_pool.push_back(mhandle);
            break;
        }
        case 16: {
            C_MethodType* mtype = new C_MethodType;
            mtype->descriptor_index = readbytes<unsigned short>(is);
            constant_pool.push_back(mtype);
            break;
        }
        case 18: {
            C_InvokeDynamic* invdyn = new C_InvokeDynamic;
            invdyn->bootstrap_method_attr_index = readbytes<unsigned short>(is);
            invdyn->name_and_type = readbytes<unsigned short>(is);
            constant_pool.push_back(invdyn);
            break;
        }
        default: {
            cout << "something went really wrong" << endl;
            exit(1);
            break;
        }
    }
    constant_pool.at(i)->tag = tag;
}

ClassFile::ClassFile(ifstream& is) {
    if(!(readbytes<unsigned int>(is) == 0xCAFEBABE)) {
        cout << "wrong file format" << endl;
    }
    // we skip versions
    is.seekg(4, ios::cur);
    int cpc = readbytes<unsigned short>(is);
    for (size_t i = 0; i < cpc - 1; i++) {
        readCPinfo(is);
    }
    // i read access flags but its pretty useless to me(at least currently)
    access_flags = readbytes<unsigned short>(is);

    this_class = readbytes<unsigned short>(is);
    // cout << this_class << " debug" << endl;
    // cout << (int)dynamic_cast<C_Class*>(constant_pool.at(6))->name_index << endl;
    // cout << dynamic_cast<C_Utf8*>(constant_pool.at(7))->bytes << endl;

    super_class = readbytes<unsigned short>(is);

    // cout << super_class << " debug" << endl;
    // cout << (int)dynamic_cast<C_Class*>(constant_pool.at(1))->name_index << endl;
    // cout << dynamic_cast<C_Utf8*>(constant_pool.at(3))->bytes << endl;

    // interface count
    unsigned short ict = readbytes<unsigned short>(is);
    for (size_t i = 0; i < ict; i++) {
        interfaces.push_back(readbytes<unsigned short>(is));
    }
    // fields count
    unsigned short fct = readbytes<unsigned short>(is);
    for (size_t i = 0; i < fct; i++) {
        Field* fld = new Field(is);
        fields.push_back(fld);
    }
    // methods count
    unsigned short mct = readbytes<unsigned short>(is);
    for (size_t i = 0; i < mct; i++) {
        Method* mthd = new Method(is, *this);
        if (getUtf8(mthd->name_index)->bytes == "main") this->main = mthd;
        methods.push_back(mthd);
    }
    // attributes count
    unsigned short act = readbytes<unsigned short>(is);
    for (size_t i = 0; i < act; i++) {
        Attribute* attr = new Attribute(is);
        attributes.push_back(attr);
    }
}

template<typename T>
void deleteVector(vector<T> vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        delete vec[i];
    }
}

ClassFile::~ClassFile() {
    deleteVector(constant_pool);
    deleteVector(fields);
    deleteVector(methods);
    deleteVector(attributes);
}

