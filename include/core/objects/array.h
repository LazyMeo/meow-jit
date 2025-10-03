#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"

/// @brief Core definition of Array in TrangMeo. Using as Array(alias ObjArray*)
/// @details Array definition for performance-critical vm, no safety, no check OOB, etc
class ObjArray : public MeowObject {
private:
    using value_type = Value;
    using reference_type = value_type&;
    using const_reference_type = const value_type&;
    using container_type = std::vector<Value>;

    container_type elements_;
public:
    // --- Constructors & destructor ---
    ObjArray() = default;
    explicit ObjArray(const container_type& elements) : elements_(elements) {}
    explicit ObjArray(container_type&& elements) noexcept : elements_(std::move(elements)) {}
    explicit ObjArray(std::initializer_list<value_type> elements) : elements_(elements) {}

    // --- Rule of 5 ---
    ObjArray(const ObjArray&) = delete;
    ObjArray(ObjArray&&) = delete;
    ObjArray& operator=(const ObjArray&) = delete;
    ObjArray& operator=(ObjArray&&) = delete;
    ~ObjArray() override = default;

    // --- Iterator types ---
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;
    using reverse_iterator = container_type::reverse_iterator;
    using const_reverse_iterator = container_type::const_reverse_iterator;

    // --- Element access ---
    
    /// @brief Unchecked element access. For performance-critical code
    [[nodiscard]] inline const_reference_type get(size_t index) const noexcept {
        return elements_[index];
    }
    /// @brief Unchecked element modification. For performance-critical code
    template <typename T> inline void set(size_t index, T&& value) noexcept {
        elements_[index] = std::forward<T>(value);
    }
    /// @brief Checked element access. Throws if index is OOB
    [[nodiscard]] inline const_reference_type at(size_t index) const {
        return elements_.at(index);
    }
    inline const_reference_type operator[](size_t index) const noexcept { return elements_[index]; }
    inline reference_type operator[](size_t index) noexcept { return elements_[index]; }
    [[nodiscard]] inline const_reference_type front() const noexcept { return elements_.front(); }
    [[nodiscard]] inline reference_type front() noexcept { return elements_.front(); }
    [[nodiscard]] inline const_reference_type back() const noexcept { return elements_.back(); }
    [[nodiscard]] inline reference_type back() noexcept { return elements_.back(); }

    // --- Capacity ---
    [[nodiscard]] inline size_t size() const noexcept { return elements_.size(); }
    [[nodiscard]] inline bool empty() const noexcept { return elements_.empty(); }
    [[nodiscard]] inline size_t capacity() const noexcept { return elements_.capacity(); }

    // --- Modifiers ---
    template <typename T> inline void push(T&& value) { 
        elements_.emplace_back(std::forward<T>(value));
    }
    inline void pop() noexcept { elements_.pop_back(); }
    template <typename... Args> inline void emplace(Args&&... args) {
        elements_.emplace_back(std::forward<Args>(args)...);
    }
    inline void resize(size_t size) { elements_.resize(size); }
    inline void reserve(size_t capacity) { elements_.reserve(capacity); }
    inline void shrink() { elements_.shrink_to_fit(); }
    inline void clear() { elements_.clear(); }

    // --- Iterators ---
    inline iterator begin() noexcept { return elements_.begin(); }
    inline iterator end() noexcept { return elements_.end(); }
    inline const_iterator begin() const noexcept { return elements_.begin(); }
    inline const_iterator end() const noexcept { return elements_.end(); }
    inline reverse_iterator rbegin() noexcept { return elements_.rbegin(); }
    inline reverse_iterator rend() noexcept { return elements_.rend(); }
    inline const_reverse_iterator rbegin() const noexcept { return elements_.rbegin(); }
    inline const_reverse_iterator rend() const noexcept { return elements_.rend(); }

    /// @brief Tracing all objects referenced
    void trace(GCVisitor& visitor) const noexcept override;
};
