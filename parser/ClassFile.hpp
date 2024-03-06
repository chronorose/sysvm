#pragma once

#include "Common.hpp"
#include "Tables.hpp"

// honestly pretty scary function since it doesn't have any restriction on type taken.
// currently too lazy to search how to do it;
// TODO: restrict it only to those who inherit CP_info
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
    std::vector<Field>* fields = nullptr;
    unsigned short methods_count;
    std::vector<Method>* methods = nullptr;
    unsigned short attributes_count;
    std::vector<Attribute*>* attributes = nullptr;
    void readCPinfo(std::ifstream& is);
    C_Utf8* getUtf8(const unsigned short index) const;
    CP_info* getCP(const unsigned short index) const; 
    public:
    ClassFile(std::ifstream& is);
    ~ClassFile(); 
};
