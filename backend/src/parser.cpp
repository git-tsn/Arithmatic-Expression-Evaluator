#include "../include/parser.h"
#include <algorithm>
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {
    initializePrecedence();
}

void Parser::initializePrecedence() {
    precedence["+"] = 1;
    precedence["-"] = 1;
    precedence["*"] = 2;
    precedence["/"] = 2;
    precedence["%"] = 2;
    precedence["^"] = 3;
    precedence["("] = 0;
    precedence[")"] = 0;
}

bool Parser::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || 
           token == "%" || token == "^" || token == "(" || token == ")";
}

bool Parser::isOperand(const std::string& token) {
    // Check if token is a number or identifier
    if (token.empty()) return false;
    
    // If it's a number
    if (std::isdigit(token[0]) || token[0] == '.') return true;
    
    // If it's an identifier (starts with letter or underscore)
    if (std::isalpha(token[0]) || token[0] == '_') return true;
    
    return false;
}

int Parser::getPrecedence(const std::string& op) {
    auto it = precedence.find(op);
    return it != precedence.end() ? it->second : -1;
}

std::string Parser::infixToPostfix(const std::vector<std::string>& expression) {
    std::stack<std::string> operatorStack;
    std::vector<std::string> output;
    
    for (const auto& token : expression) {
        if (isOperand(token)) {
            output.push_back(token);
        } else if (token == "(") {
            operatorStack.push(token);
        } else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                output.push_back(operatorStack.top());
                operatorStack.pop();
            }
            if (!operatorStack.empty() && operatorStack.top() == "(") {
                operatorStack.pop(); // Remove '('
            }
        } else if (isOperator(token)) {
            while (!operatorStack.empty() && 
                   operatorStack.top() != "(" && 
                   getPrecedence(operatorStack.top()) >= getPrecedence(token)) {
                output.push_back(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(token);
        }
    }
    
    while (!operatorStack.empty()) {
        if (operatorStack.top() != "(") {
            output.push_back(operatorStack.top());
        }
        operatorStack.pop();
    }
    
    std::string result;
    for (size_t i = 0; i < output.size(); ++i) {
        if (i > 0) result += " ";
        result += output[i];
    }
    
    return result;
}

std::vector<std::string> Parser::extractExpression(const std::vector<Token>& tokens) {
    std::vector<std::string> expression;
    bool inExpression = false;
    
    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        
        if (token.type == TOKEN_EOF) break;
        
        // Look for assignment operator '='
        if (token.value == "=") {
            inExpression = true;
            continue;
        }
        
        // If we're in an expression, collect tokens until semicolon
        if (inExpression) {
            if (token.value == ";") {
                break;
            }
            
            // Skip certain tokens that aren't part of the expression
            if (token.type == TOKEN_KEYWORD && 
                (token.value == "int" || token.value == "float" || token.value == "double" || token.value == "char" || token.value == "string")) {
                continue;
            }
            
            expression.push_back(token.value);
        }
    }
    
    return expression;
}

std::vector<std::string> Parser::parse() {
    std::vector<std::string> expression = extractExpression(tokens);
    
    // Extract variable assignments
    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        
        if (token.type == TOKEN_EOF) break;
        
        // Look for variable declarations and assignments
        if (token.type == TOKEN_KEYWORD && 
            (token.value == "int" || token.value == "float" || token.value == "double" || token.value == "char" || token.value == "string")) {
            
            // Look for identifier after type
            if (i + 1 < tokens.size() && tokens[i + 1].type == TOKEN_IDENTIFIER) {
                std::string varName = tokens[i + 1].value;
                
                // Look for assignment
                if (i + 3 < tokens.size() && tokens[i + 2].value == "=") {
                    if (tokens[i + 3].type == TOKEN_NUMBER) {
                        variables[varName] = tokens[i + 3].value;
                    } else if (tokens[i + 3].type == TOKEN_STRING) {
                        variables[varName] = tokens[i + 3].value;
                    } else if (tokens[i + 3].type == TOKEN_IDENTIFIER) {
                        // Variable assignment like a = b
                        variables[varName] = tokens[i + 3].value;
                    } else {
                        variables[varName] = "0"; // Default value
                    }
                } else {
                    variables[varName] = "0"; // Default value
                }
            }
        }
    }
    
    return expression;
}

std::string Parser::getPostfixExpression() {
    std::vector<std::string> expression = parse();
    return infixToPostfix(expression);
}

std::map<std::string, std::string> Parser::getVariables() {
    return variables;
} 