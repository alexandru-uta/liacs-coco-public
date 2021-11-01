#ifndef COCO_FRAMEWORK_INTERMEDIATECODE_IOPERAND
#define COCO_FRAMEWORK_INTERMEDIATECODE_IOPERAND

#include <string>
#include <sstream>
#include <limits>
#include <symbol.h>
#include <hacks/template_hacks.h>
#include <symboltable.h>

/*
 * This file provides three different objects:
 *   - IOperand
 *   - ImmediateIOperand<T>
 *   - SymbolIOperand
 * You can instantiate these objects with e.g:
 *   IOperand* myOperand = new ImmediateIOperand<int>(42);
 */

// The type of an operand
typedef enum ioperandtype {
    OT_UNKNOWN, // Unknown (yet)
    OT_IMM,     // Integer immediate
    OT_SYMBOL   // Symbol object
} IOperandType;

// Operand of an IStatement
class IOperand {
    public:
    IOperand() = default;
    explicit IOperand(IOperandType type, ReturnType rt) : optype(type), rt(rt) {}
    IOperand(const IOperand& iop) = default;

    virtual ~IOperand() = default;

    inline IOperandType getOperandType() const {
        return optype;
    }

    inline void setOperandType(IOperandType type) {
        optype = type;
    }

    inline ReturnType getReturnType() const {
        return rt;
    }

    inline void setReturnType(ReturnType type) {
        rt = type;
    }

    inline friend std::ostream& operator<<(std::ostream& stream, const IOperand& operand) {
        return operand.doStream(stream);
    }

    inline virtual std::ostream& doStream(std::ostream& stream) const = 0;
    inline virtual std::ostream& doStream(std::ostream& stream, const SymbolTable* const table) const = 0;

protected:
    IOperandType optype = OT_UNKNOWN;
    ReturnType rt = RT_UNKNOWN;
};

template<typename T>
class ImmediateIOperand : public IOperand {
    public:
    explicit ImmediateIOperand(ReturnType rt) : IOperand(OT_IMM, rt) {};
    ImmediateIOperand(T newvalue, ReturnType rt) : IOperand(OT_IMM, rt), value(newvalue){};
    ImmediateIOperand(const ImmediateIOperand<T>& iop) = default;

    ~ImmediateIOperand() override = default;

    inline T getValue() {
        return value;
    };
    inline void setValue(T newvalue) {
        value = newvalue;
    };

    inline virtual std::ostream& doStream(std::ostream& stream) const override {
        return doStream(stream, nullptr);
    }

    inline virtual std::ostream& doStream(std::ostream& stream, const SymbolTable* const) const override {
        const auto namedType = hack::get_name<T>();
        std::stringstream stringstream;
        stringstream << std::to_string(value) << " (" << namedType << ')';
        return stream << stringstream.str();
    }

protected:
    T value;
};

class SymbolIOperand : public IOperand {
    public:
    explicit SymbolIOperand(ReturnType rt) : IOperand(OT_SYMBOL, rt) {}
    SymbolIOperand(size_t newsym, ReturnType rt) : IOperand(OT_SYMBOL, rt), sym_id(newsym){};
    SymbolIOperand(const SymbolIOperand& iop) = default;

    ~SymbolIOperand() override = default;

    inline size_t getId() const {
        return sym_id;
    }
    inline void setId(size_t id) {
        sym_id = id;
    }

    inline virtual std::ostream& doStream(std::ostream& stream) const override {
        return doStream(stream, nullptr);
    }

    inline virtual std::ostream& doStream(std::ostream& stream, const SymbolTable* const table) const override {
        if (table)
            return stream << hack::format("%s (id: %zu)", table->getSymbol(sym_id)->getName().c_str(), sym_id);
        return stream << hack::format("(id: %zu)", sym_id);
    }

    protected:
    size_t sym_id = std::numeric_limits<size_t>::max();
};

#endif