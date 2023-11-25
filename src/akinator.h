#ifndef AKINATOR_H_
#define AKINATOR_H_

#include <assert.h>

#include "config.h"
#include "utils/statuses.h"
#include "interface.h"
#include "file/file.h"
#include "text_tree_parser/text_tree_parser.h"

#include "akinator_modes/guess.h"
#include "akinator_modes/definition.h"
#include "akinator_modes/compare.h"


#ifdef DEBUG

#include "TreeDebug/TreeDebug.h"

#else //< #ifndef DEBUG

#include "Tree/Tree.h"

#endif //< #ifdef DEBUG

inline bool akinator_str_dtor(void* elem) {
    assert(elem);

    if (((AkinatorString*)elem)->allocated) {
        FREE(((AkinatorString*)elem)->str);
    }

    ((AkinatorString*)elem)->str = nullptr;

    return true;
}

inline bool akinator_str_verify(void* elem) {
    assert(elem);

    if (((AkinatorString*)elem)->str == nullptr)
        return false;
    return true;
}

inline char* akinator_str_val(const void* elem) {
    assert(elem);

    // strdup, because tree will free pointer
    return strdup(((const AkinatorString*)elem)->str);
}

Status::Statuses akinator_main_loop(Tree* tree);

Status::Statuses akinator_save(Tree* tree, bool ask = true);

Status::Statuses akinator_process(const char* input_filename);

Status::Statuses akinator_init_tree(Tree* tree, char** input_data, const char* input_filename);

Status::Statuses akinator_menu(Tree* tree);

Status::Statuses akinator_enter_mode(Tree* tree, char input);

#ifdef DEBUG
Status::Statuses akinator_print_tree(Tree* tree);
#endif //< #ifdef DEBUG

#endif //< #ifndef AKINATOR_H_
