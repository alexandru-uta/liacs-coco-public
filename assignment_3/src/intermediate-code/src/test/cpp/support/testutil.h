/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_INTERMEDIATE_TESTUTIL
#define COCO_FRAMEWORK_INTERMEDIATE_TESTUTIL

#include "gtest/gtest.h"
#include "ictestvisitor.h"
#include <sstream>

namespace test {
	namespace symbol {
		inline testing::AssertionResult test_id_node(SymbolNode* node, ICTestVisitor& visitor) {
			std::unique_ptr<IOperand> operand = visitor.accept_expr(node);
			if (operand == nullptr)
				return testing::AssertionFailure() << "No Operand returned";
			std::stringstream error; 
			if (operand->getReturnType() != node->getReturnType())
				error << "Operand has incorrect ReturnType, expected: " << node->getReturnType() << ", got: " << operand->getReturnType() << ".";
			if (operand->getOperandType() != OT_SYMBOL)
				return testing::AssertionFailure() << error.str() << "Operand is not an OT_SYMBOL";
			if (dynamic_cast<SymbolIOperand*>(operand.get())->getId() != node->getSymbolId())
				error << "Operand has incorrect ID, expected: " << node->getSymbolId();
			if (error.str().empty())
				return testing::AssertionSuccess();
			return testing::AssertionFailure() << error.str();
		}
	}
}

#endif