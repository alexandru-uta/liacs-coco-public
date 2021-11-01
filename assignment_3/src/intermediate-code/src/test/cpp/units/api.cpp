#include "../support/fixture.h"
#include "../support/testutil.h"
#include <string>
#include <node.h>

class APITest: public IntermediateCorrectTest {
public:
    APITest() : icode(logger), visitor(table, icode) {}
protected:
    // Utility alias for returning an IOperand, to ease memory management.
    using IOperandPtr = std::unique_ptr<IOperand>;

    IntermediateCode icode;
    ICTestVisitor visitor;
};

/**
 * API tests for intermediate-code
 */

TEST_F(APITest, id_int8) {
    const size_t id = 1;
    auto* node = new SymbolNode(NODE_ID, RT_INT8, id);
    EXPECT_TRUE(test::symbol::test_id_node(node, visitor));
    delete node;
}

TEST_F(APITest, id_int) {
    const size_t id = 1;
    auto* node = new SymbolNode(NODE_ID, RT_INT, id);
    EXPECT_TRUE(test::symbol::test_id_node(node, visitor));
    delete node;
}

TEST_F(APITest, id_uint8) {
    const size_t id = 1;
    auto* node = new SymbolNode(NODE_ID, RT_UINT8, id);
    EXPECT_TRUE(test::symbol::test_id_node(node, visitor));
    delete node;
}

TEST_F(APITest, id_uint) {
    const size_t id = 1;
    auto* node = new SymbolNode(NODE_ID, RT_UINT, id);
    EXPECT_TRUE(test::symbol::test_id_node(node, visitor));
    delete node;
}

TEST_F(APITest, num_int) {
    const int num = 987;
    auto* node = new ConstantNode<int>(NODE_NUM, RT_INT, num);
    IOperandPtr operand = visitor.accept_expr(node);
    ASSERT_NE(operand, nullptr);
    EXPECT_EQ(operand->getReturnType(), RT_INT);
    ASSERT_EQ(operand->getOperandType(), OT_IMM);
    EXPECT_EQ(dynamic_cast<ImmediateIOperand<int>*>(operand.get())->getValue(), num);
    delete node;
}

TEST_F(APITest, unknown_node) {
    auto* node = new Node(NODE_UNKNOWN, RT_ERROR);
    size_t stmt_count = icode.getStatementCount();
    visitor.accept(node);
    ASSERT_EQ(icode.getStatementCount(), stmt_count);
    delete node;
}

TEST_F(APITest, error_node) {
    auto* node = new Node(NODE_ERROR, RT_ERROR);
    size_t stmt_count = icode.getStatementCount();
    visitor.accept(node);
    ASSERT_EQ(icode.getStatementCount(), stmt_count);
    delete node;
}

TEST_F(APITest, wrong_accept_id) {
    const size_t id = 1;
    auto* node = new SymbolNode(NODE_ID, RT_INT, id);
    size_t stmt_count = icode.getStatementCount();
    visitor.accept(node);
    ASSERT_EQ(icode.getStatementCount(), stmt_count);
    delete node;
}

TEST_F(APITest, wrong_accept_expr_return) {
    const size_t id = 1;
    visitor.emulate_function_start(id);
    auto* node = new UnaryNode(NODE_RETURN, RT_INT);
    size_t stmt_count = icode.getStatementCount();
    visitor.accept_expr(node);
    ASSERT_EQ(icode.getStatementCount(), stmt_count);
    visitor.emulate_function_end();
    delete node;
}

TEST_F(APITest, int8_coerce_int) {
    size_t id = 1;
    visitor.emulate_function_start(id++);

    size_t stmt_count = icode.getStatementCount();
    auto* node = new UnaryNode(NODE_COERCION, RT_INT);
    size_t child_id = id++;
    node->setChild(new SymbolNode(NODE_ID, RT_INT8, child_id));

    IOperandPtr operand = visitor.accept_expr(node);
    ASSERT_NE(operand, nullptr);
    EXPECT_EQ(operand->getReturnType(), RT_INT);
    EXPECT_EQ(operand->getOperandType(), OT_SYMBOL);
    ASSERT_EQ(icode.getStatementCount(), ++stmt_count);
    IStatement* stmt = icode.getStatement(stmt_count-1);
    ASSERT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->getIType(), IOPT_DOUBLE);
    EXPECT_EQ(stmt->getOperator(), IOP_COERCE);
    size_t tmp_id = dynamic_cast<SymbolIOperand*>(operand.get())->getId();

    std::shared_ptr<IOperand> arg = stmt->getOperand1();
    ASSERT_NE(arg, nullptr);
    EXPECT_EQ(arg->getReturnType(), RT_INT8);
    ASSERT_EQ(arg->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(arg.get())->getId(), child_id);

    std::shared_ptr<IOperand> res = stmt->getResult();
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getReturnType(), RT_INT);
    ASSERT_EQ(res->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(res.get())->getId(), tmp_id);

    visitor.emulate_function_end();

    delete node;
}

TEST_F(APITest, add_int) {
    size_t id = 1;
    visitor.emulate_function_start(id++);

    size_t stmt_count = icode.getStatementCount();
    auto* node = new BinaryNode(NODE_ADD, RT_INT);
    size_t left_id = id++;
    size_t right_id = id++;
    node->setLeftChild(new SymbolNode(NODE_ID, RT_INT, left_id));
    node->setRightChild(new SymbolNode(NODE_ID, RT_INT, right_id));

    IOperandPtr operand = visitor.accept_expr(node);
    ASSERT_NE(operand, nullptr);
    EXPECT_EQ(operand->getReturnType(), RT_INT);
    EXPECT_EQ(operand->getOperandType(), OT_SYMBOL);
    ASSERT_EQ(icode.getStatementCount(), ++stmt_count);
    IStatement* stmt = icode.getStatement(stmt_count-1);
    ASSERT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->getIType(), IOPT_DOUBLE);
    EXPECT_EQ(stmt->getOperator(), IOP_ADD);
    size_t tmp_id = dynamic_cast<SymbolIOperand*>(operand.get())->getId();

    std::shared_ptr<IOperand> arg1 = stmt->getOperand1();
    ASSERT_NE(arg1, nullptr);
    EXPECT_EQ(arg1->getReturnType(), RT_INT);
    ASSERT_EQ(arg1->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(arg1.get())->getId(), left_id);

    std::shared_ptr<IOperand> arg2 = stmt->getOperand2();
    ASSERT_NE(arg2, nullptr);
    EXPECT_EQ(arg2->getReturnType(), RT_INT);
    ASSERT_EQ(arg2->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(arg2.get())->getId(), right_id);

    std::shared_ptr<IOperand> res = stmt->getResult();
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getReturnType(), RT_INT);
    ASSERT_EQ(res->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(res.get())->getId(), tmp_id);


    visitor.emulate_function_end();
    delete node;
}

TEST_F(APITest, functioncall_noargs_void) {
    auto* node = new BinaryNode(NODE_FUNCTIONCALL, RT_VOID);
    const size_t func_id = 1;
    node->setLeftChild(new SymbolNode(NODE_ID, RT_VOID, func_id));
    node->setRightChild(new Node(NODE_EMPTY, RT_VOID));

    size_t stmt_count = icode.getStatementCount();
    IOperandPtr operand = visitor.visit_func_call(node);
    EXPECT_EQ(operand, nullptr);

    ASSERT_EQ(icode.getStatementCount(), ++stmt_count);
    IStatement* stmt = icode.getStatement(stmt_count-1);
    ASSERT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->getIType(), IOPT_VOID);
    EXPECT_EQ(stmt->getOperator(), IOP_FUNCCALL);

    std::shared_ptr<IOperand> arg1 = stmt->getOperand1();
    ASSERT_NE(arg1, nullptr);
    EXPECT_EQ(arg1->getReturnType(), RT_VOID);
    ASSERT_EQ(arg1->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(arg1.get())->getId(), func_id);

    delete node;
}

TEST_F(APITest, rarray_int) {
    size_t id = 1;
    visitor.emulate_function_start(id++);

    auto* node = new BinaryNode(NODE_RARRAY, RT_INT);
    size_t array_id = id++;
    size_t index_id = id++;
    node->setLeftChild(new SymbolNode(NODE_ID, RT_INT_ARRAY, array_id));
    node->setRightChild(new SymbolNode(NODE_ID, RT_INT, index_id));

    size_t stmt_count = icode.getStatementCount();
    IOperandPtr operand = visitor.visit_rarray_access(node);
    ASSERT_NE(operand, nullptr);
    EXPECT_EQ(operand->getReturnType(), RT_INT);
    ASSERT_EQ(operand->getOperandType(), OT_SYMBOL);
    size_t tmp_id = dynamic_cast<SymbolIOperand*>(operand.get())->getId();

    ASSERT_EQ(icode.getStatementCount(), ++stmt_count);
    IStatement* stmt = icode.getStatement(stmt_count-1);
    ASSERT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->getIType(), IOPT_DOUBLE);
    EXPECT_EQ(stmt->getOperator(), IOP_RARRAY);

    std::shared_ptr<IOperand> arg1 = stmt->getOperand1();
    ASSERT_NE(arg1, nullptr);
    EXPECT_EQ(arg1->getReturnType(), RT_INT_ARRAY);
    ASSERT_EQ(arg1->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(arg1.get())->getId(), array_id);

    std::shared_ptr<IOperand> arg2 = stmt->getOperand2();
    ASSERT_NE(arg2, nullptr);
    EXPECT_EQ(arg2->getReturnType(), RT_INT);
    ASSERT_EQ(arg2->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(arg2.get())->getId(), index_id);

    std::shared_ptr<IOperand> res = stmt->getResult();
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getReturnType(), RT_INT);
    ASSERT_EQ(res->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(res.get())->getId(), tmp_id);

    visitor.emulate_function_end();
    delete node;
}

TEST_F(APITest, exprlist_empty) {
    Node* node = new Node(NODE_EMPTY, RT_VOID);
    size_t stmt_count = icode.getStatementCount();
    visitor.visit_expr_list(node);
    EXPECT_EQ(icode.getStatementCount(), stmt_count);
    delete node;
}

TEST_F(APITest, assignment_int) {
    const size_t id_left = 1;
    const size_t id_right = id_left+1;

    auto* left = new SymbolNode(NODE_ID, RT_INT, id_left);
    auto* right = new SymbolNode(NODE_ID, RT_INT, id_right);
    size_t stmt_count = icode.getStatementCount();
    visitor.visit_assignment(left, right);
    ASSERT_EQ(icode.getStatementCount(), stmt_count+1);

    IStatement* stmt = icode.getStatement(stmt_count);
    ASSERT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->getIType(), IOPT_DOUBLE);
    ASSERT_EQ(stmt->getOperator(), IOP_ASSIGN);

    std::shared_ptr<IOperand> arg1 = stmt->getOperand1();
    ASSERT_NE(arg1, nullptr);
    EXPECT_EQ(arg1->getReturnType(), RT_INT);
    ASSERT_EQ(arg1->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(arg1.get())->getId(), id_right);

    std::shared_ptr<IOperand> res = stmt->getResult();
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getReturnType(), RT_INT);
    ASSERT_EQ(res->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(res.get())->getId(), id_left);

    delete left;
    delete right;
}

TEST_F(APITest, return_empty) {
    auto* node = new Node(NODE_EMPTY, RT_VOID);

    size_t stmt_count = icode.getStatementCount();
    visitor.visit_return(node);
    ASSERT_EQ(icode.getStatementCount(), stmt_count+1);

    IStatement* stmt = icode.getStatement(stmt_count);
    ASSERT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->getIType(), IOPT_VOID);
    EXPECT_EQ(stmt->getOperator(), IOP_RETURN);
    EXPECT_EQ(stmt->getOperand1(), nullptr);

    delete node;
}

TEST_F(APITest, return_symbol_int) {
    const size_t id = 1;
    auto* node = new SymbolNode(NODE_ID, RT_INT, id);

    size_t stmt_count = icode.getStatementCount();
    visitor.visit_return(node);
    ASSERT_EQ(icode.getStatementCount(), stmt_count+1);

    IStatement* stmt = icode.getStatement(stmt_count);
    ASSERT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->getIType(), IOPT_VOID);
    EXPECT_EQ(stmt->getOperator(), IOP_RETURN);

    std::shared_ptr<IOperand> op1 = stmt->getOperand1();
    ASSERT_NE(op1, nullptr);
    EXPECT_EQ(op1->getReturnType(), RT_INT);
    ASSERT_EQ(op1->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(op1.get())->getId(), id);

    delete node;
}

TEST_F(APITest, function) {
    Node* root = new Node(NODE_EMPTY, RT_VOID);
    size_t func_id = table.addFunction(new Symbol("main", 1, RT_INT, ST_FUNCTION));

    size_t stmt_count = icode.getStatementCount();
    visitor.visit_function(func_id, root);
    ASSERT_EQ(icode.getStatementCount(), stmt_count+1);

    IStatement* stmt = icode.getStatement(stmt_count);
    ASSERT_NE(stmt, nullptr);
    EXPECT_EQ(stmt->getIType(), IOPT_VOID);
    EXPECT_EQ(stmt->getOperator(), IOP_FUNC);

    std::shared_ptr<IOperand> op1 = stmt->getOperand1();
    ASSERT_NE(op1, nullptr);
    EXPECT_EQ(op1->getReturnType(), RT_INT);
    ASSERT_EQ(op1->getOperandType(), OT_SYMBOL);
    EXPECT_EQ(dynamic_cast<SymbolIOperand*>(op1.get())->getId(), func_id);

    delete root;
}


