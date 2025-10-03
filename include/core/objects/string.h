#pragma once

#include "common/pch.h"
#include "core/meow_object.h"

class ObjString : public MeowObject {
private:
    using string_type = std::string;
    string_type data_;
public:
    // --- Constructors & destructor ---
    ObjString() = default;
    explicit ObjString(const string_type& data): data_(data) {}
    explicit ObjString(string_type&& data) noexcept: data_(std::move(data)) {}
    explicit ObjString(const char* data): data_(data) {}

    // --- Rule of 5 ---
    ObjString(const ObjString&) = delete;
    ObjString(ObjString&&) = delete;
    ObjString& operator=(const ObjString&) = delete;
    ObjString& operator=(ObjString&&) = delete;
    ~ObjString() override = default;

    // --- Iterator types ---
    using const_iterator = string_type::const_iterator;
    using const_reverse_iterator = string_type::const_reverse_iterator;

    // --- Character access ---

    /// @brief Unchecked character access. For performance-critical code
    [[nodiscard]] inline char get(size_t index) const noexcept { return data_[index]; }
    /// @brief Checked character access. Throws if index is OOB
    [[nodiscard]] inline char at(size_t index) const { return data_.at(index); }

    // --- String access ---
    [[nodiscard]] inline const char* c_str() const noexcept { return data_.c_str(); }

    // --- Capacity ---
    [[nodiscard]] inline size_t size() const noexcept { return data_.size(); }
    [[nodiscard]] inline bool empty() const noexcept { return data_.empty(); }

    // --- Iterators ---
    inline const_iterator begin() const noexcept { return data_.begin(); }
    inline const_iterator end() const noexcept { return data_.end(); }
    inline const_reverse_iterator rbegin() const noexcept { return data_.rbegin(); }
    inline const_reverse_iterator rend() const noexcept { return data_.rend(); }

    inline void trace([[maybe_unused]] GCVisitor& visitor) const noexcept override {}
};
