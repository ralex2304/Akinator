#ifndef MENU_H_
#define MENU_H_

#include <assert.h>

#include "utils/statuses.h"
#include "ui/ui.h"
#include "config.h"


#ifdef DEBUG

#include "TreeDebug/TreeDebug.h"
#include "log/log.h"

#else //< #ifndef DEBUG

#include "Tree/Tree.h"

#endif //< #ifdef DEBUG

#ifdef _WIN32
#include "voiceover/voiceover.h"
#endif  //<#ifdef _WIN32

const size_t MAX_ATTEMPTS_NUMBER = 5; //< Max input attempts number

struct AkinatorString {
    bool allocated = false;
    char* str = nullptr;
};

#ifdef DEBUG
Status::Statuses interface_print_tree(Tree* tree);
#endif //< #ifdef DEBUG

char interface_get_mode();

Status::Statuses interface_guess_header();

Status::Statuses interface_definition_header();

Status::Statuses interface_compare_header();

Status::Statuses interface_save_header();

Status::Statuses interface_clear_console();

Status::Statuses interface_press_any_key_and_clear();

Status::Statuses interface_give_definition_characteristic(bool is_true, const char* characteristic,
                                                          bool is_last);

Status::Statuses interface_give_definition_begin(const char* object);

Status::Statuses interface_compare_begin_similar(const char* object1, const char* object2);

Status::Statuses interface_compare_totally_different();

Status::Statuses interface_ask_ask_object_for_definition(char** object);

Status::Statuses interface_ask_question_about_node(const char* text, bool* answer);

Status::Statuses interface_ask_question_about_saving(bool* answer);

Status::Statuses interface_ask_output_filename(char** output_filename);

Status::Statuses interface_ask_y_n(bool* answer);

Status::Statuses interface_greet();

Status::Statuses interface_goodbye();

Status::Statuses interface_begin_menu();

Status::Statuses interface_now_i_know();

Status::Statuses interface_object_not_found();

Status::Statuses interface_give_definition_no_characteristics(const char* object);

Status::Statuses interface_ask_new_node(AkinatorString* new_node_text, Tree* tree);

Status::Statuses interface_ask_diff(const AkinatorString* entity1, const AkinatorString* entity2,
                                    AkinatorString* diff_text, Tree* tree);

Status::Statuses interface_too_many_attempts(const size_t MAX_ATTEMPTS_NUMBER);

Status::Statuses interface_get_string(AkinatorString* dest, Tree* tree);

Status::Statuses interface_correct_answer();

Status::Statuses interface_try_again();

bool interface_verify_node_text(AkinatorString* text, Tree* tree);

#endif //< #ifndef MENU_H_
