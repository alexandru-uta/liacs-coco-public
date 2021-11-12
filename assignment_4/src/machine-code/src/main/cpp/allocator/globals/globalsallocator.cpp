#include "globalsallocator.h"

bool GlobalsAllocator::contains(size_t sym) const {
    return globals.find(sym) != globals.end();
}

void GlobalsAllocator::insert(size_t sym, IOperatorType type) {
    globals.emplace(sym, type);
}

void GlobalsAllocator::load(size_t sym, const Register& dst) const {
    if (!contains(sym))
        throw std::runtime_error("[GlobalsAllocator] " + std::to_string(sym) + " not a global");

    out << "\tmov" << util::to_string(dst.type) << " v" << sym << "(%rip), " << dst << "\n";
}

void GlobalsAllocator::load_array(size_t sym, const Register &dst) const {
    if (!contains(sym))
        throw std::runtime_error("[GlobalsAllocator] " + std::to_string(sym) + " not a global");

    out << "\tlea" << util::to_string(dst.type) << " v" << sym << "(%rip), " << dst << "\n";
}

void GlobalsAllocator::store(const Register& src, size_t sym) const {
    if (!contains(sym))
        throw std::runtime_error("[GlobalsAllocator] " + std::to_string(sym) + " not a global");

    out << "\tmov" << util::to_string(src.type) << " " << src << ", v" << sym << "(%rip)\n";
}

void GlobalsAllocator::generate_data_segment() const {
    if (globals.empty())
        return;

    out << ".LCX:\n";
    for (auto global : globals) {
        Symbol* sym = tab.getSymbol(global.first);
        if (types::isArray(sym->getReturnType())) {
            auto* arr = dynamic_cast<ArraySymbol*>(sym);
            size_t element_size = util::get_n_bytes(util::to_iopt(types::arrayTypetoReturnType(arr->getReturnType())));
            out << "\t.comm v" << global.first << ", " << element_size*arr->getSize() << "\n";
        } else {
            out << "\t.comm v" << global.first << ", " << util::get_n_bytes(global.second) << "\n";
        }
    }
}




