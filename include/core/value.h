/**
 * @file value.h
 * @author LazyPaws
 * @copyright Copyright(c) 2025 LazyPaws - All rights reserved
 */

#pragma once

#include "common/pch.h"

struct MeowObject;

using Bool = bool;
using Int64 = int64_t;
using Float64 = double;
using Object = MeowObject*;

enum class ValueType : uint8_t {
    Bool,
    Int64,
    Float64,
    Object
};

export class Value {
private:
    union {
        bool b;
        int64_t i64;
        double f32;
        MeowObject* obj;
    } data;
    ValueType type;


};