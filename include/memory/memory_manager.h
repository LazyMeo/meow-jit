#pragma once

#include "common/pch.h"
#include "memory/garbage_collector.h"
#include "core/objects/native.h"
#include "common/type.h"

struct ExecutionContext;
class MemoryManager {
private:
    std::unique_ptr<GarbageCollector> gc_;
    std::unordered_map<std::string, String> string_pool_;

    size_t gc_threshold_;
    size_t object_allocated_;
    bool gc_enabled_ = true;

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
public:
    MemoryManager(std::unique_ptr<GarbageCollector> gc);

    String new_string(const std::string& string);
    String new_string(const char* chars, size_t length);

    Array new_array(const std::vector<Value>& elements = {});
    Hash new_hash(const std::unordered_map<String, Value>& fields = {});
    Upvalue new_upvalue(size_t index);
    Proto new_proto(size_t registers, size_t upvalues, String name, Chunk&& chunk);
    Function new_function(Proto proto);
    Module new_module(String file_name, String file_path, Proto main_proto = nullptr);
    NativeFn new_native(ObjNativeFunction::NativeFnSimple fn);
    NativeFn new_native(ObjNativeFunction::NativeFnAdvanced fn);
    Class new_class(String name = nullptr);
    Instance new_instance(Class klass);
    BoundMethod new_bound_method(Instance instance, Function function);

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