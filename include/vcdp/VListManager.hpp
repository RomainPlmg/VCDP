#pragma once

#include <stdlib.h>

#include <algorithm>
#include <cstdint>
#include <iterator>

#include "Config.hpp"

namespace VCDP_NAMESPACE {

/// @brief See https://gtkwave.github.io/gtkwave/internals/vcd-recoding.html
struct VList {
    VList* next;
    uint32_t size;
    int offset;
    uint32_t elem_size;

    uint8_t* getData() { return reinterpret_cast<uint8_t*>(this) + 1; }
};

class VListManager {
   public:
    VListManager() : head_(nullptr) {}

    ~VListManager() {
        while (head_) {
            VList* temp = head_;
            head_ = head_->next;
            free(temp);
        }
    }

    static VList* allocate(const uint32_t size) {
        const size_t total_size = sizeof(VList) + size;  // Store the VList header + 'size' bytes
        const auto new_list = static_cast<VList*>(malloc(total_size));

        new_list->next = nullptr;
        new_list->size = size;
        new_list->offset = 0;
        new_list->elem_size = 1;

        return new_list;
    };

    void addElement(const uint8_t value) {
        if (head_ == nullptr) {  // First allocation if no data
            head_ = allocate(1);
        } else if (head_->offset == head_->size) {  // Need to allocate a new bloc
            const auto new_list = allocate(head_->size * 2);
            new_list->next = head_;
            head_ = new_list;
        }

        // Create a new value in the current bloc
        uint8_t* data = head_->getData();
        data[head_->offset] = value;
        head_->offset++;
    }

    uint8_t getElement(const size_t global_index) const {
        VList* current = head_;
        size_t current_base = 0;

        while (current != nullptr) {
            const size_t last_element_index = std::min(static_cast<size_t>(current->offset), static_cast<size_t>(current->size));

            if (global_index < current_base + last_element_index) {
                const size_t local_index = global_index - current_base;
                const uint8_t* data = current->getData();
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
