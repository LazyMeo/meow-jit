#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "core/type.h"

class ObjModule : public MeowObject {
private:
    enum class State { EXECUTING, EXECUTED };

    std::unordered_map<String, Value> globals_;
    std::unordered_map<String, Value> exports_;
    String file_name_;
    String file_path_;
    Proto main_proto_;

    State state;
public:

    explicit ObjModule(String file_name, String file_path, Proto main_proto = nullptr) noexcept
        : file_name_(file_name), file_path_(file_path), main_proto_(main_proto) {}

    // --- Globals ---
    [[nodiscard]] inline Value get_global(String name) noexcept { return globals_[name]; }
    [[nodiscard]] inline void set_global(String name, Value value) noexcept { globals_[name] = value; } 
    [[nodiscard]] inline bool has_global(String name) { return globals_.find(name) != globals_.end(); }

    // --- Exports ---
    [[nodiscard]] inline Value get_export(String name) noexcept { return exports_[name]; }
    [[nodiscard]] inline void set_export(String name, Value value) noexcept { exports_[name] = value; }
    [[nodiscard]] inline bool has_export(String name) { return exports_.find(name) != exports_.end(); }

    // --- File info ---
    inline String get_file_name() const noexcept { return file_name_; }
    inline String get_file_path() const noexcept { return file_path_; }

    // --- Main proto ---
    inline Proto get_main_proto() const noexcept { return main_proto_; }
    inline void set_main_proto(Proto proto) noexcept { main_proto_ = proto; }
    inline bool is_has_main() const noexcept { return main_proto_ != nullptr; }

    // --- Execution state ---
    inline void set_execution() noexcept { state = State::EXECUTING; }
    inline void set_executed() noexcept { state = State::EXECUTED;}
    [[nodiscard]] inline bool is_executing() const noexcept { return state == State::EXECUTING; }
    [[nodiscard]] inline bool is_executed() const noexcept { return state == State::EXECUTED; }

    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};