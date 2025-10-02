#pragma once

struct GCVisitor;
struct MeowObject {
    virtual ~MeowObject() = default;

    /// @brief Tracing all objects referenced
    virtual void trace(GCVisitor& visitor) const noexcept = 0;
};