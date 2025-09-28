export module meow.core.meow_object;

import meow.memory.gc_visitor;

/**
 * @struct MeowObject
 * @brief Đại diện cho mọi kiểu dữ liệu Object trong TrangMeo
 */
export struct MeowObject {
    virtual ~MeowObject() = default;

    /**
     * @brief Tự khai báo cho GC những Value, Object mà Object này tham chiếu đến
     * @param[in, out] visitor Đối tượng để đi qua từng Object, Value mà mảng khai báo để đánh dấu
     */
    virtual void trace(GCVisitor& visitor) noexcept = 0;
};