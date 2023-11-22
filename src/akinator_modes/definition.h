#ifndef AKINATOR_MODES_DEFINITION_H_
#define AKINATOR_MODES_DEFINITION_H_

#include <assert.h>

#include "../utils/statuses.h"
#include "../config.h"
#include "../interface.h"
#include "../Stack/stack.h"

#ifdef DEBUG

#include "TreeDebug/TreeDebug.h"

#else //< #ifndef DEBUG

#include "Tree/Tree.h"

#endif //< #ifdef DEBUG

struct DefObject {
    Stack stk = {};
    char* name = nullptr;
};

Status::Statuses akinator_definition(Tree* tree);

Status::Statuses akinator_definition_ask_and_search_object(Tree* tree, DefObject* obejct);

Status::Statuses akinator_definition_traversal(Tree* tree, const DefObject* obejct);

Status::Statuses akinator_definition_parse_stk(Tree* tree, const DefObject* object,
                                               TreeNode* start_node = nullptr, size_t depth = 0);

#endif //< #ifndef AKINATOR_MODES_DEFINITION_H_
