#include <stdio.h>

enum OPERATIONS_CODES {
    OPERATION_CODE_RECURSIVE_RET   = -1,
    OPERATION_CODE_HLT             = 0,
    OPERATION_CODE_PUSH            = 1,
    OPERATION_CODE_POP             = 2,
    OPERATION_CODE_ADD             = 3,
    OPERATION_CODE_SUB             = 4,
    OPERATION_CODE_MUL             = 5,
    OPERATION_CODE_DIV             = 6,
    OPERATION_CODE_OUT             = 7,
    OPERATION_CODE_IN              = 8,
    OPERATION_CODE_SQRT            = 9,
    OPERATION_CODE_JMP             = 10,
    OPERATION_CODE_CMP             = 30,
    OPERATION_CODE_JE              = 31,
    OPERATION_CODE_JNE             = 32,
    OPERATION_CODE_JA              = 33,
    OPERATION_CODE_JAE             = 34,
    OPERATION_CODE_JB              = 35,
    OPERATION_CODE_JBE             = 36,
    OPERATION_CODE_CALL            = 40,
    OPERATION_CODE_RET             = 41,
    OPERATION_CODE_MEOW            = 99
};

enum REGISTER_VARIABLES {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3
};

const char TEXT_OPERATION[][10] = {
    "hlt",
    "push",
    "pop",
    "add",
    "sub",
    "mul",
    "div",
    "out",
    "in",
    "sqrt",
    "jmp",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "cmp",
    "je",
    "jne",
    "ja",
    "jae",
    "jb",
    "jbe",
    "",
    "",
    "",
    "call",
    "ret",
    "recret",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "meow",
    ""
};

const char TEXT_REGISTERS[][4] = {
    "rax",
    "rbx",
    "rcx",
    "rdx"
};

const char RAM_TEXT_REGISTERS[][6] = {
    "[rax]",
    "[rbx]",
    "[rcx]",
    "[rdx]"
};

const int NUMBER_ARGUMENTS_FOR_OPERATION[] = {
    0,
    2,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    4,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

/*const int LENGTH_OF_TEXT_OPERATION[] = {
    3,
    4,
    3,
    3,
    3,
    3,
    3,
    3,
    2
};*/

