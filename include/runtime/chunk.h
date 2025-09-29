#pragma once

#include "common/pch.h"
#include "core/value.h"

class BytecodeParser;
class ObjFunctionProto;
class Chunk;

class Chunk {
    friend class BytecodeParser; 
    friend class ObjFunctionProto;
    // friend std::string disassemble_chunk(const Chunk& chunk) noexcept;
public:
    Chunk() = default;

    inline void write_byte(uint8_t byte, size_t line) noexcept {
        code_.push_back(byte);
    }
    
    inline void write_short(uint16_t value, size_t line) noexcept {
        if (value < 128) {
            code_.push_back(static_cast<uint8_t>(value));
        } else {
            code_.push_back(static_cast<uint8_t>((value & 0x7F) | 0x80));
            code_.push_back(static_cast<uint8_t>(value >> 7));
        }
    }

    inline void write_address(uint16_t address, size_t line) noexcept {
        code_.push_back(static_cast<uint8_t>(address & 0xFF));
        code_.push_back(static_cast<uint8_t>((address >> 8) & 0xFF));
    }

    inline size_t add_constant(Value value) noexcept {
        constant_pool_.push_back(value);
        return constant_pool_.size() - 1;
    }
    inline size_t get_code_size() const noexcept { 
        return code_.size(); 
    }
    inline bool is_code_empty() const noexcept    {
        return code_.empty();
    }
    inline size_t get_pool_size() const noexcept {
        return constant_pool_.size();
    }
    inline bool is_pool_empty() const noexcept {
        return constant_pool_.empty();
    }
    inline Value get_constant(size_t index) const { 
        return constant_pool_[index]; 
    }
private:
    std::vector<uint8_t> code_;
    std::vector<Value> constant_pool_;
};