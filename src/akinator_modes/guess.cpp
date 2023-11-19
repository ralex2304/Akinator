#include "guess.h"

Status::Statuses akinator_guess(Tree* tree) {
    assert(tree);

    STATUS_CHECK(akinator_guess_enter_node(tree, &tree->root));

    return Status::NORMAL_WORK;
}

Status::Statuses akinator_guess_enter_node(Tree* tree, TreeNode** node) {
    assert(tree);
    assert(node);
    assert(*node);

    bool answer = false;
    STATUS_CHECK(interface_ask_question_about_node(((AkinatorString*)((*node)->elem))->str, &answer));

    if ((*node)->left == nullptr || (*node)->right == nullptr) {
        assert((*node)->left == nullptr && (*node)->right == nullptr);

        if (answer) {
            STATUS_CHECK(interface_correct_answer());
        } else {
            STATUS_CHECK(akinator_add_node(tree, node));
        }

        return Status::NORMAL_WORK;
    }

    if (answer) {
        STATUS_CHECK(akinator_guess_enter_node(tree, &(*node)->right));
    } else {
        STATUS_CHECK(akinator_guess_enter_node(tree, &(*node)->left));
    }

    return Status::NORMAL_WORK;
}

Status::Statuses akinator_add_node(Tree* tree, TreeNode** node) {
    assert(tree);
    assert(node);
    assert(*node);

    AkinatorString new_node_text = {};
    STATUS_CHECK(interface_ask_new_node(&new_node_text, tree));

    int res = tree_insert(tree, &(*node)->left, *node, (*node)->elem);
    if (res != Tree::OK)
        return Status::TREE_ERROR;

    res |= tree_insert(tree, &(*node)->right, *node, &new_node_text);
    if (res != Tree::OK)
        return Status::TREE_ERROR;

    new_node_text = {};
    STATUS_CHECK(interface_ask_diff((AkinatorString*)((*node)->right->elem),
                                    (AkinatorString*)((*node)->left ->elem), &new_node_text, tree));

    res |= tree_set_elem(tree, *node, &new_node_text, false);
    if (res != Tree::OK)
        return Status::TREE_ERROR;

    STATUS_CHECK(interface_now_i_know());

    return Status::NORMAL_WORK;
}
