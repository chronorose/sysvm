#pragma once

#include "common.hpp"
// #include "Tables.hpp"

struct CP_info;
struct Field;
struct Method;
struct Attribute;
struct C_Utf8;
struct Code;

// TODO: delete all useless fields in all classes;
// TODO: do actual in-memory representation of the class that will be used.
// TODO: inline all the stuff from constant pool straight into the objects, so that no
// runtime look-ups to constant pool take place;

template<typename T> T CPi_cast(CP_info* cpi) {
    return dynamic_cast<T>(cpi);
}

class ClassFile {
    public:
    unsigned int magic;
    unsigned short minor_version;
    unsigned short major_version;  

    unsigned short constant_pool_count;
    std::vector<CP_info*> constant_pool;

    unsigned short access_flags;
    unsigned short this_class;
    unsigned short super_class;

    unsigned short interfaces_count;
    std::vector<unsigned short> interfaces;

    unsigned short fields_count;
    std::vector<Field*> fields;

    unsigned short methods_count;
    Method* main = nullptr;
    std::vector<Method*> methods;

    unsigned short attributes_count;
    std::vector<Attribute*> attributes;

    void readCPinfo(std::ifstream& is);
    CP_info* getCP(const unsigned short index) const; 

    C_Utf8* getUtf8(const unsigned short index) const;
    ClassFile(std::ifstream& is);
    ~ClassFile(); 
};
