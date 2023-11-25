#include "text_tree_parser.h"

static Status::Statuses text_tree_parser_new_node_(Tree* tree, TreeNode** node, TreeNode* parent,
                                                   char* text, long* const i);

static Status::Statuses text_tree_parser_new_node_children_(Tree* tree, TreeNode** node, char* text, long* i,
                                                            TreeChildSide side);

static Status::Statuses text_tree_parser_insert_node_(Tree* tree, TreeNode** node, TreeNode* parent,
                                                      char* node_text_begin);

static TreeNodeActionRes text_tree_write_prefix_action_(Tree* tree, TreeNode** node, va_list* args,
                                                        size_t depth, const TreeChildSide side);

static TreeNodeActionRes text_tree_write_postfix_action_(Tree* tree, TreeNode** node, va_list* args,
                                                         size_t depth, const TreeChildSide side);

static TreeNodeActionRes text_tree_write_nil_action_(Tree* tree, TreeNode** node, va_list* args,
                                                     size_t depth, const TreeChildSide side);

#define PRINT_ERR_(...) printf("Tree file err: " __VA_ARGS__)

#define SKIP_TO_CHAR_(c_, i_)   do {                                                \
                                    while (text[i_] && isspace(text[i_])) (i_)++;   \
                                                                                    \
                                    if (text[i_] != c_) {                           \
                                        PRINT_ERR_("'%c' expected\n", c_);          \
                                        return Status::INPUT_ERROR;                 \
                                    }                                               \
                                } while (0)

#define SKIP_SPACE_(i_) do {                                                \
                            while (text[i_] && isspace(text[i_])) (i_)++;   \
                                                                            \
                            if (text[i_] == ' ') {                          \
                                PRINT_ERR_("File ended unexpectedly\n");    \
                                return Status::INPUT_ERROR;                 \
                            }                                               \
                        } while (0)

Status::Statuses text_tree_parser(Tree* tree, char* text) {
    assert(tree);
    assert(text);

    long i = 0;

    while (text[i] && isspace(text[i])) i++;

    if (text[i] == '\0') return Status::NORMAL_WORK; //< Empty file is empty tree
    if (text[i++] != '(') {
        PRINT_ERR_("file must begin with '('\n");
        return Status::INPUT_ERROR;
    }

    STATUS_CHECK(text_tree_parser_new_node_(tree, &tree->root, nullptr, text, &i));

    SKIP_TO_CHAR_(')', i);

    return Status::NORMAL_WORK;
}



static Status::Statuses text_tree_parser_new_node_(Tree* tree, TreeNode** node, TreeNode* parent,
                                                   char* text, long* const i) {
    assert(tree);
    assert(node);
    assert(text);
    assert(i);

    SKIP_TO_CHAR_('"', *i);
    (*i)++;

    char* node_text_begin = text + *i;

    while (text[*i] && text[*i] != '"') (*i)++;
    text[(*i)++] = '\0';

    STATUS_CHECK(text_tree_parser_insert_node_(tree, node, parent, node_text_begin));
    assert(*node);

    STATUS_CHECK(text_tree_parser_new_node_children_(tree, node, text, i, TreeChildSide::LEFT));
    (*i)++;

    SKIP_SPACE_(*i);

    STATUS_CHECK(text_tree_parser_new_node_children_(tree, node, text, i, TreeChildSide::RIGHT));
    (*i)++;

    return Status::NORMAL_WORK;
}

static Status::Statuses text_tree_parser_new_node_children_(Tree* tree, TreeNode** node, char* text, long* i,
                                                            TreeChildSide side) {
    assert(tree);
    assert(node);
    assert(*node);
    assert(text);
    assert(i);
    assert(side == TreeChildSide::LEFT || side == TreeChildSide::RIGHT);

    SKIP_SPACE_(*i);

    if (text[*i] == '(') {
        (*i)++;
        if (side == TreeChildSide::LEFT)
            STATUS_CHECK(text_tree_parser_new_node_(tree, &(*node)->left, *node, text, i));
        else if (side == TreeChildSide::RIGHT)
            STATUS_CHECK(text_tree_parser_new_node_(tree, &(*node)->right, *node, text, i));
        else
            assert(0 && "Wrong TreeChildSide given");

        SKIP_TO_CHAR_(')', *i);
    } else if (strncmp("nil", text + *i, 3) == 0) {
        (*i) += 3 - 1;
    } else {
        PRINT_ERR_("Unexpected symbol '%c'. Expected '(' or 'nil'\n", text[*i]);
        return Status::INPUT_ERROR;
    }

    return Status::NORMAL_WORK;
}

static Status::Statuses text_tree_parser_insert_node_(Tree* tree, TreeNode** node, TreeNode* parent,
                                                      char* node_text_begin) {
    assert(tree);
    assert(node);
    assert(*node == nullptr);
    assert(parent || tree->root == nullptr);
    assert(node_text_begin);

    AkinatorString new_node_text = {.allocated = false,
                                    .str = node_text_begin};

    if (tree_insert(tree, node, parent, &new_node_text) != Tree::OK) {
        PRINT_ERR_("Tree error\n");
        return Status::TREE_ERROR;
    }

    return Status::NORMAL_WORK;
}

#undef SKIP_SPACE_
#undef SKIP_TO_CHAR_

Status::Statuses text_tree_save(Tree* tree, const char* filename) {
    assert(tree);
    assert(filename);

    FILE* file = {};
    if (!file_open(&file, filename, "wb"))
        return Status::FILE_ERROR;

    TreeNodeAction* actions[4] = {&text_tree_write_prefix_action_, nullptr,
                                  &text_tree_write_postfix_action_, &text_tree_write_nil_action_};

    if (tree_traversal(tree, &tree->root, actions, file) != Tree::OK)
        return Status::TREE_ERROR;

    if (!file_close(file))
        return Status::FILE_ERROR;

    return Status::NORMAL_WORK;
}
#undef PRINT_ERR_

static TreeNodeActionRes text_tree_write_prefix_action_(Tree* tree, TreeNode** node, va_list* args,
                                                        size_t depth, const TreeChildSide side) {
    (void) tree;
    (void) depth;
    (void) side;
    assert(node);
    assert(*node);
    assert(args);

    va_list args_dup = {};
    va_copy(args_dup, *args);

    FILE* file = va_arg(args_dup, FILE*);

    va_end(args_dup);

    if (fprintf(file, "( \"%s\" ", ((AkinatorString*)((*node)->elem))->str) <= 0)
        return TreeNodeActionRes::ERR;

    return TreeNodeActionRes::OK;
}

static TreeNodeActionRes text_tree_write_nil_action_(Tree* tree, TreeNode** node, va_list* args,
                                                     size_t depth, const TreeChildSide side) {
    (void) tree;
    (void) depth;
    (void) side;
    assert(node);
    assert(*node == nullptr);
    assert(args);

    va_list args_dup = {};
    va_copy(args_dup, *args);

    FILE* file = va_arg(args_dup, FILE*);

    va_end(args_dup);

    if (fprintf(file, "nil ") <= 0)
        return TreeNodeActionRes::ERR;

    return TreeNodeActionRes::OK;
}

static TreeNodeActionRes text_tree_write_postfix_action_(Tree* tree, TreeNode** node, va_list* args,
                                                         size_t depth, const TreeChildSide side) {
    (void) tree;
    (void) depth;
    (void) side;
    assert(node);
    assert(*node);
    assert(args);

    va_list args_dup = {};
    va_copy(args_dup, *args);

    FILE* file = va_arg(args_dup, FILE*);

    va_end(args_dup);

    if (fprintf(file, ") ") <= 0)
        return TreeNodeActionRes::ERR;

    return TreeNodeActionRes::OK;
}

