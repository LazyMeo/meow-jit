#pragma once

class Value;
struct MeowObject;

struct GCVisitor {
    virtual ~GCVisitor() = default;
    virtual void visit_value(const Value& value) noexcept = 0;
    virtual void visit_object(const MeowObject* object) noexcept = 0;
};