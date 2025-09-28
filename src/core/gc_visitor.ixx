export module meow.core:gc_visitor;

import :forward;

/**
 * @struct GCVisitor
 * @brief Lớp trừu tượng cho GC, dùng để đi duyệt và đánh dấu các Object, Value
 */
export struct GCVisitor {
    virtual ~GCVisitor() = default;

    /**
     * @brief Duyệt một Value để đánh dấu
     * @param[in] value Giá trị cần đánh dấu
     */
    virtual void visit_value(Value& value) noexcept = 0;

    /**
     * @brief Duyệt một Object để đánh dấu
     * @param[in] object Con trỏ tới Object cần đánh dấu
     */
    virtual void visit_object(MeowObject* object) noexcept = 0;
};