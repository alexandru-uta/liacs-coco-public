#ifndef COCO_FRAMEWORK_LEXICAL_ENTRYPOINT
#define COCO_FRAMEWORK_LEXICAL_ENTRYPOINT

#include "basevisitor.h"

namespace lexical {
    int generate(FILE* file, BaseVisitor& visitor);

    int generate(const std::string& filename, BaseVisitor& vis);
}

#endif
