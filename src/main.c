#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/utils.h"
#include "include/runtime.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("No file given\n");
        return 1;
    }
    
    char* ext = get_filename_ext(argv[1]);
    char* content = read_file(argv[1]);
    size_t len = strlen(argv[1]);
    if (strcmp(ext, "mcs") == 0)
    {
        lexer_T* lexer = (void*)0;
        runtime_T* runtime = (void*)0;
        lexer = init_lexer(content);

        parser_T* parser = (void*)0;
        parser = init_parser(lexer);

        char* export_name = argv[1];
        export_name[len - 1] = 'a';
        remove(export_name);

        while (parser->current_token->type != T_EOF)
        {    
            parser = parser_parse(parser);
            write_to_file(parser, export_name);
        }
        content = read_file(export_name);
        runtime = init_runtime(content);
        runtime_read(runtime);
    }
    else if (strcmp(ext, "mca") == 0)
    {
        runtime_T* runtime = (void*)0;
        content = read_file(argv[1]);
        runtime = init_runtime(content);
        runtime_read(runtime);
    }
    else
    {
        printf("incompatible file extension\n");
    }
    
    return 0;
}