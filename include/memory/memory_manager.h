#pragma once

#include "common/pch.h"
#include "memory/garbage_collector.h"

struct ExecutionContext;
class MemoryManager {
private:
    std::unique_ptr<GarbageCollector> gc_;

    size_t gc_threshold_;
    size_t object_allocated_;
    bool gc_enabled_ = true;
public:
    MemoryManager(std::unique_ptr<GarbageCollector> gc): gc_(std::move(gc)), gc_threshold_(1024), object_allocated_(0) {}

    template<typename T, typename... Args>
    T* new_object(Args&&... args) noexcept {
        if (object_allocated_ >= gc_threshold_ && gc_enabled_) {
            collect();
            gc_threshold_ *= 2;
        }
        T* newObj = new T(std::forward<Args>(args)...);
        gc->register_object(static_cast<MeowObject*>(newObj));
        ++object_allocated_;
        return newObj;
    }

    inline void enable_gc() noexcept {
        gc_enabled_ = true;
    }

    inline void disable_gc() noexcept {
        gc_enabled_ = false;
    }

    inline void collect() noexcept {
        object_allocated_ = gc_->collect();
    }
};