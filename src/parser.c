#include "include/parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));

    parser->lexer = lexer;

    parser->prev_token = (void*)0;
    parser->current_token = get_next_token(lexer);

    parser->acttype = (void*)0;
    
    parser->varval = (void*)0;
    parser->varname = (void*)0;

    parser->hier_lvl = 0;

    return parser;
}

// ============================================================================================================

void parser_advance(parser_T* parser)
{      
    parser->prev_token = parser->current_token;
    parser->current_token = get_next_token(parser->lexer);  
}

void parser_eat_lnend(parser_T* parser)
{
    if (parser->current_token->type == T_EOF)
    {
        parser_eat(parser, T_EOF);
    }
    else
    {
        parser_eat(parser, T_LNEND);
    }
}

void parser_eat(parser_T* parser, int token_type)
{
    if (parser->current_token->type != token_type)
    {
        parser_token_error(parser, token_type);
    }
    else if (parser->current_token->type == token_type)
    {
        parser_advance(parser);
    }
}

void parser_token_error(parser_T* parser, int token_type)
{
    printf(
        "[Line %d] Unexpected token '%s', was expecting '%d'.\n",
        parser->lexer->line_n,
        parser->current_token->value,
        token_type
    );
    exit(1);
}

// ============================================================================================================

parser_T* parser_parse(parser_T* parser)
{
    return parser_parser_statement(parser);
}

parser_T* parser_parser_statement(parser_T* parser)
{
    if (parser->current_token->type == T_ID)
    {
        return check_id(parser);
    }
}

// ============================================================================================================

parser_T* check_id(parser_T* parser)
{
    if (strcmp(parser->current_token->value, "if") == 0)
    {
        /* code */
    }
    else if (strcmp(parser->current_token->value, "else") == 0)
    {
        /* code */
    }
    else if (strcmp(parser->current_token->value, "defun") == 0)
    {
        parser->acttype = "DEF";
    }
    else if (strcmp(parser->current_token->value, "while") == 0)
    {
        /* code */
    }
    else if (strcmp(parser->current_token->value, "for") == 0)
    {
        /* code */
    }
    else if (strcmp(parser->current_token->value, "print") == 0)
    {
        parser->acttype = "PRINT";
        parser_eat(parser, T_ID);
        parser_eat(parser, T_LPAREN);
        parser->varname = (void*)0;
        parser->varval = "";
        char* args_val;
        args_val = malloc(strlen(parser->current_token->value)+1);
        while (parser->current_token->type == T_ID || parser->current_token->type == T_COMMA)
        {
            args_val[0] = '\0';
            strcat(args_val, parser->varval);
            strcat(args_val, parser->current_token->value);
            parser->varval = args_val;
            parser_advance(parser);
            args_val = malloc(strlen(parser->varval)+strlen(parser->current_token->value)+1);
        }
        parser_eat(parser, T_RPAREN);
        parser_eat_lnend(parser);
        return parser;
    }
    else
    {
        parser->acttype = "DEF";
        parser->varname = parser->current_token->value;
        parser_eat(parser, T_ID);
        parser_eat(parser, T_EQ);
        parser->varval = parser->current_token->value;
        parser_advance(parser);
        parser_eat_lnend(parser);
        return parser;
    }
}

// ============================================================================================================
