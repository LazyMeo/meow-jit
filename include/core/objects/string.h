#pragma once

#include "common/pch.h"
#include "core/meow_object.h"

/**
 * @class ObjString
 * @brief Định nghĩa của kiểu dữ liệu String trong TrangMeo
 */
class ObjString : public MeowObject {
private:
    std::string data_;
    using ConstIterator = std::string::const_iterator;
    using ConstReverseIterator = std::string::const_reverse_iterator;
public:
    /**
     * @brief Khởi tạo chuỗi từ chuỗi đã tồn tại
     * @details Sao chép dữ liệu từ chuỗi đã tồn tại để tạo chuỗi mới
     * @param[in] data chuỗi được sao chép
     */
    ObjString(const std::string& data): data_(data) {}
    ObjString(std::string&& data) noexcept: data_(std::move(data)) {}

    /**
     * @brief Lấy giá trị của kí tự trong chuỗi tại vị trí index
     * @param[in] index Giá trị index của chuỗi tại vị trí cần truy cập
     * @return Giá trị của chuỗi tại index đó
     * @warning Hàm sẽ không kiểm tra OOB khi truy cập
     */
    inline char get(size_t index) const noexcept {
        return data_[index];
    }

    // Các hàm về capacity

    /**
     * @brief Lấy kích thước (số kí tự) của chuỗi tại
     * @return Số kí tự của chuỗi
     */

    inline size_t size() const noexcept {
        return data_.size();
    }

    /**
     * @brief Kiểm tra xem chuỗi có rỗng không?
     * @return 'true' nếu chuỗi rỗng, ngược lại là 'false'
     */
    inline bool empty() const noexcept {
        return data_.empty();
    }

    // Các hàm range-for

    /**
     * @brief Trả về iterator trỏ đến kí tự đầu tiên trong chuỗi
     * @return Iterator trỏ đến đầu chuỗi
     */
    inline ConstIterator begin() const noexcept {
        return data_.begin();
    }

    /**
     * @brief Lấy iterator trỏ đến vị trí sau kí tự cuối cùng của chuỗi
     * @return Iterator trỏ đến vị trí đứng sau kí tự cuối cùng của chuỗi
     */
    inline ConstIterator end() const noexcept {
        return data_.end();
    }

    inline ConstReverseIterator rbegin() const noexcept {
        return data_.rbegin();
    }

    inline ConstReverseIterator rend() const noexcept {
        return data_.rend();
    }

    /**
     * @brief Tự khai báo những object, value mà mảng tham chiếu đến
     * @param[in, out] visitor Đối tượng để đi qua từng object, value mà mảng khai báo để đánh dấu
     * @note Chuỗi này không cần trace
     */
    void trace([[maybe_unused]] GCVisitor& visitor) const noexcept override {}
};
