#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"

class ObjArray : public MeowObject {
private:
    std::vector<Value> elements_;
public:
    // --- Constructors ---
    explicit ObjArray(const std::vector<Value>& elements) : elements_(elements) {}
    explicit ObjArray(std::vector<Value>&& elements) noexcept : elements_(std::move(elements)) {}
    explicit ObjArray(std::initializer_list<Value> elements) : elements_(elements) {}

    // --- Iterator types ---
    using Iterator = std::vector<Value>::iterator;
    using ConstIterator = std::vector<Value>::const_iterator;
    using ReverseIterator = std::vector<Value>::reverse_iterator;
    using ConstReverseIterator = std::vector<Value>::const_reverse_iterator;

    // --- Element access ---
    
    /// @brief Unchecked element access. For performance-critical code
    [[nodiscard]] inline const Value& get(size_t index) const noexcept {
        return elements_[index];
    }
    /// @brief Unchecked element modification. For performance-critical code
    template <typename T>
    inline void set(size_t index, T&& value) noexcept {
        elements_[index] = std::forward<T>(value);
    }
    /// @brief Checked element access. Throws if index is OOB
    [[nodiscard]] inline const Value& at(size_t index) const {
        return elements_.at(index);
    }

    // --- Capacity ---
    [[nodiscard]] inline size_t size() const noexcept { return elements_.size(); }
    [[nodiscard]] inline bool empty() const noexcept { return elements_.empty(); }
    [[nodiscard]] inline size_t capacity() const noexcept { return elements_.capacity(); }

    // --- Modifiers ---
    template <typename T>
    inline void push(T&& value) { 
        elements_.emplace_back(std::forward<T>(value));
    }
    inline void pop() noexcept { elements_.pop_back(); }
    template <typename... Args>
    inline void emplace(Args&&... args) {
        elements_.emplace_back(std::forward<Args>(args)...);
    }
    inline void resize(size_t size) { elements_.resize(size); }
    inline void reserve(size_t capacity) { elements_.reserve(capacity); }
    inline void shrink_to_fit() { elements_.shrink_to_fit(); }

    // --- Iterators ---
    inline Iterator begin() noexcept { return elements_.begin(); }
    inline Iterator end() noexcept { return elements_.end(); }
    inline ConstIterator begin() const noexcept { return elements_.begin(); }
    inline ConstIterator end() const noexcept { return elements_.end(); }
    inline ReverseIterator rbegin() noexcept { return elements_.rbegin(); }
    inline ReverseIterator rend() noexcept { return elements_.rend(); }
    inline ConstReverseIterator rbegin() const noexcept { return elements_.rbegin(); }
    inline ConstReverseIterator rend() const noexcept { return elements_.rend(); }

    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};
