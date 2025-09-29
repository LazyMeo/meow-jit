#include "vm/meow_vm.h"

MeowVM::MeowVM(const std::string& entry_point_directory, const std::string& entry_path, int argc, char* argv[]) {
    args_.command_line_arguments_.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args_.command_line_arguments_.push_back(argv[i]);
    }
    args_.entry_point_directory_ = entry_point_directory;
    args_.entry_path = entry_path;
}

MeowVM::~MeowVM() = default;

void MeowVM::prepare() noexcept {
    
}



// #include "vm/meow_vm.h"
// #include "memory/mark_sweep_gc.h"
// #include "runtime/operator_dispatcher.h"
// #include "module/module_manager.h"
// #include "memory/memory_manager.h"
// #include "memory/meow_object.h"
// #include "memory/gc_disable_guard.h"
// #include "runtime/execution_context.h"
// #include <iomanip> // Thêm thư viện này

// MeowVM::MeowVM(const std::string& entryPointDir) {
//     args.entryPointDir = entryPointDir;
//     memoryManager = std::make_unique<MemoryManager>(std::make_unique<MarkSweepGC>());
//     context = std::make_unique<ExecutionContext>();
//     opDispatcher = std::make_unique<OperatorDispatcher>();
//     moduleManager = std::make_unique<ModuleManager>();
//     memoryManager->setContext(context.get());
//     opDispatcher->setMemoryManager(memoryManager.get());
// }

// MeowVM::MeowVM(const std::string& entryPointDir, int argc, char* argv[]) {
//     args.entryPointDir = entryPointDir;
//     memoryManager = std::make_unique<MemoryManager>(std::make_unique<MarkSweepGC>());
//     context = std::make_unique<ExecutionContext>();
//     opDispatcher = std::make_unique<OperatorDispatcher>();
//     moduleManager = std::make_unique<ModuleManager>();
//     memoryManager->setContext(context.get());
//     opDispatcher->setMemoryManager(memoryManager.get());

//     args.commandLineArgs.reserve(argc);
//     for (int i = 0; i < argc; ++i) {
//         args.commandLineArgs.push_back(argv[i]);
//     }
// }

// MeowVM::~MeowVM() = default;

// void MeowVM::interpret(const std::string& entryPath, bool isBinary) {
//     context->reset();
//     this->moduleManager->setEngine(this);
//     this->moduleManager->setEntryPath(entryPath);
//     this->moduleManager->setMemoryManager(memoryManager.get());

//     defineNativeFunctions();

//     try {
//         auto loadedModule = moduleManager->loadModule(entryPath, args.entryPointDir, isBinary);

//         if (!loadedModule) {
//             std::cerr << "💥 Lỗi nghiêm trọng trong MeowScript VM: " << loadedModule.error().getMessage() << std::endl;
//             return;
//         }

//         auto entryMod = loadedModule.value();

//         if (!entryMod->isExecuted) {
//             if (!entryMod->hasMain) throw VMError("Entry module thiếu @main.");
//             entryMod->isExecuting = true;

//             auto closure = memoryManager->newObject<ObjClosure>(entryMod->getMain());
//             Int base = static_cast<Int>(context->stackSlots.size());
//             context->stackSlots.resize(base + entryMod->getMain()->getNumRegisters(), Value(Null{}));
//             CallFrame frame(closure, base, entryMod, entryMod->getMain()->getChunk().getCode(), -1);
//             context->callStack.push_back(frame);
//         }
//         run();
//         entryMod->isExecuted = true;

//     } catch (const VMError& e) {
//         std::cerr << "💥 Lỗi nghiêm trọng trong MeowScript VM: " << e.what() << std::endl;
//     } catch (const std::exception& e) {
//         std::cerr << "🤯 Lỗi C++ không lường trước: " << e.what() << std::endl;
//     }
// }


// void MeowVM::run() {
//     // TODO: Thay toàn bộ đoạn hardcode text dưới đây bằng ErrorCode, tạo Diagnostic qua helper,... không hardcode text để có i18n
//     // TODO: Mấy code này là code cũ, không phù hợp với cấu trúc hiện tại, cần thay toàn bộ
//     // TODO: Làm tương tự với handle_method.cpp, handle_call.cpp, handle_errors.cpp, define_natives.cpp,.. vì toàn cái cũ cần thay
//     while (!context->callStack.empty()) {
//         currentFrame = &context->callStack.back();
//         auto proto = currentFrame->closure->getProto();
//         context->currentBase = currentFrame->slotStart;
//         const uint8_t* code = proto->getChunk().getCode();
        
//         // Helper để đọc một đối số 1-2 byte có độ dài thay đổi
//         auto READ_SHORT = [&]() -> uint16_t {
//             uint8_t byte1 = *currentFrame->ip++;
//             if ((byte1 & 0x80) == 0) {
//                 return byte1;
//             } else {
//                 uint8_t byte2 = *currentFrame->ip++;
//                 return ((byte1 & 0x7F) | (static_cast<uint16_t>(byte2) << 7));
//             }
//         };

//         // Helper để đọc một địa chỉ nhảy 2 byte
//         auto READ_ADDRESS = [&]() -> uint16_t {
//             uint8_t byte1 = *currentFrame->ip++;
//             uint8_t byte2 = *currentFrame->ip++;
//             return (static_cast<uint16_t>(byte1) | (static_cast<uint16_t>(byte2) << 8));
//         };
        
//         try {
//             uint8_t opcode = *currentFrame->ip++;
//             {
//                 GCScopeGuard gcGuard(memoryManager.get());

//                 switch (static_cast<OpCode>(opcode)) {
//                     // --- Loads / Moves ---
//                     case OpCode::MOVE: {
//                         size_t dst = READ_SHORT(), src = READ_SHORT();
//                         context->stackSlots[context->currentBase + dst] = context->stackSlots[context->currentBase + src];
//                         break;
//                     }

//                     case OpCode::LOAD_CONST: {
//                         size_t dst = READ_SHORT(), cidx = READ_SHORT();
//                         if (cidx >= proto->getChunk().getConstantPoolSize())
//                             newVMError("LOAD_CONST index OOB");
//                         context->stackSlots[context->currentBase + dst] = proto->getChunk().getConstant(cidx);
//                         break;
//                     }

//                     case OpCode::LOAD_INT: {
//                         size_t dst = READ_SHORT();
//                         int64_t val = read_i64_le(proto->getChunk().code, currentFrame->ip, proto->getChunk().getCodeSize());
//                         context->stackSlots[context->currentBase + dst] = Value(val);
//                         break;
//                     }

//                     case OpCode::LOAD_NULL: {
//                         context->stackSlots[context->currentBase + static_cast<size_t>(READ_SHORT())] = Value(Null{});
//                         break;
//                     }

//                     case OpCode::LOAD_TRUE: {
//                         context->stackSlots[context->currentBase + static_cast<size_t>(READ_SHORT())] = Value(true);
//                         break;
//                     }

//                     case OpCode::LOAD_FALSE: {
//                         context->stackSlots[context->currentBase + static_cast<size_t>(READ_SHORT())] = Value(false);
//                         break;
//                     }

//                     // --- Binary operators (ADD, SUB, ...) ---
//                     case OpCode::ADD:
//                     case OpCode::SUB:
//                     case OpCode::MUL:
//                     case OpCode::DIV:
//                     case OpCode::MOD:
//                     case OpCode::POW:
//                     case OpCode::EQ:
//                     case OpCode::NEQ:
//                     case OpCode::GT:
//                     case OpCode::GE:
//                     case OpCode::LT:
//                     case OpCode::LE:
//                     case OpCode::BIT_AND:
//                     case OpCode::BIT_OR:
//                     case OpCode::BIT_XOR:
//                     case OpCode::LSHIFT:
//                     case OpCode::RSHIFT: {
//                         Int dst = READ_SHORT(), r1 = READ_SHORT(), r2 = READ_SHORT();
//                         auto& left = context->stackSlots[context->currentBase + dst];
//                         auto& right = context->stackSlots[context->currentBase + r2];
//                         auto& leftSrc = context->stackSlots[context->currentBase + r1];
//                         Value result;
//                         if (auto func = opDispatcher->find(static_cast<OpCode>(opcode), leftSrc, right)) {
//                             result = (*func)(leftSrc, right);
//                         } else {
//                             newVMError("Unsupported binary operator");
//                         }
//                         context->stackSlots[context->currentBase + dst] = result;
//                         break;
//                     }

//                     // --- Unary ops ---
//                     case OpCode::NEG:
//                     case OpCode::NOT:
//                     case OpCode::BIT_NOT: {
//                         size_t dst = READ_SHORT(), src = READ_SHORT();
//                         auto& val = context->stackSlots[context->currentBase + src];
//                         Value result;
//                         if (auto func = opDispatcher->find(static_cast<OpCode>(opcode), val)) {
//                             result = (*func)(val);
//                         } else {
//                             newVMError("Unsupported unary operator");
//                         }
//                         context->stackSlots[context->currentBase + dst] = result;
//                         break;
//                     }

//                     // --- Globals ---
//                     case OpCode::GET_GLOBAL: {
//                         size_t dst = READ_SHORT(), cidx = READ_SHORT();
//                         if (cidx >= proto->getChunk().getConstantPoolSize())
//                             newVMError("GET_GLOBAL index OOB");
//                         if (!proto->getChunk().getConstant(cidx).is<String>())
//                             newVMError("GET_GLOBAL name must be a string");
//                         auto name = proto->getChunk().getConstant(cidx).get<String>();
//                         auto it = currentFrame->module->globals.find(name->utf8());
//                         if (it != currentFrame->module->globals.end()) {
//                             context->stackSlots[context->currentBase + dst] = it->second;
//                         } else {
//                             context->stackSlots[context->currentBase + dst] = Value(Null{});
//                         }
//                         break;
//                     }

//                     case OpCode::SET_GLOBAL: {
//                         size_t cidx = READ_SHORT(), src = READ_SHORT();
//                         if (cidx >= proto->getChunk().getConstantPoolSize())
//                             newVMError("SET_GLOBAL index OOB");
//                         if (!proto->getChunk().getConstant(cidx).is<String>())
//                             newVMError("Global variable name must be a string");
//                         auto name = proto->getChunk().getConstant(cidx).get<String>();
//                         currentFrame->module->globals[name->utf8()] = context->stackSlots[context->currentBase + src];
//                         break;
//                     }

//                     // --- Upvalues / Closure ---
//                     case OpCode::GET_UPVALUE: {
//                         size_t dst = READ_SHORT(), uvIndex = READ_SHORT();
//                         if (uvIndex >= currentFrame->closure->upvalues.size())
//                             newVMError("GET_UPVALUE index OOB");
//                         auto uv = currentFrame->closure->upvalues[uvIndex];
//                         if (uv->isClosed()) {
//                             context->stackSlots[context->currentBase + dst] = uv->getClosed();
//                         } else {
//                             context->stackSlots[context->currentBase + dst] = context->stackSlots[uv->slotIndex];
//                         }
//                         break;
//                     }

//                     case OpCode::SET_UPVALUE: {
//                         size_t uvIndex = READ_SHORT(), src = READ_SHORT();
//                         if (uvIndex >= currentFrame->closure->upvalues.size())
//                             newVMError("SET_UPVALUE index OOB");
//                         auto uv = currentFrame->closure->upvalues[uvIndex];
//                         if (!uv->isClosed()) {
//                             context->stackSlots[uv->slotIndex] = context->stackSlots[context->currentBase + src];
//                         } else {
//                             uv->close(context->stackSlots[context->currentBase + src]);
//                         }
//                         break;
//                     }

//                     case OpCode::CLOSURE: {
//                         size_t dst = READ_SHORT(), protoIdx = READ_SHORT();
//                         if (protoIdx >= proto->getChunk().getConstantPoolSize() || !(proto->getChunk().getConstant(protoIdx)).is<Proto>()) {
//                             newVMError("CLOSURE constant must be a FunctionProto.");
//                         }
//                         auto childProto = proto->getChunk().getConstant(protoIdx).get<Proto>();
//                         auto closure = memoryManager->newObject<ObjClosure>(childProto);
//                         closure->upvalues.resize(childProto->upvalueDescs.size(), nullptr);

//                         for (size_t i = 0; i < childProto->upvalueDescs.size(); ++i) {
//                             auto& desc = childProto->upvalueDescs[i];
//                             if (desc.isLocal) {
//                                 closure->upvalues[i] = captureUpvalue(context->currentBase + desc.index);
//                             } else {
//                                 if (desc.index >= static_cast<Int>(currentFrame->closure->upvalues.size())) {
//                                     newVMError("CLOSURE: parent upvalue index OOB");
//                                 }
//                                 closure->upvalues[i] = currentFrame->closure->upvalues[desc.index];
//                             }
//                         }
//                         context->stackSlots[context->currentBase + dst] = Value(closure);
//                         break;
//                     }

//                     case OpCode::CLOSE_UPVALUES: {
//                         closeUpvalues(context->currentBase + READ_SHORT());
//                         break;
//                     }

//                     // --- Control flow ---
//                     case OpCode::JUMP: {
//                         Int target = READ_ADDRESS();
//                         if (target >= static_cast<Int>(proto->getChunk().getCodeSize()))
//                             newVMError("JUMP target OOB");
//                         currentFrame->ip = proto->getChunk().getCode() + target;
//                         break;
//                     }

//                     case OpCode::JUMP_IF_FALSE: {
//                         Int reg = READ_SHORT(), target = READ_ADDRESS();
//                         if (!asBool(context->stackSlots[context->currentBase + reg])) {
//                             if (target >= static_cast<Int>(proto->getChunk().getCodeSize()))
//                                 newVMError("JUMP_IF_FALSE target OOB");
//                             currentFrame->ip = proto->getChunk().getCode() + target;
//                         }
//                         break;
//                     }

//                     case OpCode::JUMP_IF_TRUE: {
//                         Int reg = READ_SHORT(), target = READ_ADDRESS();
//                         if (asBool(context->stackSlots[context->currentBase + reg])) {
//                             if (target >= static_cast<Int>(proto->getChunk().getCodeSize()))
//                                 newVMError("JUMP_IF_TRUE target OOB");
//                             currentFrame->ip = proto->getChunk().getCode() + target;
//                         }
//                         break;
//                     }
                    
//                     case OpCode::CALL: {
//                         Int dst = READ_SHORT(), fnReg = READ_SHORT(), argStart = READ_SHORT(), argc = READ_SHORT();
//                         auto& callee = context->stackSlots[context->currentBase + fnReg];
//                         executeCall(callee, dst, argStart, argc, context->currentBase);
//                         break;
//                     }
//                     case OpCode::CALL_VOID: {
//                         Int fnReg = READ_SHORT(), argStart = READ_SHORT(), argc = READ_SHORT();
//                         auto& callee = context->stackSlots[context->currentBase + fnReg];
//                         executeCall(callee, -1, argStart, argc, context->currentBase);
//                         break;
//                     }

//                     case OpCode::RETURN: {
//                         Value retVal = (currentFrame->retReg == -1) ? Value(Null{}) : context->stackSlots[context->currentBase + currentFrame->retReg];
//                         closeUpvalues(context->currentBase);

//                         CallFrame poppedFrame = *currentFrame;
//                         context->callStack.pop_back();

//                         if (context->callStack.empty()) {
//                             context->stackSlots.clear();
//                             currentFrame = nullptr;
//                             currentInst = nullptr;
//                             break;
//                         }
//                         currentFrame = &context->callStack.back();
//                         context->currentBase = currentFrame->slotStart;
//                         Int destReg = poppedFrame.retReg;
//                         if (destReg != -1) {
//                             Int need = currentFrame->slotStart + destReg + 1;
//                             if (static_cast<Int>(context->stackSlots.size()) < need) {
//                                 context->stackSlots.resize(need, Value(Null{}));
//                             }
//                             context->stackSlots[currentFrame->slotStart + destReg] = retVal;
//                         }
//                         currentFrame->ip = poppedFrame.ip;
//                         break;
//                     }

//                     case OpCode::HALT: {
//                         context->callStack.clear();
//                         break;
//                     }

//                     // --- Arrays / Hashes / Indexing ---
//                     case OpCode::NEW_ARRAY: {
//                         Int dst = READ_SHORT(), startIdx = READ_SHORT(), count = READ_SHORT();
//                         if (count < 0 || startIdx < 0) newVMError("NEW_ARRAY: invalid range");
//                         if (context->currentBase + startIdx + count > static_cast<Int>(context->stackSlots.size()))
//                             newVMError("NEW_ARRAY: register range OOB");

//                         Array arr = memoryManager->newObject<ObjArray>();
//                         arr->elements_.reserve(static_cast<size_t>(count));
//                         for (Int i = 0; i < count; ++i) {
//                             arr->elements_.push_back(context->stackSlots[context->currentBase + startIdx + i]);
//                         }
//                         context->stackSlots[context->currentBase + dst] = Value(arr);
//                         break;
//                     }

//                     case OpCode::NEW_HASH: {
//                         Int dst = READ_SHORT(), startIdx = READ_SHORT(), count = READ_SHORT();
//                         if (count < 0 || startIdx < 0) newVMError("NEW_HASH: invalid range");
//                         if (context->currentBase + startIdx + count*2 > static_cast<Int>(context->stackSlots.size()))
//                             newVMError("NEW_HASH: register range OOB");

//                         Object hm = memoryManager->newObject<ObjObject>();
//                         for (Int i = 0; i < count; ++i) {
//                             Value& key = context->stackSlots[context->currentBase + startIdx + i * 2];
//                             Value& val = context->stackSlots[context->currentBase + startIdx + i * 2 + 1];
//                             hm->fields[toString(key)] = val;
//                         }
//                         context->stackSlots[context->currentBase + dst] = Value(hm);
//                         break;
//                     }

//                     case OpCode::GET_INDEX: {
//                         Int dst = READ_SHORT(), srcReg = READ_SHORT(), keyReg = READ_SHORT();
//                         if (context->currentBase + srcReg >= static_cast<Int>(context->stackSlots.size()) ||
//                             context->currentBase + keyReg >= static_cast<Int>(context->stackSlots.size()) ||
//                             context->currentBase + dst >= static_cast<Int>(context->stackSlots.size()))
//                             newVMError("GET_INDEX register OOB");

//                         Value& src = context->stackSlots[context->currentBase + srcReg];
//                         Value& key = context->stackSlots[context->currentBase + keyReg];

//                         if (auto mm = getMagicMethod(src, "__getindex__")) {
//                             Value res = call(*mm, { key });
//                             context->stackSlots[context->currentBase + dst] = res;
//                             break;
//                         }
//                         if (key.is<Int>()) {
//                             Int idx = toInt(key);
//                             if (src.is<Array>()) {
//                                 Array arr = src.get<Array>();
//                                 if (!arr) newVMError("Array null in GET_INDEX");
//                                 auto &elems = arr->elements_;
//                                 if (idx < 0 || idx >= static_cast<Int>(elems.size())) {
//                                     std::ostringstream os;
//                                     os << "Chỉ số vượt quá phạm vi: '" << idx << "'. Truy cập trên mảng: `\n" << toString(arr) << "\n`";
//                                     newVMError(os.str());
//                                 }
//                                 context->stackSlots[context->currentBase + dst] = elems[idx];
//                                 break;
//                             }
//                             if (src.is<String>()) {
//                                 Str s = src.get<String>();
//                                 if (idx < 0 || idx >= static_cast<Int>(s->size())) {
//                                     std::ostringstream os;
//                                     os << "Chỉ số vượt quá phạm vi: '" << idx << "'. Truy cập trên string: `\n" << toString(s) << "\n`";
//                                     newVMError(os.str());
//                                 }
//                                 context->stackSlots[context->currentBase + dst] = Value(memoryManager->newObject<ObjString>(std::string(1, s->utf8()[idx])));
//                                 break;
//                             }
//                             if (src.is<Object>()) {
//                                 Object m = src.get<Object>();
//                                 std::string k = toString(key);
//                                 auto it = m->fields.find(k);
//                                 context->stackSlots[context->currentBase + dst] = (it != m->fields.end()) ? it->second : Value(Null{});
//                                 break;
//                             }
//                             newVMError("Numeric index not supported on type '" + toString(src) + "'");
//                         }
//                         std::string keyName = isString(key) ? key.get<String>()->utf8() : toString(key);

//                         if (auto mm = getMagicMethod(src, "__getprop__")) {
//                             Value res = call(*mm, { Value(memoryManager->newObject<ObjString>(keyName)) });
//                             context->stackSlots[context->currentBase + dst] = res;
//                             break;
//                         }

//                         if (auto mm2 = getMagicMethod(src, keyName)) {
//                             context->stackSlots[context->currentBase + dst] = *mm2;
//                             break;
//                         }
//                         context->stackSlots[context->currentBase + dst] = Value(Null{});
//                         break;
//                     }

//                     case OpCode::SET_INDEX: {
//                         Int srcReg = READ_SHORT(), keyReg = READ_SHORT(), valReg = READ_SHORT();
//                         if (context->currentBase + srcReg >= static_cast<Int>(context->stackSlots.size()) ||
//                             context->currentBase + keyReg >= static_cast<Int>(context->stackSlots.size()) ||
//                             context->currentBase + valReg >= static_cast<Int>(context->stackSlots.size()))
//                             newVMError("SET_INDEX register OOB");

//                         Value& src = context->stackSlots[context->currentBase + srcReg];
//                         Value& key = context->stackSlots[context->currentBase + keyReg];
//                         Value& val = context->stackSlots[context->currentBase + valReg];

//                         if (auto mm = getMagicMethod(src, "__setindex__")) {
//                             (void) call(*mm, { key, val });
//                             break;
//                         }

//                         if (key.is<Int>()) {
//                             Int idx = toInt(key);
//                             if (src.is<Array>()) {
//                                 Array arr = src.get<Array>();
//                                 if (idx < 0) newVMError("Invalid index");
//                                 if (idx >= static_cast<Int>(arr->elements_.size())) {
//                                     if (idx > 10000000) newVMError("Index too large");
//                                     arr->elements_.resize(static_cast<size_t>(idx + 1));
//                                 }
//                                 arr->elements_[static_cast<size_t>(idx)] = val;
//                                 break;
//                             }
//                             if (src.is<String>()) {
//                                 if (!val.is<String>() || val.get<String>()->empty()) newVMError("String assign must be non-empty string");
//                                 Str s = src.get<String>();
//                                 if (idx < 0 || idx >= static_cast<Int>(s->size())) {
//                                     std::ostringstream os;
//                                     os << "Chỉ số vượt quá phạm vi: '" << idx << "'. Truy cập trên string: `\n" << toString(s) << "\n`";
//                                     newVMError(os.str());
//                                 }
//                                 s->utf8()[static_cast<size_t>(idx)] = val.get<String>()->utf8()[0];
//                                 break;
//                             }
//                             if (src.is<Object>()) {
//                                 Object m = src.get<Object>();
//                                 std::string k = toString(key);
//                                 m->fields[k] = val;
//                                 break;
//                             }
//                             newVMError("Numeric index not supported on type '" + toString(src) + "'");
//                         }
//                         std::string keyName = isString(key) ? key.get<String>()->utf8() : toString(key);
//                         if (auto mm = getMagicMethod(src, "__setprop__")) {
//                             (void) call(*mm, { Value(memoryManager->newObject<ObjString>(keyName)), val });
//                             break;
//                         }

//                         if (src.is<Instance>()) {
//                             Instance inst = src.get<Instance>();
//                             inst->fields[keyName] = val;
//                             break;
//                         }
//                         if (src.is<Object>()) {
//                             Object m = src.get<Object>();
//                             m->fields[keyName] = val;
//                             break;
//                         }
//                         if (src.is<Class>()) {
//                             Class cls = src.get<Class>();
//                             if (!val.is<Function>() && !val.is<BoundMethod>()) newVMError("Method must be closure");
//                             cls->methods[keyName] = val;
//                             break;
//                         }

//                         newVMError("SET_INDEX not supported on type '" + toString(src) + "'");
//                         break;
//                     }

//                     case OpCode::GET_KEYS: {
//                         Int dst = READ_SHORT(), srcReg = READ_SHORT();
//                         if (context->currentBase + srcReg >= static_cast<Int>(context->stackSlots.size()))
//                             newVMError("GET_KEYS register OOB");
//                         Value& src = context->stackSlots[context->currentBase + srcReg];

//                         Array keysArr = memoryManager->newObject<ObjArray>();
//                         if (src.is<Instance>()) {
//                             Instance inst = src.get<Instance>();
//                             keysArr->elements_.reserve(inst->fields.size());
//                             for (const auto& pair : inst->fields) keysArr->elements_.push_back(Value(memoryManager->newObject<ObjString>(pair.first)));
//                         } else if (src.is<Object>()) {
//                             Object obj = src.get<Object>();
//                             keysArr->elements_.reserve(obj->fields.size());
//                             for (const auto& pair : obj->fields) keysArr->elements_.push_back(Value(memoryManager->newObject<ObjString>(pair.first)));
//                         } else if (src.is<Array>()) {
//                             Array arr = src.get<Array>();
//                             Int size = static_cast<Int>(arr->elements_.size());
//                             keysArr->elements_.reserve(size);
//                             for (Int i = 0; i < size; ++i) keysArr->elements_.push_back(Value(i));
//                         } else if (src.is<String>()) {
//                             String s = src.get<String>();
//                             Int size = static_cast<Int>(s->size());
//                             keysArr->elements_.reserve(size);
//                             for (Int i = 0; i < size; ++i) keysArr->elements_.push_back(Value(i));
//                         }

//                         context->stackSlots[context->currentBase + dst] = Value(keysArr);
//                         break;
//                     }

//                     case OpCode::GET_VALUES: {
//                         Int dst = READ_SHORT(), srcReg = READ_SHORT();
//                         if (context->currentBase + srcReg >= static_cast<Int>(context->stackSlots.size()))
//                             newVMError("GET_VALUES register OOB");
//                         Value& src = context->stackSlots[context->currentBase + srcReg];

//                         Array valueArr = memoryManager->newObject<ObjArray>();
//                         if (src.is<Instance>()) {
//                             Instance inst = src.get<Instance>();
//                             valueArr->elements_.reserve(inst->fields.size());
//                             for (const auto& pair : inst->fields) valueArr->elements_.push_back(pair.second);
//                         } else if (src.is<Object>()) {
//                             Object obj = src.get<Object>();
//                             valueArr->elements_.reserve(obj->fields.size());
//                             for (const auto& pair : obj->fields) valueArr->elements_.push_back(pair.second);
//                         } else if (src.is<Array>()) {
//                             Array arr = src.get<Array>();
//                             valueArr->elements_.reserve(arr->elements_.size());
//                             for (const auto& element : arr->elements_) valueArr->elements_.push_back(element);
//                         } else if (src.is<String>()) {
//                             String s = src.get<String>();
//                             for (const auto& c : s->utf8()) valueArr->elements_.push_back(Value(memoryManager->newObject<ObjString>(std::string(1, c))));
//                         }

//                         context->stackSlots[context->currentBase + dst] = Value(valueArr);
//                         break;
//                     }

//                     // --- Modules / imports / exports ---
//                     case OpCode::IMPORT_MODULE: {
//                         Int dst = READ_SHORT(), pathIdx = READ_SHORT();
//                         if (pathIdx >= static_cast<Int>(proto->getChunk().getConstantPoolSize()))
//                             newVMError("IMPORT_MODULE index OOB");
//                         if (!proto->getChunk().getConstant(pathIdx).is<String>())
//                             newVMError("IMPORT_MODULE path must be a string");

//                         Str importPath = proto->getChunk().getConstant(pathIdx).get<String>();
//                         Bool importerBinary = currentFrame->module->isBinary;

//                         auto loadedModule = moduleManager->loadModule(importPath->utf8(), currentFrame->module->getPath(), importerBinary);
//                         if (!loadedModule) {
//                             newVMError("Failed to load module: " + importPath->utf8());
//                         }
//                         auto mod = loadedModule.value();
//                         context->stackSlots[context->currentBase + dst] = Value(mod);

//                         if (mod->hasMain && !mod->isExecuted) {
//                             if (!mod->isExecuting) {
//                                 mod->isExecuting = true;
//                                 auto moduleClosure = memoryManager->newObject<ObjClosure>(mod->getMain());
//                                 Int newStart = static_cast<Int>(context->stackSlots.size());
//                                 context->stackSlots.resize(newStart + mod->getMain()->getNumRegisters(), Value(Null{}));
//                                 CallFrame newFrame(moduleClosure, newStart, mod, mod->getMain()->getChunk().getCode(), -1);
//                                 context->callStack.push_back(newFrame);
//                                 mod->isExecuted = true;
//                             }
//                         }
//                         break;
//                     }

//                     case OpCode::EXPORT: {
//                         Int nameIdx = READ_SHORT(), srcReg = READ_SHORT();
//                         if (nameIdx >= static_cast<Int>(proto->getChunk().getConstantPoolSize()))
//                             newVMError("EXPORT index OOB");
//                         if (!proto->getChunk().getConstant(nameIdx).is<String>())
//                             newVMError("EXPORT name must be a string");
//                         Str exportName = proto->getChunk().getConstant(nameIdx).get<String>();
//                         currentFrame->module->exports[exportName->utf8()] = context->stackSlots[context->currentBase + srcReg];
//                         break;
//                     }

//                     case OpCode::GET_EXPORT: {
//                         Int dst = READ_SHORT(), moduleReg = READ_SHORT(), nameIdx = READ_SHORT();
//                         if (context->currentBase + moduleReg >= static_cast<Int>(context->stackSlots.size()))
//                             newVMError("GET_EXPORT module register OOB");
//                         Value& moduleVal = context->stackSlots[context->currentBase + moduleReg];
//                         if (!moduleVal.is<Module>())
//                             newVMError("GET_EXPORT chỉ dùng với module");
//                         if (nameIdx >= static_cast<Int>(proto->getChunk().getConstantPoolSize()))
//                             newVMError("GET_EXPORT index OOB");
//                         if (!proto->getChunk().getConstant(nameIdx).is<String>())
//                             newVMError("Export name must be a string");
//                         Str exportName = proto->getChunk().getConstant(nameIdx).get<String>();
//                         auto mod = moduleVal.get<Module>();
//                         auto it = mod->exports.find(exportName->utf8());
//                         if (it == mod->exports.end())
//                             newVMError("Module '" + mod->getName() + "' không có export '" + exportName->utf8() + "'.");
//                         context->stackSlots[context->currentBase + dst] = it->second;
//                         break;
//                     }

//                     case OpCode::GET_MODULE_EXPORT: {
//                         Int dst = READ_SHORT(), moduleReg = READ_SHORT(), nameIdx = READ_SHORT();
//                         if (context->currentBase + moduleReg >= static_cast<Int>(context->stackSlots.size()))
//                             newVMError("GET_MODULE_EXPORT module register OOB");
//                         Value& moduleVal = context->stackSlots[context->currentBase + moduleReg];
//                         if (!moduleVal.is<Module>()) newVMError("GET_MODULE_EXPORT chỉ dùng với module.");
//                         if (nameIdx >= static_cast<Int>(proto->getChunk().getConstantPoolSize()) || !proto->getChunk().getConstant(nameIdx).is<String>())
//                             newVMError("Export name phải là string hợp lệ");
//                         Str exportName = proto->getChunk().getConstant(nameIdx).get<String>();
//                         auto mod = moduleVal.get<Module>();
//                         auto it = mod->exports.find(exportName->utf8());
//                         if (it == mod->exports.end())
//                             newVMError("Module '" + mod->getName() + "' không có export '" + exportName->utf8() + "'.");
//                         context->stackSlots[context->currentBase + dst] = it->second;
//                         break;
//                     }

//                     case OpCode::IMPORT_ALL: {
//                         Int moduleReg = READ_SHORT();
//                         if (context->currentBase + moduleReg >= static_cast<Int>(context->stackSlots.size()))
//                             newVMError("IMPORT_ALL register OOB");
//                         Value& moduleVal = context->stackSlots[context->currentBase + moduleReg];
//                         if (!moduleVal.is<Module>()) newVMError("IMPORT_ALL chỉ dùng với module.");
//                         auto importedModule = moduleVal.get<Module>();
//                         auto currentModule = currentFrame->module;
//                         for (const auto& pair : importedModule->exports) {
//                             currentModule->globals[pair.first] = pair.second;
//                         }
//                         break;
//                     }

//                     // --- Classes / instances / methods ---
//                     case OpCode::NEW_CLASS: {
//                         Int dst = READ_SHORT(), nameIdx = READ_SHORT();
//                         if (nameIdx >= static_cast<Int>(proto->getChunk().getConstantPoolSize()) || !proto->getChunk().getConstant(nameIdx).is<String>())
//                             newVMError("NEW_CLASS name must be a string");
//                         Str name = proto->getChunk().getConstant(nameIdx).get<String>();
//                         auto klass = memoryManager->newObject<ObjClass>(name->utf8());
//                         context->stackSlots[context->currentBase + dst] = Value(klass);
//                         break;
//                     }

//                     case OpCode::NEW_INSTANCE: {
//                         Int dst = READ_SHORT(), classReg = READ_SHORT();
//                         Value& clsVal = context->stackSlots[context->currentBase + classReg];
//                         if (!clsVal.is<Class>()) newVMError("NEW_INSTANCE trên giá trị không phải class");
//                         auto klass = clsVal.get<Class>();
//                         auto instObj = memoryManager->newObject<ObjInstance>(klass);
//                         context->stackSlots[context->currentBase + dst] = Value(instObj);
//                         break;
//                     }

//                     case OpCode::GET_PROP: {
//                         Int dst = READ_SHORT(), objReg = READ_SHORT(), nameIdx = READ_SHORT();
//                         if (context->currentBase + objReg >= static_cast<Int>(context->stackSlots.size()) ||
//                             context->currentBase + dst >= static_cast<Int>(context->stackSlots.size()))
//                             newVMError("GET_PROP register OOB");
//                         if (nameIdx >= static_cast<Int>(proto->getChunk().getConstantPoolSize()) || !proto->getChunk().getConstant(nameIdx).is<String>())
//                             newVMError("Property name must be a string");
//                         Str name = proto->getChunk().getConstant(nameIdx).get<String>();
//                         Value& obj = context->stackSlots[context->currentBase + objReg];

//                         if (obj.is<Instance>()) {
//                             Instance inst = obj.get<Instance>();
//                             auto it = inst->fields.find(name->utf8());
//                             if (it != inst->fields.end()) {
//                                 context->stackSlots[context->currentBase + dst] = it->second;
//                                 break;
//                             }
//                         }
//                         if (auto prop = getMagicMethod(obj, name->utf8())) {
//                             context->stackSlots[context->currentBase + dst] = *prop;
//                             break;
//                         }
//                         context->stackSlots[context->currentBase + dst] = Value(Null{});
//                         break;
//                     }

//                     case OpCode::SET_PROP: {
//                         Int objReg = READ_SHORT(), nameIdx = READ_SHORT(), valReg = READ_SHORT();
//                         if (context->currentBase + objReg >= static_cast<Int>(context->stackSlots.size()) ||
//                             context->currentBase + valReg >= static_cast<Int>(context->stackSlots.size()))
//                             newVMError("SET_PROP register OOB");
//                         if (nameIdx >= static_cast<Int>(proto->getChunk().getConstantPoolSize()) || !proto->getChunk().getConstant(nameIdx).is<String>())
//                             newVMError("Property name must be a string");
//                         Str name = proto->getChunk().getConstant(nameIdx).get<String>();
//                         Value& obj = context->stackSlots[context->currentBase + objReg];
//                         Value& val = context->stackSlots[context->currentBase + valReg];

//                         if (auto mm = getMagicMethod(obj, "__setprop__")) {
//                             (void) call(*mm, { Value(memoryManager->newObject<ObjString>(name->utf8())), val });
//                             break;
//                         }
//                         if (obj.is<Instance>()) {
//                             Instance inst = obj.get<Instance>();
//                             inst->fields[name->utf8()] = val;
//                             break;
//                         }
//                         if (obj.is<Object>()) {
//                             Object m = obj.get<Object>();
//                             m->fields[name->utf8()] = val;
//                             break;
//                         }
//                         if (obj.is<Class>()) {
//                             Class cls = obj.get<Class>();
//                             if (!val.is<Function>() && !val.is<BoundMethod>()) newVMError("Method must be closure");
//                             cls->methods[name->utf8()] = val;
//                             break;
//                         }
//                         newVMError("SET_PROP not supported on type '" + toString(obj) + "'");
//                         break;
//                     }

//                     case OpCode::SET_METHOD: {
//                         size_t classReg = READ_SHORT(), nameIdx = READ_SHORT(), methodReg = READ_SHORT();
//                         Value& klassVal = context->stackSlots[context->currentBase + classReg];
//                         if (!klassVal.is<Class>()) newVMError("SET_METHOD only for class");
//                         if (nameIdx >= static_cast<Int>(proto->getChunk().getConstantPoolSize()) || !proto->getChunk().getConstant(nameIdx).is<String>())
//                             newVMError("Method name must be a string");
//                         Str name = proto->getChunk().getConstant(nameIdx).get<String>();
//                         if (!(context->stackSlots[context->currentBase + methodReg]).is<Function>())
//                             newVMError("Method value must be a closure");
//                         klassVal.get<Class>()->methods[name->utf8()] = context->stackSlots[context->currentBase + methodReg];
//                         break;
//                     }

//                     case OpCode::INHERIT: {
//                         size_t subClassReg = READ_SHORT(), superClassReg = READ_SHORT();
//                         Value& subClassVal = context->stackSlots[context->currentBase + subClassReg];
//                         Value& superClassVal = context->stackSlots[context->currentBase + superClassReg];
//                         if (!subClassVal.is<Class>() || !superClassVal.is<Class>()) newVMError("Cả hai toán hạng cho kế thừa phải là class.");
//                         subClassVal.get<Class>()->superclass = superClassVal.get<Class>();
//                         auto& subMethods = subClassVal.get<Class>()->methods;
//                         auto& superMethods = superClassVal.get<Class>()->methods;
//                         for (const auto& pair : superMethods) {
//                             if (subMethods.find(pair.first) == subMethods.end()) {
//                                 subMethods[pair.first] = pair.second;
//                             }
//                         }
//                         break;
//                     }

//                     case OpCode::GET_SUPER: {
//                         size_t dst = READ_SHORT(), nameIdx = READ_SHORT();
//                         if (nameIdx >= proto->getChunk().getConstantPoolSize() || !proto->getChunk().getConstant(nameIdx).is<String>())
//                             newVMError("GET_SUPER name must be a string");
//                         Str methodName = proto->getChunk().getConstant(nameIdx).get<String>();

//                         Value& receiverVal = context->stackSlots[context->currentBase + 0];
//                         if (!receiverVal.is<Instance>()) newVMError("`super` can only be used inside a method.");
//                         Instance receiver = receiverVal.get<Instance>();

//                         if (!receiver->klass->superclass) newVMError("Class '" + receiver->klass->getName() + "' has no superclass.");
//                         Class superclass = *receiver->klass->superclass;

//                         auto it = superclass->methods.find(methodName->utf8());
//                         if (it == superclass->methods.end()) newVMError("Superclass '" + superclass->getName() + "' has no method named '" + methodName->utf8() + "'.");
//                         Value& method = it->second;
//                         if (!method.is<Function>()) newVMError("Superclass method is not a callable closure.");

//                         auto bound = memoryManager->newObject<ObjBoundMethod>(receiver, method.get<Function>());
//                         context->stackSlots[context->currentBase + dst] = Value(bound);
//                         break;
//                     }

//                     // --- Exception handling ---
//                     case OpCode::SETUP_TRY: {
//                         size_t target = READ_SHORT();
//                         ExceptionHandler h(target, context->callStack.size() - 1, context->stackSlots.size());
//                         context->exceptionHandlers.push_back(h);
//                         break;
//                     }

//                     case OpCode::POP_TRY: {
//                         if (!context->exceptionHandlers.empty()) context->exceptionHandlers.pop_back();
//                         break;
//                     }

//                     case OpCode::THROW: {
//                         size_t reg = READ_SHORT();
//                         throw VMError(toString(context->stackSlots[context->currentBase + reg]));
//                     }

//                     // --- Fallback ---
//                     default: {
//                         newVMError("Unsupported OpCode!");
//                         break;
//                     }
//                 } // end switch
//             } // end GC scope
//         } catch (const VMError& e) {
//             handleRuntimeException(e);
//         } catch (const std::exception& e) {
//             std::cerr << "🤯 Lỗi C++ không lường trước trong VM.run: " << e.what() << std::endl;
//             context->callStack.clear();
//         }
//     } // end while
// }


// #include "vm/meow_vm.h"

// Function MeowVM::wrapClosure(const Value& maybeCallable) {
//     if (maybeCallable.is<Function>()) {
//         return maybeCallable.get<Function>();
//     }

//     if (maybeCallable.is<BoundMethod>()) {
//         BoundMethod bm = maybeCallable.get<BoundMethod>();
//         if (!bm || !bm->callable) newVMError("wrapClosure: BoundMethod không có callable.");
//         return bm->callable;
//     }


//     newVMError("wrapClosure: Giá trị không phải Closure/BoundMethod.");
// }

// std::optional<Value> MeowVM::getMagicMethod(const Value& obj, const std::string& name) {

//     if (obj.is<Instance>()) {
//         Instance inst = obj.get<Instance>();
//         if (!inst) return std::nullopt;


//         auto fit = inst->fields.find(name);
//         if (fit != inst->fields.end()) {
//             const Value& v = fit->second;

//             if (v.is<Function>()) {
//                 Function f = v.get<Function>();
//                 auto bm = memoryManager->newObject<ObjBoundMethod>(inst, f);
//                 return Value(bm);
//             }

//             if (v.is<BoundMethod>()) {
//                 BoundMethod inbm = v.get<BoundMethod>();
//                 if (inbm && inbm->callable) {
//                     auto bm = memoryManager->newObject<ObjBoundMethod>(inst, inbm->callable);
//                     return Value(bm);
//                 }
//             }

//             if (v.is<NativeFn>()) {
//                 NativeFn orig = v.get<NativeFn>();
//                 NativeFnAdvanced wrapper = [orig, inst](MeowEngine* engine, Arguments args)->Value {
//                     std::vector<Value> newArgs;
//                     newArgs.reserve(1 + args.size());
//                     newArgs.push_back(Value(inst));
//                     newArgs.insert(newArgs.end(), args.begin(), args.end());
//                     return std::visit([&](auto&& fn) -> Value {
//                         using T = std::decay_t<decltype(fn)>;
//                         if constexpr (std::is_same_v<T, NativeFnSimple>) {
//                             return fn(newArgs);
//                         } else {
//                             return fn(engine, newArgs);
//                         }
//                     }, orig);
//                 };
//                 return Value(wrapper);
//             }

//             return Value(v);
//         }


//         Class cur = inst->klass;
//         while (cur) {
//             auto mit = cur->methods.find(name);
//             if (mit != cur->methods.end()) {
//                 const Value& mv = mit->second;
//                 if (mv.is<Function>()) {
//                     Function f = mv.get<Function>();
//                     auto bm = memoryManager->newObject<ObjBoundMethod>(inst, f);
//                     return Value(bm);
//                 }
//                 if (mv.is<BoundMethod>()) {
//                     BoundMethod inbm = mv.get<BoundMethod>();
//                     if (inbm && inbm->callable) {
//                         auto bm = memoryManager->newObject<ObjBoundMethod>(inst, inbm->callable);
//                         return Value(bm);
//                     }
//                 }
//                 if (mv.is<NativeFn>()) {
//                     NativeFn orig = mv.get<NativeFn>();
//                     NativeFnAdvanced wrapper = [orig, inst](MeowEngine* engine, Arguments args)->Value {
//                         std::vector<Value> newArgs;
//                         newArgs.reserve(1 + args.size());
//                         newArgs.push_back(Value(inst));
//                         newArgs.insert(newArgs.end(), args.begin(), args.end());
//                         return std::visit([&](auto&& fn) -> Value {
//                             using T = std::decay_t<decltype(fn)>;
//                             if constexpr (std::is_same_v<T, NativeFnSimple>) {
//                                 return fn(newArgs);
//                             } else {
//                                 return fn(engine, newArgs);
//                             }
//                         }, orig);
//                     };
//                     return Value(wrapper);
//                 }

//                 return Value(mv);
//             }
//             if (cur->superclass) cur = *cur->superclass;
//             else break;
//         }
//     }


//     if (obj.is<Object>()) {
//         Object objPtr = obj.get<Object>();
//         if (!objPtr) return std::nullopt;
//         auto fit = objPtr->fields.find(name);
//         if (fit != objPtr->fields.end()) return Value(fit->second);
//         auto pgit = builtinGetters.find("Object");
//         if (pgit != builtinGetters.end()) {
//             auto it = pgit->second.find(name);
//             if (it != pgit->second.end()) {
//                 return this->call(it->second, { obj }); 
//             }
//         }
//         auto pit = builtinMethods.find("Object");
//         if (pit != builtinMethods.end()) {
//             auto it = pit->second.find(name);
//             if (it != pit->second.end()) {
//                 const Value& mv = it->second;
//                 if (mv.is<NativeFn>()) {
//                     NativeFn orig = mv.get<NativeFn>();
//                     NativeFnAdvanced wrapper = [orig, objPtr](MeowEngine* engine, Arguments args)->Value {
//                         std::vector<Value> newArgs;
//                         newArgs.reserve(1 + args.size());
//                         newArgs.push_back(Value(objPtr));
//                         newArgs.insert(newArgs.end(), args.begin(), args.end());
//                         return std::visit([&](auto&& fn) -> Value {
//                             using T = std::decay_t<decltype(fn)>;
//                             if constexpr (std::is_same_v<T, NativeFnSimple>) {
//                                 return fn(newArgs);
//                             } else {
//                                 return fn(engine, newArgs);
//                             }
//                         }, orig);
//                     };
//                     return Value(wrapper);
//                 }
//                 return Value(it->second);
//             }
//         }
//         return std::nullopt;
//     }


//     if (obj.is<Array>()) {
//         Array arr = obj.get<Array>();
//         if (!arr) return std::nullopt;

//         auto pgit = builtinGetters.find("Array");
//         if (pgit != builtinGetters.end()) {
//             auto it = pgit->second.find(name);
//             if (it != pgit->second.end()) {
//                 return this->call(it->second, { obj }); 
//             }
//         }
//         auto pit = builtinMethods.find("Array");
//         if (pit != builtinMethods.end()) {
//             auto it = pit->second.find(name);
//             if (it != pit->second.end()) {
//                 const Value& mv = it->second;
//                 if (mv.is<NativeFn>()) {
//                     NativeFn orig = mv.get<NativeFn>();
//                     NativeFnAdvanced wrapper = [orig, arr](MeowEngine* engine, Arguments args)->Value {
//                         std::vector<Value> newArgs;
//                         newArgs.reserve(1 + args.size());
//                         newArgs.push_back(Value(arr));
//                         newArgs.insert(newArgs.end(), args.begin(), args.end());
//                         return std::visit([&](auto&& fn) -> Value {
//                             using T = std::decay_t<decltype(fn)>;
//                             if constexpr (std::is_same_v<T, NativeFnSimple>) {
//                                 return fn(newArgs);
//                             } else {
//                                 return fn(engine, newArgs);
//                             }
//                         }, orig);
//                     };
//                     return Value(wrapper);
//                 }
//                 return Value(it->second);
//             }
//         }
//         return std::nullopt;
//     }


//     if (obj.is<String>()) {
//         auto pgit = builtinGetters.find("String");
//         if (pgit != builtinGetters.end()) {
//             auto it = pgit->second.find(name);
//             if (it != pgit->second.end()) {
//                 return this->call(it->second, { obj }); 
//             }
//         }
//         auto pit = builtinMethods.find("String");
//         if (pit != builtinMethods.end()) {
//             auto it = pit->second.find(name);
//             if (it != pit->second.end()) {
//                 const Value& mv = it->second;
//                 if (mv.is<NativeFn>()) {
//                     NativeFn orig = mv.get<NativeFn>();
//                     NativeFnAdvanced wrapper = [orig, obj](MeowEngine* engine, Arguments args)->Value {
//                         std::vector<Value> newArgs;
//                         newArgs.reserve(1 + args.size());
//                         newArgs.push_back(obj);
//                         newArgs.insert(newArgs.end(), args.begin(), args.end());
//                         return std::visit([&](auto&& fn) -> Value {
//                             using T = std::decay_t<decltype(fn)>;
//                             if constexpr (std::is_same_v<T, NativeFnSimple>) {
//                                 return fn(newArgs);
//                             } else {
//                                 return fn(engine, newArgs);
//                             }
//                         }, orig);
//                     };
//                     return Value(wrapper);
//                 }
//                 return Value(it->second);
//             }
//         }
//         return std::nullopt;
//     }


//     if (obj.is<Int>() || obj.is<Real>() || obj.is<Bool>()) {
//         std::string typeName;
//         if (obj.is<Int>()) typeName = "Int";
//         else if (obj.is<Real>()) typeName = "Real";
//         else typeName = "Bool";

//         auto pgit = builtinGetters.find(typeName);
//         if (pgit != builtinGetters.end()) {
//             auto it = pgit->second.find(name);
//             if (it != pgit->second.end()) {
//                 return this->call(it->second, { obj }); 
//             }
//         }

//         auto pit = builtinMethods.find(typeName);
//         if (pit != builtinMethods.end()) {
//             auto it = pit->second.find(name);
//             if (it != pit->second.end()) {
//                 const Value& mv = it->second;
//                 if (mv.is<NativeFn>()) {
//                     NativeFn orig = mv.get<NativeFn>();
//                     NativeFnAdvanced wrapper = [orig, obj](MeowEngine* engine, Arguments args)->Value {
//                         std::vector<Value> newArgs;
//                         newArgs.reserve(1 + args.size());
//                         newArgs.push_back(obj);
//                         newArgs.insert(newArgs.end(), args.begin(), args.end());
//                         return std::visit([&](auto&& fn) -> Value {
//                             using T = std::decay_t<decltype(fn)>;
//                             if constexpr (std::is_same_v<T, NativeFnSimple>) {
//                                 return fn(newArgs);
//                             } else {
//                                 return fn(engine, newArgs);
//                             }
//                         }, orig);
//                     };
//                     return Value(wrapper);
//                 }
//                 return Value(it->second);
//             }
//         }
//         return std::nullopt;
//     }


//     if (obj.is<Class>()) {
//         Class klass = obj.get<Class>();
//         if (!klass) return std::nullopt;
//         auto mit = klass->methods.find(name);
//         if (mit != klass->methods.end()) {
//             return Value(mit->second);
//         }
//     }

//     return std::nullopt;
// }


// #include "vm/meow_vm.h"
// #include "common/pch.h"

// Str MeowVM::opToString(OpCode op) const {
//     switch (op) {
//         case OpCode::LOAD_CONST: return "LOAD_CONST";
//         case OpCode::LOAD_NULL: return "LOAD_NULL";
//         case OpCode::LOAD_TRUE: return "LOAD_TRUE";
//         case OpCode::LOAD_FALSE: return "LOAD_FALSE";
//         case OpCode::LOAD_INT: return "LOAD_INT";
//         case OpCode::MOVE: return "MOVE";
//         case OpCode::ADD: return "ADD";
//         case OpCode::SUB: return "SUB";
//         case OpCode::MUL: return "MUL";
//         case OpCode::DIV: return "DIV";
//         case OpCode::MOD: return "MOD";
//         case OpCode::POW: return "POW";
//         case OpCode::EQ: return "EQ";
//         case OpCode::NEQ: return "NEQ";
//         case OpCode::GT: return "GT";
//         case OpCode::GE: return "GE";
//         case OpCode::LT: return "LT";
//         case OpCode::LE: return "LE";
//         case OpCode::NEG: return "NEG";
//         case OpCode::NOT: return "NOT";
//         case OpCode::GET_GLOBAL: return "GET_GLOBAL";
//         case OpCode::SET_GLOBAL: return "SET_GLOBAL";
//         case OpCode::GET_UPVALUE: return "GET_UPVALUE";
//         case OpCode::SET_UPVALUE: return "SET_UPVALUE";
//         case OpCode::CLOSURE: return "CLOSURE";
//         case OpCode::CLOSE_UPVALUES: return "CLOSE_UPVALUES";
//         case OpCode::JUMP: return "JUMP";
//         case OpCode::JUMP_IF_FALSE: return "JUMP_IF_FALSE";
//         case OpCode::JUMP_IF_TRUE: return "JUMP_IF_TRUE";
//         case OpCode::CALL: return "CALL";
//         case OpCode::RETURN: return "RETURN";
//         case OpCode::HALT: return "HALT";
//         case OpCode::NEW_ARRAY: return "NEW_ARRAY";
//         case OpCode::NEW_HASH: return "NEW_HASH";
//         case OpCode::GET_INDEX: return "GET_INDEX";
//         case OpCode::SET_INDEX: return "SET_INDEX";
//         case OpCode::GET_KEYS: return "GET_KEYS";
//         case OpCode::GET_VALUES: return "GET_VALUES";
//         case OpCode::NEW_CLASS: return "NEW_CLASS";
//         case OpCode::NEW_INSTANCE: return "NEW_INSTANCE";
//         case OpCode::GET_PROP: return "GET_PROP";
//         case OpCode::SET_PROP: return "SET_PROP";
//         case OpCode::SET_METHOD: return "SET_METHOD";
//         case OpCode::INHERIT: return "INHERIT";
//         case OpCode::GET_SUPER: return "GET_SUPER";
//         case OpCode::BIT_AND: return "BIT_AND";
//         case OpCode::BIT_OR: return "BIT_OR";
//         case OpCode::BIT_XOR: return "BIT_XOR";
//         case OpCode::BIT_NOT: return "BIT_NOT";
//         case OpCode::LSHIFT: return "LSHIFT";
//         case OpCode::RSHIFT: return "RSHIFT";
//         case OpCode::THROW: return "THROW";
//         case OpCode::SETUP_TRY: return "SETUP_TRY";
//         case OpCode::POP_TRY: return "POP_TRY";
//         case OpCode::IMPORT_MODULE: return "IMPORT_MODULE";
//         case OpCode::EXPORT: return "EXPORT";
//         case OpCode::GET_EXPORT: return "GET_EXPORT";
//         case OpCode::GET_MODULE_EXPORT: return "GET_MODULE_EXPORT";
//         case OpCode::IMPORT_ALL: return "IMPORT_ALL";
//         case OpCode::TOTAL_OPCODES: return "TOTAL_OPCODES";
//         default: return "UNKNOWN_OPCODE";
//     }
// }

// void MeowVM::newVMError(const Str& msg) {
//     std::ostringstream os;


//     auto valueToString = [&](const Value& v) -> Str {
//         if (v.is<Null>()) return "<null>";
//         if (v.is<Int>()) return std::to_string(v.get<Int>());
//         if (v.is<Real>()) {
//             std::ostringstream t;
//             Real r = v.get<Real>();
//             if (std::isnan(r)) return "NaN";
//             if (std::isinf(r)) return (r > 0) ? "Infinity" : "-Infinity";
//             t << r;
//             return t.str();
//         }
//         if (v.is<Bool>()) return v.get<Bool>() ? "true" : "false";
//         if (v.is<Str>()) return Str("\"") + v.get<Str>() + Str("\"");
//         if (v.is<Proto>()) return "<function proto>";
//         if (v.is<Function>()) return "<closure>";
//         if (v.is<Instance>()) return "<instance>";
//         if (v.is<Class>()) return "<class>";
//         if (v.is<Array>()) return "<array>";
//         if (v.is<Object>()) return "<object>";
//         if (v.is<Upvalue>()) return "<upvalue>";
//         if (v.is<Module>()) return "<module>";
//         if (v.is<BoundMethod>()) return "<bound method>";
//         if (v.is<NativeFn>()) return "<native fn>";
//         return "<unknown value>";
//     };


//     os << "!!! 🐛 LỖI NGHIÊM TRỌNG: `" << msg << "` 🐛 !!!\n";
//     os << "  - VM Base: " << context->currentBase << "\n";


//     ObjFunctionProto* proto = nullptr;
//     if (currentFrame && currentFrame->closure) proto = currentFrame->closure->proto;
//     if (proto) {
//         Int rawIp = currentFrame->ip;
//         Int errorIndex = (rawIp > 0) ? (rawIp - 1) : 0;
//         const Int codeSize = static_cast<Int>(proto->code.size());

//         os << "  - Source: " << proto->sourceName << "\n";
//         os << "  - Bytecode index (in-func): " << errorIndex << "\n";
//         os << "  - Opcode at error: "
//            << ((errorIndex >= 0 && errorIndex < codeSize) ? opToString(proto->code[static_cast<size_t>(errorIndex)].op) : Str("<out-of-range>"))
//            << "\n\n";


//         const Int range = 5;
//         if (codeSize > 0) {
//             const Int start = std::max<Int>(0, errorIndex - range);
//             const Int end = std::min<Int>(codeSize - 1, errorIndex + range);


//             int maxOpLen = 0;
//             for (Int i = start; i <= end; ++i) {
//                 int len = static_cast<int>(opToString(proto->code[static_cast<size_t>(i)].op).size());
//                 if (len > maxOpLen) maxOpLen = len;
//             }
//             int opField = std::max(10, maxOpLen + 2);

//             os << "  - Vùng bytecode (±" << range << "):\n";

//             std::ios::fmtflags savedFlags = os.flags();
//             for (Int i = start; i <= end; ++i) {
//                 const Instruction& inst = proto->code[static_cast<size_t>(i)];

//                 const char* prefix = (i == errorIndex) ? "  >> " : "     ";
//                 os << prefix;


//                 os << std::right << std::setw(4) << i << ": ";


//                 os << std::left << std::setw(opField) << opToString(inst.op);


//                 if (!inst.args.empty()) {
//                     os << "  args=[";
//                     for (size_t a = 0; a < inst.args.size(); ++a) {
//                         if (a) os << ", ";
//                         Int arg = inst.args[a];
//                         os << arg;
//                         // if (!proto->constantPool.empty() && arg >= 0 && static_cast<size_t>(arg) < proto->constantPool.size()) {
//                         //     os << " -> " << valueToString(proto->constantPool[static_cast<size_t>(arg)]);
//                         // }
//                     }
//                     os << "]";
//                 } else {
//                     os << "  args=[]";
//                 }

//                 if (i == errorIndex) os << "    <-- lỗi\n"; else os << "\n";


//                 os.flags(savedFlags);
//             }
//         } else {
//             os << "  - (Bytecode rỗng)\n";
//         }

//         if (!proto->constantPool.empty()) {
//             os << "\n  - Constant pool (preview up to 10):\n";
//             size_t maxShow = std::min<size_t>(proto->constantPool.size(), 10);
//             for (size_t ci = 0; ci < maxShow; ++ci) {
//                 os << "     [" << ci << "]: " << valueToString(proto->constantPool[ci]) << "\n";
//             }
//         }
//     } else {
//         os << "  (Không có proto/closure để in chi tiết bytecode.)\n";
//     }
//     os << "\n";
//     os << "  - Call stack (most recent first):\n";
//     if (context->callStack.empty()) {
//         os << "     <empty call stack>\n";
//     } else {
//         for (int i = static_cast<int>(context->callStack.size()) - 1, depth = 0; i >= 0; --i, ++depth) {
//             const CallFrame& f = context->callStack[static_cast<size_t>(i)];
//             Str src = "<native>";
//             Int ip = f.ip;
//             if (f.closure && f.closure->proto) {
//                 src = f.closure->proto->sourceName;
//             }
//             os << "     #" << depth << " " << src << "  ip=" << ip << "  slotStart=" << f.slotStart << "  retReg=" << f.retReg << "\n";
//         }
//     }

//     os << "\n";


//     os << "  - Stack snapshot (context->stackSlots size = " << context->stackSlots.size() << "):\n";
//     if (context->stackSlots.empty()) {
//         os << "     <empty stack>\n";
//     } else {
//         const size_t maxAround = 8;
//         Int base = context->currentBase;
//         size_t start = static_cast<size_t>(std::max<Int>(0, base));
//         size_t end = std::min(context->stackSlots.size(), start + maxAround);

//         std::ios::fmtflags savedFlags = os.flags();
//         for (size_t i = start; i < end; ++i) {
//             os << ((static_cast<Int>(i) == context->currentBase) ? "  >> " : "     ");
//             os << std::right << std::setw(4) << i << ": " << valueToString(context->stackSlots[i]) << "\n";
//             os.flags(savedFlags);
//         }
//         if (end < context->stackSlots.size()) {
//             os << "     ...\n";
//             size_t topCount = std::min<size_t>(3, context->stackSlots.size());
//             for (size_t i = context->stackSlots.size() - topCount; i < context->stackSlots.size(); ++i) {
//                 os << "     (top) " << std::right << std::setw(4) << i << ": " << valueToString(context->stackSlots[i]) << "\n";
//                 os.flags(savedFlags);
//             }
//         }
//     }

//     os << "\n";


//     os << "  - Open upvalues (" << context->openUpvalues.size() << "):\n";
//     if (context->openUpvalues.empty()) {
//         os << "     <none>\n";
//     } else {
//         for (size_t i = 0; i < context->openUpvalues.size(); ++i) {
//             const Upvalue uv = context->openUpvalues[i];
//             if (!uv) {
//                 os << "     [" << i << "]: <null upvalue>\n";
//             } else {
//                 os << "     [" << i << "]: slotIndex=" << uv->slotIndex << " state="
//                    << (uv->state == ObjUpvalue::State::OPEN ? "OPEN" : "CLOSED")
//                    << " value=" << (uv->state == ObjUpvalue::State::CLOSED ? valueToString(uv->closed) : "<live slot>") << "\n";
//             }
//         }
//     }

//     os << "\n";

//     os << "  - Exception handlers (" << context->exceptionHandlers.size() << "):\n";
//     if (context->exceptionHandlers.empty()) {
//         os << "     <none>\n";
//     } else {
//         for (size_t i = 0; i < context->exceptionHandlers.size(); ++i) {
//             const auto& h = context->exceptionHandlers[i];
//             os << "     [" << i << "] catchIp=" << h.catchIp << " frameDepth=" << h.frameDepth << " stackDepth=" << h.stackDepth << "\n";
//         }
//     }
//     throw VMError(os.str());
// }


// #include "vm/meow_vm.h"
// #include "common/expected.h"
// #include "diagnostics/diagnostic.h"
// #include "runtime/execution_context.h"
// #include "memory/memory_manager.h"

// Upvalue MeowVM::captureUpvalue(size_t slotIndex) {
//     if (slotIndex >= context->stackSlots.size())
//         newVMError("captureUpvalue: slotIndex OOB: " + std::to_string(slotIndex) + " stackSize=" + std::to_string(context->stackSlots.size()));

//     for (auto it = context->openUpvalues.rbegin(); it != context->openUpvalues.rend(); ++it) {
//         if ((*it)->slotIndex == slotIndex) {
//             return *it;
//         }

//         if ((*it)->slotIndex < slotIndex) {
//             break; 
//         }
//     }

//     auto newUv = memoryManager->newObject<ObjUpvalue>(slotIndex);

//     auto it = context->openUpvalues.begin();
//     while (it != context->openUpvalues.end() && (*it)->slotIndex < slotIndex) {
//         ++it;
//     }
//     context->openUpvalues.insert(it, newUv);
    
//     return newUv;
// }

// void MeowVM::closeUpvalues(size_t slotIndex) {
//     while (!context->openUpvalues.empty() && context->openUpvalues.back()->slotIndex >= slotIndex) {
//         auto up = context->openUpvalues.back();
//         up->close(context->stackSlots[up->slotIndex]);
//         context->openUpvalues.pop_back();
//     }
// }

// void MeowVM::executeCall(const Value callee, size_t dst, size_t argStart, size_t argc, size_t base) {

//     std::vector<Value> args;
//     args.reserve(argc);
//     for (size_t i = 0; i < argc; ++i) {
//         args.push_back(context->stackSlots[base + argStart + i]);
//     }

//     if (callee.is<Function>()) {
//         auto closure = callee.get<Function>();
//         size_t newStart = context->stackSlots.size();
//         size_t needed = newStart + closure->getProto()->getNumRegisters();
//         context->stackSlots.resize(needed, Value(Null{}));

//         if (context->stackSlots.size() < needed) {
//             newVMError("context->stackSlots quá nhỏ khi resize trong executeCall. "
//                         "cần=" + std::to_string(needed) +
//                         " đã có=" + std::to_string(context->stackSlots.size()));
//         }

//         CallFrame newFrame(closure, newStart, context->callStack.back().module, closure->getProto()->getChunk().getCode(), dst);
//         context->callStack.push_back(newFrame);

//         for (size_t i = 0; i < std::min(argc, closure->getProto()->getNumRegisters()); ++i) {
//             context->stackSlots[newStart + i] = args[i];
//         }
//     } else if (callee.is<BoundMethod>()) {
//         auto boundMethod = callee.get<BoundMethod>();
//         if (!(boundMethod->callable).is<Function>()) newVMError("Bound method không chứa một closure có thể gọi được.");
//         auto methodClosure = boundMethod->callable;
//         size_t newStart = context->stackSlots.size();
//         context->stackSlots.resize(newStart + methodClosure->getProto()->getNumRegisters());
//         CallFrame newFrame(methodClosure, newStart, context->callStack.back().module, methodClosure->getProto()->getChunk().getCode(), dst);
//         context->callStack.push_back(newFrame);
//         context->stackSlots[newStart + 0] = Value(boundMethod->receiver);
//         for (size_t i = 0; i < std::min(argc, methodClosure->getProto()->getNumRegisters() - 1); ++i) {
//             context->stackSlots[newStart + 1 + i] = args[i];
//         }
//     } else if (callee.is<Class>()) {
//         auto klass = callee.get<Class>();
//         auto instance = memoryManager->newObject<ObjInstance>(klass);
//         if (dst != -1) context->stackSlots[base + dst] = Value(instance);
//         auto it = klass->methods.find("init");
//         if (it != klass->methods.end() && (it->second).is<Function>()) {
//             auto boundInit = memoryManager->newObject<ObjBoundMethod>(instance, it->second.get<Function>());
//             executeCall(Value(boundInit), -1, argStart, argc, base);
//         }
//     } else if (callee.is<NativeFn>()) {
//         auto func = callee.get<NativeFn>();
//         Value result = std::visit(
//             [&](auto&& func) -> Value {
//                 using T = std::decay_t<decltype(func)>;
//                 if constexpr (std::is_same_v<T, NativeFnSimple>) {
//                     return func(args);
//                 } else if constexpr (std::is_same_v<T, NativeFnAdvanced>) {
//                     return func(this, args);
//                 }
//                 return Value(Null{});
//             },
//             func
//         );
//         if (dst != -1) context->stackSlots[base + dst] = result;
//     } else {
//         std::ostringstream os;
//         os << "Giá trị kiểu '" << toString(callee) << "' không thể gọi được: '" + toString(callee) + "' ";
//         os << "với các tham số là: ";
//         for (const auto& arg : args) {
//             os << toString(arg) << " ";
//         }
//         os << "\n";
//         newVMError(os.str());
//     }
// }

// Value MeowVM::call(const Value callee, Arguments args) {
//     size_t startCallDepth = context->callStack.size();

//     size_t argStartAbs = context->stackSlots.size();
//     context->stackSlots.resize(argStartAbs + args.size());
//     for (size_t i = 0; i < args.size(); ++i) {
//         context->stackSlots[argStartAbs + i] = args[i];
//     }

//     size_t dstAbs = context->stackSlots.size();
//     context->stackSlots.resize(dstAbs + 1);

//     size_t argStartRel = argStartAbs - context->currentBase;
//     Int dstRel      = static_cast<Int>(dstAbs - context->currentBase);
//     if (argStartRel >= static_cast<size_t>(context->callStack.back().closure->getProto()->getNumRegisters()) || dstRel < -1) {
//         newVMError("Internal error: invalid relative arg/dst in VM::call");
//     }

//     executeCall(callee, dstRel, argStartRel, args.size(), context->currentBase);

//     while (context->callStack.size() > startCallDepth) {

//         currentFrame = &context->callStack.back();
//         auto proto = currentFrame->closure->getProto();
//         context->currentBase = currentFrame->slotStart;

//         if ((currentFrame->ip - proto->getChunk().getCode()) >= static_cast<Int>(proto->getChunk().getCodeSize())) {

//             if (currentFrame->retReg != -1 && context->callStack.size() > 1) {
//                 CallFrame& parent = context->callStack[context->callStack.size() - 2];
//                 size_t idx = parent.slotStart + currentFrame->retReg;
//                 if (idx < context->stackSlots.size()) context->stackSlots[idx] = Value(Null{});
//             }
//             context->callStack.pop_back();
//             continue;
//         }

//         try {
//             uint8_t opcode = *currentFrame->ip++;
//             // Dù code cũ dùng jumpTable, nhưng đoạn này bị lặp lại trong run() và call()
//             // Tốt nhất là refactor để chỉ dùng một vòng lặp run() duy nhất.
//             // Tạm thời, tôi sẽ để lại phần này như cũ để không làm thay đổi quá nhiều cấu trúc.
//         } catch (const VMError& e) {
//             handleRuntimeException(e);
//         } catch (const std::exception& e) {
//             std::cerr << "🤯 Lỗi C++ không lường trước trong VM::call loop: " << e.what() << std::endl;
//             context->callStack.clear();
//         }
//     }

//     Value result = context->stackSlots[dstAbs];

//     context->stackSlots.resize(argStartAbs);
//     return result;
// }


// #include "vm/meow_vm.h"
// #include "common/pch.h"

// template<class... Ts> 
// struct overloaded : Ts... {
//     using Ts::operator()...;
// };

// template<class... Ts> 
// overloaded(Ts...) -> overloaded<Ts...>;

// void MeowVM::defineNativeFunctions() {
//     auto nativePrint = [this](Arguments args) -> Value {
//         Str outputString;
//         for (size_t i = 0; i < args.size(); ++i) {
//             if (i > 0) outputString += " ";
//             outputString += _toString(args[i]);
//         }

//         std::cout << outputString << std::endl;
//         return Value(Null{});
//     };

//     auto typeOf = [](Arguments args) {
//         return Value(visit([](auto&& arg) -> std::string {
//             using T = std::decay_t<decltype(arg)>;
//             if constexpr (std::is_same_v<T, Null>) return "null";
//             if constexpr (std::is_same_v<T, Int>) return "int";
//             if constexpr (std::is_same_v<T, Real>) {
//                 Real r = arg;
//                 if (std::isinf(r)) return "real";
//                 if (std::isnan(r)) return "real";
//                 return "real";
//             }
//             if constexpr (std::is_same_v<T, Bool>) return "bool";
//             if constexpr (std::is_same_v<T, Str>) return "string";
//             if constexpr (std::is_same_v<T, Array>) return "array";
//             if constexpr (std::is_same_v<T, Object>) return "object";
//             if constexpr (std::is_same_v<T, Function>) return "function";
//             if constexpr (std::is_same_v<T, NativeFn>) return "native";
//             if constexpr (std::is_same_v<T, Upvalue>) return "upvalue";
//             if constexpr (std::is_same_v<T, Module>) return "module";
//             if constexpr (std::is_same_v<T, Proto>) return "proto";
            
        
//             if constexpr (std::is_same_v<T, Class>) return "class";
//             if constexpr (std::is_same_v<T, Instance>) return "instance";
//             if constexpr (std::is_same_v<T, BoundMethod>) return "bound_method";
//             return "unknown";
//         }, args[0]));
//     };

//     auto toInt = [this](Arguments args) {
//         return Value(this->_toInt(args[0]));
//     };

//     auto toReal = [this](Arguments args) {
//         return Value(this->_toDouble(args[0]));
//     };

//     auto toBool = [this](Arguments args) {
//         return Value(this->_isTruthy(args[0]));
//     };

//     auto toStr = [this](Arguments args) {
//         return Value(this->_toString(args[0]));
//     };

//     auto nativeLen = [](Arguments args) {
//         const auto& value = args[0];
//         return visit(overloaded{
//             [](const Str& s) { return Value((Int)s.length()); },
//             [](const Array& a)  { return Value((Int)a->elements_.size()); },
//             [](const Object& o) { return Value((Int)o->fields.size()); },
//             [](const auto&) -> Value { 
//                 return Int(-1);
//             }
//         }, value);
//     };


//     auto nativeAssert = [this](Arguments args) {
//         if (!this->_isTruthy(args[0])) {
//             std::string message = "Assertion failed.";
//             if (args.size() > 1 && args[1].is<String>()) {
//                 message = args[1].get<String>()->utf8();
//             }
//             newVMError(message);
//         }
//         return Value(Null{});
//     };


//     auto nativeOrd = [this](Arguments args) {
//         const auto& str = args[0].get<Str>();
//         if (str.length() != 1) {
//             newVMError("Hàm ord() chỉ chấp nhận chuỗi có đúng 1 ký tự.");
//         }
//         return Value((Int)static_cast<unsigned char>(str[0]));
//     };


//     auto nativeChar = [this](Arguments args) {
//         Int code = args[0].get<Int>();
//         if (code < 0 || code > 255) {
//             newVMError("Mã ASCII của hàm chr() phải nằm trong khoảng [0, 255].");

//         }
//         return Value(Str(1, static_cast<char>(code)));
//     };

//     auto nativeRange = [this](Arguments args) {
//         Int start = 0;
//         Int stop = 0;
//         Int step = 1;
//         size_t argCount = args.size();

//         if (argCount == 1) {
//             stop = args[0].get<Int>();
//         } else if (argCount == 2) {
//             start = args[0].get<Int>();
//             stop = args[1].get<Int>();
//         } else {
//             start = args[0].get<Int>();
//             stop = args[1].get<Int>();
//             step = args[2].get<Int>();
//         }

//         if (step == 0) {
//             newVMError("Tham số 'step' của hàm range() không thể bằng 0.");
//         }

//         auto resultArrayData = this->memoryManager->newObject<ObjArray>();
        
//         if (step > 0) {
//             for (Int i = start; i < stop; i += step) {
//                 resultArrayData->elements_.push_back(Value(i));
//             }
//         } else {
//             for (Int i = start; i > stop; i += step) {
//                 resultArrayData->elements_.push_back(Value(i));
//             }
//         }

//         return Value(Array(resultArrayData));
//     };


//     Value printFunc = nativePrint;
//     std::unordered_map<Str, Value> natives;
//     natives["print"]  = Value(nativePrint);
//     natives["typeof"] = Value(typeOf);
//     natives["len"]    = Value(nativeLen);
//     natives["assert"] = Value(nativeAssert);
//     natives["int"]  = Value(toInt);
//     natives["real"] = Value(toReal);
//     natives["bool"] = Value(toBool);
//     natives["str"]  = Value(toStr);
//     natives["ord"]    = Value(nativeOrd);
//     natives["char"]   = Value(nativeChar);
//     natives["range"]  = Value(nativeRange);


//     auto nativeModule = memoryManager->newObject<ObjModule>("native", "native");
//     nativeModule->globals = natives;

//     moduleManager->addCache("native", nativeModule);

//     std::vector<Str> list = {"array", "object", "string"};

//     for (const auto& moduleName : list) {
//         try {
//             moduleManager->loadModule(moduleName, "native", true); 
//         } catch (const VMError& e) {
//             // std::cerr << "Warning: Could not preload standard module '" 
//                     //   << moduleName << "'. " << e.what() << std::endl;
//         }
//     }
// }