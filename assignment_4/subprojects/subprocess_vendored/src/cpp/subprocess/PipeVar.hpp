#pragma once

#include <string>
#include <mpark/variant.h>
#include <iostream>
#include <cstdio>

#include "basic_types.hpp"


namespace subprocess {
    enum class PipeVarIndex {
        option,
        string,
        handle,
        istream,
        ostream,
        file
    };

    typedef mpark::variant<PipeOption, std::string, PipeHandle,
        std::istream*, std::ostream*, FILE*> PipeVar;


    inline PipeOption get_pipe_option(const PipeVar& option) {
        PipeVarIndex index = static_cast<PipeVarIndex>(option.index());

        switch(index) {
        case PipeVarIndex::option:  return mpark::get<PipeOption>(option);
        case PipeVarIndex::handle:  return PipeOption::specific;

        default:                    return PipeOption::pipe;
        }
    }
}