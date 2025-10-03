#include "vm/meow_vm.h"

MeowVM::MeowVM(const std::string& entry_point_directory, const std::string& entry_path, int argc, char* argv[]) {
    args_.command_line_arguments_.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args_.command_line_arguments_.push_back(argv[i]);
    }
    args_.entry_point_directory_ = entry_point_directory;
    args_.entry_path_ = entry_path;
}

MeowVM::~MeowVM() = default;

#include "vm/meow_vm.h"
#include "runtime/execution_context.h"
#include "runtime/builtin_registry.h"
#include "memory/memory_manager.h"
#include "memory/mark_sweep_gc.h"
#include "module/module_manager.h"
#include "runtime/operator_dispatcher.h"
#include "core/op_code.h"
#include "core/objects/function.h"
#include "core/objects/module.h"

// Constructor và Destructor (đã có)
MeowVM::MeowVM(const std::string& entry_point_directory, const std::string& entry_path, int argc, char* argv[]) {
    args_.command_line_arguments_.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args_.command_line_arguments_.push_back(argv[i]);
    }
    args_.entry_point_directory_ = entry_point_directory;
    args_.entry_path_ = entry_path;
}

MeowVM::~MeowVM() = default;

void MeowVM::prepare() noexcept {
    context_ = std::make_unique<ExecutionContext>();
    builtins_ = std::make_unique<BuiltinRegistry>();

    auto gc = std::make_unique<MarkSweepGC>(context_.get(), builtins_.get());
    heap_ = std::make_unique<MemoryManager>(std::move(gc));

    mod_manager_ = std::make_unique<ModuleManager>();
    op_dispatcher_ = std::make_unique<OperatorDispatcher>(heap_.get());

    String entry_path_obj = heap_->new_string(args_.entry_path_.c_str());
    Module entry_module = mod_manager_->load_module(entry_path_obj, nullptr, heap_.get(), this);

    if (!entry_module || !entry_module->is_has_main()) {
        throwVMError("Không thể tải entry point hoặc không tìm thấy hàm main.");
    }

    Function main_closure = heap_->new_function(entry_module->get_main_proto());
    
    context_->registers_.push_back(main_closure);
    context_->call_stack_.emplace_back(main_closure, 0, 0, 0);
    context_->current_frame_ = &context_->call_stack_.back();
}

void MeowVM::interpret() {
    try {
        prepare();
        run();
    } catch (const VMError& e) {
        std::cerr << "Lỗi VM: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Lỗi hệ thống: " << e.what() << std::endl;
    }
}

void MeowVM::run() {
    CallFrame* frame = context_->current_frame_;
    const uint8_t* ip = frame->ip_;
    #define READ_BYTE() (*ip++)
    #define READ_SHORT() \
        ([&]() -> uint16_t { \
            uint8_t first_byte = READ_BYTE(); \
            if ((first_byte & 0x80) == 0) { \
                return first_byte; \
            } \
            uint8_t second_byte = READ_BYTE(); \
            return static_cast<uint16_t>((first_byte & 0x7F) | (static_cast<uint16_t>(second_byte) << 7)); \
        }())

    #define READ_CONSTANT() (frame->function_->get_proto()->get_chunk().get_constant(READ_SHORT()))

    #define REGISTER(idx) (context_->registers_[frame->start_reg_ + idx])
    #define CONSTANT(cidx) (frame->function_->get_proto()->get_chunk().get_constant(cidx))

    while (42) {
        frame->ip_ = ip;
        
        OpCode op_code = static_cast<OpCode>(READ_BYTE());

        switch (op_code) {
            case OpCode::LOAD_CONST: {
                uint16_t dst = READ_SHORT();
                Value constant = READ_CONSTANT();
                REGISTER(dst) = constant;
                break;
            }

            case OpCode::ADD:
            case OpCode::SUB:
            case OpCode::MUL:
            case OpCode::DIV:
            case OpCode::MOD:
            case OpCode::POW:
            case OpCode::EQ:
            case OpCode::NEQ:
            case OpCode::GT:
            case OpCode::GE:
            case OpCode::LT:
            case OpCode::LE:
            case OpCode::BIT_AND:
            case OpCode::BIT_OR:
            case OpCode::BIT_XOR:
            case OpCode::LSHIFT:
            case OpCode::RSHIFT: {
                uint16_t dst = READ_SHORT(), r1 = READ_SHORT(), r2 = READ_SHORT();
                auto& right = REGISTER(r1);
                auto& left = REGISTER(r2);
                Value result;
                if (auto func = op_dispatcher_->find(op_code, left, right)) {
                    result = (*func)(left, right);
                } else {
                    throwVMError("Phép toán nhị phân chưa được hỗ trợ");
                }
                REGISTER(dst) = result;
                break;
            }

            case OpCode::NEG:
            case OpCode::NOT:
            case OpCode::BIT_NOT: {
                size_t dst = READ_SHORT(), src = READ_SHORT();
                auto& value = REGISTER(dst);
                Value result;
                if (auto func = op_dispatcher_->find(op_code, value)) {
                    result = (*func)(value);
                } else {
                    throwVMError("Phép toán một ngôi chưa được hỗ trợ");
                }
                REGISTER(dst) = result;
                break;
            }

            case OpCode::HALT: {
                context_->call_stack_.clear();
                return;
            }

            default: {
                frame->ip_ = ip;
                throwVMError("OpCode không xác định.");
            }
        }
    }
}



// Computed goto tính sau đi
// Comment lại

// void MeowVM::run() {
//     CallFrame* frame = context_->current_frame_;
//     const uint8_t* ip = frame->ip_;

//     static void* dispatch_table[] = {
//         &&op_load_const, &&op_load_null, &&op_load_true, &&op_load_false, 
//         &&op_load_int, &&op_move, &&op_add, &&op_sub, &&op_mul, &&op_div, 
//         &&op_mod, &&op_pow, &&op_eq, &&op_neq, &&op_gt, &&op_ge, &&op_lt, 
//         &&op_le, &&op_neg, &&op_not, &&op_get_global, &&op_set_global, 
//         &&op_get_upvalue, &&op_set_upvalue, &&op_closure, &&op_close_upvalues, 
//         &&op_jump, &&op_jump_if_false, &&op_jump_if_true, &&op_call, 
//         &&op_call_void, &&op_return, &&op_halt, &&op_new_array, &&op_new_hash, 
//         &&op_get_index, &&op_set_index, &&op_get_keys, &&op_get_values, 
//         &&op_new_class, &&op_new_instance, &&op_get_prop, &&op_set_prop, 
//         &&op_set_method, &&op_inherit, &&op_get_super, &&op_bit_and, 
//         &&op_bit_or, &&op_bit_xor, &&op_bit_not, &&op_lshift, &&op_rshift, 
//         &&op_throw, &&op_setup_try, &&op_pop_try, &&op_import_module, 
//         &&op_export, &&op_get_export, &&op_import_all
//     };

//     #define READ_BYTE() (*ip++)
//     #define READ_SHORT() ([&]() -> uint16_t { \
//         uint8_t b = READ_BYTE(); \
//         if ((b & 0x80) == 0) return b; \
//         return static_cast<uint16_t>((b & 0x7F) | (READ_BYTE() << 7)); \
//     }())
//     #define READ_CONSTANT() (frame->function_->get_proto()->get_chunk().get_constant(READ_SHORT()))
    
//     #define DISPATCH() goto *dispatch_table[READ_BYTE()]
    
//     DISPATCH();

//     {
//         op_load_const: {
//             uint16_t dest_reg = READ_SHORT();
//             Value constant = READ_CONSTANT();
//             context_->registers_[frame->start_reg_ + dest_reg] = constant;
//             DISPATCH();
//         }

//         op_add: {
//             // ... (Logic for ADD)
//             DISPATCH();
//         }

//         op_return: {
//             frame->ip_ = ip;
//             Value return_value = context_->registers_[frame->start_reg_];
//             context_->call_stack_.pop_back();

//             if (context_->call_stack_.empty()) {
//                 context_->current_frame_ = nullptr;
//                 return; // Kết thúc
//             }

//             context_->current_frame_ = &context_->call_stack_.back();
//             frame = context_->current_frame_;
//             ip = frame->ip_;
//             context_->registers_[frame->ret_reg_] = return_value;
            
//             // Vì đã nạp lại ip từ frame mới, chúng ta cần dispatch tới lệnh tiếp theo trong frame đó
//             DISPATCH();
//         }

//         op_halt: {
//             return; // Dừng máy ảo
//         }

//         // ... Các label khác ...
        
//         // Các OpCode còn lại sẽ nhảy tới đây
//         op_load_null: op_load_true: op_load_false: op_load_int: op_move: 
//         op_sub: op_mul: op_div: op_mod: op_pow: op_eq: op_neq: op_gt: op_ge: 
//         op_lt: op_le: op_neg: op_not: op_get_global: op_set_global: op_get_upvalue: 
//         op_set_upvalue: op_closure: op_close_upvalues: op_jump: op_jump_if_false: 
//         op_jump_if_true: op_call: op_call_void: op_new_array: op_new_hash: op_get_index: 
//         op_set_index: op_get_keys: op_get_values: op_new_class: op_new_instance: 
//         op_get_prop: op_set_prop: op_set_method: op_inherit: op_get_super: 
//         op_bit_and: op_bit_or: op_bit_xor: op_bit_not: op_lshift: op_rshift: 
//         op_throw: op_setup_try: op_pop_try: op_import_module: op_export: 
//         op_get_export: op_import_all:
//         {
//             frame->ip_ = ip - 1; // Trỏ lại opcode gây lỗi
//             throw VMError("OpCode chưa được implement.");
//         }
//     }
// }