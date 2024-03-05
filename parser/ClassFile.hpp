#pragma once

#include "Common.hpp"
#include "Tables.hpp"

class ClassFile {
    unsigned int counter = 0;
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
    std::vector<Field*>* fields = nullptr;
    unsigned short methods_count;
    std::vector<Method*>* methods = nullptr;
    unsigned short attributes_count;
    std::vector<Attribute*>* attributes = nullptr;
    void readCPinfo(std::ifstream& is);
    public:
    ClassFile(std::ifstream& is);
    ~ClassFile(); 
};
