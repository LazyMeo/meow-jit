#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "common/type.h"

class ObjModule : public MeowObject {
private:
    enum class State { EXECUTING, EXECUTED };

    String file_name_;
    String file_path_;
    std::unordered_map<String, Value> globals_;
    std::unordered_map<String, Value> exports_;
    Proto main_proto_;

    State state;
public:

    ObjModule(String file_name, String file_path, Proto main_proto = nullptr)
        : file_name_(file_name), file_path_(file_path), main_proto_(main_proto) {}

    inline String get_file_name() const noexcept {
        return file_name_;
    }

    inline String get_file_path() const noexcept {
        return file_path_;
    }

    inline Value get_global(String name) noexcept {
        return globals_[name];
    }

    inline void set_global(String name, Value value) noexcept {
        globals_[name] = value;
    } 

    inline bool has_global(String name) {
        return globals_.find(name) != globals_.end();
    }

    inline Value get_export(String name) noexcept {
        return exports_[name];
    }

    inline void set_export(String name, Value value) noexcept {
        exports_[name] = value;
    }

    inline bool has_export(String name) {
        return exports_.find(name) != exports_.end();
    }

    inline Proto get_main_proto() const noexcept {
        return main_proto_;
    }

    inline void set_main_proto(Proto proto) noexcept {
        main_proto_ = proto;
    }

    inline bool is_has_main() const noexcept {
        return main_proto_ != nullptr;
    }

    inline void set_execution() noexcept {
        state = State::EXECUTING;
    }

    inline void set_executed() noexcept {
        state = State::EXECUTED;
    }

    inline bool is_executing() const noexcept {
        return state == State::EXECUTING;
    }

    inline bool is_executed() const noexcept {
        return state == State::EXECUTED;
    }

    void trace(GCVisitor& visitor) const noexcept override;
};