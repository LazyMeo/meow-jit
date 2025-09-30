#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"

class ObjNativeFunction : public MeowObject {
private:
    class MeowEngine;
    using Arguments = const std::vector<Value>&;
    using NativeFnSimple = std::function<Value(Arguments)>;
    using NativeFnAdvanced = std::function<Value(MeowEngine*, Arguments)>;

    std::variant<NativeFnSimple, NativeFnAdvanced> function_;
public:
    ObjNativeFunction(NativeFnSimple f) : function_(f) {}
    ObjNativeFunction(NativeFnAdvanced f) : function_(f) {}

    inline Value call(Arguments args) {
        if (auto p = std::get_if<NativeFnSimple>(&function_)) {
            return (*p)(args);
        }

        return Value();
    }

    inline Value call(MeowEngine* engine, Arguments args) {
        if (auto p = std::get_if<NativeFnAdvanced>(&function_)) {
            return (*p)(engine, args);
        } else if (auto p = std::get_if<NativeFnSimple>(&function_)) {
            return (*p)(args);
        }

        return Value();
    }

    inline void trace([[maybe_unused]] GCVisitor& visitor) noexcept {}
};
