#ifndef RUNTIME_H
#define RUNTIME_H
#include <stdlib.h>

typedef struct Node {
    enum
    {
        variable,
        function,
        object
    } type;
    char* name;
    char* value;
    struct Node *next;
} Node;

typedef struct RUNTIME_STRUCT
{
    char* contents;
    size_t contents_length;

    char current_char;
    unsigned int index;
    unsigned int hier_lvl;
    struct Node *var_hash[9];
} runtime_T;

typedef struct argums
{
    unsigned int count;
    char** arguments;
} argums;

runtime_T* init_runtime(char* contents);

Node* runtime_get_var(runtime_T* runtime, char* name);

int getSum(int n);

int string_hash(char* str);

char* str_concat(const char *s1, const char *s2);

void runtime_read(runtime_T* runtime);

char* runtime_readaction(runtime_T* runtime);

char* runtime_readarg(runtime_T* runtime);

runtime_T* runtime_skip_line(runtime_T* runtime);

runtime_T* runtime_check_action(runtime_T* runtime, char* buffer);

runtime_T* init_variable(runtime_T* runtime, char* name, char* args);

runtime_T* runtime_print(runtime_T* runtime, char* args);

#endif