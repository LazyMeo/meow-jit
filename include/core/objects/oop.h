#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "core/type.h"

namespace meow::core::objects {
    class ObjClass : public MeowObject {
    private:
        using string_type = meow::core::String;
        using class_type = meow::core::Class;
        using value_type = meow::core::Value;
        using const_reference_type = const value_type&;
        using method_map = std::unordered_map<string_type, value_type>;
        using visitor_type = meow::memory::GCVisitor;

        string_type name_;
        class_type superclass_;
        method_map methods_;
    public:
        explicit ObjClass(string_type name = nullptr) noexcept: name_(name) {}

        // --- Metadata ---
        [[nodiscard]] inline string_type get_name() const noexcept { return name_; }
        [[nodiscard]] inline class_type get_super() const noexcept { return superclass_; }
        inline void set_super(class_type super) noexcept { superclass_ = super; }

        // --- Methods ---
        [[nodiscard]] inline bool has_method(string_type name) const noexcept { return methods_.find(name) != methods_.end(); }
        [[nodiscard]] inline const_reference_type get_method(string_type name) noexcept { return methods_[name]; }
        inline void set_method(string_type name, const_reference_type value) noexcept { methods_[name] = value; }

        /// @brief Tracing all objects referenced
        void trace(visitor_type& visitor) const noexcept override;
    };


    class ObjInstance : public MeowObject {
    private:
        using string_type = meow::core::String;
        using class_type = meow::core::Class;
        using value_type = meow::core::Value;
        using const_reference_type = const value_type&;
        using field_map = std::unordered_map<string_type, value_type>;
        using visitor_type = meow::memory::GCVisitor;

        class_type klass_;
        field_map fields_;
    public:
        explicit ObjInstance(class_type k = nullptr) noexcept: klass_(k) {}

        // --- Metadata ---
        [[nodiscard]] inline class_type get_class() const noexcept { return klass_; }
        inline void set_class(class_type klass) noexcept { klass_ = klass; }

        // --- Fields ---
        [[nodiscard]] inline const_reference_type get_field(string_type name) noexcept { return fields_[name];}
        [[nodiscard]] inline void set_field(string_type name, const_reference_type value) noexcept { fields_[name] = value; }
        [[nodiscard]] inline bool has_field(string_type name) const { return fields_.find(name) != fields_.end(); }
        
        /// @brief Tracing all objects referenced
        void trace(visitor_type& visitor) const noexcept override;
    };

    class ObjBoundMethod : public MeowObject {
    private:
        using instance_type = meow::core::Instance;
        using function_type = meow::core::Function;
        using visitor_type = meow::memory::GCVisitor;

        instance_type instance_;
        function_type function_;
    public:
        explicit ObjBoundMethod(instance_type instance = nullptr, function_type function = nullptr) noexcept: instance_(instance), function_(function) {}

        inline instance_type get_instance() const noexcept { return instance_; }
        inline function_type get_function() const noexcept { return function_; }

        /// @brief Tracing all objects referenced
        void trace(visitor_type& visitor) const noexcept override;
    };
}