#include <iostream>
#include <ctype.h> //Funções de caracteres
#include <string>

using namespace std;

enum Names
{
    UNDEF,
    ID,              // 1
    CLASS,           // 2
    EXTENDS,         // 3
    INT,             // 4
    STRING,          // 5
    BREAK,           // 6
    PRINT,           // 7
    READ,            // 8
    RETURN,          // 9
    SUPER,           // 10
    IF,              // 11
    ELSE,            // 12
    FOR,             // 13
    NEW,             // 14
    CONSTRUCTOR,     // 15
    INTEGER_LITERAL, // 16
    STRING_LITERAL,  // 17
    OP_LT,           // 18
    OP_GT,           // 19
    OP_LEQ,          // 20
    OP_GEQ,          // 21
    OP_MAIS,         // 22
    OP_MENOS,        // 23
    OP_MUL,          // 24
    OP_DIV,          // 25
    OP_MOD,          // 26
    OP_EQ,           // 27
    OP_EQEQ,         // 28
    OP_DIF,          // 29
    SEP_LPAREN,      // 30
    SEP_RPAREN,      // 31
    SEP_LCOLC,       // 32
    SEP_RCOLC,       // 33
    SEP_LCHAVE,      // 34
    SEP_RCHAVE,      // 35
    SEP_PONTOVIRG,   // 36
    SEP_PONTO,       // 37
    SEP_VIRG,        // 38
    END_OF_FILE
};

class Token
{
public:
    int name;
    int attribute;
    string lexeme;

    Token(int name)
    {
        this->name = name;
        attribute = UNDEF;
    }

    Token(int name, string l)
    {
        this->name = name;
        attribute = UNDEF;
        lexeme = l;
    }

    Token(int name, int attr)
    {
        this->name = name;
        attribute = attr;
    }
};
