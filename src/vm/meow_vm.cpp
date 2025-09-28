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