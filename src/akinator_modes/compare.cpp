#include "compare.h"

#define LOCAL_DTOR_()   stk_res |= stk_dtor(&objects[0].stk);   \
                        stk_res |= stk_dtor(&objects[1].stk);   \
                        FREE(objects[0].name);                  \
                        FREE(objects[1].name)

Status::Statuses akinator_compare(Tree* tree) {
    assert(tree);

    STATUS_CHECK(interface_compare_header());

    int stk_res = Stack::OK;

    DefObject objects[2] = {};

    stk_res |= STK_CTOR(&objects[0].stk);
    stk_res |= STK_CTOR(&objects[1].stk);

    STATUS_CHECK(akinator_definition_ask_and_search_object(tree, &objects[0]),
                 LOCAL_DTOR_());

    STATUS_CHECK(akinator_definition_ask_and_search_object(tree, &objects[1]),
                 LOCAL_DTOR_());

    STATUS_CHECK(akinator_compare_parse_objects(tree, objects),
                 LOCAL_DTOR_());

    LOCAL_DTOR_();
    if (stk_res != Stack::OK)
        return Status::STACK_ERROR;

    return Status::NORMAL_WORK;
}
#undef LOCAL_DTOR_

Status::Statuses akinator_compare_parse_objects(Tree* tree, const DefObject objects[2]) {
    assert(tree);
    assert(objects);
    assert(objects[0].name);
    assert(objects[1].name);

    size_t depth = 0;

    STATUS_CHECK(interface_compare_begin_similar(objects[0].name, objects[1].name));

    TreeNode* node = tree->root;

    size_t min_stk_size = MIN(objects[0].stk.size, objects[1].stk.size);

    for (depth = 0; depth < min_stk_size && objects[0].stk.data[depth] ==
                                            objects[1].stk.data[depth]; depth++) {

        bool is_last_iter = depth + 1 == min_stk_size || objects[0].stk.data[depth + 1] !=
                                                         objects[1].stk.data[depth + 1];

        STATUS_CHECK(interface_give_definition_characteristic(objects[0].stk.data[depth],
                            ((AkinatorString*)node->elem)->str, is_last_iter, true));

        node = objects[0].stk.data[depth] ? node->right : node->left;
    }

    if (depth == 0)
        STATUS_CHECK(interface_compare_totally_different());

    STATUS_CHECK(akinator_definition_parse_stk(tree, &objects[0], node, depth));

    STATUS_CHECK(akinator_definition_parse_stk(tree, &objects[1], node, depth));

    return Status::NORMAL_WORK;
}
