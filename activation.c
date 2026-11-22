
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "activation.h"

typedef struct ActivationNode {
    char *name;

    int has_return;
    int has_return_value;
    int return_value;

    struct ActivationNode *children;
    struct ActivationNode *last_child;
    struct ActivationNode *next;
} ActivationNode;

static ActivationNode *root = NULL;
static ActivationNode *last_root = NULL;
static ActivationNode *current_function = NULL;

static ActivationNode *create_node(const char *name)
{
    ActivationNode *node = malloc(sizeof(ActivationNode));

    if (!node) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    node->name = malloc(strlen(name) + 1);

    if (!node->name) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    strcpy(node->name, name);

    node->has_return = 0;
    node->has_return_value = 0;
    node->return_value = 0;
    node->children = NULL;
    node->last_child = NULL;
    node->next = NULL;

    return node;
}

static void add_child(ActivationNode *parent, ActivationNode *child)
{
    if (!parent->children) {
        parent->children = child;
    } else {
        parent->last_child->next = child;
    }

    parent->last_child = child;
}

void activation_begin_function(const char *name)
{
    ActivationNode *node = create_node(name);

    if (!root) {
        root = node;
    } else {
        last_root->next = node;
    }

    last_root = node;
    current_function = node;
}

void activation_add_call(const char *name)
{
    if (current_function) {
        ActivationNode *node = create_node(name);
        add_child(current_function, node);
    }
}

void activation_add_return(int value)
{
    if (current_function) {
        current_function->has_return = 1;
        current_function->has_return_value = 1;
        current_function->return_value = value;
    }
}

void activation_add_void_return(void)
{
    if (current_function) {
        current_function->has_return = 1;
        current_function->has_return_value = 0;
    }
}

void activation_end_function(void)
{
    current_function = NULL;
}

static void print_tree(ActivationNode *node, const char *prefix,
                       int is_last, int is_root)
{
    if (!node) return;

    printf("%s", prefix);

    if (!is_root) {
        printf("%s", is_last ? "└── " : "├── ");
    }

    printf("%s()\n", node->name);

    char new_prefix[1024];

    snprintf(new_prefix, sizeof(new_prefix), "%s%s",
             prefix,
             is_root ? "" : (is_last ? "    " : "│   "));

    ActivationNode *child = node->children;

    while (child) {
        print_tree(child, new_prefix,
                   child->next == NULL, 0);
        child = child->next;
    }

    if (node->has_return) {
        printf("%s%s", new_prefix, "└── ");

        if (node->has_return_value) {
            printf("Return: %d\n", node->return_value);
        } else {
            printf("Return\n");
        }
    }
}

void activation_print_tree(void)
{
    printf("\n========== ACTIVATION TREE ==========\n");

    ActivationNode *node = root;

    while (node) {
        print_tree(node, "", 1, 1);
        node = node->next;
    }

    printf("=====================================\n");
}
