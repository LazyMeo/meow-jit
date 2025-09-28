#pragma once

#include "common/pch.h"

struct ObjString;
struct ObjArray;
struct ObjHash;
struct ObjClass;
struct ObjInstance;
struct ObjBoundMethod;
struct ObjUpvalue;
struct ObjFunctionProto;
struct ObjNativeFunction;
struct ObjClosure;
struct ObjModule;

using String = ObjString*;
using Array = ObjArray*;
using Hash = ObjHash*;

using Instance = ObjInstance*;
using Class = ObjClass*;
using Upvalue = ObjUpvalue*;
using NativeFn = ObjNativeFunction*;
using Function = ObjClosure*;
using Module = ObjModule*;
using BoundMethod = ObjBoundMethod*;
using Proto = ObjFunctionProto*;

struct MeowObject;

using Null = std::monostate;
using Bool = bool;
using Int = int64_t;
using Real = double;
using Object = MeowObject*;
