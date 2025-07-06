#include "../include/lexer.h"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& source) : source(source), position(0), line(1), column(1) {
    initializeKeywords();
    initializeOperators();
}

void Lexer::initializeKeywords() {
    keywords["int"] = true;
    keywords["float"] = true;
    keywords["double"] = true;
    keywords["char"] = true;
    keywords["string"] = true;
    keywords["bool"] = true;
    keywords["if"] = true;
    keywords["else"] = true;
    keywords["for"] = true;
    keywords["while"] = true;
    keywords["return"] = true;
    keywords["cout"] = true;
    keywords["cin"] = true;
    keywords["endl"] = true;
}

void Lexer::initializeOperators() {
    operators['+'] = "+";
    operators['-'] = "-";
    operators['*'] = "*";
    operators['/'] = "/";
    operators['%'] = "%";
    operators['='] = "=";
    operators['<'] = "<";
    operators['>'] = ">";
    operators['!'] = "!";
    operators['&'] = "&";
    operators['|'] = "|";
    operators['^'] = "^";
    operators['~'] = "~";
    operators['.'] = ".";
    operators[':'] = ":";
    operators['?'] = "?";
    operators['#'] = "#";
    operators['@'] = "@";
    operators['$'] = "$";
}

char Lexer::peek() {
    if (position >= source.length()) return '\0';
    return source[position];
}

char Lexer::advance() {
    if (position >= source.length()) return '\0';
    char current = source[position++];
    if (current == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return current;
}

void Lexer::skipWhitespace() {
    while (position < source.length() && std::isspace(peek())) {
        advance();
    }
}

void Lexer::skipComment() {
    if (peek() == '/' && position + 1 < source.length()) {
        if (source[position + 1] == '/') {
            // Single line comment
            while (position < source.length() && peek() != '\n') {
                advance();
            }
        } else if (source[position + 1] == '*') {
            // Multi-line comment
            advance(); // consume '/'
            advance(); // consume '*'
            while (position < source.length()) {
                if (peek() == '*' && position + 1 < source.length() && source[position + 1] == '/') {
                    advance(); // consume '*'
                    advance(); // consume '/'
                    break;
                }
                advance();
            }
        }
    }
}

Token Lexer::readNumber() {
    std::string number;
    int startColumn = column;
    
    while (position < source.length() && (std::isdigit(peek()) || peek() == '.')) {
        number += advance();
    }
    
    return Token(TOKEN_NUMBER, number, line, startColumn);
}

Token Lexer::readIdentifier() {
    std::string identifier;
    int startColumn = column;
    
    while (position < source.length() && (std::isalnum(peek()) || peek() == '_')) {
        identifier += advance();
    }
    
    TokenType type = keywords.find(identifier) != keywords.end() ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    return Token(type, identifier, line, startColumn);
}

Token Lexer::readString() {
    std::string str;
    int startColumn = column;
    
    advance(); // consume opening quote
    
    while (position < source.length() && peek() != '"') {
        if (peek() == '\\') {
            advance(); // consume backslash
            if (position < source.length()) {
                str += advance(); // consume escaped character
            }
        } else {
            str += advance();
        }
    }
    
    if (position < source.length()) {
        advance(); // consume closing quote
    }
    
    return Token(TOKEN_STRING, str, line, startColumn);
}

Token Lexer::readOperator() {
    std::string op;
    int startColumn = column;
    
    char current = peek();
    
    // Handle multi-character operators
    if (current == '+' && position + 1 < source.length() && source[position + 1] == '+') {
        op += advance(); // consume first '+'
        op += advance(); // consume second '+'
    } else if (current == '-' && position + 1 < source.length() && source[position + 1] == '-') {
        op += advance(); // consume first '-'
        op += advance(); // consume second '-'
    } else if (current == '=' && position + 1 < source.length() && source[position + 1] == '=') {
        op += advance(); // consume first '='
        op += advance(); // consume second '='
    } else if (current == '!' && position + 1 < source.length() && source[position + 1] == '=') {
        op += advance(); // consume '!'
        op += advance(); // consume '='
    } else if (current == '<' && position + 1 < source.length() && source[position + 1] == '<') {
        op += advance(); // consume first '<'
        op += advance(); // consume second '<'
    } else if (current == '>' && position + 1 < source.length() && source[position + 1] == '>') {
        op += advance(); // consume first '>'
        op += advance(); // consume second '>'
    } else if (current == '<' && position + 1 < source.length() && source[position + 1] == '=') {
        op += advance(); // consume '<'
        op += advance(); // consume '='
    } else if (current == '>' && position + 1 < source.length() && source[position + 1] == '=') {
        op += advance(); // consume '>'
        op += advance(); // consume '='
    } else if (current == '&' && position + 1 < source.length() && source[position + 1] == '&') {
        op += advance(); // consume first '&'
        op += advance(); // consume second '&'
    } else if (current == '|' && position + 1 < source.length() && source[position + 1] == '|') {
        op += advance(); // consume first '|'
        op += advance(); // consume second '|'
    } else {
        op += advance(); // consume single character operator
    }
    
    return Token(TOKEN_OPERATOR, op, line, startColumn);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (position < source.length()) {
        skipWhitespace();
        
        if (position >= source.length()) break;
        
        char current = peek();
        
        if (std::isdigit(current)) {
            tokens.push_back(readNumber());
        } else if (std::isalpha(current) || current == '_') {
            tokens.push_back(readIdentifier());
        } else if (current == '"') {
            tokens.push_back(readString());
        } else if (current == '/' && position + 1 < source.length() && 
                   (source[position + 1] == '/' || source[position + 1] == '*')) {
            skipComment();
        } else if (operators.find(current) != operators.end()) {
            tokens.push_back(readOperator());
        } else {
            // Handle delimiters and other characters
            std::string delimiter(1, current);
            tokens.push_back(Token(TOKEN_DELIMITER, delimiter, line, column));
            advance();
        }
    }
    
    tokens.push_back(Token(TOKEN_EOF, "", line, column));
    return tokens;
}

std::string Lexer::getTokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "Keyword";
        case TOKEN_IDENTIFIER: return "Identifier";
        case TOKEN_NUMBER: return "Number";
        case TOKEN_OPERATOR: return "Operator";
        case TOKEN_DELIMITER: return "Delimiter";
        case TOKEN_STRING: return "String";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "Unknown";
        default: return "Unknown";
    }
} 