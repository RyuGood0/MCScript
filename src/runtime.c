#include "include/runtime.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

runtime_T* init_runtime(char* contents)
{
    runtime_T* runtime = calloc(1, sizeof(struct RUNTIME_STRUCT));

    runtime->contents = contents;
    runtime->contents_length = strlen(contents);

    runtime->index = 0;
    runtime->hier_lvl = 0;

    for (int i = 0; i < 9; i++)
    {
        runtime->var_hash[i] = NULL;
    }

    return runtime;
}

Node* runtime_get_var(runtime_T* runtime, char* name)
{
    int hash = string_hash(name);
    Node* temp = runtime->var_hash[hash];
    if (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        else
        {
            while (temp->next != NULL)
            {
                if (strcmp(temp->name, name) == 0)
                {
                    return temp;
                }
                temp = temp->next;
            }
            if (strcmp(temp->name, name) == 0)
            {
                return temp;
            }
        }
    }
    printf("Invalid variable name\n");
    exit(1);
}

// ============================================================================================================

int getSum(int n) 
{  
    int sum = 0;
    while (n != 0) 
    { 
        sum = sum + n % 10; 
        n = n/10; 
    }

    return sum;
}

int string_hash(char* str)
{
    int letter_sum = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        letter_sum += (int) str[i];
    }
    int result = getSum(letter_sum);
    while (result >= 10)
    {
        result = getSum(result);
    }
    return result - 1;
}

char* str_concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    return result;
}

// ============================================================================================================

void runtime_read(runtime_T* runtime)
{
    while (runtime->index < runtime->contents_length)
    {
        char* content_hier_lvl = runtime_readaction(runtime);
        if (strtol(content_hier_lvl,NULL,10) == runtime->hier_lvl)
        {
            while (runtime->contents[runtime->index] != ' ')
            {
                runtime->index++;
            }
            while (runtime->contents[runtime->index] == ' ')
            {
                runtime->index++;
            }
            
            char* act = runtime_readaction(runtime);
            runtime = runtime_check_action(runtime, act);
        }
        else
        {
            runtime = runtime_skip_line(runtime);
        }
    }
}

char* runtime_readaction(runtime_T* runtime)
{
    size_t initial_index = runtime->index;

    while (runtime->contents[runtime->index] != ' ')
    {
        runtime->index++;
    }
    size_t length = runtime->index - initial_index + 1;
    char *buffer = calloc(length, sizeof(char));
    memcpy(buffer, &runtime->contents[initial_index], length);
    buffer[length - 1] = '\0';

    return buffer;
}

char* runtime_readarg(runtime_T* runtime)
{
    size_t initial_index = runtime->index + 1;
    unsigned int count = 0;

    while (runtime->contents[runtime->index] != ')')
    {
        if (runtime->contents[runtime->index] != ',')
        {
            count++;
        }
        runtime->index++;
    }
    runtime->index++;
    size_t length = runtime->index - initial_index;
    char *buffer = calloc(length, sizeof(char));
    memcpy(buffer, &runtime->contents[initial_index], length);
    buffer[length - 1] = '\0';

    return buffer;
}

runtime_T* runtime_skip_line(runtime_T* runtime)
{
    while (runtime->contents[runtime->index] != 10)
    {
        runtime->index++;
    }
    runtime->index++;
}

// ============================================================================================================

runtime_T* runtime_check_action(runtime_T* runtime, char* act)
{
    char* name;
    char* args;
    while (runtime->contents[runtime->index] == ' ')
    {
        runtime->index++;
    }
    if (runtime->contents[runtime->index] != '(')
    {
        name = runtime_readaction(runtime);
        while (runtime->contents[runtime->index] == ' ')
        {
            runtime->index++;
        }
        if (runtime->contents[runtime->index] == '(')
        {
            args = runtime_readarg(runtime);
        }
    }
    else
    {
        args = runtime_readarg(runtime);
    }
    
    if (strcmp(act, "PRINT") == 0)
    {
        runtime = runtime_print(runtime, args);
    }
    else if (strcmp(act, "DEF") == 0)
    {
        runtime = init_variable(runtime, name, args);
    }

    return runtime;
}

// ============================================================================================================

runtime_T* init_variable(runtime_T* runtime, char* name, char* args)
{
    int hash = string_hash(name);
    Node* new_var = calloc(1, sizeof(struct Node));

    new_var->name = name;
    new_var->value = args;
    new_var->type = variable;

    if (runtime->var_hash[hash] == NULL)
    {
        runtime->var_hash[hash] = new_var;
    }
    else
    {
        Node* temp = runtime->var_hash[hash];
        if (strcmp(runtime->var_hash[hash]->name, new_var->name) == 0)
        {
            runtime->var_hash[hash] = new_var;
            return runtime;
        }
        while (temp->next != NULL)
        {
            temp = temp->next;
            if (strcmp(temp->name, new_var->name) == 0)
            {
                temp = new_var;
                return runtime;
            }
        }
        temp->next = new_var;
        return runtime;
    }
}

runtime_T* runtime_print(runtime_T* runtime, char* args)
{
    int arg_count;
    if(strchr(args, ',') != NULL)
    {
        for (int i=0, arg_count=1; args[i]; i++)
        {
            arg_count += (args[i] == ',');
        }
        for (int i = 0; i < arg_count; i++)
        {
            /* code */
        }
    }
    else
    {
        Node* variable = runtime_get_var(runtime, args);
        char* printing_args = variable->value;

        printf("%s\n", printing_args);
        
        return runtime;
    }
}