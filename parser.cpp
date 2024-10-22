#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cctype>
using namespace std;
enum TokenType
{
    T_ID,
    T_LT,
    T_NEQ,
    T_MUL,
    T_EQ,
    T_LE,
    T_ASSIGN,
    T_GT,
    T_INT,
    T_EOF,
    T_FLOAT, //float data type
    T_AND,
    T_DIV,
    T_RETURN,
    // T_IF,
    T_AGAR,
    T_NUM,
    T_ELSE,
    T_PLUS,
    T_MINUS,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_STRING, //string data type
    T_SEMICOLON,
    // extra additions
    T_AND_OP,
    T_OR_OP,
    T_BREAK,
    T_CONTINUE,
    T_BOOL,
    T_TRUE,
    T_FALSE,
    T_PRINT,
    // loops
    T_WHILE,
    T_FOR
};
string tokenTypeToString(TokenType type)
{
    switch (type)
    {
        // loops
    case T_WHILE:
        return "T_WHILE";
    case T_FOR:
        return "T_FOR";
    case T_AGAR:
        return "T_AGAR";
    // case T_IF:
    //     return "T_IF";
    case T_ID:
        return "T_ID";
    case T_EQ:
        return "T_EQ";
    case T_LE:
        return "T_LE";
    case T_LT:
        return "T_LT";
    case T_AND_OP:
        return "T_AND_OP";
    case T_OR_OP:
        return "T_OR_OP";
    case T_GT:
        return "T_GT";
    case T_INT:
        return "T_INT";
    case T_EOF:
        return "T_EOF";
    case T_AND:
        return "T_AND";
    case T_MUL:
        return "T_MUL";
    case T_DIV:
        return "T_DIV";
    case T_NUM:
        return "T_NUM";
    case T_ELSE:
        return "T_ELSE";
    case T_PLUS:
        return "T_PLUS";
    case T_MINUS:
        return "T_MINUS";
    case T_FLOAT:
        return "T_FLOAT";
    case T_RETURN:
        return "T_RETURN";
    case T_ASSIGN:
        return "T_ASSIGN";
    case T_LPAREN:
        return "T_LPAREN";
    case T_RPAREN:
        return "T_RPAREN";
    case T_LBRACE:
        return "T_LBRACE";
    case T_RBRACE:
        return "T_RBRACE";
    case T_STRING:
        return "T_STRING";
    case T_SEMICOLON:
        return "T_SEMICOLON";
    case T_BREAK:
        return "T_BREAK";
    case T_CONTINUE:
        return "T_CONTINUE";
    case T_BOOL:
        return "T_BOOL";
    case T_TRUE:
        return "T_TRUE";
    case T_FALSE:
        return "T_FALSE";
    case T_PRINT:
        return "T_PRINT";
    default:
        return "Unknown Token";
    };
}

struct Token
{
    TokenType type;
    string value;
    int line; // for storing line number
};

class Lexer
{
private:
    string src;
    size_t pos;
    int line;

public:
    Lexer(const string &src)
    {
        this->src = src;
        this->pos = 0;
        this->line = 1;
    }
    void advance()
    {
        if (src[pos] == '\n')
            // Increment line number on newline
            line++;
        pos++;
    }
    string consumeNumber()
    {
        size_t start = pos;
        while (isdigit(peek()))
            advance();
        return src.substr(start, pos - start);
    }

    string consumeWord()
    {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos]))
            pos++;
        return src.substr(start, pos - start);
    }
    char peek() const
    {
        return pos < src.size() ? src[pos] : '\0';
    }
    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];
            if (isspace(current))
            {
                advance();
                continue;
            }
            else if (isdigit(current))
            {
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
                continue;
            }
            else if (isalpha(current))
            {
                string word = consumeWord();
                if (word == "int")
                    tokens.push_back(Token{TokenType::T_INT, word, line});
                // else if (word == "if")
                //     tokens.push_back(Token{TokenType::T_IF, word, line});
                else if (word == "agar")
                    tokens.push_back(Token{TokenType::T_AGAR, word, line});
                else if (word == "else")
                    tokens.push_back(Token{TokenType::T_ELSE, word, line});
                else if (word == "return")
                    tokens.push_back(Token{TokenType::T_RETURN, word, line});
                else if (word == "break")
                    tokens.push_back(Token{TokenType::T_BREAK, word, line});
                else if (word == "continue")
                    tokens.push_back(Token{TokenType::T_CONTINUE, word, line});
                else if (word == "true")
                    tokens.push_back(Token{TokenType::T_TRUE, word, line});
                else if (word == "false")
                    tokens.push_back(Token{TokenType::T_FALSE, word, line});
                else if (word == "print")
                    tokens.push_back(Token{TokenType::T_PRINT, word, line});
                else if (word == "while")
                    tokens.push_back(Token{TokenType::T_WHILE, word, line});
                else if (word == "for")
                    tokens.push_back(Token{TokenType::T_FOR, word, line});
                else
                    tokens.push_back(Token{TokenType::T_ID, word, line});
                continue;
            }
            // Handle symbols and operators
            switch (current)
            {
            case '+':
                tokens.push_back(Token{TokenType::T_PLUS, "+", line});
                break;
            case '-':
                tokens.push_back(Token{TokenType::T_MINUS, "-", line});
                break;
            case '*':
                tokens.push_back(Token{TokenType::T_MUL, "*", line});
                break;
            case '/':
                tokens.push_back(Token{TokenType::T_DIV, "/", line});
                break;
            case '=':
                advance();
                if (peek() == '=')
                {
                    tokens.push_back(Token{T_EQ, "==", line});
                    advance();
                }
                else
                {
                    tokens.push_back(Token{T_ASSIGN, "=", line});
                }
                break;
            case '<':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{TokenType::T_LE, "<=", line});
                    pos++;
                }
                else
                {
                    tokens.push_back(Token{TokenType::T_LT, "<", line}); // Handle '<'
                }
                break;
            case '>':
                tokens.push_back(Token{TokenType::T_GT, ">"});
                break;
            case '!':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{TokenType::T_NEQ, "!="}); // Handle '!='
                    pos++;
                }
                break;
            case '&':
                if (pos + 1 < src.size() && src[pos + 1] == '&')
                {
                    tokens.push_back(Token{TokenType::T_AND_OP, "&&"}); // Handle '&&'
                    pos++;
                }
                break;
            case '|':
                if (pos + 1 < src.size() && src[pos + 1] == '|')
                {
                    tokens.push_back(Token{TokenType::T_OR_OP, "||", line}); // Handle '||'
                    pos++;
                }
                break;
            case '(':
                tokens.push_back(Token{TokenType::T_LPAREN, "(", line});
                break;
            case ')':
                tokens.push_back(Token{TokenType::T_RPAREN, ")", line});
                break;
            case '{':
                tokens.push_back(Token{TokenType::T_LBRACE, "{", line});
                break;
            case '}':
                tokens.push_back(Token{TokenType::T_RBRACE, "}", line});
                break;
            case ';':
                tokens.push_back(Token{TokenType::T_SEMICOLON, ";", line});
                break;
            default:
                cout << "Unexpected character: " << current << "on line" << line << endl;
                break;
            }
            pos++;
        }
        tokens.push_back(Token{TokenType::T_EOF, "EOF", line});
        return tokens;
    }
};

class Parser
{
private:
    vector<Token> tokens;
    size_t pos;

public:
    Parser(vector<Token> &tokens)
    {
        this->tokens = tokens;
        this->pos = 0;
    }
    Token peek() const
    {
        return pos < tokens.size() ? tokens[pos] : Token{T_EOF, "EOF", -1};
    }
    void parseStatement()
    {
        if (tokens[pos].type == T_INT)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_WHILE)
        {
            parseWhileStatement();
        }
        else if (tokens[pos].type == T_FOR)
        {
            parseForStatement();
        }
        else if (tokens[pos].type == T_AGAR)
        {
            parseIfStatement();
        }
        // else if (tokens[pos].type == T_IF)
        // {
        //     parseIfStatement();
        // }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else if (tokens[pos].type == T_BREAK)
        {
            expect(T_BREAK);
            expect(T_SEMICOLON);
        }
        else if (tokens[pos].type == T_CONTINUE)
        {
            expect(T_CONTINUE);
            expect(T_SEMICOLON);
        }
        else if (tokens[pos].type == T_PRINT)
        {
            expect(T_PRINT);
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
            expect(T_SEMICOLON);
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else
        {
            cout << "Syntax error: unexpected token " << tokens[pos].value << "on line no " << tokens[pos].line << endl;
            exit(1);
        }
    }
    void parseProgram()
    {
        while (peek().type != T_EOF)
        {
            parseStatement();
        }
        cout << "Parsing completed successfully! No Syntax Error" << endl;
    }
    void parseBlock()
    {
        expect(T_LBRACE);
        while (peek().type != T_RBRACE && peek().type != T_EOF)
        {
            parseStatement();
        }
        expect(T_RBRACE);
    }
    void parseDeclaration()
    {
        expect(T_INT);
        expect(T_ID);
        expect(T_SEMICOLON);
    }
    void parseAssignment()
    {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }
    void parseWhileStatement()
    {
        expect(T_WHILE);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
    }
    void parseForStatement()
    {
        expect(T_FOR);
        expect(T_LPAREN);
        parseAssignment();
        expect(T_SEMICOLON);
        parseExpression();
        expect(T_SEMICOLON);
        parseAssignment();
        expect(T_RPAREN);
        parseStatement();
    }

    void parseIfStatement()
    {
        expect(T_AGAR);
        // expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (peek().type == T_ELSE)
        {
            expect(T_ELSE);
            parseStatement();
        }
    }
    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }
    void parseExpression()
    {
        // Check for a number, identifier, or left parenthesis
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID)
        {
            pos++;
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        }
        else
        {
            cout << "Syntax error: expected a number, identifier, or '(' but found " << tokens[pos].value << " on line " << tokens[pos].line << endl;
            exit(1);
        }
        parseComparison();
    }

    void parseComparison()
    {
        parseTerm();
        while (peek().type == T_GT || peek().type == T_LT || peek().type == T_EQ || peek().type == T_NEQ)
        {
            pos++; // Consume comparison operator
            parseTerm();
        }
    }
    void parseLogicalExpression()
    {
        parseComparison();
        while (peek().type == T_AND_OP || peek().type == T_OR_OP)
        {
            pos++; // Consume logical operator (&& or ||)
            parseComparison();
        }
    }

    void parseTerm()
    {
        parseFactor();
        while (peek().type == T_PLUS || peek().type == T_MINUS)
        {
            pos++;
            parseFactor();
        }
    }
    void parseFactor()
    {
        if (peek().type == T_NUM || peek().type == T_ID)
        {
            pos++; // Consume numbers or identifiers
        }
        else if (peek().type == T_LPAREN)
        {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN); // Ensure matching parentheses
        }
        else
        {
            cout << "Syntax error: unexpected token '" << peek().value << "' on line " << peek().line << endl;
            exit(1);
        }
    }
    void expect(TokenType type)
    {
        if (peek().type == type)
        {
            pos++;
        }
        else
        {
            cout << "Syntax error: " << tokenTypeToString(type) << " but found '" << peek().value << "' on line " << peek().line << endl;
            exit(1);
        }
    }
};
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Provide a file" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file)
    {
        cout << "Error: Unable to open file " << argv[1] << endl;
        return 1;
    }

    string sourceCode((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    Lexer lexer(sourceCode);
    vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    parser.parseProgram();
    return 0;
}
