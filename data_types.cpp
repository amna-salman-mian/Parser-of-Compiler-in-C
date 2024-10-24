#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>

using namespace std;

enum TokenType {
    T_INT, T_FLOAT, T_DOUBLE, T_STRING, T_BOOL, T_CHAR,
    T_ID, T_NUM, T_IF, T_ELSE, T_RETURN,
    T_ASSIGN, T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,
    T_SEMICOLON, T_GT, T_EOF, T_COMMA, T_EX
};

struct Token {
    TokenType type;
    string value;
};

class Lexer {
private:
    string src; //source code
    size_t pos; //current position in code

public:
    Lexer(const string &src) {
        this->src = src;
        this->pos = 0;
    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < src.size()) {
            char current = src[pos];

            if (isspace(current)) {
                pos++;
                continue;
            }
            if (isdigit(current)) {
                tokens.push_back(Token{T_NUM, consumeNumber()});
                continue;
            }
            if (isalpha(current)) {
                string word = consumeWord();
                if (word == "int") tokens.push_back(Token{T_INT, word});
                else if (word == "float") tokens.push_back(Token{T_FLOAT, word});
                else if (word == "double") tokens.push_back(Token{T_DOUBLE, word});
                else if (word == "string") tokens.push_back(Token{T_STRING, word});
                else if (word == "bool") tokens.push_back(Token{T_BOOL, word});
                else if (word == "char") tokens.push_back(Token{T_CHAR, word});
                else if (word == "if") tokens.push_back(Token{T_IF, word});
                else if (word == "else") tokens.push_back(Token{T_ELSE, word});
                else if (word == "return") tokens.push_back(Token{T_RETURN, word});
                else tokens.push_back(Token{T_ID, word});
                continue;
            }

            switch (current) {
                case '=': tokens.push_back(Token{T_ASSIGN, "="}); break;
                case '+': tokens.push_back(Token{T_PLUS, "+"}); break;
                case '-': tokens.push_back(Token{T_MINUS, "-"}); break;
                case '*': tokens.push_back(Token{T_MUL, "*"}); break;
                case '/': tokens.push_back(Token{T_DIV, "/"}); break;
                case '(': tokens.push_back(Token{T_LPAREN, "("}); break;
                case ')': tokens.push_back(Token{T_RPAREN, ")"}); break;
                case '{': tokens.push_back(Token{T_LBRACE, "{"}); break;
                case '}': tokens.push_back(Token{T_RBRACE, "}"}); break;
                case ';': tokens.push_back(Token{T_SEMICOLON, ";"}); break;
                case '>': tokens.push_back(Token{T_GT, ">"}); break;
                //case ',': tokens.push_back(Token{T_COMMA, ","}); break;
                case '!': tokens.push_back(Token{T_EX, "!"}); break;
                default: cout << "Unexpected character: " << current << endl; exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, ""});
        return tokens;
    }

    string consumeNumber() {
        size_t start = pos;
        while (pos < src.size() && isdigit(src[pos])) pos++;
        if (src[pos] == '.') { // Handling floats and doubles
            pos++;
            while (pos < src.size() && isdigit(src[pos])) pos++;
        }
        return src.substr(start, pos - start);
    }

    string consumeWord() {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos])) pos++;
        return src.substr(start, pos - start);
    }
};

class Parser {
public:
    Parser(const vector<Token> &tokens) {
        this->tokens = tokens;
        this->pos = 0;
    }

    void parseProgram() {
        while (tokens[pos].type != T_EOF) {
            parseStatement();
        }
        cout << "Parsing completed successfully! No Syntax Error" << endl;
    }

private:
    vector<Token> tokens;
    size_t pos;

    void parseStatement() {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT ||
            tokens[pos].type == T_DOUBLE || tokens[pos].type == T_STRING ||
            tokens[pos].type == T_BOOL || tokens[pos].type == T_CHAR) {
            parseDeclaration();
        } else if (tokens[pos].type == T_ID) {
            parseAssignment();
        } else if (tokens[pos].type == T_IF) {
            parseIfStatement();
        } else if (tokens[pos].type == T_RETURN) {
            parseReturnStatement();
        } else if (tokens[pos].type == T_LBRACE) {
            parseBlock();
        } else {
            cout << "Syntax error: unexpected token " << tokens[pos].value << endl;
            exit(1);
        }
    }

    void parseBlock() {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF) {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    void parseDeclaration() {
        expect(tokens[pos].type); // Expect the type (int, float, etc.)
        expect(T_ID);
        expect(T_SEMICOLON);
    }

    void parseAssignment() {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseIfStatement() {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE) {
            expect(T_ELSE);
            parseStatement();
        }
    }

    void parseReturnStatement() {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression() {
        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            pos++;
            parseTerm();
        }
        if (tokens[pos].type == T_GT) {
            pos++;
            parseExpression();  // After relational operator, parse the next expression
        }
    }

    void parseTerm() {
        parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
            pos++;
            parseFactor();
        }
    }

    void parseFactor() {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID) {
            pos++;
        } else if (tokens[pos].type == T_LPAREN) {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        } else {
            cout << "Syntax error: unexpected token " << tokens[pos].value << endl;
            exit(1);
        }
    }

    void expect(TokenType type) {
        if (tokens[pos].type == type) {
            pos++;
        } else {
            cout << "Syntax error: expected " << type << " but found " << tokens[pos].value << endl;
            exit(1);
        }
    }
};

int main() {
    string input = R"(
        int a;
        float f;
        double d;
        string s;
        bool b;
        a = 5;
        f = 3.14;
        d = 2.718281828;
        b = true;
        if (a > 10) {
            return a;
        } else {
            return 0;
        }
    )";

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}
