#ifndef COCO_FRAMEWORK_MACHINECODE_GLOBALSALLOCATOR
#define COCO_FRAMEWORK_MACHINECODE_GLOBALSALLOCATOR

#include "../../register/register.h"
#include <cstddef>
#include <ostream>
#include <symbol.h>
#include <unordered_map>
#include <unordered_set>

// Class to allocate global variables to memory and generate a data section for them.
class GlobalsAllocator {
    // The output stream to write instructions to.
    std::ostream& out;

    // A store containing offsets for a symbol.
    std::unordered_map<size_t, IOperatorType> globals;

    SymbolTable& tab;

    // The total size of the data section needed.
    size_t total_size;

    public:
    // Create a GlobalsAllocator.
    GlobalsAllocator(std::ostream& out, SymbolTable& tab) : out(out), tab(tab), total_size(0) {}

    // Check if this allocator contains information about `sym`.
    bool contains(size_t sym) const;

    // Insert a symbol into this allocator and allocate a place for it.
    void insert(size_t sym, IOperatorType type);

    // Load the value of the global to the register
    void load(size_t sym, const Register& dst) const;

    // Load the address of a global array to the register
    void load_array(size_t sym, const Register& dst) const;

    // store the value of the register to the global
    void store(const Register& src, size_t sym) const;

    // Generate the data segment of the globals.
    void generate_data_segment() const;
};

#endif
