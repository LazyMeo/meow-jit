#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/op_code.h"

class Value;
class MemoryManager;

enum class ValueType : uint8_t {
    Null, Int, Real, Bool, String, Array, Object, Upvalue, Function, Class, Instance, BoundMethod, Proto, NativeFn, TotalValueTypes
};

constexpr size_t NUM_VALUE_TYPES = static_cast<size_t>(ValueType::TotalValueTypes); 
constexpr size_t NUM_OPCODES = static_cast<size_t>(OpCode::TOTAL_OPCODES);

using BinaryOpFunction = std::function<Value(const Value&, const Value&)>;
using UnaryOpFunction = std::function<Value(const Value&)>;

class OperatorDispatcher {
private:
    MemoryManager* heap_;
    
    BinaryOpFunction binary_dispatch_table_[NUM_OPCODES][NUM_VALUE_TYPES][NUM_VALUE_TYPES];
    UnaryOpFunction unary_dispatch_table_[NUM_OPCODES][NUM_VALUE_TYPES];

public:
    OperatorDispatcher();

    inline BinaryOpFunction* find(OpCode op_code, ValueType left, ValueType right) {
        BinaryOpFunction* func = &binary_dispatch_table_[static_cast<size_t>(op_code)][left][right];
        return (*func) ? func : nullptr;
    }

    inline UnaryOpFunction* find(OpCode op_code, ValueType right) {
        UnaryOpFunction* func = &unary_dispatch_table_[static_cast<size_t>(op_code)][right];
        return (*func) ? func : nullptr;
    }

    inline void setMemoryManager(MemoryManager* heap) noexcept {
        heap_ = heap;
    }
};