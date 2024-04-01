#pragma once

#include "common.hpp"
#include "class_file.hpp"
#include "../vm/value.hpp"

using namespace std;

struct CP_info {
    unsigned char tag = 0;
    virtual ~CP_info() {}
};

struct C_Utf8: public CP_info {
    unsigned char tag = 1;
    string bytes;
};

struct Attribute {
    unsigned short name_index;
    unsigned int attribute_length;
    unsigned char* info;
    Attribute() {
        name_index = 0;
        attribute_length = 0;
        info = nullptr;
    }
    Attribute(ifstream& is) {
        name_index = readbytes<unsigned short>(is);
        attribute_length = readbytes<unsigned int>(is);
        info = new unsigned char[attribute_length + 1];
        readToBuf(is, (char*)info, attribute_length);
        info[attribute_length] = '\0';
    }
    Attribute(ifstream& is, unsigned short n_index) {
        name_index = n_index;
        attribute_length = readbytes<unsigned int>(is);
        info = new unsigned char[attribute_length + 1];
        readToBuf(is, (char*)info, attribute_length);
        info[attribute_length] = '\0';
    }
    Attribute(const Attribute& other) {
        this->name_index = other.name_index;
        this->attribute_length = other.attribute_length;
        this->info = new unsigned char[attribute_length + 1];
        for (size_t i = 0; i < attribute_length + 1; i++) {
            this->info[i] = other.info[i];
        }
    }
    Attribute& operator=(const Attribute& other) {
        if (this != &other) {
            delete[] info;
            name_index = other.name_index;
            attribute_length = other.attribute_length;
            info = new unsigned char[attribute_length + 1];
            for (size_t i = 0; i < attribute_length + 1; i++) {
                info[i] = other.info[i];
            }
        }
        return *this;
    }
    ~Attribute() {
        delete[] info;
    }
};

struct ExceptionTable {
    unsigned short start_pc;
    unsigned short end_pc;
    unsigned short handler_pc;
    unsigned short catch_type;
    ExceptionTable(ifstream& is) {
        start_pc = readbytes<unsigned short>(is);
        end_pc = readbytes<unsigned short>(is);
        handler_pc = readbytes<unsigned short>(is);
        catch_type = readbytes<unsigned short>(is);
    }
    ExceptionTable(const ExceptionTable& other) {
        start_pc = other.start_pc;
        end_pc = other.end_pc;
        handler_pc = other.handler_pc;
        catch_type = other.catch_type;
    }
    ExceptionTable& operator=(const ExceptionTable& other) {
        start_pc = other.start_pc;
        end_pc = other.end_pc;
        handler_pc = other.handler_pc;
        catch_type = other.catch_type;
        return *this;
    }
};

struct Code {
    unsigned short max_stack;
    unsigned short max_locals;
    vector<unsigned char> code;
    vector<ExceptionTable> exception_table;
    vector<Attribute> attributes;
    Code() {}
    Code(ifstream& is) {
        readbytes<unsigned int>(is);
        max_stack = readbytes<unsigned short>(is);
        max_locals = readbytes<unsigned short>(is);
        unsigned int code_length = readbytes<unsigned int>(is);
        unsigned char* code_buf = new unsigned char[code_length + 1];
        readToBuf(is, (char*)code_buf, code_length);
        for (size_t i = 0; i < code_length; i++) {
            code.push_back(code_buf[i]);
        }
        delete[] code_buf;
        unsigned short exception_table_len = readbytes<unsigned short>(is);
        for (size_t i = 0; i < exception_table_len; i++) {
            ExceptionTable ex_table(is);
            exception_table.push_back(ex_table);
        }
        unsigned short attribute_length = readbytes<unsigned short>(is);
        for (size_t i = 0; i < attribute_length; i++) {
            Attribute attr(is);
            attributes.push_back(attr);
        }
    }
};

struct Field {
    unsigned short access_flags;
    unsigned short name_index;
    unsigned short descriptor_index;
    unsigned short attributes_count;  
    vector<Attribute> attributes;
    Field(ifstream& is) {
        access_flags = readbytes<unsigned short>(is);
        name_index = readbytes<unsigned short>(is);
        descriptor_index = readbytes<unsigned short>(is);
        attributes_count = readbytes<unsigned short>(is);
        for (size_t i = 0; i < attributes_count; i++) {
            Attribute attr(is);
            attributes.push_back(attr);
        }
    }
    Field(const Field& other) {
        access_flags = other.access_flags;
        name_index = other.access_flags;
        descriptor_index = other.descriptor_index;
        attributes_count = other.attributes_count;
        attributes = other.attributes;
    }
    Field& operator=(const Field& other) {
        if (this != &other) {
            access_flags = other.access_flags;
            name_index = other.access_flags;
            descriptor_index = other.descriptor_index;
            attributes_count = other.attributes_count;
            attributes = other.attributes;
        }
        return *this;
    }
};

struct Method {
    unsigned short access_flags;
    unsigned short name_index;  
    unsigned short descriptor_index;
    unsigned short attributes_count;
    Code code;
    vector<ValueType> args;
    vector<Attribute> attributes;
    Method() {}
    Method(ifstream& is, ClassFile& cf) {
        access_flags = readbytes<unsigned short>(is);    
        name_index = readbytes<unsigned short>(is);    
        descriptor_index = readbytes<unsigned short>(is);
        getArgs(cf);
        attributes_count = readbytes<unsigned short>(is);    
        for (size_t i = 0; i < attributes_count; i++) {
            unsigned short n_index = readbytes<unsigned short>(is);
            if (cf.getUtf8(n_index)->bytes == "Code") {
                Code cd(is);
                code = cd;
            } else {
                Attribute attr(is, n_index);
                attributes.push_back(attr);
            } 
        }
    }
    // for now i only take BaseTypes since it's basically useless with my current impl to take anything else
    // i do not take arrays or references.
    // i also do not typecheck, for now the use of this vector is to just know the
    // amount of arguments i have to pop from the stack
    void getArgs(ClassFile& cf) {
        string descriptor = cf.getUtf8(descriptor_index)->bytes;
        for (size_t i = 1; i < descriptor.size(); i++) {
            switch(descriptor[i]) {
                case 'B': 
                    args.push_back(ValueType::Byte);
                    break;
                case 'C':
                    args.push_back(ValueType::Char);
                    break;
                case 'D':
                    args.push_back(ValueType::Double);
                    break;
                case 'F':
                    args.push_back(ValueType::Float);
                    break;
                case 'I':
                    args.push_back(ValueType::Int);
                    break;
                case 'J':
                    args.push_back(ValueType::Long);
                    break;
                case 'S':
                    args.push_back(ValueType::Short);
                    break;
                case 'Z':
                    args.push_back(ValueType::Bool);
                    break;
                case ')':
                    return;
                default:
                    break;
            }
        }
    }
    Method(const Method& other) {
        access_flags = other.access_flags;
        name_index = other.name_index;
        descriptor_index = other.descriptor_index;
        attributes_count = other.attributes_count;
        code = other.code;
        attributes = other.attributes;
    }
    Method& operator=(const Method& other) {
        if (this != &other) {
            access_flags = other.access_flags;
            name_index = other.name_index;
            descriptor_index = other.descriptor_index;
            attributes_count = other.attributes_count;
            code = other.code;
            attributes = other.attributes;
        }
        return *this;
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
