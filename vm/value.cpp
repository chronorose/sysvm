#include "value.hpp"

Int* to_int(Value* value) {
    return dynamic_cast<Int*>(value); 
}

Byte* to_char(Value* value) {
    return dynamic_cast<Byte*>(value);
}
