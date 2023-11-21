#ifndef AKINATOR_MODES_COMPARE_H_
#define AKINATOR_MODES_COMPARE_H_

#include <assert.h>

#include "../utils/statuses.h"
#include "../config.h"
#include "../interface.h"
#include "../Stack/stack.h"
#include "definition.h"

#ifdef DEBUG

#include "TreeDebug/TreeDebug.h"

#else //< #ifndef DEBUG

#include "Tree/Tree.h"

#endif //< #ifdef DEBUG

Status::Statuses akinator_compare(Tree* tree);

Status::Statuses akinator_compare_parse_objects(Tree* tree, const DefObject objects[2]);

#endif //< #ifndef AKINATOR_MODES_COMPARE_H_
