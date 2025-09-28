/**
 * @file definition.h
 * @author LazyPaws
 * @copyright Copyright(c) 2025 LazyPaws - All rights reserved
 */

#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "core/meow_object.h"
#include "core/type.h"
#include "memory/gc_visitor.h"
#include "runtime/chunk.h"

/**
 * @class ObjArray
 * @brief Định nghĩa của kiểu dữ liệu Array trong TrangMeo
 */
export class ObjArray : public MeowObject {
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


// Tác giả quá lười để viết thêm đống comment đấy
// Nên từ giờ tác giả chỉ viết comment chỗ cần thiết

/**
 * @class ObjString
 * @brief Định nghĩa của kiểu dữ liệu String trong TrangMeo
 */
export class ObjString : public MeowObject {
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
    void trace(GCVisitor& visitor) noexcept override {}
};

/**
 * @class ObjHash
 * @brief Định nghĩa của kiểu dữ liệu Hash trong TrangMeo
 */
export class ObjHash : public MeowObject {
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
    inline const bool has(String key) const {
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
    void trace(GCVisitor& visitor) noexcept override {
        for (auto& pair : fields_) {
            visitor.visit_value(pair.second);
        }
    }
};


// Đoạn này tác giả lười thật, không comment nữa

class ObjClass : public MeowObject {
private:
    String name_;
    Class superclass_;
    std::unordered_map<String, Value> methods_;
public:
    ObjClass(String name) : name_(name) {}

    inline String get_name() const noexcept {
        return name_;
    }

    inline Class get_super() const noexcept {
        return superclass_;
    }

    inline void set_super(Class super) noexcept {
        superclass_ = super;
    }

    inline bool has_method(String name) const noexcept {
        return methods_.find(name) != methods_.end();
    }

    inline Value get_method(String name) noexcept {
        return methods_[name];
    }

    inline void set_method(String name, Value value) noexcept {
        methods_[name] = value;
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        if (superclass_) {
            visitor.visit_object(superclass_);
        }
        for (auto& method : methods_) {
            visitor.visit_value(method.second);
        }
    }
};


class ObjInstance : public MeowObject {
private:
    Class klass_;
    std::unordered_map<String, Value> fields_;
public:
    ObjInstance(Class k = nullptr) : klass_(k) {}

    inline Class get_class() const noexcept {
        return klass_;
    }

    inline void set_class(Class klass) noexcept {
        klass_ = klass;
    }

    inline Value get_method(String name) noexcept {
        return fields_[name];
    }

    inline void set_method(String name, Value value) noexcept {
        fields_[name] = value;
    }

    inline bool has_method(String name) const {
        return fields_.find(name) != fields_.end();
    }
    
    inline void trace(GCVisitor& visitor) noexcept override {
        visitor.visit_object(klass_);
        for (auto& pair : fields_) {
            visitor.visit_value(pair.second);
        }
    }
};

class ObjBoundMethod : public MeowObject {
private:
    Instance instance_;
    Function function_;
public:
    ObjBoundMethod(Instance instance = nullptr, Function function = nullptr) : instance_(instance), function_(function) {}

    inline Instance get_instance() const noexcept {
        return instance_;
    }

    inline Function get_function() const noexcept {
        return function_;
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        visitor.visit_object(instance_);
        visitor.visit_object(function_);
    }
};

class ObjNativeFunction : public MeowObject {
private:
    class MeowEngine;
    using Arguments = const std::vector<Value>&;
    using NativeFnSimple = std::function<Value(Arguments)>;
    using NativeFnAdvanced = std::function<Value(MeowEngine*, Arguments)>;

    std::variant<NativeFnSimple, NativeFnAdvanced> function_;
public:
    ObjNativeFunction(NativeFnSimple f) : function_(f) {}
    ObjNativeFunction(NativeFnAdvanced f) : function_(f) {}

    inline Value call(Arguments args) {
        if (auto p = std::get_if<NativeFnSimple>(&function_)) {
            return (*p)(args);
        }

        return Value();
    }

    inline Value call(MeowEngine* engine, Arguments args) {
        if (auto p = std::get_if<NativeFnAdvanced>(&function_)) {
            return (*p)(engine, args);
        } else if (auto p = std::get_if<NativeFnSimple>(&function_)) {
            return (*p)(args);
        }

        return Value();
    }
};


struct UpvalueDesc {
    bool isLocal;
    size_t index;
    UpvalueDesc(bool local = false, size_t idx = 0) : isLocal(local), index(idx) {}
};

class ObjUpvalue : public MeowObject {
private:
    enum class State { OPEN, CLOSED };
    State state_ = State::OPEN;
    size_t index_ = 0;
    Value closed_ = Null{};
public:
    ObjUpvalue(size_t index = 0) : index_(index) {}
    inline void close(Value value) noexcept { 
        closed_ = value; 
        state_ = State::CLOSED; 
    }

    inline bool is_closed() const noexcept {
        return state_ == State::CLOSED;
    }

    inline Value get_value() const noexcept {
        return closed_;
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        visitor.visit_value(closed_);
    }
};

class BytecodeParser;
class ObjFunctionProto : public MeowObject {
    friend class BytecodeParser; 
private:
    size_t num_registers_;
    size_t num_upvalues_;
    String name_;
    Chunk chunk_;
    std::vector<UpvalueDesc> upvalueDescs;
public:

    ObjFunctionProto(size_t registers = 0, size_t upvalues = 0, String name)
        : num_registers_(registers), num_upvalues_(upvalues), name_(name) {}

    inline size_t get_num_registers() const noexcept {
        return num_registers_;
    }

    inline size_t get_num_upvalues() const noexcept {
        return num_upvalues_;
    }

    inline String get_name() const noexcept {
        return name_;
    }

    inline const Chunk& get_chunk() const noexcept {
        return chunk_;
    }

    inline const UpvalueDesc& get_desc(size_t index) const noexcept {
        return upvalueDescs[index];
    }

    inline size_t desc_size() const noexcept {
        return upvalueDescs.size();
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        for (auto& constant : chunk_.constant_pool) {
            visitor.visit_value(constant);
        }
    }
};

class ObjClosure : public MeowObject {
private:
    Proto proto_;
    std::vector<Upvalue> upvalues_;
public:
    ObjClosure(Proto p = nullptr) : proto_(p), upvalues_(p ? p->get_num_upvalues() : 0) {}

    inline Proto get_proto() const noexcept {
        return proto_;
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        visitor.visit_object(proto_);
        for (auto& upvalue : upvalues_) {
            visitor.visit_object(upvalue);
        }
    }
};

class ObjModule : public MeowObject {
private:
    String filename_;
    String filepath_;
    std::unordered_map<String, Value> globals_;
    std::unordered_map<String, Value> exports_;
    Proto mainProto;
public:
    bool isExecuted = false;
    bool hasMain = false;
    bool isExecuting = false;

    ObjModule(String filename, String filepath, bool binary = false)
        : filename_(filename), filepath_(filepath) {}

    inline String get_file_name() const noexcept {
        return filename_;
    }

    inline String get_file_path() const noexcept {
        return filepath_;
    }

    inline Value get_global(String name) noexcept {
        return globals_[name];
    }

    inline void set_global(String name, Value value) noexcept {
        globals_[name] = value;
    } 

    inline bool has_global(String name) {
        return globals_.find(name) != globals_.end();
    }

    inline Value get_export(String name) noexcept {
        return exports_[name];
    }

    inline void set_export(String name, Value value) noexcept {
        exports_[name] = value;
    }

    inline bool has_export(String name) {
        return exports_.find(name) != exports_.end();
    }

    inline Proto get_main_proto() const noexcept {
        return mainProto;
    }

    inline void set_main_proto(Proto proto) noexcept {
        mainProto = proto;
        hasMain = true;
    }

    inline void trace(GCVisitor& visitor) noexcept override {
        for (auto& pair : globals_) visitor.visit_value(pair.second);
        for (auto& pair : exports_) visitor.visit_value(pair.second);
        visitor.visit_object(mainProto);
    }
};

struct CallFrame {
    Function closure;
    size_t slotStart;
    uint8_t* ip;
    size_t retReg;
    Module module;
    CallFrame(Function func, size_t start, Module mod, uint8_t* instPointer, size_t ret)
        : closure(func), slotStart(start), module(mod), ip(instPointer), retReg(ret) {}
};

struct ExceptionHandler {
    size_t catchIp;
    size_t frameDepth;
    size_t stackDepth;
    ExceptionHandler(size_t c = 0, size_t f = 0, size_t s = 0) : catchIp(c), frameDepth(f), stackDepth(s) {}
};