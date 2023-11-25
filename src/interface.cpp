#include "interface.h"

#ifdef DEBUG
extern LogFileData log_file;
#endif //< #ifdef DEBUG

#ifdef _WIN32

#define PRINT_(...) if (printf(__VA_ARGS__) <= 0) return Status::OUTPUT_ERROR;  \
                    voiceover(false, true, __VA_ARGS__)

#define VOICE_SPEAK(async) voiceover(true, async, nullptr)

#else //< #ifndef _WIN32

#define PRINT_(...) if (printf(__VA_ARGS__) <= 0) return Status::OUTPUT_ERROR

#define VOICE_SPEAK(async) (void) 0

#endif //< #ifdef _WIN32

char interface_get_mode() {
    VOICE_SPEAK(true);

    return ui_get_char_no_enter();
}

Status::Statuses interface_guess_header() {
    PRINT_("������, ��� ������\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_definition_header() {
    PRINT_("��� �����������. ����� ������\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_compare_header() {
    PRINT_("������ �������. ������ ��\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_save_header() {
    PRINT_("����������\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_clear_console() {
    if (ui_clear_console() != 0)
        return Status::INPUT_ERROR;
    return Status::NORMAL_WORK;
}

Status::Statuses interface_press_any_key_and_clear() {
    PRINT_("����� ����� ������� ��� �����������\n");

    VOICE_SPEAK(true);

    if (ui_get_char_no_enter() == '\0')
        return Status::INPUT_ERROR;

    STATUS_CHECK(interface_clear_console());

    return Status::NORMAL_WORK;
}

Status::Statuses interface_greet() {
    PRINT_("�������!\n");

    VOICE_SPEAK(false);

    return Status::NORMAL_WORK;
}

Status::Statuses interface_goodbye() {
    PRINT_("���\n");

    VOICE_SPEAK(false);

    return Status::NORMAL_WORK;
}

Status::Statuses interface_give_definition_no_characteristics(const char* object) {
    assert(object);

    PRINT_("��� %s. ��� ��� �� �� ���� ������? � ���� ��� ������ �������!\n", object);

    return Status::NORMAL_WORK;
}

Status::Statuses interface_object_not_found() {
    PRINT_("�� ���� �����\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_try_again() {
    PRINT_("�� �� �� �� �� ���. �������� ��� ���\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_now_i_know() {
    PRINT_("������-�� � � ��� ��� ���� �����, ������ ���� ��������\n\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_begin_menu() {
    STATUS_CHECK(interface_clear_console());

    PRINT_("������ �����:\n"
           "'�' - ������ ������\n"
           "'�' - ��� �����������\n"
           "'�' - ������\n"
#ifdef DEBUG
           "'�' - ������ ����\n"
#endif //< #ifdef DEBUG
           "'�' - �����\n"
           "'�' - ���������\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_too_many_attempts(const size_t max_attempts_num) {
    PRINT_("�� �����! � ���� ���� %zu �������. � �� ���� � ����� �������������. ���\n",
                            max_attempts_num);

    VOICE_SPEAK(false);

    return Status::NORMAL_WORK;
}

Status::Statuses interface_ask_question_about_node(const char* text, bool* answer) {
    assert(text);
    assert(answer);

    PRINT_("��� %s? (�/�)\n", text);

    STATUS_CHECK(interface_ask_y_n(answer));

    return Status::NORMAL_WORK;
}

Status::Statuses interface_ask_output_filename(char** output_filename) {
    assert(output_filename);
    assert(*output_filename == nullptr);

    PRINT_("����� ��� �����:\n");

    VOICE_SPEAK(true);

    *output_filename = ui_get_string();
    if (*output_filename == nullptr) {
        PRINT_("�����-�� ����������� ����� � input stream. ������\n");
        VOICE_SPEAK(false);
        return Status::INPUT_ERROR;
    }

    return Status::NORMAL_WORK;
}

Status::Statuses interface_ask_question_about_saving(bool* answer) {
    assert(answer);

    PRINT_("������ ��������� ��� ������? (�/�)\n");

    STATUS_CHECK(interface_ask_y_n(answer));

    return Status::NORMAL_WORK;
}

Status::Statuses interface_ask_y_n(bool* answer) {
    assert(answer);

    size_t attempts = MAX_ATTEMPTS_NUMBER;

    do {
        VOICE_SPEAK(true);

        char c = ui_get_char_no_enter();

        if (c == '�' || c == '�') {
            *answer = (c == '�');
            return Status::NORMAL_WORK;
        }
        STATUS_CHECK(interface_try_again());

    } while (--attempts);

    return Status::WRONG_CMD;
}

Status::Statuses interface_ask_new_node(AkinatorString* new_node_text, Tree* tree) {
    assert(new_node_text);

    PRINT_("� ��� �� ���?\n");

    return interface_get_string(new_node_text, tree);
}

Status::Statuses interface_ask_ask_object_for_definition(char** object) {
    assert(object);
    assert(*object == nullptr);

    PRINT_("����� ��� �������:\n");

    VOICE_SPEAK(true);

    *object = ui_get_string();
    if (*object == nullptr)
        return Status::INPUT_ERROR;

    return Status::NORMAL_WORK;
}

Status::Statuses interface_give_definition_begin(const char* object) {
    assert(object);

    PRINT_("%s ", object);

    return Status::NORMAL_WORK;
}

Status::Statuses interface_compare_begin_similar(const char* object1, const char* object2) {
    assert(object1);
    assert(object2);

    PRINT_("%s � %s ", object1, object2);

    return Status::NORMAL_WORK;
}

Status::Statuses interface_compare_totally_different() {
    PRINT_("������ �� ������.\n");

    return Status::NORMAL_WORK;
}

Status::Statuses interface_give_definition_characteristic(bool is_true, const char* characteristic,
                                                          bool is_last) {
    assert(characteristic);

    if (!is_true) {
        PRINT_("�� ");
    }

    PRINT_("%s", characteristic);

    if (!is_last) {
        PRINT_(", ");
    } else {
        PRINT_(".\n");
    }

    return Status::NORMAL_WORK;
}

Status::Statuses interface_ask_diff(const AkinatorString* entity1, const AkinatorString* entity2,
                                    AkinatorString* diff_text, Tree* tree) {
    assert(entity1);
    assert(entity1->str);
    assert(entity2);
    assert(entity2->str);
    assert(diff_text);

    PRINT_("��� \"%s\" ���������� �� \"%s\"? ��...\n", entity1->str, entity2->str);

    return interface_get_string(diff_text, tree);
}

Status::Statuses interface_get_string(AkinatorString* dest, Tree* tree) {
    assert(dest);
    assert(tree);

    size_t attempts = MAX_ATTEMPTS_NUMBER;

    do {
        VOICE_SPEAK(true);

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
    PRINT_("������? � �� ����!\n\n");

    return Status::NORMAL_WORK;
}


#ifdef DEBUG

Status::Statuses interface_print_tree(Tree* tree) {
    assert(tree);

    PRINT_("��������. ������ ����� ����\n");

    char img_filename[log_file.MAX_FILENAME_LEN] = {};

    log_printf(&log_file, "Tree print mode was called\n");

    if (!tree_dump_dot(tree, img_filename))
        return Status::TREE_ERROR;

    char command[log_file.MAX_FILENAME_LEN] = {};

    snprintf(command, log_file.MAX_FILENAME_LEN, "start %s", img_filename);

#ifdef _WIN32
    if (system(command) != 0) {
        PRINT_("��������� ������\n");
        VOICE_SPEAK(0);
        return Status::OUTPUT_ERROR;
    }
#endif //< #ifdef _WIN32

    return Status::NORMAL_WORK;
}
#endif //< #ifdef DEBUG

#undef PRINT_
#undef VOICE_SPEAK

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
    assert(*is_found == false);

    if (strcmp(str, ((AkinatorString*)(*node)->elem)->str) == 0) {
        *is_found = true;
        va_end(args_dup);
        return TreeNodeActionRes::EXIT_FULL;
    }

    va_end(args_dup);

    return TreeNodeActionRes::OK;
}

#ifdef _WIN32

#define PRINT_(...) if (printf(__VA_ARGS__) <= 0) return false;  \
                    voiceover(false, true, __VA_ARGS__)

#else //< #ifndef _WIN32

#define PRINT_(...) if (printf(__VA_ARGS__) <= 0) return false

#define VOICE_SPEAK() (void) 0

#endif //< #ifdef _WIN32

bool interface_verify_node_text(AkinatorString* text, Tree* tree) {
    assert(text);
    assert(tree);

    if (text->str[0] == '\0') {
        PRINT_("����� �������� ������ ������?\n");
        return false;
    }

#ifdef unix
    char* not_ptr = strcasestr(text->str, "��");
#else //< #ifndef unix
    char* not_ptr = strstr(text->str, "��");
#endif //< #ifdef unix

    if (not_ptr != nullptr && (text->str - not_ptr == 0 || isspace(not_ptr[-1])) &&
                                  (strlen(not_ptr) == 2 || isspace(not_ptr[2]))) {
        PRINT_("���� �� ���� �����, �� ��������� \"��\"\n");
        return false;
    }

    bool is_found = false;
    if (tree_preorder(tree, &tree->root, &search_string_in_tree_, &is_found, text->str) != Tree::OK)
        return false;

    if (is_found) {
        PRINT_("�� �� ���� ������ �� �����. � ��� ����� ����\n");
        return false;
    }

    return true;
}
#undef PRINT_
