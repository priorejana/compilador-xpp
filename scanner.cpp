#include "scanner.h"

// Construtor
Scanner::Scanner(string input, SymbolTable *table)
{
    pos = 0;
    line = 1;

    st = table;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile, line))
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else
        cout << "Unable to open file\n";

    // cout << this->input;
}

int Scanner::getLine()
{
    return line;
}

// Método que retorna o próximo token da entrada
Token *
Scanner::nextToken()
{
    Token *tok;
    string lexeme;

    while (true)
    {

        if (pos >= input.length())
        {
            return new Token(END_OF_FILE);
        }

        if (isspace(input[pos]))
        {
            if (input[pos] == '\n')
                line++;
            pos++;
            continue;
        }

        // Trecho que reconhece Identificadores
        if (isalpha(input[pos]))
        {
            lexeme.push_back(input[pos]);
            pos++;
            while (isalnum(input[pos]) || input[pos] == '_')
            {
                lexeme.push_back(input[pos]);
                pos++;
            }

            // Pesquisar na tabela de palavras reservadas
            // caso encontrem, retornem o token adequado,
            // caso contrário, é ID
            STEntry *obj = st->get(lexeme);
            if (!obj)
                tok = new Token(ID, lexeme);
            else
                tok = new Token(obj->token->name);
        }

        if (isdigit(input[pos]))
        {
            lexeme.push_back(input[pos]);
            pos++;
            while (isdigit(input[pos]))
            {
                lexeme.push_back(input[pos]);
                pos++;
            }
            return new Token(INTEGER_LITERAL, lexeme);
        }

        if (input[pos] == '"')
        {
            pos++;
            while (input[pos] != '"')
            {
                if (pos >= input.length())
                {
                    lexicalError("String literal não fechada");
                }
                lexeme.push_back(input[pos]);
                pos++;
            }
            pos++;
            return new Token(STRING_LITERAL, lexeme);
        }

        switch (input[pos])
        {
        case '<':
            pos++;
            if (input[pos] == '=')
            {
                pos++;
                return new Token(OP_LEQ);
            }
            return new Token(OP_LT);
        case '>':
            pos++;
            if (input[pos] == '=')
            {
                pos++;
                return new Token(OP_GEQ);
            }
            return new Token(OP_GT);
        case '=':
            pos++;
            if (input[pos] == '=')
            {
                pos++;
                return new Token(OP_EQEQ);
            }
            return new Token(OP_EQ);
        case '!':
            pos++;
            if (input[pos] == '=')
            {
                pos++;
                return new Token(OP_DIF);
            }
            lexicalError("Erro !");
            break;
        case '/':
            pos++;
            if (input[pos] == '/')
            {
                pos++;
                while (input[pos] != '\n' && pos < input.length())
                {
                    pos++;
                }
                line++;
                continue;
            }
            else if (input[pos] == '*')
            {
                pos++;
                while (pos < input.length())
                {
                    if (input[pos] == '*')
                    {
                        pos++;
                        if (input[pos] == '/')
                        {
                            pos++;
                            break;
                        }
                    }
                    else
                    {
                        if (input[pos] == '\n')
                            line++;
                        pos++;
                    }
                }
                if (pos >= input.length())
                {
                    lexicalError("Comentário de bloco não fechado");
                }
                continue;
            }
            else
            {
                return new Token(OP_DIV);
            }
        case '+':
            pos++;
            return new Token(OP_MAIS);
        case '-':
            pos++;
            return new Token(OP_MENOS);
        case '*':
            pos++;
            return new Token(OP_MUL);
        case '%':
            pos++;
            return new Token(OP_MOD);
        case '(':
            pos++;
            return new Token(SEP_LPAREN);
        case ')':
            pos++;
            return new Token(SEP_RPAREN);
        case '[':
            pos++;
            return new Token(SEP_LCOLC);
        case ']':
            pos++;
            return new Token(SEP_RCOLC);
        case '{':
            pos++;
            return new Token(SEP_LCHAVE);
        case '}':
            pos++;
            return new Token(SEP_RCHAVE);
        case ';':
            pos++;
            return new Token(SEP_PONTOVIRG);
        case '.':
            pos++;
            return new Token(SEP_PONTO);
        case ',':
            pos++;
            return new Token(SEP_VIRG);
        default:
            lexicalError("Caractere inválido");
            pos++;
            continue;
        }
    }

    return tok;
}

void Scanner::lexicalError(string msg)
{
    cout << "Linha " << line << ": " << msg << endl;

    exit(EXIT_FAILURE);
}
