#pragma once

#include "common/pch.h"
#include "memory/gc_visitor.h"
#include "memory/garbage_collector.h"

struct ExecutionContext;
struct BuiltinRegistry;

struct GCMetadata { bool is_marked_ = false; };

class MarkSweepGC : public GarbageCollector, public GCVisitor {
private:
    std::unordered_map<const MeowObject*, GCMetadata> metadata_;
    ExecutionContext* context_ = nullptr;
    BuiltinRegistry* builtins_ = nullptr;
public:
    explicit MarkSweepGC(ExecutionContext* context, BuiltinRegistry* builtins) noexcept: context_(context), builtins_(builtins) {}
    ~MarkSweepGC() override;

    // -- Collector ---
    void register_object(const MeowObject* object) override;
    size_t collect() noexcept override;

    // --- Visitor ---
    void visit_value(const Value& value) noexcept override;
    void visit_object(const MeowObject* object) noexcept override;
private:
    void mark(const MeowObject* object);
};