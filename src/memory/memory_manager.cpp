#include "memory/memory_manager.h"
#include "core/objects.h"

MemoryManager::MemoryManager(std::unique_ptr<GarbageCollector> gc) 
    : gc_(std::move(gc)), gc_threshold_(1024), object_allocated_(0) {}

MemoryManager::~MemoryManager() = default;

String MemoryManager::new_string(const std::string& string) {
    auto it = string_pool_.find(string);
    if (it != string_pool_.end()) {
        return it->second;
    }

    String new_string_object = new_object<ObjString>(string);
    string_pool_[string] = new_string_object;
    return new_string_object;
}

String MemoryManager::new_string(const char* chars, size_t length) {
    return new_string(std::string(chars, length));
}

Array MemoryManager::new_array(const std::vector<Value>& elements) {
    return new_object<ObjArray>(elements);
}

Hash MemoryManager::new_hash(const std::unordered_map<String, Value>& fields) {
    return new_object<ObjHash>(fields);
}

Upvalue MemoryManager::new_upvalue(size_t index) {
    return new_object<ObjUpvalue>(index);
}

Proto MemoryManager::new_proto(size_t registers, size_t upvalues, String name, Chunk&& chunk) {
    return new_object<ObjFunctionProto>(registers, upvalues, name, std::move(chunk));
}

Function MemoryManager::new_function(Proto proto) {
    return new_object<ObjClosure>(proto); 
}

Module MemoryManager::new_module(String file_name, String file_path, Proto main_proto) {
    return new_object<ObjModule>(file_name, file_path, main_proto);
}

NativeFn MemoryManager::new_native(ObjNativeFunction::NativeFnSimple fn) {
    return new_object<ObjNativeFunction>(fn);
}

NativeFn MemoryManager::new_native(ObjNativeFunction::NativeFnAdvanced fn) {
    return new_object<ObjNativeFunction>(fn);
}

Class MemoryManager::new_class(String name) {
    return new_object<ObjClass>(name);
}

Instance MemoryManager::new_instance(Class klass) {
    return new_object<ObjInstance>(klass);
}

BoundMethod MemoryManager::new_bound_method(Instance instance, Function function) {
    return new_object<ObjBoundMethod>(instance, function);
}