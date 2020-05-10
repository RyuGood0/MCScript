#include "include/parser.h"
#include "include/utils.h"
#include <errno.h>
#include <string.h>

char* read_file(char* filename)
{
    char* buffer = 0;
    long length;
    FILE* f = fopen(filename, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell (f);
        fseek(f, 0, SEEK_SET);
        buffer = calloc(length, length);

        if (buffer)
            fread(buffer, 1, length, f);

        fclose (f);

        return buffer;
    }

    printf("Incorrect file name\n");
    exit(2);
}

void write_to_file(parser_T* parser, char* filename)
{
    FILE* file;
    file = fopen(filename, "a");

    if (parser->varname != (void*)0)
    {
        fprintf(file, "%d ", parser->hier_lvl);
        fputs(parser->acttype, file);
        fprintf(file, "   ");
        fputs(parser->varname, file);
        fprintf(file, " (");
        fputs(parser->varval, file);
        fprintf(file, ");\n");
    }
    else
    {
        fprintf(file, "%d ", parser->hier_lvl);
        fputs(parser->acttype, file);
        fprintf(file, " (");
        fputs(parser->varval, file);
        fprintf(file, ");\n");
    }
    

    fclose(file);
}

char* get_filename_ext(const char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

// ============================================================================================================
