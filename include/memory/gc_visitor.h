/**
 * @file gc_visitor.h
 * @author LazyPaws
 * @copyright Copyright(c) 2025 LazyPaws - All rights reserved
 */

#pragma once

class Value;
struct MeowObject;

/**
 * @struct GCVisitor
 * @brief Lớp trừu tượng cho GC, dùng để đi duyệt và đánh dấu các Object, Value
 */
struct GCVisitor {
    virtual ~GCVisitor() = default;

    /**
     * @brief Duyệt một Value để đánh dấu
     * @param[in] value Giá trị cần đánh dấu
     */
    virtual void visit_value(const Value& value) noexcept = 0;

    /**
     * @brief Duyệt một Object để đánh dấu
     * @param[in] object Con trỏ tới Object cần đánh dấu
     */
    virtual void visit_object(const MeowObject* object) noexcept = 0;
};