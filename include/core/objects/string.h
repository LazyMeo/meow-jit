#pragma once

#include "common/pch.h"
#include "core/meow_object.h"

class ObjString : public MeowObject {
private:
    std::string data_;
public:
    // --- Constructors ---
    explicit ObjString(const std::string& data): data_(data) {}
    explicit ObjString(std::string&& data) noexcept: data_(std::move(data)) {}
    explicit ObjString(const char* data): data_(data) {}

    // --- Iterator types ---
    using ConstIterator = std::string::const_iterator;
    using ConstReverseIterator = std::string::const_reverse_iterator;

    // --- Character access ---

    /// @brief Unchecked character access. For performance-critical code
    [[nodiscard]] inline char get(size_t index) const noexcept {
        return data_[index];
    }
    /// @brief Checked character access. Throws if index is OOB
    [[nodiscard]] inline char at(size_t index) const {
        return data_.at(index);
    }

    // --- String access ---
    [[nodiscard]] inline const char* c_str() const noexcept {
        return data_.c_str();
    }

    // --- Capacity ---
    [[nodiscard]] inline size_t size() const noexcept { return data_.size(); }
    [[nodiscard]] inline bool empty() const noexcept { return data_.empty(); }

    // --- Iterators ---
    inline ConstIterator begin() const noexcept { return data_.begin(); }
    inline ConstIterator end() const noexcept { return data_.end(); }
    inline ConstReverseIterator rbegin() const noexcept { return data_.rbegin(); }
    inline ConstReverseIterator rend() const noexcept { return data_.rend(); }

    inline void trace([[maybe_unused]] GCVisitor& visitor) const noexcept override {}
};
