#include "definition.h"

static TreeNodeActionRes akinator_compare_preorder_(Tree* tree, TreeNode** node, va_list* args,
                                                       size_t depth, const TreeChildSide side);

static TreeNodeActionRes akinator_compare_postorder_(Tree* tree, TreeNode** node, va_list* args,
                                                        size_t depth, const TreeChildSide side);

#define LOCAL_DTOR_()   stk_res |= stk_dtor(&object.stk);  \
                        FREE(object.name)

Status::Statuses akinator_definition(Tree* tree) {
    assert(tree);

    STATUS_CHECK(interface_definition_header());

    DefObject object = {};

    int stk_res = STK_CTOR(&object.stk);

    STATUS_CHECK(akinator_definition_ask_and_search_object(tree, &object), LOCAL_DTOR_());

    STATUS_CHECK(akinator_definition_parse_stk(tree, &object), LOCAL_DTOR_());

    LOCAL_DTOR_();

    if (stk_res != Stack::OK)
        return Status::STACK_ERROR;

    return Status::NORMAL_WORK;
}
#undef LOCAL_DTOR_

Status::Statuses akinator_definition_ask_and_search_object(Tree* tree, DefObject* object) {
    assert(tree);
    assert(object);
    assert(object->name == nullptr);

    size_t attempts = MAX_ATTEMPTS_NUMBER;

    Status::Statuses res = Status::NORMAL_WORK;

    do {
        STATUS_CHECK(interface_ask_ask_object_for_definition(&object->name));

        res = akinator_definition_traversal(tree, object);

        if (res != Status::WRONG_CMD)
            break;

        FREE(object->name);
        if (--attempts) {
            STATUS_CHECK(interface_object_not_found());
            STATUS_CHECK(interface_try_again());
        }

    } while (attempts);

    if (attempts == 0) {
        STATUS_CHECK(interface_too_many_attempts(MAX_ATTEMPTS_NUMBER));
        return Status::TOO_MANY_ATTEMPTS;
    }

    return res;
}

Status::Statuses akinator_definition_traversal(Tree* tree, const DefObject* object) {
    assert(tree);
    assert(object);

    TreeNodeAction* actions[4] = {&akinator_compare_preorder_, nullptr,
                                  &akinator_compare_postorder_, nullptr};

    bool is_found = false;
    int tree_res = tree_traversal(tree, &tree->root, actions, object, &is_found);
    if (tree_res != Tree::OK)
        return Status::TREE_ERROR;

    assert(is_found || object->stk.size == 0);

    if (!is_found)
        return Status::WRONG_CMD;

    return Status::NORMAL_WORK;
}

Status::Statuses akinator_definition_parse_stk(Tree* tree, const DefObject* object,
                                               TreeNode* start_node, size_t depth) {
    assert(tree);
    assert(object);
    assert(object->name);

    STATUS_CHECK(interface_give_definition_begin(object->name));

    if (object->stk.size == (ssize_t)depth) {
        STATUS_CHECK(interface_give_definition_no_characteristics(object->name));

        return Status::NORMAL_WORK;
    }

    TreeNode* node = tree->root;
    if (start_node != nullptr)
        node = start_node;

    for (; (ssize_t)depth < object->stk.size; depth++) {
        bool is_last_iter = (ssize_t)depth + 1 == object->stk.size;

        STATUS_CHECK(interface_give_definition_characteristic(object->stk.data[depth],
                                                              ((AkinatorString*)(node->elem))->str,
                                                              is_last_iter));

        node = object->stk.data[depth] ? node->right : node->left;
    }

    return Status::NORMAL_WORK;
}

static TreeNodeActionRes akinator_compare_preorder_(Tree* tree, TreeNode** node, va_list* args,
                                                    size_t depth, const TreeChildSide side) {
    (void) depth;
    assert(tree);
    assert(node);
    assert(*node);
    assert(args);

    va_list args_dup = {};
    va_copy(args_dup, *args);

    DefObject* object = va_arg(args_dup, DefObject*);
    bool* is_found = va_arg(args_dup, bool*);

    assert(object);
    assert(object->name);
    assert(is_found);

    va_end(args_dup);

    int res = Stack::OK;

    if (side == TreeChildSide::LEFT)
        res |= stk_push(&object->stk, false);
    else if (side == TreeChildSide::RIGHT)
        res |= stk_push(&object->stk, true);

    if (res != Stack::OK)
        return TreeNodeActionRes::ERROR;

    if (!(*node)->left && !(*node)->right &&
        strcmp(object->name, ((AkinatorString*)((*node)->elem))->str) == 0) {

        *is_found = true;
        return TreeNodeActionRes::EXIT_FULL;
    }

    return TreeNodeActionRes::OK;
}

static TreeNodeActionRes akinator_compare_postorder_(Tree* tree, TreeNode** node, va_list* args,
                                                        size_t depth, const TreeChildSide side) {
    (void) depth;
    assert(tree);
    assert(node);
    assert(*node);
    assert(args);

    if (side == TreeChildSide::ROOT)
        return TreeNodeActionRes::OK;

    va_list args_dup = {};
    va_copy(args_dup, *args);

    DefObject* object = va_arg(args_dup, DefObject*);

    assert(object);

    va_end(args_dup);

    Elem_t buf = Stack::POISON;
    int res = stk_pop(&object->stk, &buf);

    if (res != Stack::OK)
        return TreeNodeActionRes::ERROR;

    return TreeNodeActionRes::OK;
}
