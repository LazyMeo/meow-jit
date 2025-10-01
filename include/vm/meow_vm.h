#pragma once

#include "common/pch.h"
#include "core/value.h"
#include "vm/meow_engine.h"

struct VMError : public std::runtime_error {
    VMError(const std::string& message): std::runtime_error(message) {}
};

struct VMArgs {
    std::vector<std::string> command_line_arguments_;
    std::string entry_point_directory_;
    std::string entry_path;
};

struct ExecutionContext;
struct BuiltinRegistry;
class MemoryManager;
class ModuleManager;
class OperatorDispatcher;

/**
 * @class MeowVM
 * @brief MeowVM là phần quan trọng nhất của toàn bộ chương trình
 */
class MeowVM : MeowEngine {
public:
    /**
     * @brief Khởi tạo MeowVM từ thư mục bắt đầu, và file khởi động, cùng với các tham số đầu vào
     * @param[in] entry_point_directory Thư mục bắt đầu để tìm file, thường lấy thư mục hiện tại làm gốc
     * @param[in] entry_path Đường dẫn tương đối/tuyệt đối của file bytecode được vm chạy đầu tiên
     */
    MeowVM(const std::string& entry_point_directory = ".", const std::string& entry_path, int argc, char* argv[]);
    MeowVM(const MeowVM&) = delete;
    MeowVM& operator=(const MeowVM&) = delete;
    ~MeowVM();
    void interpret();
private:
    // Các thành phần chính của MeowVM
    std::unique_ptr<ExecutionContext> context_;
    std::unique_ptr<BuiltinRegistry> builtins_;
    std::unique_ptr<MemoryManager> heap_;
    std::unique_ptr<ModuleManager> mod_manager_;
    std::unique_ptr<OperatorDispatcher> op_dispatcher_;


    VMArgs args_;

    void prepare() noexcept;
    void run();


    [[noreturn]] inline void newVMError(const std::string& message) {
        throw VMError(message);
    }
};