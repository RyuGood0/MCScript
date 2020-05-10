#ifndef TOKEN_H
#define TOKEN_H
typedef struct TOKEN_STRUCT
{
    enum
    {
        T_STRVAL,
        T_INTVAL,
        T_FLOATVAL,
        T_CHARVAL,
        T_BOOLVAL,
        //-------------------------------------------
        T_ID,
        //-------------------------------------------
        T_EQ,
        T_AT,
        T_SUP,
        T_INF,
        T_INFEQ,
        T_SUPEQ,
        T_NOTEQ,
        T_NOT,
        T_OR,
        T_AND,
        //-------------------------------------------
        T_PLUS,
        T_MIN,
        T_DIVIDE,
        T_MULT,
        T_EXP,
        T_INCR,
        T_DECR,
        T_PLUSEQ,
        T_MINEQ,
        T_DIVEQ,
        T_MULTEQ,
        T_WIG,
        T_PERCENT,
        //-------------------------------------------
        T_RBRAK,
        T_LBRAK,
        T_RCURLY,
        T_LCURLY,
        T_RPAREN,
        T_LPAREN,
        T_COMMA,
        T_QUOTE,
        T_HASHTAG,
        T_DOT,
        T_COLON,
        T_SEMI,
        T_LNEND,
        T_EOF
    } type;

    char* value;
} token_T;

token_T* init_token(int type, char* value);

void token_free(token_T* token);

char* token_to_string(token_T* token);

#endif