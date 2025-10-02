#pragma once

enum class OpCode : unsigned char {
    // --- Load/store ---
    LOAD_CONST, LOAD_NULL, LOAD_TRUE, LOAD_FALSE, LOAD_INT, MOVE,
    // --- Binary ---
    ADD, SUB, MUL, DIV, MOD, POW, EQ, NEQ, GT, GE, LT, LE,
    // --- Unary ---
    NEG, NOT,
    // --- Constant & variable
    GET_GLOBAL, SET_GLOBAL, GET_UPVALUE, SET_UPVALUE, CLOSURE, CLOSE_UPVALUES,
    // --- Control flow ---
    JUMP, JUMP_IF_FALSE, JUMP_IF_TRUE, CALL, CALL_VOID, RETURN, HALT,
    // --- Data structure ---
    NEW_ARRAY, NEW_HASH, GET_INDEX, SET_INDEX, GET_KEYS, GET_VALUES,
    // --- Classes & objexts ---
    NEW_CLASS, NEW_INSTANCE, GET_PROP, SET_PROP, SET_METHOD, INHERIT, GET_SUPER,
    // --- Bitwise ---
    BIT_AND, BIT_OR, BIT_XOR, BIT_NOT, LSHIFT, RSHIFT,
    // --- Try/catch ---
    THROW, SETUP_TRY, POP_TRY,
    // --- Module ---
    IMPORT_MODULE, EXPORT, GET_EXPORT, IMPORT_ALL,
    // --- Metadata ---
    TOTAL_OPCODES
};