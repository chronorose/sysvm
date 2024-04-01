// i know that enum members are supposed to be CAPSed out, but i don't feel like doing it
// here. perhaps i will change it at some point.
enum OpCode {
    nop = 0,

    aconst_null = 1,

    // ops that push int constant onto the stack
    iconst_m1 = 2,
    iconst_0 = 3,
    iconst_1 = 4,
    iconst_2 = 5,
    iconst_3 = 6,
    iconst_4 = 7,
    iconst_5 = 8,

    bipush = 16,

    iload_0 = 26,
    iload_1 = 27,
    iload_2 = 28,
    iload_3 = 29,
    istore_0 = 59,
    istore_1 = 60,
    istore_2 = 61,
    istore_3 = 62,

    aload_0 = 42,

    pop = 87,

    isub = 100,
    imul = 104,
    iinc = 132,
    // if comparisons with int
    // if succeeds, then we go to the opcode address specified in the next two bytes in the Code section
    // else, we just go to the next opcode.
    if_icmpeq = 159,
    if_icmpneq = 160,
    if_icmplt = 161,
    if_icmpge = 162,
    if_icmpgt = 163,
    if_icmple = 164,

    // return int
    ireturn = 172,
    // return void
    op_return = 177,

    // ops that invoke methods, they take two arguments from the Code section, not from the stack
    invoke_special = 183,      
    invoke_static = 184,
    sp_void = 300,
};
