#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/op_code.h"

class Value;
class MemoryManager;

constexpr size_t NUM_VALUE_TYPES = static_cast<size_t>(ValueType::TotalValueTypes); 
constexpr size_t NUM_OPCODES = static_cast<size_t>(OpCode::TOTAL_OPCODES);

// using BinaryOpFunction = std::function<Value(const Value&, const Value&)>;
// using UnaryOpFunction = std::function<Value(const Value&)>;
using BinaryOpFunction = Value(*)(const Value&, const Value&);
using UnaryOpFunction = Value(*)(const Value&);

class OperatorDispatcher {
private:
    MemoryManager* heap_;
    
    BinaryOpFunction binary_dispatch_table_[NUM_OPCODES][NUM_VALUE_TYPES][NUM_VALUE_TYPES];
    UnaryOpFunction unary_dispatch_table_[NUM_OPCODES][NUM_VALUE_TYPES];

public:
    OperatorDispatcher(MemoryManager* heap);

    inline const BinaryOpFunction* find(OpCode op_code, ValueType left, ValueType right) const noexcept {
        const BinaryOpFunction* function = &binary_dispatch_table_[static_cast<size_t>(op_code)][static_cast<size_t>(static_cast<size_t>(left))][static_cast<size_t>(right)];
        return (*function) ? function : nullptr;
    }

    inline const UnaryOpFunction* find(OpCode op_code, ValueType right) const noexcept {
        const UnaryOpFunction* function = &unary_dispatch_table_[static_cast<size_t>(op_code)][static_cast<size_t>(right)];
        return (*function) ? function : nullptr;
    }
};