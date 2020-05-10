#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

typedef struct PARSER_STRUCT
{
    lexer_T* lexer;
    
    token_T* current_token;
    token_T* prev_token;

    char* acttype;

    char* varval;
    char* varname;

    int hier_lvl;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

void parser_advance(parser_T* parser);

void parser_eat_lnend(parser_T* parser);

void parser_eat(parser_T* parser, int token_type);

void parser_token_error(parser_T* parser, int token_type);

parser_T* parser_parse(parser_T* parser);

parser_T* parser_parser_statement(parser_T* parser);

parser_T* check_id(parser_T* parser);

#endif