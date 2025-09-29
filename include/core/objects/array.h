#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"

/**
 * @class ObjArray
 * @brief Định nghĩa của kiểu dữ liệu Array trong TrangMeo
 */
class ObjArray : public MeowObject {
private:
    std::vector<Value> elements_;
    using Iterator = std::vector<Value>::iterator;
    using ConstIterator = std::vector<Value>::const_iterator;
    using ReverseIterator = std::vector<Value>::reverse_iterator;
    using ConstReverseIterator = std::vector<Value>::const_reverse_iterator;
public:
    /**
     * @brief Khởi tạo mảng từ mảng đã tồn tại
     * @details Sao chép dữ liệu từ mảng đã tồn tại để tạo mảng mới
     * @param[in] elements Mảng được sao chép
     */
    ObjArray(const std::vector<Value>& elements) : elements_(elements) {}
    ObjArray(std::vector<Value>&& elements) noexcept : elements_(std::move(elements)) {}
    ObjArray(std::initializer_list<Value> elements) : elements_(elements) {}

    /**
     * @brief Lấy giá trị của một phần tử trong mảng tại vị trí index
     * @param[in] index Giá trị index của phần tử tại vị trí cần truy cập
     * @return Giá trị của phần tử tại index đó
     * @warning Hàm sẽ không kiểm tra OOB khi truy cập
     */
    inline const Value& get(size_t index) const noexcept {
        return elements_[index];
    }

    /**
     * @brief Gán giá trị cho một phần tử trong mảng tại vị trí index
     * @param[in] index Giá trị index của phần tử tại vị trí cần gán
     * @param[in] value Giá trị được gán cho phần tử tại index đó
     * @tparam T Kiểu của mọi phần tử được set
     * @warning Hàm sẽ không kiểm tra OOB khi gán
     */
    template <typename T>
    inline void set(size_t index, T&& value) noexcept {
        elements_[index] = std::forward<T>(value);
    }

    /**
     * @brief Lấy giá trị an toàn của một phần tử trong mảng tại vị trí index
     * @details Hàm sẽ thả một ngoại lệ nếu không tìm thấy phần tử
     * @param[in] index Giá trị index của phần tử tại vị trí cần truy cập
     * @return Giá trị của phần tử tại index đó
     */
    inline const Value& at(size_t index) const {
        return elements_.at(index);
    }

    // Các hàm về capacity

    /**
     * @brief Lấy kích thước (số phần tử) của mảng hiện tại
     * @return Số phần tử của mảng
     */
    inline size_t size() const noexcept {
        return elements_.size();
    }

    /**
     * @brief Kiểm tra xem mảng có rỗng không?
     * @return 'true' nếu mảng rỗng, ngược lại là 'false'
     */
    inline bool empty() const noexcept {
        return elements_.empty();
    }

    /**
     * @brief Lấy số phần tử tối đa mà mảng có thể chứa
     * @return Dung tích tối đa của mảng
     */
    inline size_t capacity() const noexcept {
        return elements_.capacity();
    }

    // Các hàm sửa đổi

    /**
     * @brief Thêm phần tử vào cuối của mảng
     * @param[in] value Phần tử cần thêm
     * @tparam T Kiểu của mọi value được push
     */
    template <typename T>
    inline void push(T&& value) {
        elements_.emplace_back(std::forward<T>(value));
    }

    /**
     * @brief Xóa phần tử cuối cùng của mảng
     */
    inline void pop() noexcept {
        elements_.pop_back();
    }


    /**
     * @brief Tạo phần tử tại chỗ và thêm vào cuối mảng
     * @param[in] args Một gói chứa các tham số được truyền để tạo phần tử tại chỗ
     * @tparam Args Gói mọi kiểu đối số được truyền vào
     */
    template <typename... Args>
    inline void emplace(Args&&... args) {
        elements_.emplace_back(std::forward<Args>(args)...);
    }

    /**
     * @brief Thay đổi kích thước của mảng về size
     * @param[in] size Kích thước mảng muốn thay đổi thành
     */
    inline void resize(size_t size) {
        elements_.resize(size);
    }

    /**
     * @brief Tăng số phần tử tối đa mà mảng có thể chứa
     * @param[in] capacity Dung tích của mảng muốn thay đổi thành
     */
    inline void reserve(size_t capacity) {
        elements_.reserve(capacity);
    }

    /**
     * @brief Giảm capacity của mảng để vừa với size của mảng hiện tại
     */
    inline void shrink() {
        elements_.shrink_to_fit();
    }

    // Các hàm range-for

    /**
     * @brief Trả về iterator trỏ đến phần tử đầu tiên trong mảng
     * @return Iterator trỏ đến đầu mảng
     */
    inline Iterator begin() noexcept {
        return elements_.begin();
    }
    
    /**
     * @brief Lấy iterator trỏ đến vị trí sau phần tử cuối cùng của mảng
     * @return Iterator trỏ đến vị trí đứng sau phần tử cuối cùng của mảng
     */
    inline Iterator end() noexcept {
        return elements_.end();
    }

    inline ConstIterator begin() const noexcept {
        return elements_.begin();
    }

    inline ConstIterator end() const noexcept {
        return elements_.end();
    }

    inline ReverseIterator rbegin() noexcept {
        return elements_.rbegin();
    }

    inline ReverseIterator rend() noexcept {
        return elements_.rend();
    }

    inline ConstReverseIterator rbegin() const noexcept {
        return elements_.rbegin();
    }

    inline ConstReverseIterator rend() const noexcept {
        return elements_.rend();
    }

    /**
     * @brief Tự khai báo những object, value mà mảng tham chiếu đến
     * @param[in, out] visitor Đối tượng để đi qua từng object, value mà mảng khai báo để đánh dấu
     */
    void trace(GCVisitor& visitor) noexcept override {
        for (auto& element : elements_) {
            visitor.visit_value(element);
        }
    }
};
