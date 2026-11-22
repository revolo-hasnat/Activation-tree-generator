#ifndef ACTIVATION_H
#define ACTIVATION_H

void activation_begin_function(const char *name);
void activation_add_call(const char *name);
void activation_add_return(int value);
void activation_add_void_return(void);
void activation_end_function(void);
void activation_print_tree(void);

#endif