#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "memory/gc_visitor.h"
#include "common/type.h"

/**
 * @class ObjHash
 * @brief Định nghĩa của kiểu dữ liệu Hash trong TrangMeo
 */
class ObjHash : public MeowObject {
private:
    std::unordered_map<String, Value> fields_;
    using Iterator = std::unordered_map<String, Value>::iterator;
    using ConstIterator = std::unordered_map<String, Value>::const_iterator;
public:
    /**
     * @brief Khởi tạo hash từ hash đã tồn tại
     * @details Sao chép dữ liệu từ hash đã tồn tại để tạo hash mới
     * @param[in] fields Hash được sao chép
     */
    ObjHash(const std::unordered_map<String, Value>& fields): fields_(fields) {}
    ObjHash(std::unordered_map<String, Value>&& fields): fields_(std::move(fields)) {}

    /**
     * @brief Lấy giá trị của một value trong hash tại key
     * @param[in] key Giá trị key của value cần truy cập
     * @return Giá trị của value tại key đó
     * @warning Hàm sẽ không kiểm tra OOB khi truy cập
     */
    inline const Value& get(String key) noexcept {
        return fields_[key];
    }

    /**
     * @brief Gán giá trị cho một value trong hash tại vị trí key
     * @param[in] key Giá trị key của value tại vị trí cần gán
     * @param[in] value Giá trị được gán cho value tại key đó
     * @tparam T Kiểu của mọi giá trị được set
     * @warning Hàm sẽ không kiểm tra OOB khi gán
     */
    template <typename T>
    inline void set(String key, T&& value) noexcept {
        fields_[key] = std::forward<T>(value);
    }

    /**
     * @brief Lấy giá trị an toàn của một value trong mảng tại vị trí key
     * @details Hàm sẽ thả một ngoại lệ nếu không tìm thấy value
     * @param[in] key Giá trị key của value tại vị trí cần truy cập
     * @return Giá trị của value tại key đó
     */
    inline const Value& at(String key) const {
        return fields_.at(key);
    }

    /**
     * @brief Kiểm tra xem một key có tồn tại hay không
     * @param[in] key Giá trị key của value tại vị trí cần kiểm tra
     * @return 'true' nếu key tồn tại, ngược lại là 'false'
     */
    inline bool has(String key) const {
        return fields_.find(key) != fields_.end();
    }

    // Các hàm về capacity

    /**
     * @brief Lấy kích thước (số pair) của hash hiện tại
     * @return Số pair của hash
     */
    inline size_t size() const noexcept {
        return fields_.size();
    }

    /**
     * @brief Kiểm tra xem hash có rỗng không?
     * @return 'true' nếu hash rỗng, ngược lại là 'false'
     */
    inline bool empty() const noexcept {
        return fields_.empty();
    }

    // Các hàm range-for

    /**
     * @brief Trả về iterator trỏ đến pair đầu tiên trong hash
     * @return Iterator trỏ đến đầu hash
     */
    inline Iterator begin() noexcept {
        return fields_.begin();
    }
    
    /**
     * @brief Lấy iterator trỏ đến vị trí sau pair cuối cùng của hash
     * @return Iterator trỏ đến vị trí đứng sau pair cuối cùng của hash
     */
    inline Iterator end() noexcept {
        return fields_.end();
    }

    inline ConstIterator begin() const noexcept {
        return fields_.begin();
    }

    inline ConstIterator end() const noexcept {
        return fields_.end();
    }

    /**
     * @brief Tự khai báo những object, value mà mảng tham chiếu đến
     * @param[in, out] visitor Đối tượng để đi qua từng object, value mà mảng khai báo để đánh dấu
     */
    void trace(GCVisitor& visitor) const noexcept override;
};
