#pragma once

#include "Common.hpp"

struct CP_info {
    unsigned char tag = 0;
    virtual ~CP_info() {}
};

struct Attribute {
    unsigned short name_index;
    unsigned int attribute_length;
    unsigned char* info;
    Attribute(std::ifstream& is) {
        name_index = readbytes<unsigned short>(is);
        attribute_length = readbytes<unsigned int>(is);
        info = new unsigned char[attribute_length + 1];
        readToBuf(is, (char*)info, attribute_length);
        info[attribute_length] = '\0';
    }
    // Attribute(const Attribute& other) {
    //     name_index = other.name_index;
    //     attribute_length = other.attribute_length;
    //     info = new unsigned char[attribute_length + 1];
    //     for (size_t i = 0; i < attribute_length + 1; i++) {
    //         info[i] = other.info[i];
    //     }
    // }
    // Attribute& operator=(const Attribute& other) {
    //     delete[] info;
    //     name_index = other.name_index;
    //     attribute_length = other.attribute_length;
    //     info = new unsigned char[attribute_length + 1];
    //     for (size_t i = 0; i < attribute_length + 1; i++) {
    //         info[i] = other.info[i];
    //     }
    //     return *this;
    // }
    ~Attribute() {
        delete[] info;
    }
};

struct Field {
    unsigned short access_flags;
    unsigned short name_index;
    unsigned short descriptor_index;
    unsigned short attributes_count;  
    std::vector<Attribute*>* attributes;
    Field(std::ifstream& is) {
        access_flags = readbytes<unsigned short>(is);
        name_index = readbytes<unsigned short>(is);
        descriptor_index = readbytes<unsigned short>(is);
        attributes_count = readbytes<unsigned short>(is);
        attributes = new std::vector<Attribute*>;
        for (size_t i = 0; i < attributes_count; i++) {
            Attribute* attr = new Attribute(is);
            attributes->push_back(attr);
        }
    }
    ~Field() {
        for (size_t i = 0; i < attributes->size(); i++) {
            delete attributes->at(i);
        }
        delete attributes;
    }
};

struct Method {
    unsigned short access_flags;
    unsigned short name_index;  
    unsigned short descriptor_index;
    unsigned short attributes_count;
    std::vector<Attribute*>* attributes;
    Method(std::ifstream& is) {
        access_flags = readbytes<unsigned short>(is);    
        name_index = readbytes<unsigned short>(is);    
        descriptor_index = readbytes<unsigned short>(is);    
        attributes_count = readbytes<unsigned short>(is);    
        attributes = new std::vector<Attribute*>;
        for (size_t i = 0; i < attributes_count; i++) {
            Attribute* attr = new Attribute(is);
            attributes->push_back(attr);
        }
    }
    // Method(const Method& other) {
    //     access_flags = other.access_flags;
    //     name_index = other.name_index;
    //     descriptor_index = other.descriptor_index;
    //     attributes_count = other.attributes_count;
    //     attributes = new std::vector<Attribute>(attributes_count);
    //     for (size_t i = 0; i < attributes_count; i++) {
    //         attributes->at(i) = other.attributes->at(i);
    //     }
    // }
    ~Method() {
        for (size_t i = 0; i < attributes->size(); i++) {
            delete attributes->at(i);
        }
        delete attributes;
    }
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
    std::string* bytes;
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
