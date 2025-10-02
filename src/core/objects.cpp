#include "core/objects.h"
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
    visitor.visit_object(name_);
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

void ObjModule::trace(GCVisitor& visitor) const noexcept {
    visitor.visit_object(file_name_);
    visitor.visit_object(file_path_);
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