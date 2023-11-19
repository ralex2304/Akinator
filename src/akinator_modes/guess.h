#ifndef AKINATOR_MODES_GUESS_H_
#define AKINATOR_MODES_GUESS_H_

#include <assert.h>

#include "../utils/statuses.h"
#include "../ui/ui.h"
#include "../interface.h"
#include "../config.h"

#ifdef DEBUG

#include "TreeDebug/TreeDebug.h"

#else //< #ifndef DEBUG

#include "Tree/Tree.h"

#endif //< #ifdef DEBUG

Status::Statuses akinator_guess(Tree* tree);

Status::Statuses akinator_guess_enter_node(Tree* tree, TreeNode** node);

Status::Statuses akinator_add_node(Tree* tree, TreeNode** node);


#endif //< #ifndef AKINATOR_MODES_GUESS_H_
