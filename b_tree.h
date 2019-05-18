#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct b_tree_node
{
    int data;
    struct b_tree_node *left, *right;
};

struct b_tree
{
    struct b_tree_node *root;
};

static struct b_tree_node* b_tree_new_node(int data)
{
    struct b_tree_node* node = (struct b_tree_node*)malloc(sizeof(struct b_tree_node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static struct b_tree_node* b_tree_min_value_node(struct b_tree_node *node)
{
    return node->left == NULL ? node: b_tree_min_value_node(node->left);
}

struct b_tree* b_tree_new()
{
    struct b_tree *thiz = (struct b_tree*)malloc(sizeof(struct b_tree));
    thiz->root = NULL;
    return thiz;
}

static struct b_tree_node* _b_tree_insert_recursive(struct b_tree_node* node, int data)
{
    if(!node)
    {
        node = b_tree_new_node(data);
        return node;
    }

    if(node->data > data)
    {
        node->left = _b_tree_insert_recursive(node->left, data);
    }
    else
    {
        node->right = _b_tree_insert_recursive(node->right, data);
    }
    return node;
}

int b_tree_insert2(struct b_tree *thiz, int data)
{
    thiz->root = _b_tree_insert_recursive(thiz->root, data);
    return 1;
}

int b_tree_insert(struct b_tree *thiz, int data)
{
    struct b_tree_node *current = thiz->root;
    struct b_tree_node *parent = thiz->root;

    if(thiz->root == NULL) {
        thiz->root = b_tree_new_node(data);
    } else {
        while(1) {
            parent = current;
            if(data < current->data) {
                current = current->left;
                if(current == NULL) {
                    parent->left = b_tree_new_node(data);
                    return 1;
                }
            } else if(data > current->data) {
                current = current->right;
                if(current == NULL) {
                    parent->right = b_tree_new_node(data);
                    return 1;
                }
            } else {
                return 0;
            }
        }
    }

    return 0;
}

int b_tree_search(struct b_tree *thiz, int data)
{
    struct b_tree_node* root = thiz->root;
    struct b_tree_node* current = root;
    while(1) {
        if(!current) {
            return 0;
        }
        if(current->data == data) {
            return 1;
        } else if(current->data > data) {
            current = current->right;
        } else {
            current = current->left;
        }
    }

    return 0;
}

static struct b_tree_node* _b_tree_search_recursive(struct b_tree_node* node, int data) {
    if(!node) {
        return NULL;
    }

    if(node->data == data) {
        return node;
    } else if(node->data > data) {
        return _b_tree_search_recursive(node->right, data);
    } else {
        return _b_tree_search_recursive(node->left, data);
    }
}

int b_tree_search2(struct b_tree *thiz, int data) {
    return _b_tree_search_recursive(thiz->root, data) == NULL ? 0 : 1;
}

int _b_tree_delete(struct b_tree_node* root, int data) {
    struct b_tree_node *current, *parent;
    current = parent = root;

    while(1) {
        if(!current) {
            return 0;
        }
        if(current->data == data) {
            if(current->left == NULL && current->right == NULL) {
                if(parent->left == current) {
                    parent->left = NULL;
                } else if (parent->right == current) {
                    parent->right = NULL;
                } else {
                    assert(false);
                }
                free(current);
            } else if(current->left == NULL) {
                if(parent->left == current) {
                    parent->left = current->right;
                } else if(parent->right == current) {
                    parent->right = current->right;
                } else {
                    assert(false);
                }

                current->right = NULL;
                free(current);
            }
            else if(current->right == NULL) {
                if(parent->left == current) {
                    parent->left = current->left;
                } else if(parent->right == current) {
                    parent->right = current->left;
                } else {
                    assert(false);
                }
                current->left = NULL;
                free(current);
            } else {
                struct b_tree_node *tmp = b_tree_min_value_node(current->right);
                current->data = tmp->data;
                _b_tree_delete(current->right, tmp->data);
            }
            return 1;
        } else if(current->data > data) {
            parent = current;
            current = current->left;
        } else {
            parent = current;
            current = current->right;
        }
    }
}

int b_tree_delete(struct b_tree *thiz, int data) {
    if(thiz->root) {
        if(thiz->root->data == data) {
            struct b_tree_node *root = thiz->root;
            if(root->right == NULL && root->left == NULL) {
                thiz->root = NULL;
                free(root);
            } else if(root->left == NULL) {
                thiz->root = root->right;
                free(root);
            } else if(root->right == NULL) {
                thiz->root = root->left;
                free(root);
            } else {
                struct b_tree_node* tmp = b_tree_min_value_node(root->right);
                int data = tmp->data;
                int ret = _b_tree_delete(thiz->root, tmp->data);
                thiz->root->data = data;
            }
            return 1;
        }
        return _b_tree_delete(thiz->root, data);
    }

    return 0;
}

static void _b_tree_inorder(struct b_tree_node *node, FILE* pFile) {
    if(node == NULL) {
        return;
    }

    _b_tree_inorder(node->left, pFile);
    fprintf(pFile, "%u", node->data);
    _b_tree_inorder(node->right, pFile);
}

void b_tree_inorder2(struct b_tree *thiz, FILE* pFile)
{
    _b_tree_inorder(thiz->root, pFile);
}

