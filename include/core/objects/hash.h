#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "core/type.h"

class ObjHash : public MeowObject {
private:
    std::unordered_map<String, Value> fields_;
public:
    // --- Constructors ---
    explicit ObjHash(const std::unordered_map<String, Value>& fields): fields_(fields) {}
    explicit ObjHash(std::unordered_map<String, Value>&& fields) noexcept: fields_(std::move(fields)) {}

    // --- Iterator types ---
    using Iterator = std::unordered_map<String, Value>::iterator;
    using ConstIterator = std::unordered_map<String, Value>::const_iterator;

    // --- Lookup ---

    // Unchecked lookup. For performance-critical code
    [[nodiscard]] inline const Value& get(String key) noexcept {
        return fields_[key];
    }
    // Unchecked lookup/update. For performance-critical code
    template <typename T>
    inline void set(String key, T&& value) noexcept {
        fields_[key] = std::forward<T>(value);
    }
    // Checked lookup. Throws if key is not found
    [[nodiscard]] inline const Value& at(String key) const {
        return fields_.at(key);
    }
    [[nodiscard]] inline bool has(String key) const { return fields_.find(key) != fields_.end(); }

    // --- Capacity ---
    [[nodiscard]] inline size_t size() const noexcept { return fields_.size(); }
    [[nodiscard]] inline bool empty() const noexcept { return fields_.empty(); }

    // --- Iterators ---
    inline Iterator begin() noexcept { return fields_.begin(); }
    inline Iterator end() noexcept { return fields_.end(); }
    inline ConstIterator begin() const noexcept { return fields_.begin(); }
    inline ConstIterator end() const noexcept { return fields_.end();}

    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};
