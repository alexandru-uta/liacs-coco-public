#include "flowgraph.h"
#include "utility.h"

#include <algorithm>
#include <utility>



struct BasicBlockBuilder {
    std::unordered_map<size_t, std::shared_ptr<BasicBlock>>& block_map; // line --> basic block
    const std::vector<size_t>& leaders; // lines of leaders
    const std::unordered_map<size_t, size_t>& labels; // label --> line number
    const std::unordered_map<size_t, std::vector<size_t>>& calls; // function id --> lines where it is called
    const std::unordered_map<size_t, size_t>& funcs; // line --> func_id
    const IntermediateCode& ic;

    BasicBlockBuilder(std::unordered_map<size_t, std::shared_ptr<BasicBlock>>& block_map, const ICInfo& info, const IntermediateCode& ic) : block_map(block_map), leaders(info.leaders), labels(info.labels), calls(info.calls), funcs(info.funcs), ic(ic) {};
    /**
     * Builds a BasicBlock and all its children
     * @param start the starting line of the BasicBlock
     * @return a std::weak_ptr to the constructed BasicBlock
     * Note: when block_map goes out of scope, the std::weak_ptr is expired
     */
    std::weak_ptr<BasicBlock> build(size_t start);
};

/**
 * Constructs an ICInfo given an IntermediateCode
 * Leaders are instructions which:
 * 1. is target of goto
 * 2. immediately follows goto
 * 3. is the first instruction
 * @param ic the IntermediateCode to find leaders for
 * @return the constructed ICInfo
 */
static ICInfo get_ic_info(const IntermediateCode& ic) {
    //TODO: implement me
}

/**
 * Finds the instruction which is the starting point of the 'main' function
 * @param leaders a std::vector<size_t> of leader-statements
 * @param table the corresponding SymbolTable
 * @param ic the corresponding IntermediateCode
 * @return the size_t line-number of the starting point of main, or std::numeric_limits<size_t>::max() if main is not defined
 */
static size_t find_main(const std::vector<size_t>& leaders, const SymbolTable& table, const IntermediateCode& ic) {
    for (size_t leader : leaders) {
        IStatement* stmt = ic.getStatement(leader);
        if (stmt->getOperator() == IOP_FUNC)
            if (table.getSymbol(std::dynamic_pointer_cast<SymbolIOperand>(stmt->getOperand1())->getId())->getName() == "main")
                return leader;
    }
    return std::numeric_limits<size_t>::max();
}

void FlowGraph::add_to_gen(size_t i, size_t sym) {
    auto gen_it = gen.find(i);
    if (gen_it != gen.end())
        gen_it->second.emplace(sym);
    else
        gen[i] = std::unordered_set<size_t>({sym});
}

void FlowGraph::add_to_gen(size_t i, std::unordered_set<size_t> sym) {
    auto gen_it = gen.find(i);
    if (gen_it != gen.end())
        gen_it->second.insert(sym.begin(), sym.end());
    else
        gen[i] = sym;
}

void FlowGraph::add_to_kill(size_t i, size_t sym) {
    auto kill_it = kill.find(i);
    if (kill_it != kill.end())
        kill_it->second.emplace(sym);
    else
        kill[i] = std::unordered_set<size_t>({sym});
}

void FlowGraph::add_to_kill(size_t i, std::unordered_set<size_t> syms) {
    auto kill_it = kill.find(i);
    if (kill_it != kill.end())
        kill_it->second.insert(syms.begin(), syms.end());
    else
        kill[i] = syms;
}

void FlowGraph::compute_liveness_block(const std::weak_ptr<BasicBlock>& block, const IntermediateCode& ic, const SymbolTable& table) {
    //TODO: implement me
}


void FlowGraph::compute_liveness(const SymbolTable& table, const IntermediateCode& ic, const ICInfo& info) {
    //TODO: implement me
}

std::weak_ptr<BasicBlock> BasicBlockBuilder::build(size_t start) {
    //TODO: implement me
    return std::weak_ptr<BasicBlock>();
}

FlowGraph::FlowGraph(const SymbolTable& table, const IntermediateCode& ic, Logger& logger) : logger(logger) {
    ICInfo info = get_ic_info(ic);
    entry = BasicBlockBuilder(block_map, info, ic).build(find_main(info.leaders, table, ic));

    compute_liveness(table, ic, info);
}

bool FlowGraph::is_reachable(size_t line) const {
    //TODO: implement me
}


bool FlowGraph::live_at(size_t line, size_t sym) const {
    auto line_it = live_out.find(line);
    if (line_it == live_out.end())
        return false;
    if (line_it->second.find(sym) != line_it->second.end())
        return true;
    line_it = live_in.find(line);
    if (line_it == live_in.end())
        return false;
    return (line_it->second.find(sym) != line_it->second.end());
}

std::unordered_set<size_t> FlowGraph::get_live_at(size_t line) const {
    auto line_it = live_out.find(line);
    std::unordered_set<size_t> live;
    if (line_it != live_out.end())
        live = line_it->second;
    line_it = live_in.find(line);
    if (line_it != live_in.end())
        live.insert(line_it->second.begin(), line_it->second.end());
    return live;
}

std::unordered_set<size_t> FlowGraph::get_live_out_at(size_t line) const {
    auto line_out = live_out.find(line);
    if (line_out != live_out.end())
        return line_out->second;
    return std::unordered_set<size_t>();
}






