#ifndef COCO_FRAMEWORK_INTERMEDIATECODE_FLOWGRAPH
#define COCO_FRAMEWORK_INTERMEDIATECODE_FLOWGRAPH

#include <cstdio>
#include <utility>
#include <vector>
#include <unordered_set>
#include <set>
#include "intermediatecode.h"

struct ICInfo {
    std::vector<size_t> leaders; // all leader statements
    std::unordered_map<size_t, size_t> labels; // label --> line number
    std::unordered_map<size_t, std::vector<size_t>> calls; // function id --> lines where it is called
    std::unordered_map<size_t, size_t> funcs; // line --> func_id

    /**
     * Adds a call at a certain line to a function
     * @param id the identifier of the function
     * @param line the line at which the call occured
     */
    void add_call(size_t id, size_t line) {
        auto calls_it = calls.find(id);
        if (calls_it == calls.end())
            calls[id] = std::vector<size_t>({line});
        else
            calls_it->second.push_back(line);
    }
};

struct BasicBlock {
    size_t start; // number of first instruction of a basic block
    size_t end; // number of last instruction of a basic block
    std::vector<std::weak_ptr<BasicBlock>> children;
    std::vector<std::weak_ptr<BasicBlock>> parents;

    virtual ~BasicBlock() = default;

    // Block range [start, end]
    BasicBlock(size_t start, size_t end) : start(start), end(end) {};
    inline virtual std::ostream& doStream(std::ostream& stream, const std::unordered_map<size_t, std::unordered_set<size_t>>& live_in, const std::unordered_map<size_t, std::unordered_set<size_t>>& live_out) {
        const size_t width1 = 8;
        const size_t width2 = 32;

        stream << "\n";
        stream << "Block [" << start << ", " << end << "]\n";
        for (size_t i = 0; i < width1+width2+width2; ++i)
            stream << "=";
        stream << "\n";
        stream << std::setw(width1) << "line" << std::setw(width2) << "| live in" << std::setw(width2) << "| live out";
        stream << "\n";
        for (size_t i = 0; i < width1+width2+width2; ++i)
            stream << "-";
        stream << "\n";
        for (size_t i = start; i <= end; ++i) {
            stream << std::setw(width1) << i;
            auto in = live_in.find(i);
            std::string in_string = "| ";
            if (in != live_in.end())
                for (size_t id : in->second)
                    in_string += std::to_string(id) + " ";
            stream << std::setw(width2) << in_string;
            std::string out_string = "| ";
            auto out = live_out.find(i);
            if (out != live_out.end())
                for (size_t id : out->second)
                    out_string += std::to_string(id) + " ";
            stream << std::setw(width2) << out_string;
            stream << "\n";
        }
        for (size_t i = 0; i < width1+width2+width2; ++i)
            stream << "-";
        stream << "\n";
        stream << "children: ";
        for (const auto& child : children)
            stream << child.lock()->start << " ";
        stream << "\n";
        stream << "parents: ";
        for (const auto& parent: parents)
            stream << parent.lock()->start << " ";
        stream << "\n";
        for (size_t i = 0; i < width1+width2+width2; ++i)
            stream << "=";
        stream << "\n";
        return stream;
    }

    inline friend bool operator<(const std::weak_ptr<BasicBlock>& b1, const std::weak_ptr<BasicBlock>& b2) {
        return b1.lock()->start < b2.lock()->start;
    }

};

class FlowGraph {
public:
    FlowGraph(const SymbolTable& table, const IntermediateCode& ic, Logger& logger);

    inline friend std::ostream& operator<<(std::ostream& stream, FlowGraph& graph) {
        FlowGraph::graph_walk(graph.entry, [&stream, &graph](const std::weak_ptr<BasicBlock> &block) {
            block.lock()->doStream(stream, graph.live_in, graph.live_out);
        });
        return stream;
    }

    /**
     * check if line is reachable
     * @param line the line number
     * @return whether the line is reachable
     */
    bool is_reachable(size_t line) const;

    /**
     * check if variable is live at line
     * @param line the line number
     * @param sym the identifier of the symbol
     * @return whether the given symbol is live at the given line
     */
    bool live_at(size_t line, size_t sym) const;

    /**
     * Get all variables which are live at given line
     * @param line the line number
     * @return an unordered_set of variable identifiers who are live at the given line
     */
    std::unordered_set<size_t> get_live_at(size_t line) const;

    /**
     * Get all variables which are live after the given line
     * @param line the line number
     * @return an unordered_set of variable identifiers who are live after the given line
     */
    std::unordered_set<size_t> get_live_out_at(size_t line) const;

private:
    std::weak_ptr<BasicBlock> entry; // BasicBlock to main or the first instruction
    std::unordered_map<size_t, std::shared_ptr<BasicBlock>> block_map; // line --> basic block
    Logger& logger;

    // line --> variable ids
    std::unordered_map<size_t, std::unordered_set<size_t>> live_out; // variables live after line
    std::unordered_map<size_t, std::unordered_set<size_t>> live_in; // variables live before line
    std::unordered_map<size_t, std::unordered_set<size_t>> gen; // variables generated by line
    std::unordered_map<size_t, std::unordered_set<size_t>> kill; // variables killed by line

    /**
     * Adds a variable to a line
     * @param i the line number
     * @param sym the identifier of the variable
     */
    void add_to_gen(size_t i, size_t sym);
    /**
     * Adds a set of variables to a line
     * @param i the line number
     * @param syms set with identifiers of variables
     */
    void add_to_gen(size_t i, std::unordered_set<size_t> syms);
    /**
     * Adds a variable to a line
     * @param i the line number
     * @param sym identifier of the variable
     */
    void add_to_kill(size_t i, size_t sym);

    /**
     * Adds a set of variables to a line
     * @param i the line number
     * @param syms set with identifiers of the variable
     */
    void add_to_kill(size_t i, std::unordered_set<size_t> syms);

    /**
     * Computes the liveness according to the entry, for all blocks
     * @param table the corresponding SymbolTable
     * @param ic the corresponding IntermediateCode
     */
    void compute_liveness(const SymbolTable& table, const IntermediateCode& ic, const ICInfo& info);

    /**
     * Computes the liveness for one block
     * @param block the BasicBlock to compute liveness for
     */
    void compute_liveness_block(const std::weak_ptr<BasicBlock>& block, const IntermediateCode& ic, const SymbolTable& table);

    /**
     * Performs a graph walk starting at root, for each node performing the provided action
     * @tparam UnaryPredicate Predicate accepting a single std::weak_ptr<BasicBlock>
     * @param root the entrypoint
     * @param action the action to perform
     */
    template<class UnaryPredicate>
    static void graph_walk(std::weak_ptr<BasicBlock> root, UnaryPredicate action) {
        // TODO: implement me
    }
};

#endif
