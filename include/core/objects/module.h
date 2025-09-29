#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "common/type.h"
#include "core/objects/function.h"

class ObjModule : public MeowObject {
private:
    String filename_;
    String filepath_;
    std::unordered_map<String, Value> globals_;
    std::unordered_map<String, Value> exports_;
    Proto main_proto_;

    bool has_main_ = false;
    bool is_excuting_ = false;
    bool is_executed_ = false;
public:

    ObjModule(String filename, String filepath, bool binary = false)
        : filename_(filename), filepath_(filepath) {}

    inline String get_file_name() const noexcept {
        return filename_;
    }

    inline String get_file_path() const noexcept {
        return filepath_;
    }

    inline Value get_global(String name) noexcept {
        return globals_[name];
    }

    inline void set_global(String name, Value value) noexcept {
        globals_[name] = value;
    } 

    inline bool has_global(String name) {
        return globals_.find(name) != globals_.end();
    }

    inline Value get_export(String name) noexcept {
        return exports_[name];
    }

    inline void set_export(String name, Value value) noexcept {
        exports_[name] = value;
    }

    inline bool has_export(String name) {
        return exports_.find(name) != exports_.end();
    }

    inline Proto get_main_proto() const noexcept {
        return main_proto_;
    }

    inline void set_main_proto(Proto proto) noexcept {
        main_proto_ = proto;
        has_main_ = true;
    }

    inline bool is_has_main() const noexcept {
        return has_main_;
    }

    inline bool is_excuting() const noexcept {
        return is_excuting_;
    }

    inline bool is_executed() const noexcept {
        return is_executed_;
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        for (auto& pair : globals_) visitor.visit_value(pair.second);
        for (auto& pair : exports_) visitor.visit_value(pair.second);
        visitor.visit_object(main_proto_);
    }
};