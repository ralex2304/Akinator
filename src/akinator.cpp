#include "akinator.h"

#define LOCAL_DTOR_()   if (tree_dtor(&tree) != Tree::OK)   \
                            return Status::TREE_ERROR;      \
                        FREE(text)

Status::Statuses akinator_process(const char* input_filename) {
    assert(input_filename);

    STATUS_CHECK(interface_greet());

    Tree tree = {};
    if (TREE_CTOR(&tree, sizeof(AkinatorString), &akinator_str_dtor,
                  &akinator_str_verify, &akinator_str_val) != Tree::OK)
        return Status::TREE_ERROR;

    char* text = nullptr;
    STATUS_CHECK(akinator_init_tree(&tree, &text, input_filename), LOCAL_DTOR_());

    STATUS_CHECK(akinator_main_loop(&tree), LOCAL_DTOR_());

    STATUS_CHECK(akinator_save(&tree), LOCAL_DTOR_());

    STATUS_CHECK(interface_goodbye(), LOCAL_DTOR_());

    LOCAL_DTOR_();

    return Status::NORMAL_WORK;
}
#undef LOCAL_DTOR

Status::Statuses akinator_main_loop(Tree* tree) {
    assert(tree);

    while (1) {
        Status::Statuses res = akinator_menu(tree);

        if (res == Status::OK_EXIT)
            break;
        else if (res != Status::NORMAL_WORK)
            return res;
    }
    return Status::NORMAL_WORK;
}

Status::Statuses akinator_init_tree(Tree* tree, char** input_data, const char* input_filename) {
    assert(tree);
    assert(input_data);
    assert(*input_data == nullptr);
    assert(input_filename);

    STATUS_CHECK(file_open_read_close(input_filename, input_data));
    assert(input_data);

    STATUS_CHECK(text_tree_parser(tree, *input_data));

    static char DEFAULT_TREE_ROOT_TEXT[] = "unknown entity";
    static AkinatorString DEFAULT_TREE_ROOT = {.allocated = false, .str = DEFAULT_TREE_ROOT_TEXT};

    if (tree->size == 0) {
        if (tree_insert_root(tree, &DEFAULT_TREE_ROOT) != Tree::OK)
            return Status::TREE_ERROR;
    }

    return Status::NORMAL_WORK;
}

Status::Statuses akinator_menu(Tree* tree) {
    STATUS_CHECK(interface_begin_menu());

    size_t attempts = MAX_ATTEMPTS_NUMBER;

    Status::Statuses res = Status::WRONG_CMD;

    do {
        res = akinator_enter_mode(tree, ui_get_char_no_enter());

        if (res != Status::WRONG_CMD)
            break;

        if (--attempts)
            STATUS_CHECK(interface_try_again());

    } while (attempts);

    if (res == Status::WRONG_CMD) {
        STATUS_CHECK(interface_too_many_attempts(MAX_ATTEMPTS_NUMBER));
        return Status::WRONG_CMD;
    }

    if (res != Status::OK_EXIT)
        STATUS_CHECK(interface_press_any_key_and_clear());

    return res;
}

#define MENU_CASE_(c_, action_) case c_:                                        \
                                    STATUS_CHECK(interface_clear_console());    \
                                    res = action_;                              \
                                    break

Status::Statuses akinator_enter_mode(Tree* tree, char input) { // TODO console no echo
    Status::Statuses res = Status::DEFAULT;
    switch (input) {

        MENU_CASE_('g', akinator_guess(tree));
        MENU_CASE_('d', akinator_definition(tree));
        MENU_CASE_('c', akinator_compare(tree));
        MENU_CASE_('s', akinator_save(tree, false));

        case 'q':
            STATUS_CHECK(interface_clear_console());
            res = Status::OK_EXIT;
            break;
        default:
            return Status::WRONG_CMD;
    }
    return res;
}
#undef MENU_CASE_

Status::Statuses akinator_save(Tree* tree, bool ask) {
    assert(tree);

    STATUS_CHECK(interface_save_header());

    bool answer = false;
    if (ask)
        STATUS_CHECK(interface_ask_question_about_saving(&answer));

    if (answer || !ask) {
        char* filename = nullptr;
        STATUS_CHECK(interface_ask_output_filename(&filename), FREE(filename));
        assert(filename);

        STATUS_CHECK(text_tree_save(tree, filename), FREE(filename));

        FREE(filename);
    }

    return Status::NORMAL_WORK;
}
