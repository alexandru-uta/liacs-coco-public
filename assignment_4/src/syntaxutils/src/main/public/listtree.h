/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_SYNTAXUTILS_LISTTREE
#define COCO_FRAMEWORK_SYNTAXUTILS_LISTTREE

#include "node.h"

struct ListTree {
    Node* root = nullptr;
    Node* current = nullptr;

    ListTree(Node* root, Node* current) : root(root), current(current){};
    ListTree() = default;
    ~ListTree() {
        root = nullptr;
        current = nullptr;
    };
};

#endif
