#include "include/lexer.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// ============================================================================================================

lexer_T* init_lexer(char* contents)
{
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));

    lexer->contents = contents;
    lexer->contents_length = strlen(contents);

    lexer->index = 0;
    lexer->is_next_lnend = 0;
    lexer->is_prev_lnend = 0;
    lexer->is_comment = 0;
    lexer->line_n = 1;
    lexer->current_char = lexer->contents[lexer->index];

    return lexer;
}

void advance_one(lexer_T* lexer)
{
    lexer->index += 1;
    lexer->current_char = lexer->contents[lexer->index];
}

void lexer_advance(lexer_T* lexer)
{
    if (lexer->current_char == '\n' || lexer->current_char == 10 || lexer->current_char == 13)
    {
        check_lnend(lexer);
    }

    if (lexer->current_char != '\0' && lexer->index < strlen(lexer->contents))
    {
        advance_one(lexer);
    }
    else
    {
        lexer->current_char = '\0';
    }
}

void check_lnend(lexer_T* lexer)
{
    if (lexer->is_next_lnend == 1)
    {
        return;
    }

    while (lexer->current_char == ' ' || lexer->current_char == 13)
    {
        advance_one(lexer);
    }
    
    if (lexer->current_char == '\n' || lexer->current_char == 10)
    {
        lexer->line_n += 1;

        if (lexer->is_comment == 1)
        {
            return;
        }

        size_t temp_ind = lexer->index - 1;

        advance_one(lexer);

        size_t adv_ind = lexer->index;

        while (lexer->current_char == 13 || lexer->current_char == 10)
        {
            advance_one(lexer);
        }
        

        while (lexer->contents[temp_ind] == ' ' || lexer->contents[temp_ind] == 13)
        {
            temp_ind--;
        }

        while (lexer->contents[adv_ind] == ' ' || lexer->contents[adv_ind] == 13 || lexer->contents[adv_ind] == 10)
        {
            adv_ind++;

            if (lexer->contents[adv_ind] == 35)
            {
                check_comment(lexer);
            }
            
        }

        if (lexer->contents[temp_ind] == '\n' || lexer->contents[temp_ind] == 10 || lexer->contents[temp_ind] == 123 || lexer->contents[temp_ind] == 125)
        {
            lexer->is_next_lnend = 0;
        }
        else if (lexer->contents[adv_ind] == 123)
        {
            lexer->is_next_lnend = 0;
        }
        else if (lexer->is_prev_lnend == 1)
        {
            lexer->is_next_lnend = 0;
        }
        else
        {
            lexer->is_next_lnend = 1;
            return;
        }
        
    }

    lexer->is_next_lnend = 0;
}

// ============================================================================================================

void lexer_expect_char(lexer_T* lexer, char c)
{
    if (lexer->current_char != c)
    {
        printf("Error: [Line %d] Lexer expected the current char to be `%c`, but it was `%c`.", c, lexer->current_char, lexer->line_n);
        exit(1);
    }
}

// ============================================================================================================

void is_comment_on_line(lexer_T* lexer)
{
    size_t temp_ind = lexer->index - 1;

    while (lexer->contents[temp_ind] == ' ')
    {
        temp_ind--;
    }

    if (lexer->contents[temp_ind] == '\n' || lexer->contents[temp_ind] == 10 || lexer->contents[temp_ind] == 123)
    {
        lexer->is_next_lnend = 0;
    }
    else if (lexer->is_prev_lnend == 1)
    {
        lexer->is_next_lnend = 0;
    }
    else
    {
        lexer->is_next_lnend = 1;
        return;
    }
}

void check_comment(lexer_T* lexer)
{
    if (lexer->current_char == '#')
    {
        lexer->is_comment = 1;

        is_comment_on_line(lexer);

        advance_one(lexer);

        if (lexer->current_char == '#')  // block comment
        {
            lexer_advance(lexer);
            skip_block_comment(lexer);
            return;
        }

        skip_inline_comment(lexer);
    }
}

void skip_whitespace(lexer_T* lexer)
{
    while (lexer->current_char == ' ' || lexer->current_char == 10 || lexer->current_char == 13)
    {
        if (lexer->is_next_lnend == 1)
        {
            break;
        }
        
        advance_one(lexer);
        if (lexer->current_char == 13 || lexer->current_char == 10)
        {
            check_lnend(lexer);
        }
        if (lexer->current_char == 35)
        {
            check_comment(lexer);
        }
    }
}

void skip_inline_comment(lexer_T* lexer)
{
    while (1)
    {
        advance_one(lexer);

        if (lexer->current_char == '\n' || lexer->current_char == 10 || lexer->current_char == 13)
        {
            skip_whitespace(lexer);
            break;
        }
    }

    lexer->is_comment = 0;
}

void skip_block_comment(lexer_T* lexer)
{
    while (1)
    {
        lexer_advance(lexer);

        if (lexer->current_char == '#')
        {
            advance_one(lexer);

            if (lexer->current_char == '#')
            {
                advance_one(lexer);
                while (lexer->current_char == 10 || lexer->current_char == 13 || lexer->current_char == ' ')
                {
                    lexer_advance(lexer);
                }
                break;
            }
        }
    }
    lexer->is_comment = 0;
}

// ============================================================================================================

token_T* get_next_token(lexer_T* lexer)
{
    while (lexer->current_char != '\0' && lexer->index < lexer->contents_length)
    {
        if (lexer->current_char == ' ')
        {
            skip_whitespace(lexer);
        }

        check_comment(lexer);

        if (lexer->current_char == '\n' || (int) lexer->current_char == 10 || lexer->current_char == 13)
        {
            check_lnend(lexer);
        }

        if (lexer->is_next_lnend == 1 && lexer->is_prev_lnend != 1)
        {
            lexer->is_next_lnend = 0;
            lexer->is_prev_lnend = 1;
            return init_token(T_LNEND, "LNEND");
        }
        else
        {
            lexer->is_next_lnend = 0;
            lexer->is_prev_lnend = 0;
        }
        
        check_comment(lexer);

        if (lexer->is_next_lnend == 1 && lexer->is_prev_lnend != 1)
        {
            lexer->is_next_lnend = 0;
            lexer->is_prev_lnend = 1;
            return init_token(T_LNEND, "LNEND");
        }
        else
        {
            lexer->is_next_lnend = 0;
            lexer->is_prev_lnend = 0;
        }
        
        if (lexer->current_char == ' ')
        {
            skip_whitespace(lexer);
        }

        if (isdigit(lexer->current_char))
        {
            return collect_number(lexer);
        }

        if (isalnum(lexer->current_char))
        {
            return collect_id(lexer);
        }

        if (lexer->current_char == '+')
        {
            char* value = lexer_get_char_as_string(lexer);

            int type = T_PLUS;
            lexer_advance(lexer);

            if (lexer->current_char == '=')  // +=
            {
                type = T_PLUSEQ;
                value = realloc(value, strlen(value) + 2);
                char* strchar = lexer_get_char_as_string(lexer);
                strcat(value, strchar);
                free(strchar);

                lexer_advance(lexer);
            }
            else if (lexer->current_char == '+')  // ++
            {
                type = T_INCR;
                value = realloc(value, strlen(value) + 2);
                char* strchar = lexer_get_char_as_string(lexer);
                strcat(value, strchar);
                free(strchar);

                lexer_advance(lexer);
            }

            return init_token(type, value);
        }

        if (lexer->current_char == '-')
        {
            char* value = lexer_get_char_as_string(lexer);

            int type = T_INF;
            lexer_advance(lexer);

            if (lexer->current_char == '=')  // -=
            {
                type = T_MINEQ;
                value = realloc(value, strlen(value) + 2);
                char* strchar = lexer_get_char_as_string(lexer);
                strcat(value, strchar);
                free(strchar);

                lexer_advance(lexer);
            }
            else if (lexer->current_char == '-')  // --
            {
                type = T_DECR;
                value = realloc(value, strlen(value) + 2);
                char* strchar = lexer_get_char_as_string(lexer);
                strcat(value, strchar);
                free(strchar);

                lexer_advance(lexer);
            }

            return init_token(type, value);
        }

        if (lexer->current_char == '*')
        {
            char* value = lexer_get_char_as_string(lexer);

            int type = T_MULT;
            lexer_advance(lexer);

            if (lexer->current_char == '=')  // *=
            {
                type = T_MULTEQ;
                value = realloc(value, strlen(value) + 2);
                char* strchar = lexer_get_char_as_string(lexer);
                strcat(value, strchar);
                free(strchar);

                lexer_advance(lexer);
            }

            return init_token(type, value);
        }

        if (lexer->current_char == '/')
        {
            char* value = lexer_get_char_as_string(lexer);

            int type = T_DIVIDE;
            lexer_advance(lexer);

            if (lexer->current_char == '=')  // /=
            {
                type = T_DIVEQ;
                value = realloc(value, strlen(value) + 2);
                char* strchar = lexer_get_char_as_string(lexer);
                strcat(value, strchar);
                free(strchar);

                lexer_advance(lexer);
            }

            return init_token(type, value);
        }

        if (lexer->current_char == '>')
        {
            char* value = lexer_get_char_as_string(lexer);

            int type = T_SUP;
            lexer_advance(lexer);

            if (lexer->current_char == '=')  // >=
            {
                type = T_SUPEQ;
                value = realloc(value, strlen(value) + 2);
                char* strchar = lexer_get_char_as_string(lexer);
                strcat(value, strchar);
                free(strchar);

                lexer_advance(lexer);
            }

            return init_token(type, value);
        }

        if (lexer->current_char == '<')
        {
            char* value = lexer_get_char_as_string(lexer);

            int type = T_INF;
            lexer_advance(lexer);

            if (lexer->current_char == '=')  // <=
            {
                type = T_INFEQ;
                value = realloc(value, strlen(value) + 2);
                char* strchar = lexer_get_char_as_string(lexer);
                strcat(value, strchar);
                free(strchar);

                lexer_advance(lexer);
            }

            return init_token(type, value);
        }

        if (lexer->current_char == '!')
        {
            char* value = lexer_get_char_as_string(lexer);

            int type = T_NOT;
            lexer_advance(lexer);

            if (lexer->current_char == '=')  // !=
            {
                type = T_NOTEQ;
                value = realloc(value, strlen(value) + 2);
                char* strchar = lexer_get_char_as_string(lexer);
                strcat(value, strchar);
                free(strchar);

                lexer_advance(lexer);
            }

            return init_token(type, value);
        }

        switch(lexer->current_char)
        {
            case '"': return collect_string(lexer); break;
            case '{': return lexer_advance_with_token(lexer, T_LCURLY); break;
            case '}': return lexer_advance_with_token(lexer, T_RCURLY); break;
            case '[': return lexer_advance_with_token(lexer, T_LBRAK); break;
            case ']': return lexer_advance_with_token(lexer, T_RBRAK); break;
            case '(': return lexer_advance_with_token(lexer, T_LPAREN); break;
            case ')': return lexer_advance_with_token(lexer, T_RPAREN); break;
            case ';': return lexer_advance_with_token(lexer, T_SEMI); break;
            case ',': return lexer_advance_with_token(lexer, T_COMMA); break;
            case '%': return lexer_advance_with_token(lexer, T_PERCENT); break;
            case '.': return lexer_advance_with_token(lexer, T_DOT); break;
            case '@': return lexer_advance_with_token(lexer, T_AT); break;
            case '~': return lexer_advance_with_token(lexer, T_WIG); break;
            case '^': return lexer_advance_with_token(lexer, T_EXP); break;
            case ':': return lexer_advance_with_token(lexer, T_COLON); break;
            case '=': return lexer_advance_with_token(lexer, T_EQ); break;
            case '\0': return init_token(T_EOF, "EOF"); break;
            default: printf("[Line %d] Unexpected %i\n", lexer->line_n, lexer->current_char); exit(1); break;
        }
    }

    return init_token(T_EOF, "EOF");
}

token_T* lexer_advance_with_token(lexer_T* lexer, int type)
{
    char *value = lexer_get_char_as_string(lexer);
    lexer_advance(lexer);
    token_T* token = init_token(type, value);
    free(value);

    // ensures that the lexer state is correct if exited through this function.
    skip_whitespace(lexer);

    return token;
}

token_T* collect_string(lexer_T* lexer)
{
    lexer_advance(lexer);

    size_t initial_index = lexer->index;

    while (lexer->current_char != '"')
    {
        if (lexer->current_char == '\0')
        {
            printf("[Line %d] Missing closing quotation mark\n", lexer->line_n); exit(1);
        }

        lexer_advance(lexer);
    }

    size_t length = lexer->index - initial_index + 1;
    char *buffer = calloc(length, sizeof(char));
    memcpy(buffer, &lexer->contents[initial_index], length);
    buffer[length - 1] = '\0';

    lexer_advance(lexer);

    token_T* token = init_token(T_STRVAL, buffer);
    free(buffer);

    return token;
}

token_T* collect_number(lexer_T* lexer)
{
    int type = T_INTVAL;

    char* buffer = calloc(1, sizeof(char));
    buffer[0] = '\0';

    while (isdigit(lexer->current_char))
    {
        char* strchar = lexer_get_char_as_string(lexer);
        buffer = realloc(buffer, strlen(buffer) + 2);
        strcat(buffer, strchar);
        free(strchar);

        lexer_advance(lexer);
    }

    if (lexer->current_char == '.')
    {
        char* strchar = lexer_get_char_as_string(lexer);
        buffer = realloc(buffer, strlen(buffer) + 2);
        strcat(buffer, strchar);
        free(strchar);

        lexer_advance(lexer);

        type = T_FLOATVAL;

        while (isdigit(lexer->current_char))
        {
            char* strchar = lexer_get_char_as_string(lexer);
            buffer = realloc(buffer, strlen(buffer) + 2);
            strcat(buffer, strchar);
            free(strchar);

            lexer_advance(lexer);
        }
    }

    return init_token(type, buffer);
}

token_T* collect_bool(lexer_T* lexer)
{

}

token_T* collect_id(lexer_T* lexer)
{
    size_t initial_index = lexer->index;

    while (isalnum(lexer->current_char) || lexer->current_char == '_')
    {
        lexer_advance(lexer);
    }

    size_t length = lexer->index - initial_index + 1;
    char *buffer = calloc(length, sizeof(char));
    memcpy(buffer, &lexer->contents[initial_index], length);
    buffer[length - 1] = '\0';

    token_T* token;
    token = init_token(T_ID, buffer);
    
    free(buffer);

    return token;
}

char* lexer_get_char_as_string(lexer_T* lexer)
{
    char *str = calloc(2, sizeof(char));
    str[0] = lexer->current_char;
    str[1] = '\0';

    return str;
}