#pragma once

enum class ValueType {
    Byte = 0,
    Short = 1,
    Int = 2,
    Long = 3,
    Char = 4,
    Float = 5,
    Double = 6,
    Bool = 7,
    ReturnAddress = 8,
    ClassRef = 9,
    ArrayRef = 10,
    InterfaceRef = 11,
};

struct Value {
    ValueType type;
    virtual ~Value() {}
};

struct Int: public Value {
    ValueType type = ValueType::Int;
    int value;
    Int(): value(0) {}
    Int(int value): value(value) {}
};

struct Byte: public Value {
    ValueType type = ValueType::Byte;
    char value;
    Byte(): value(0) {}
    Byte(char value): value(value) {}
};
