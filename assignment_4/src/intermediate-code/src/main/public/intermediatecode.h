#ifndef COCO_FRAMEWORK_INTERMEDIATECODE_INTERMEDIATECODE
#define COCO_FRAMEWORK_INTERMEDIATECODE_INTERMEDIATECODE


#include "istatement.h"
#include <symboltable.h>

#include <logger.h>

#include <string>
#include <vector>
#include <iomanip>

// This class stores a linear list of intermediate statements and provides
// operations to modify that list.
class IntermediateCode {
    public:
    explicit IntermediateCode(Logger& logger) : logger(logger) {}

    IntermediateCode(IntermediateCode&& other) noexcept : logger(other.logger), programName(std::move(other.programName)), statements(std::move(other.statements)) {}
    IntermediateCode(IntermediateCode& other) = delete;

    virtual ~IntermediateCode();

    // gets the program name
    std::string getProgramName();

    // sets the program name
    void setProgramName(std::string name);

    // Returns the number of statements
    unsigned getStatementCount() const;

    // gets the i-th statement
    IStatement* getStatement(unsigned i) const;

    // Appends a statement
    void appendStatement(IStatement* stmt);

    // Inserts a statement before the i-th statement
    void insertStatement(IStatement* stmt, unsigned i);

    // Removes the i-th statement
    void removeStatement(unsigned i);

    inline friend std::ostream& operator<<(std::ostream& stream, const IntermediateCode& code) {
        return code.doStream(stream);
    }

    inline virtual std::ostream& doStream(std::ostream& stream) const {
        return doStream(stream, nullptr);
    }
    inline virtual std::ostream& doStream(std::ostream& stream, const SymbolTable* const table) const {
        stream << "===== Intermediate code dump =====\n";

        const auto idxSize = std::to_string(statements.size()).length();
        const auto colSize = 20;

        stream << std::setw(idxSize) << '#' << " | " << std::setw(colSize) << "operator" << " | " << std::setw(colSize) << "operand1" << " | " << std::setw(colSize) << "operand2" << " | " << std::setw(colSize) << "result" << '\n';
        stream << std::setfill('-') << std::setw(idxSize) << "" << "-+-" << std::setw(colSize) << "" << "-+-" << std::setw(colSize) << "" << "-+-" << std::setw(colSize) << "" << "-+-" << std::setw(colSize) << "" << '\n';
        stream << std::setfill(' '); // Need to return default fill for next stream input.
        for (size_t x = 0; x < statements.size(); ++x) {
            const auto* statement = statements[x];
            stream << std::setw(idxSize) << x << " | ";
            statement->doStream(stream, table, colSize);
        }
        stream << std::setfill('-') << std::setw(idxSize) << "" << "-+-" << std::setw(colSize) << "" << "-+-" << std::setw(colSize) << "" << "-+-" << std::setw(colSize) << "" << "-+-" << std::setw(colSize) << "" << '\n';
        stream << std::setfill(' '); // Need to return default fill for the rest of the stream.
        stream << "===== End of intermediate code dump =====\n";

        return stream;
    }

    private:
    Logger& logger;
    std::string programName;
    std::vector<IStatement*> statements;
};

#endif
