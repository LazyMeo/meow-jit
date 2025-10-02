#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"

class MeowEngine;
class ObjNativeFunction : public MeowObject {
public:
    using Arguments = const std::vector<Value>&;
    using NativeFnSimple = std::function<Value(Arguments)>;
    using NativeFnAdvanced = std::function<Value(MeowEngine*, Arguments)>;
private:
    std::variant<NativeFnSimple, NativeFnAdvanced> function_;
public:
    explicit ObjNativeFunction(NativeFnSimple f): function_(f) {}
    explicit ObjNativeFunction(NativeFnAdvanced f): function_(f) {}

    [[nodiscard]] inline Value call(Arguments args) {
        if (auto p = std::get_if<NativeFnSimple>(&function_)) {
            return (*p)(args);
        }

        return Value();
    }
    [[nodiscard]] inline Value call(MeowEngine* engine, Arguments args) {
        if (auto p = std::get_if<NativeFnAdvanced>(&function_)) {
            return (*p)(engine, args);
        } else if (auto p = std::get_if<NativeFnSimple>(&function_)) {
            return (*p)(args);
        }

        return Value();
    }

    inline void trace([[maybe_unused]] GCVisitor& visitor) noexcept {}
};
