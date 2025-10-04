#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"

namespace meow::vm { class MeowEngine; }

namespace meow::core::objects {
    class ObjNativeFunction : public MeowObject {
    private:
        using value_type = meow::core::Value;
        using engine_type = meow::vm::MeowEngine;
        using visitor_type = meow::memory::GCVisitor;
    public:
        using arguments = const std::vector<value_type>&;
        using native_fn_simple = std::function<value_type(arguments)>;
        using native_fn_double = std::function<value_type(engine_type*, arguments)>;
    private:
        std::variant<native_fn_simple, native_fn_double> function_;
    public:
        explicit ObjNativeFunction(native_fn_simple f): function_(f) {}
        explicit ObjNativeFunction(native_fn_double f): function_(f) {}

        [[nodiscard]] inline value_type call(arguments args) {
            if (auto p = std::get_if<native_fn_simple>(&function_)) {
                return (*p)(args);
            }
            return value_type();
        }
        [[nodiscard]] inline value_type call(engine_type* engine, arguments args) {
            if (auto p = std::get_if<native_fn_double>(&function_)) {
                return (*p)(engine, args);
            } else if (auto p = std::get_if<native_fn_simple>(&function_)) {
                return (*p)(args);
            }
            return value_type();
        }

        inline void trace(visitor_type&) noexcept {}
    };
}