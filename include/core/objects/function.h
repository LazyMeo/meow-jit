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
    explicit UpvalueDesc(bool is_local = false, size_t index = 0) noexcept: is_local_(is_local), index_(index) {}
};

class ObjUpvalue : public MeowObject {
private:
    enum class State { OPEN, CLOSED };
    State state_ = State::OPEN;
    size_t index_ = 0;
    Value closed_ = Null{};
public:
    explicit ObjUpvalue(size_t index = 0) noexcept: index_(index) {}
    inline void close(Value value) noexcept { 
        closed_ = value; 
        state_ = State::CLOSED; 
    }
    inline bool is_closed() const noexcept { return state_ == State::CLOSED; }
    inline Value get_value() const noexcept { return closed_; }
    inline size_t get_index() const noexcept { return index_; }

    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};

class BytecodeParser;
class ObjFunctionProto : public MeowObject {
    friend class BytecodeParser; 
private:
    size_t num_registers_;
    size_t num_upvalues_;
    String name_;
    Chunk chunk_;
    std::vector<UpvalueDesc> upvalue_descs_;
public:
    explicit ObjFunctionProto(size_t registers = 0, size_t upvalues = 0, String name = nullptr, Chunk&& chunk) noexcept
        : num_registers_(registers), num_upvalues_(upvalues), name_(name), chunk_(std::move(chunk)) {}

    /// @brief Unchecked upvalue desc access. For performance-critical code
    [[nodiscard]] inline const UpvalueDesc& get_desc(size_t index) const noexcept {
        return upvalue_descs_[index];
    }
    /// @brief Checked upvalue desc access. For performence-critical code
    [[nodiscard]] inline const UpvalueDesc& at_desc(size_t index) const {
        return upvalue_descs_.at(index);
    }
    [[nodiscard]] inline size_t get_num_registers() const noexcept { return num_registers_; }
    [[nodiscard]] inline size_t get_num_upvalues() const noexcept { return num_upvalues_; }
    [[nodiscard]] inline String get_name() const noexcept { return name_; }
    [[nodiscard]] inline const Chunk& get_chunk() const noexcept { return chunk_; }
    [[nodiscard]] inline size_t desc_size() const noexcept { return upvalue_descs_.size(); }

    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};

class ObjClosure : public MeowObject {
private:
    Proto proto_;
    std::vector<Upvalue> upvalues_;
public:
    explicit ObjClosure(Proto proto = nullptr) noexcept: proto_(proto), upvalues_(proto ? proto->get_num_upvalues() : 0) {}

    [[nodiscard]] inline Proto get_proto() const noexcept { return proto_; }
    /// @brief Unchecked upvalue access. For performance-critical code
    [[nodiscard]] inline Upvalue get_upvalue(size_t index) const noexcept {
        return upvalues_[index];
    }
    /// @brief Unchecked upvalue modification. For performance-critical code
    [[nodiscard]] inline void set_upvalue(size_t index, Upvalue upvalue) noexcept {
        upvalues_[index] = upvalue;
    }
    /// @brief Checked upvalue access. Throws if index is OOB
    [[nodiscard]] inline Upvalue at_upvalue(size_t index) const {
        return upvalues_.at(index);
    }

    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};