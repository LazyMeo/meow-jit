#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "common/type.h"
#include "runtime/chunk.h"

struct UpvalueDesc {
    bool is_local_;
    size_t index_;
    UpvalueDesc(bool is_local = false, size_t index = 0) : is_local_(is_local), index_(index) {}
};

class ObjUpvalue : public MeowObject {
private:
    enum class State { OPEN, CLOSED };
    State state_ = State::OPEN;
    size_t index_ = 0;
    Value closed_ = Null{};
public:
    ObjUpvalue(size_t index = 0) : index_(index) {}
    inline void close(Value value) noexcept { 
        closed_ = value; 
        state_ = State::CLOSED; 
    }

    inline bool is_closed() const noexcept {
        return state_ == State::CLOSED;
    }

    inline Value get_value() const noexcept {
        return closed_;
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        visitor.visit_value(closed_);
    }
};

class BytecodeParser;
class ObjFunctionProto : public MeowObject {
    friend class BytecodeParser; 
private:
    size_t num_registers_;
    size_t num_upvalues_;
    String name_;
    Chunk chunk_;
    std::vector<UpvalueDesc> upvalueDescs;
public:

    ObjFunctionProto(size_t registers = 0, size_t upvalues = 0, String name)
        : num_registers_(registers), num_upvalues_(upvalues), name_(name) {}

    inline size_t get_num_registers() const noexcept {
        return num_registers_;
    }

    inline size_t get_num_upvalues() const noexcept {
        return num_upvalues_;
    }

    inline String get_name() const noexcept {
        return name_;
    }

    inline const Chunk& get_chunk() const noexcept {
        return chunk_;
    }

    inline const UpvalueDesc& get_desc(size_t index) const noexcept {
        return upvalueDescs[index];
    }

    inline size_t desc_size() const noexcept {
        return upvalueDescs.size();
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        for (auto& constant : chunk_.constant_pool) {
            visitor.visit_value(constant);
        }
    }
};

class ObjClosure : public MeowObject {
private:
    Proto proto_;
    std::vector<Upvalue> upvalues_;
public:
    ObjClosure(Proto p = nullptr) : proto_(p), upvalues_(p ? p->get_num_upvalues() : 0) {}

    inline Proto get_proto() const noexcept {
        return proto_;
    }

    inline Upvalue get_upvalue(size_t index) noexcept {
        return upvalues_[index];
    }

    inline void set_upvalue(size_t index, Upvalue upvalue) noexcept {
        upvalues_[index] = upvalue;
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        visitor.visit_object(proto_);
        for (auto& upvalue : upvalues_) {
            visitor.visit_object(upvalue);
        }
    }
};