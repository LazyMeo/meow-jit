module meow.core:definition;

import :meow_object;
import :gc_visitor;

void ObjArray::trace(GCVisitor& visitor) noexcept {
    for (auto& element : elements_) {
        visitor.visit_value(element);
    }
}

void ObjHash::trace(GCVisitor& visitor) noexcept {
    for (auto& pair : fields_) {
        visitor.visit_value(pair.second);
    }
}