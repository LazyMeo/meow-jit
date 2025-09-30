#include "core/objects/array.h"
#include "core/objects/string.h"
#include "core/objects/hash.h"
#include "core/objects/oop.h"
#include "core/objects/function.h"
#include "core/objects/native.h"
#include "core/objects/module.h"
#include "memory/gc_visitor.h"

void ObjArray::trace(GCVisitor& visitor) const noexcept {
    for (const auto& element : elements_) {
        visitor.visit_value(element);
    }
}

void ObjHash::trace(GCVisitor& visitor) const noexcept {
    for (const auto& [key, value] : fields_) {
        visitor.visit_object(key);
        visitor.visit_value(value);
    }
}

void ObjClass::trace(GCVisitor& visitor) const noexcept {
    visitor.visit_object(name_);
    visitor.visit_object(superclass_);
    for (const auto& [name, method] : methods_) {
        visitor.visit_object(name);
        visitor.visit_value(method);
    }
}

void ObjInstance::trace(GCVisitor& visitor) const noexcept {
    visitor.visit_object(klass_);
    for (const auto& [key, value] : fields_) {
        visitor.visit_object(key);
        visitor.visit_value(value);
    }
}

void ObjBoundMethod::trace(GCVisitor& visitor) const noexcept {
    visitor.visit_object(instance_);
    visitor.visit_object(function_);
}

void ObjUpvalue::trace(GCVisitor& visitor) const noexcept {
    visitor.visit_value(closed_);
}

void ObjFunctionProto::trace(GCVisitor& visitor) const noexcept {
    for (const auto& constant : chunk_.constant_pool_) {
        visitor.visit_value(constant);
    }
}

void ObjClosure::trace(GCVisitor& visitor) const noexcept {
    visitor.visit_object(proto_);
    for (const auto& upvalue : upvalues_) {
        visitor.visit_object(upvalue);
    }
}

inline void ObjModule::trace(GCVisitor& visitor) const noexcept {
    visitor.visit_object(filename_);
    visitor.visit_object(filepath_);
    for (const auto& [key, value] : globals_) {
        visitor.visit_object(key);
        visitor.visit_value(value);
    }
    for (const auto& [key, value] : exports_) {
        visitor.visit_object(key);
        visitor.visit_value(value);
    }
    visitor.visit_object(main_proto_);
}