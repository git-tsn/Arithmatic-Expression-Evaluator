#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stack>
#include <vector>
#include <string>

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;
    
    std::map<std::string, int> precedence;
    std::map<std::string, std::string> variables;
    
    void initializePrecedence();
    bool isOperator(const std::string& token);
    bool isOperand(const std::string& token);
    int getPrecedence(const std::string& op);
    std::string infixToPostfix(const std::vector<std::string>& expression);
    std::vector<std::string> extractExpression(const std::vector<Token>& tokens);

public:
    Parser(const std::vector<Token>& tokens);
    std::vector<std::string> parse();
    std::string getPostfixExpression();
    std::map<std::string, std::string> getVariables();
};

#endif 