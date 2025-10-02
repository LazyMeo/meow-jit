#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "common/type.h"

class ObjClass : public MeowObject {
private:
    String name_;
    Class superclass_;
    std::unordered_map<String, Value> methods_;
public:
    explicit ObjClass(String name = nullptr) noexcept: name_(name) {}

    // --- Metadata ---
    [[nodiscard]] inline String get_name() const noexcept { return name_; }
    [[nodiscard]] inline Class get_super() const noexcept { return superclass_; }
    inline void set_super(Class super) noexcept { superclass_ = super; }

    // --- Methods ---
    [[nodiscard]] inline bool has_method(String name) const noexcept { return methods_.find(name) != methods_.end(); }
    [[nodiscard]] inline Value get_method(String name) noexcept { return methods_[name]; }
    inline void set_method(String name, Value value) noexcept { methods_[name] = value; }

    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};


class ObjInstance : public MeowObject {
private:
    Class klass_;
    std::unordered_map<String, Value> fields_;
public:
    explicit ObjInstance(Class k = nullptr) noexcept: klass_(k) {}

    // --- Metadata ---
    [[nodiscard]] inline Class get_class() const noexcept { return klass_; }
    inline void set_class(Class klass) noexcept { klass_ = klass; }

    // --- Fields ---
    [[nodiscard]] inline Value get_field(String name) noexcept { return fields_[name];}
    [[nodiscard]] inline void set_field(String name, Value value) noexcept { fields_[name] = value; }
    [[nodiscard]] inline bool has_field(String name) const { return fields_.find(name) != fields_.end(); }
    
    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};

class ObjBoundMethod : public MeowObject {
private:
    Instance instance_;
    Function function_;
public:
    explicit ObjBoundMethod(Instance instance = nullptr, Function function = nullptr) noexcept: instance_(instance), function_(function) {}

    inline Instance get_instance() const noexcept { return instance_; }
    inline Function get_function() const noexcept { return function_; }

    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};