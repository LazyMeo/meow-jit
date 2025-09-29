#include "memory/mark_sweep_gc.h"
#include "core/value.h"
#include "runtime/execution_context.h"

MarkSweepGC::~MarkSweepGC() {
    std::cout << "[destroy] Đang xử lí các object khi hủy GC" << std::endl;
    for (auto const& [obj, data] : metadata_) {
        delete obj;
    }
}

void MarkSweepGC::register_object(MeowObject* object) noexcept {
    std::cout << "[register] Đang đăng kí object: " << object << std::endl;
    metadata_.emplace(object, GCMetadata{});
}

// size_t MarkSweepGC::collect() noexcept {
//     std::cout << "[collect] Đang collect các object" << std::endl;

//     context_->trace_roots(*this);

//     for (auto it = metadata_.begin(); it != metadata_.end();) {
//         MeowObject* object = it->first;
//         GCMetadata& data = it->second;

//         if (data.is_marked_) {
//             data.is_marked_ = false;
//             ++it;
//         } else {
//             delete object;
//             it = metadata_.erase(it);
//         }
//     }

//     return metadata_.size();
// }

void MarkSweepGC::visit_value(Value& value) noexcept {
    if (value.is_object()) {
        mark(value.as_object());
    }
}

void MarkSweepGC::visit_object(MeowObject* object) noexcept {
    mark(object);
}

void MarkSweepGC::mark(MeowObject* object) {
    if (object == nullptr) {
        return;
    }

    auto it = metadata_.find(object);
    if (it == metadata_.end()) {
        return;
    }

    if (it->second.is_marked_) {
        return;
    }

    it->second.is_marked_ = true;

    object->trace(*this);
}