#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/op_code.h"

class MemoryManager;

constexpr size_t NUM_VALUE_TYPES = static_cast<size_t>(ValueType::TotalValueTypes); 
constexpr size_t NUM_OPCODES = static_cast<size_t>(OpCode::TOTAL_OPCODES);

inline ValueType get_value_type(const Value& value) noexcept {
    if (value.is_null()) return ValueType::Null;
    if (value.is_int()) return ValueType::Int;
    if (value.is_real()) return ValueType::Real;
    if (value.is_object()) return ValueType::Bool;

    return ValueType::Null;
}

using BinaryOpFunction = Value(*)(const Value&, const Value&);
using UnaryOpFunction = Value(*)(const Value&);

class OperatorDispatcher {
private:
    MemoryManager* heap_;
    
    BinaryOpFunction binary_dispatch_table_[NUM_OPCODES][NUM_VALUE_TYPES][NUM_VALUE_TYPES];
    UnaryOpFunction unary_dispatch_table_[NUM_OPCODES][NUM_VALUE_TYPES];

public:
    OperatorDispatcher(MemoryManager* heap);

    inline const BinaryOpFunction* find(OpCode op_code, const Value& left, const Value& right) const noexcept {
        auto left_type = get_value_type(left);
        auto right_type = get_value_type(right);
        const BinaryOpFunction* function = &binary_dispatch_table_[static_cast<size_t>(op_code)][static_cast<size_t>(static_cast<size_t>(left_type))][static_cast<size_t>(right_type)];
        return (*function) ? function : nullptr;
    }

    inline const UnaryOpFunction* find(OpCode op_code, const Value& right) const noexcept {
        auto right_type = get_value_type(right);
        const UnaryOpFunction* function = &unary_dispatch_table_[static_cast<size_t>(op_code)][static_cast<size_t>(right_type)];
        return (*function) ? function : nullptr;
    }
};