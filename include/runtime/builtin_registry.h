#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "common/type.h"

struct BuiltinRegistry {
    std::unordered_map<String, std::unordered_map<String, Value>> methods_;
    std::unordered_map<String, std::unordered_map<String, Value>> getters_;
};