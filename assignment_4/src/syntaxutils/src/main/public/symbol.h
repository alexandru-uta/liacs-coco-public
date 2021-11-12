/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_SYNTAXUTILS_SYMBOL
#define COCO_FRAMEWORK_SYNTAXUTILS_SYMBOL

#include "types.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <to_string.h>

// The base class for Symbols
class Symbol {
    public:
    Symbol() = default;
    Symbol(std::string name, int line, ReturnType returnType, SymbolType symbolType) : name(std::move(name)), line(line), returnType(returnType), symbolType(symbolType) {}

    virtual ~Symbol() = default;

    const std::string& getName() const;
    void setName(const std::string& name);

    int getLine() const;
    void setLine(int line);

    ReturnType getReturnType() const;
    void setReturnType(ReturnType newReturnType);

    SymbolType getSymbolType() const;
    void setSymbolType(SymbolType newSymbolType);


    inline bool operator==(const Symbol& other) const {
        return equal_to(other);
    }

    /**
     * Validates symbol equality.
     * @param other Object to compare with.
     * @return `true` iff other is equivalent to this, `false` otherwise.
     */
    inline virtual bool equal_to(const Symbol& other) const {
        return equals(other);
    }

    /**
     * Validates symbol similarity.
     * @param other Object to compare with.
     * @return `true` iff other is similar to this, `false` otherwise.
     * @note We enforce equivalent names, since compiler implementations really should do that.
     */
    inline virtual bool similar_to(const Symbol& other) const {
        return similar(other);
    }

    inline friend std::ostream& operator<<(std::ostream& stream, const Symbol& symbol) {
        return symbol.doStream(stream);
    }

    inline virtual std::ostream& doStream(std::ostream& stream, int width_line, int width_name, int width_returntype) const {
        return stream << std::setw(width_line) << line << std::setw(width_name) << name << std::setw(width_returntype) << util::to_string(returnType) << util::to_string(symbolType);
    }

    inline virtual std::ostream& doStream(std::ostream& stream) const {
        return doStream(stream, 6, 16, 12);
    }

    protected:
    inline virtual bool equals(const Symbol& other) const {
        return name == other.name && line == other.line && returnType == other.returnType && symbolType == other.symbolType;
    }

    inline virtual bool similar(const Symbol& other) const {
        return name == other.name && line == other.line && returnType == other.returnType && symbolType == other.symbolType;
    }

    std::string name;
    int line = -1;
    ReturnType returnType = RT_UNKNOWN;
    SymbolType symbolType = ST_UNKNOWN;
};

class ArraySymbol : public Symbol {
    public:
    ArraySymbol() = default;
    ArraySymbol(std::string name, int line, ReturnType returnType, SymbolType symbolType, ssize_t arraySize) : Symbol(std::move(name), line, returnType, symbolType), size(arraySize) {}


    ssize_t getSize() const;
    void setSize(ssize_t new_value);

    inline std::ostream& doStream(std::ostream& stream) const override {
        return stream << std::setw(6) << line << std::setw(12) << util::to_string(returnType) << std::setw(12) << util::to_string(symbolType) << name << '[' << size << ']';
    }

    protected:
    inline bool equals(const Symbol& other) const override {
        if (const auto* arraySymbol = dynamic_cast<const ArraySymbol*>(&other))
            return Symbol::equals(other) && size == arraySymbol->size;
        return false;
    }

    inline bool similar(const Symbol& other) const override {
        if (const auto* arraySymbol = dynamic_cast<const ArraySymbol*>(&other))
            return Symbol::similar(other) && size == arraySymbol->size;
        return false;
    }

    ssize_t size = 0;
};

#endif
