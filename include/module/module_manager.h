#pragma once

#include "common/pch.h"
#include "core/objects/module.h"
#include "common/type.h"

class MeowEngine;
class MemoryManager;

/**
 * @class ModuleManager
 * @brief Quản lí toàn bộ module được tải trong quá trình VM thực thi
 */
class ModuleManager {
public:
    Module load_module(String module_path, String importer_path);

    inline void reset_module_cache() noexcept {
        module_cache_.clear();
    }

    inline void add_cache(String name, const Module& mod) {
        module_cache_[name] = mod;
    }

    inline void set_engine(MeowEngine* engine) noexcept {
        engine_ = engine;
    }

    inline void set_heap(MemoryManager* heap) noexcept {
        heap_ = heap;
    }
private:
    std::unordered_map<String, Module> module_cache_;
    String entry_path_;

    MeowEngine* engine_;
    MemoryManager* heap_;
};