#pragma once

#include <algorithm>
#include <iostream>

#include "Config.hpp"

namespace VCDP_NAMESPACE {

/// @brief See https://gtkwave.github.io/gtkwave/internals/vcd-recoding.html
struct VList {
    VList* next;
    uint32_t size;
    int offset;
    uint32_t elem_size;

    uint8_t* getDataAddr() { return reinterpret_cast<uint8_t*>(this + 1); }
};

class VListManager {
   public:
    VListManager() : head_(nullptr) {}

    ~VListManager() {
        while (head_ != nullptr) {
            VList* temp = head_;
            head_ = head_->next;
            free(temp);
        }
    }

    static VList* allocate(const uint32_t size) {
        const size_t total_size = sizeof(VList) + size;  // Store the VList header + 'size' bytes
        const auto new_list = static_cast<VList*>(malloc(total_size));

        if (!new_list) {
            throw std::runtime_error("Error: malloc failed in VList::allocate");
        }

        new_list->next = nullptr;
        new_list->size = size;
        new_list->offset = 0;
        new_list->elem_size = 1;

        return new_list;
    };

    void addData(uint32_t data) {
        // Recover only useful bits (varint baby!!!)
        uint8_t buffer[5];    // Varint for uint32_t is 5 bytes (because MSB show if existing next byte)
        uint8_t* p = buffer;  // Typically buffer[0]

        while (data >= 0x80) {     // 0x80 -> 0b1000_0000
            *p++ = (data & 0x7F);  // 0x7F -> 0b0111_1111
            data >>= 7;            // Next byte
        }
        *p++ = data | 0x80;  // Mark last byte (because we want to store the last byte also!)

        const size_t size = p - buffer;  // The real size occupied

        // Iterate in each byte of the data
        for (size_t i = 0; i < size; i++) {
            if (head_ == nullptr || head_->offset >= head_->size) {  // If no remaining space, create a new bloc
                const auto new_size = (head_ ? head_->size * 2 : 1);
                const auto new_list = allocate(new_size);
                new_list->next = head_;
                head_ = new_list;
            }

            uint8_t* dest = head_->getDataAddr();  // Start of the data container of the bloc
            dest[head_->offset++] = buffer[i];
        }
    }

    [[nodiscard]] uint32_t getData(const size_t global_index) const {
        VList* current = head_;
        size_t current_base = 0;

        while (current != nullptr) {
            const size_t last_element_index = std::min(static_cast<size_t>(current->offset), static_cast<size_t>(current->size));

            if (global_index < current_base + last_element_index) {
                const size_t local_index = global_index - current_base;
                const uint8_t* data = current->getDataAddr();
                return data[local_index];
            }

            current_base += last_element_index;
            current = current->next;
        }

        throw std::out_of_range("Index out of range");
    }

   private:
    VList* head_;
};

}  // namespace VCDP_NAMESPACE
