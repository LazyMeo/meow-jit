#pragma once

#include "common/pch.h"
#include "core/definition.h"

struct ExecutionContext {
    std::vector<CallFrame> call_stack;
    std::vector<Value> registers;
    std::vector<Upvalue> open_upvalues;
    std::vector<ExceptionHandler> execption_handlers;

    size_t currentBase = 0;

    inline void reset() noexcept {
        call_stack.clear();
        registers.clear();
        open_upvalues.clear();
        execption_handlers.clear();
    }
};