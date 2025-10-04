#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "core/type.h"

namespace meow::core::objects {
    class ObjModule : public MeowObject {
    private:
        using value_type = meow::core::Value;
        using const_reference_type = const value_type&;
        using string_type = meow::core::String;
        using proto_type = meow::core::Proto;
        using module_map = std::unordered_map<String, value_type>;
        using visitor_type = meow::memory::GCVisitor;

        enum class State { EXECUTING, EXECUTED };

        module_map globals_;
        module_map exports_;
        string_type file_name_;
        string_type file_path_;
        proto_type main_proto_;

        State state;
    public:

        explicit ObjModule(string_type file_name, string_type file_path, proto_type main_proto = nullptr) noexcept
            : file_name_(file_name), file_path_(file_path), main_proto_(main_proto) {}

        // --- Globals ---
        [[nodiscard]] inline const_reference_type get_global(string_type name) noexcept { return globals_[name]; }
        [[nodiscard]] inline void set_global(string_type name, const_reference_type value) noexcept { globals_[name] = value; } 
        [[nodiscard]] inline bool has_global(string_type name) { return globals_.find(name) != globals_.end(); }

        // --- Exports ---
        [[nodiscard]] inline const_reference_type get_export(string_type name) noexcept { return exports_[name]; }
        [[nodiscard]] inline void set_export(string_type name, const_reference_type value) noexcept { exports_[name] = value; }
        [[nodiscard]] inline bool has_export(string_type name) { return exports_.find(name) != exports_.end(); }

        // --- File info ---
        inline string_type get_file_name() const noexcept { return file_name_; }
        inline string_type get_file_path() const noexcept { return file_path_; }

        // --- Main proto ---
        inline proto_type get_main_proto() const noexcept { return main_proto_; }
        inline void set_main_proto(proto_type proto) noexcept { main_proto_ = proto; }
        inline bool is_has_main() const noexcept { return main_proto_ != nullptr; }

        // --- Execution state ---
        inline void set_execution() noexcept { state = State::EXECUTING; }
        inline void set_executed() noexcept { state = State::EXECUTED;}
        [[nodiscard]] inline bool is_executing() const noexcept { return state == State::EXECUTING; }
        [[nodiscard]] inline bool is_executed() const noexcept { return state == State::EXECUTED; }

        /// @brief Tracing all objects referenced
        void trace(visitor_type& visitor) const noexcept override;
    };
}