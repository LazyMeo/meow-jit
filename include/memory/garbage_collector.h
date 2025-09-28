#pragma once

struct MeowObject;
struct ExecutionContext;

/**
 * @class GarbageCollector
 * @brief Dọn dẹp các object không còn được sử dụng, tránh memory leak
 */
class GarbageCollector {
public:
    virtual ~GarbageCollector() = default;
    
    /**
     * @brief Đăng kí một object để GC quản lí
     * @param[in] object Object cần được GC quản li
     */
    virtual void register_object(MeowObject* object) noexcept = 0;
    
    /**
     * @brief Dọn dẹp các object không còn dược sử dụng
     * @param[in] context Môi trường đang thực thi và cần dọn dẹp
     */
    virtual unsigned long collect(ExecutionContext& context) noexcept = 0;
};