#pragma once

#include "Common.hpp"
// #include "Tables.hpp"

struct CP_info;
struct Field;
struct Method;
struct Attribute;
struct C_Utf8;

template<typename T> T CPi_cast(CP_info* cpi) {
    return dynamic_cast<T>(cpi);
}

class ClassFile {
    unsigned int magic;
    unsigned short minor_version;
    unsigned short major_version;  

    unsigned short constant_pool_count;
    std::vector<CP_info*>* constant_pool = nullptr;

    unsigned short access_flags;
    unsigned short this_class;
    unsigned short super_class;

    unsigned short interfaces_count;
    unsigned short* interfaces;

    unsigned short fields_count;
    std::vector<Field> fields;

    unsigned short methods_count;
    std::vector<Method> methods;

    unsigned short attributes_count;
    std::vector<Attribute*> attributes;

    void readCPinfo(std::ifstream& is);
    CP_info* getCP(const unsigned short index) const; 

    public:
    C_Utf8* getUtf8(const unsigned short index) const;
    ClassFile(std::ifstream& is);
    ~ClassFile(); 
};
