#pragma once

#include "common/pch.h"
#include "memory/gc_visitor.h"
#include "memory/garbage_collector.h"

struct ExecutionContext;

struct GCMetadata {
    bool is_marked = false;
};

class MarkSweepGC : public GarbageCollector, public GCVisitor {
private:
    std::unordered_map<MeowObject*, GCMetadata> metadata_;
    ExecutionContext* context_ = nullptr;

public:
    ~MarkSweepGC() override;

    void register_object(MeowObject* object) noexcept override;

    size_t collect(ExecutionContext& context) noexcept override;

    void visit_value(Value& value) noexcept override;

    void visit_object(MeowObject* obj) noexcept override;

private:
    void mark(MeowObject* obj);
};