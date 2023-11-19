#ifndef TEXT_TREE_PARSER_H_
#define TEXT_TREE_PARSER_H_

#include <assert.h>
#include <ctype.h>

#include "../utils/statuses.h"
#include "../config.h"
#include "../interface.h"
#include "../file/file.h"

#ifdef DEBUG

#include "TreeDebug/TreeDebug.h"

#else //< #ifndef DEBUG

#include "Tree/Tree.h"

#endif //< #ifdef DEBUG

Status::Statuses text_tree_parser(Tree* tree, char* text);

Status::Statuses text_tree_save(Tree* tree, const char* filename);

#endif //< #ifndef TEXT_TREE_PARSER_H_
