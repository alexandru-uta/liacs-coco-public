#ifndef COCO_FRAMEWORK_INTERMEDIATECODE_ISTATEMENT
#define COCO_FRAMEWORK_INTERMEDIATECODE_ISTATEMENT

#include "ioperand.h"
#include "ioperator.h"
#include "ioperatortype.h"
#include <hacks/template_hacks.h>
#include <symboltable.h>
#include <iomanip>
#include <memory>
#include <utility>

// A statement in the intermediate code (quadruple)
class IStatement {
    public:
    IStatement(IOperatorType type, IOperator op, std::shared_ptr<IOperand>  operand1, std::shared_ptr<IOperand>  operand2, std::shared_ptr<IOperand>  result) : itype(type), ioperator(op), operand1(std::move(operand1)), operand2(std::move(operand2)), result(std::move(result)){}
    explicit IStatement(IOperator op) : itype(IOPT_WORD), ioperator(op), operand1(nullptr), operand2(nullptr), result(nullptr){}
    IStatement() : itype(IOPT_WORD), ioperator(IOP_UNKNOWN), operand1(nullptr), operand2(nullptr), result(nullptr){}

    inline virtual ~IStatement() = default;

    inline IOperatorType getIType() const {
        return itype;
    }

    inline IOperator getOperator() const {
        return ioperator;
    }
    inline void setOperator(IOperator op) {
        ioperator = op;
    }

    inline std::shared_ptr<IOperand> getOperand1() const {
        return operand1;
    };
    inline void setOperand1(const std::shared_ptr<IOperand>& operand) {
        operand1 = operand;
    };

    inline std::shared_ptr<IOperand> getOperand2() const {
        return operand2;
    }
    inline void setOperand2(const std::shared_ptr<IOperand>& operand) {
        operand2 = operand;
    }

    inline std::shared_ptr<IOperand> getResult() {
        return result;
    }
    inline void setResult(const std::shared_ptr<IOperand>& operand) {
        result = operand;
    }

    inline friend std::ostream& operator<<(std::ostream& stream, const IStatement& statement) {
        return statement.doStream(stream, 20);
    }

    inline virtual std::ostream& doStream(std::ostream& stream, unsigned width) const {
        return doStream(stream, nullptr, width);
    }

    inline virtual std::ostream& doStream(std::ostream& stream, const SymbolTable* const table, unsigned width) const {
        stream << std::setw(width) << hack::format("%s (%s)", util::to_string(ioperator).c_str(), util::to_string(itype).c_str()) << " | ";
        stream << std::setw(width);
        if (operand1)
           operand1->doStream(stream, table);
        else
            stream  << ' ';

        stream << " | " << std::setw(width);
        if (operand2)
            operand2->doStream(stream, table);
        else
            stream << ' ';

        stream << " | " << std::setw(width);
        if (result)
            result->doStream(stream, table);
        else
            stream << ' ';
        stream << '\n';

        if (ioperator == IOP_RETURN)
            stream << '\n';
        return stream;
    }

    private:
    IOperatorType itype;
    IOperator ioperator;
    std::shared_ptr<IOperand> operand1;
    std::shared_ptr<IOperand> operand2;
    std::shared_ptr<IOperand> result;
};

#endif
