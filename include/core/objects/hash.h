#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "core/type.h"

class ObjHash : public MeowObject {
private:
    using key_type = String;
    using value_type = Value;
    using reference_type = value_type&;
    using const_reference_type = const value_type&;
    using map_type = std::unordered_map<key_type, value_type>;
    map_type fields_;
public:
    // --- Constructors & destructor---
    ObjHash() = default;
    explicit ObjHash(const map_type& fields): fields_(fields) {}
    explicit ObjHash(map_type&& fields) noexcept: fields_(std::move(fields)) {}

    // --- Rule of 5 ---
    ObjHash(const ObjHash&) = delete;
    ObjHash(ObjHash&&) = delete;
    ObjHash& operator=(const ObjHash&) = delete;
    ObjHash& operator=(ObjHash&&) = delete;
    ~ObjHash() override = default;

    // --- Iterator types ---
    using iterator = map_type::iterator;
    using const_iterator = map_type::const_iterator;

    // --- Lookup ---

    // Unchecked lookup. For performance-critical code
    [[nodiscard]] inline const_reference_type get(key_type key) noexcept {
        return fields_[key];
    }
    // Unchecked lookup/update. For performance-critical code
    template <typename T> inline void set(key_type key, T&& value) noexcept {
        fields_[key] = std::forward<T>(value);
    }
    // Checked lookup. Throws if key is not found
    [[nodiscard]] inline const_reference_type at(key_type key) const {
        return fields_.at(key);
    }
    [[nodiscard]] inline bool has(key_type key) const { return fields_.find(key) != fields_.end(); }

    // --- Capacity ---
    [[nodiscard]] inline size_t size() const noexcept { return fields_.size(); }
    [[nodiscard]] inline bool empty() const noexcept { return fields_.empty(); }

    // --- Iterators ---
    inline iterator begin() noexcept { return fields_.begin(); }
    inline iterator end() noexcept { return fields_.end(); }
    inline const_iterator begin() const noexcept { return fields_.begin(); }
    inline const_iterator end() const noexcept { return fields_.end();}

    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};
