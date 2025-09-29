#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/objects/function.h"

struct CallFrame {
    Function function_;
    Module module_;
    size_t start_reg_;
    size_t ret_reg_;
    size_t ip_;
    CallFrame(Function function, Module module, size_t start_reg, size_t ret_reg, size_t ip)
        : function_(function), module_(module), start_reg_(start_reg), ret_reg_(ret_reg), ip_(ip) {}
};

struct ExceptionHandler {
    size_t catch_ip_;
    size_t frame_depth_;
    size_t stack_depth_;
    ExceptionHandler(size_t catch_ip = 0, size_t frame_depth = 0, size_t stack_depth = 0) 
        : catch_ip_(catch_ip), frame_depth_(frame_depth), stack_depth_(stack_depth) {}
};

struct ExecutionContext {
    std::vector<CallFrame> call_stack_;
    std::vector<Value> registers_;
    std::vector<Upvalue> open_upvalues_;
    std::vector<ExceptionHandler> execption_handlers_;

    size_t current_base_ = 0;
    CallFrame* current_frame_ = nullptr;

    inline void reset() noexcept {
        call_stack_.clear();
        registers_.clear();
        open_upvalues_.clear();
        execption_handlers_.clear();
    }
};