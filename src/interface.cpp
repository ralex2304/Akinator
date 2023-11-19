#include "interface.h"

#define PRINT_(...) if (printf(__VA_ARGS__) <= 0) return Status::OUTPUT_ERROR;

Status::Statuses interface_greet() {
    PRINT_("# Akinator greets you!\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_goodbye() {
    PRINT_("# Goodbye\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_try_again() {
    PRINT_("# Incorrect input. Try again\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_now_i_know() {
    PRINT_("# Actually, I had already known the answer, I was just testing you\n\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_begin_menu() {
    PRINT_("# Choose mode:\n"
           "# 'g' - guess\n"
           "# 'd' - give definition\n"
           "# 'c' - compare\n"
           "# 'q' - quit\n"
           "# 's' - save\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_too_many_attempts(const size_t max_attempts_num) {
    PRINT_("# Oh, come on! You had %zu tries. I refuse to work with you. Chao\n",
                            max_attempts_num);

    return Status::NORMAL_WORK;
}

Status::Statuses interface_ask_question_about_node(const char* text, bool* answer) {
    assert(text);
    assert(answer);

    PRINT_("# Is it %s? (y/n)\n", text);

    STATUS_CHECK(interface_ask_y_n(answer));

    return Status::NORMAL_WORK;
}


Status::Statuses interface_ask_output_filename(char** output_filename) {
    assert(output_filename);
    assert(*output_filename == nullptr);

    PRINT_("Enter output file name:\n");
    *output_filename = ui_get_string();
    if (*output_filename == nullptr) {
        PRINT_("Unexpected input stream error\n");
        return Status::INPUT_ERROR;
    }

    return Status::NORMAL_WORK;
}

Status::Statuses interface_ask_question_about_saving(bool* answer) {
    assert(answer);

    PRINT_("# Do you want to save my knowledge? (y/n)\n");

    STATUS_CHECK(interface_ask_y_n(answer));

    return Status::NORMAL_WORK;
}

Status::Statuses interface_ask_y_n(bool* answer) {
    assert(answer);

    size_t attempts = MAX_ATTEMPTS_NUMBER;

    do {
        char c = ui_get_char();

        if (c == 'y' || c == 'n') {
            *answer = (c == 'y');
            return Status::NORMAL_WORK;
        }
        STATUS_CHECK(interface_try_again());

    } while (--attempts);

    return Status::WRONG_CMD;
}

Status::Statuses interface_ask_new_node(AkinatorString* new_node_text, Tree* tree) {
    assert(new_node_text);

    PRINT_("# And what it is?\n");

    return interface_get_string(new_node_text, tree);
}

Status::Statuses interface_ask_diff(const AkinatorString* entity1, const AkinatorString* entity2,
                                    AkinatorString* diff_text, Tree* tree) {
    assert(entity1);
    assert(entity1->str);
    assert(entity2);
    assert(entity2->str);
    assert(diff_text);

    PRINT_("# What's the differnce between \"%s\" and \"%s\"?\n", entity1->str, entity2->str);

    return interface_get_string(diff_text, tree);
}

Status::Statuses interface_get_string(AkinatorString* dest, Tree* tree) {
    assert(dest);
    assert(tree);

    size_t attempts = MAX_ATTEMPTS_NUMBER;

    do {
        dest->str = ui_get_string();
        if (dest->str == nullptr)
            return Status::INPUT_ERROR;

        dest->allocated = true;

        if (interface_verify_node_text(dest, tree))
            return Status::NORMAL_WORK;

        STATUS_CHECK(interface_try_again());

    } while (--attempts);

    return Status::WRONG_CMD;
}

Status::Statuses interface_correct_answer() {
    PRINT_("# You see? I know everything\n\n");

    return Status::NORMAL_WORK;
}

static TreeNodeActionRes search_string_in_tree_(Tree* tree, TreeNode** node, va_list* args,
                                                size_t depth, const TreeChildSide side) {
    (void) depth;
    (void) side;

    assert(tree);
    assert(node);
    assert(args);

    va_list args_dup = {};
    va_copy(args_dup, *args);

    bool* is_found = va_arg(args_dup, bool*);
    char* str = va_arg(args_dup, char*);

    assert(str);
    if (*is_found) return TreeNodeActionRes::EXIT;

    if (strcmp(str, ((AkinatorString*)(*node)->elem)->str) == 0) {
        *is_found = true;
        va_end(args_dup);
        return TreeNodeActionRes::EXIT;
    }

    va_end(args_dup);

    return TreeNodeActionRes::OK;
}

bool interface_verify_node_text(AkinatorString* text, Tree* tree) {
    assert(text);
    assert(tree);

    if (text->str[0] == '\0') {
        printf("# Text mustn't be empty\n");
        return false;
    }

    char* not_ptr = strcasestr(text->str, "not");

    if (not_ptr != nullptr && (text->str - not_ptr == 0 || isspace(not_ptr[-1])) &&
                                  (strlen(not_ptr) == 3 || isspace(not_ptr[3]))) {
        printf("# Text mustn't contain \"not\"\n");
        return false;
    }

    bool is_found = false;
    if (tree_preorder(tree, &tree->root, &search_string_in_tree_, &is_found, text->str) != Tree::OK)
        return false;

    if (is_found) {
        printf("# I already know this object or question\n");
        return false;
    }

    return true;
}
#undef PRINT_
