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
    Module load_module(String module_path, String importer_path, MemoryManager* heap, MeowEngine* engine);

    inline void reset_module_cache() noexcept {
        module_cache_.clear();
    }

    inline void add_cache(String name, const Module& mod) {
        module_cache_[name] = mod;
    }
private:
    std::unordered_map<String, Module> module_cache_;
    String entry_path_;

    MeowEngine* engine_;
    MemoryManager* heap_;
};