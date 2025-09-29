/**
 * @file value.h
 * @author LazyPaws
 * @copyright Copyright(c) 2025 LazyPaws - All rights reserved
 */

#pragma once

#include "common/pch.h"

struct MeowObject;

/**
 * @class Value
 * @brief Đại diện cho kiểu dữ liệu động trong TrangMeo
 */
class Value {
private:
    // NOTE: Đoạn này hơi cân nhắc tí, vì tagged-union có thể nhanh hơn std::variant
    // Nhưng, bottle-neck không nằm ở Value, không ở std::variant
    // Nếu tốt hơn có thể dùng NaN-Boxing nhưng chỉ dùng được trên 64-bit, không dùng trên 32-bit được
    // Khi thực sự đây là bottle-neck thì mới thay đổi

    using Null = std::monostate;
    using Bool = bool;
    using Int = int64_t;
    using Real = double;
    using Object = MeowObject*;

    // enum class ValueType {
    //     Null,
    //     Bool,
    //     Int,
    //     Real,
    //     Object
    // };

    // union {
    //     bool b;
    //     int64_t i64;
    //     double f64;
    //     MeowObject* obj;
    // } data_;
    // ValueType type_;

    using BaseValue = std::variant<Null, Bool, Int, Real, Object>;
    BaseValue data_;
public:
    // Value(): type_(ValueType::Null) { data_.b = 0; }
    // Value(std::monostate): type_(ValueType::Null) { data_.b = 0; }
    // Value(bool b): type_(ValueType::Bool) { data_.b = b; }
    // Value(int64_t i): type_(ValueType::Int) { data_.i64 = i; }
    // Value(double f): type_(ValueType::Real) { data_.f64 = f; }
    // Value(MeowObject* o): type_(ValueType::Object) { data_.obj = o; }

    // inline bool is_null() const noexcept { return type_ == ValueType::Null; }
    // inline bool is_bool() const noexcept { return type_ == ValueType::Bool; }
    // inline bool is_int() const noexcept { return type_ == ValueType::Int; }
    // inline bool is_real() const noexcept { return type_ == ValueType::Real; }
    // inline bool is_object() const noexcept { return type_ == ValueType::Object; }

    // inline bool as_bool() const noexcept { return data_.b; }
    // inline int64_t as_int() const noexcept { return data_.i64; }
    // inline double as_real() const noexcept { return data_.f64; }
    // inline MeowObject* as_object() const noexcept { return data_.obj; }

    Value(): data_(Null{}) {}
    Value(std::monostate): data_(Null{}) {}
    Value(bool b): data_(b) {}
    Value(int64_t i): data_(i) {}
    Value(double f): data_(f) {}
    Value(MeowObject* o): data_(o) {}

    inline bool is_bool() const noexcept { return std::holds_alternative<Bool>(data_); }
    inline bool is_int() const noexcept { return std::holds_alternative<Int>(data_); }
    inline bool is_real() const noexcept { return std::holds_alternative<Real>(data_); }
    inline bool is_object() const noexcept { return std::holds_alternative<Object>(data_); }

    inline bool as_bool() const noexcept { return std::get<Bool>(data_); }
    inline int64_t as_int() const noexcept { return std::get<Int>(data_); }
    inline double as_real() const noexcept { return std::get<Real>(data_); }
    inline MeowObject* as_object() const noexcept { return std::get<Object>(data_); }
};