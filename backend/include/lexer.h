#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <map>

enum TokenType {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_STRING,
    TOKEN_EOF,
    TOKEN_UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    Token(TokenType t, const std::string& v, int l, int c) 
        : type(t), value(v), line(l), column(c) {}
};

class Lexer {
private:
    std::string source;
    size_t position;
    int line;
    int column;
    
    std::map<std::string, bool> keywords;
    std::map<char, std::string> operators;
    
    void initializeKeywords();
    void initializeOperators();
    char peek();
    char advance();
    void skipWhitespace();
    void skipComment();
    Token readNumber();
    Token readIdentifier();
    Token readString();
    Token readOperator();

public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();
    std::string getTokenTypeName(TokenType type);
};

#endif 