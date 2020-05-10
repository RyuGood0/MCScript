#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include <stdlib.h>

typedef struct LEXER_STRUCT
{
    char* contents;
    size_t contents_length;

    char current_char;
    unsigned int index;
    unsigned int line_n;
    int is_next_lnend;
    int is_prev_lnend;
    int is_comment;
} lexer_T;

lexer_T* init_lexer(char* contents);

void lexer_advance(lexer_T* lexer);

void advance_one(lexer_T* lexer);

void skip_lnend(lexer_T* lexer);

void is_comment_on_line(lexer_T* lexer);

void check_comment(lexer_T* lexer);

void skip_whitespace(lexer_T* lexer);

void skip_inline_comment(lexer_T* lexer);

void skip_block_comment(lexer_T* lexer);

void check_lnend(lexer_T* lexer);

token_T* get_next_token(lexer_T* lexer);

token_T* collect_string(lexer_T* lexer);

token_T* collect_number(lexer_T* lexer);

token_T* collect_bool(lexer_T* lexer);

token_T* collect_id(lexer_T* lexer);

token_T* lexer_advance_with_token(lexer_T* lexer, int type);

char* lexer_get_char_as_string(lexer_T* lexer);

#endif