#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "common/type.h"
#include "core/objects/function.h"

class ObjClass : public MeowObject {
private:
    String name_;
    Class superclass_;
    std::unordered_map<String, Value> methods_;
public:
    ObjClass(String name) : name_(name) {}

    inline String get_name() const noexcept {
        return name_;
    }

    inline Class get_super() const noexcept {
        return superclass_;
    }

    inline void set_super(Class super) noexcept {
        superclass_ = super;
    }

    inline bool has_method(String name) const noexcept {
        return methods_.find(name) != methods_.end();
    }

    inline Value get_method(String name) noexcept {
        return methods_[name];
    }

    inline void set_method(String name, Value value) noexcept {
        methods_[name] = value;
    }

    inline void trace(GCVisitor& visitor) const noexcept override {
        if (superclass_) {
            visitor.visit_object(superclass_);
        }
        for (auto& method : methods_) {
            visitor.visit_value(method.second);
        }
    }
};


class ObjInstance : public MeowObject {
private:
    Class klass_;
    std::unordered_map<String, Value> fields_;
public:
    ObjInstance(Class k = nullptr) : klass_(k) {}

    inline Class get_class() const noexcept {
        return klass_;
    }

    inline void set_class(Class klass) noexcept {
        klass_ = klass;
    }

    inline Value get_method(String name) noexcept {
        return fields_[name];
    }

    inline void set_method(String name, Value value) noexcept {
        fields_[name] = value;
    }

    inline bool has_method(String name) const {
        return fields_.find(name) != fields_.end();
    }
    
    inline void trace(GCVisitor& visitor) const noexcept override {
        visitor.visit_object(klass_);
        for (auto& pair : fields_) {
            visitor.visit_value(pair.second);
        }
    }
};

class ObjBoundMethod : public MeowObject {
private:
    Instance instance_;
    Function function_;
public:
    ObjBoundMethod(Instance instance = nullptr, Function function = nullptr) : instance_(instance), function_(function) {}

    inline Instance get_instance() const noexcept {
        return instance_;
    }

    inline Function get_function() const noexcept {
        return function_;
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        visitor.visit_object(instance_);
        visitor.visit_object(function_);
    }
};