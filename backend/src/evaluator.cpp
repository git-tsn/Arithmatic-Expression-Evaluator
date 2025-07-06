#include "../include/evaluator.h"
#include <sstream>
#include <cmath>
#include <iostream>

Evaluator::Evaluator() {}

void Evaluator::setVariable(const std::string& name, double value) {
    variables[name] = value;
}

bool Evaluator::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || 
           token == "%" || token == "^";
}

bool Evaluator::isNumber(const std::string& token) {
    if (token.empty()) return false;
    
    // Check if it's a number
    std::istringstream iss(token);
    double num;
    iss >> std::noskipws >> num;
    return iss.eof() && !iss.fail();
}

double Evaluator::applyOperator(const std::string& op, double a, double b) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0) {
            std::cerr << "Error: Division by zero!" << std::endl;
            return 0;
        }
        return a / b;
    }
    if (op == "%") {
        if (b == 0) {
            std::cerr << "Error: Modulo by zero!" << std::endl;
            return 0;
        }
        return static_cast<int>(a) % static_cast<int>(b);
    }
    if (op == "^") return std::pow(a, b);
    
    std::cerr << "Error: Unknown operator '" << op << "'" << std::endl;
    return 0;
}

double Evaluator::evaluatePostfix(const std::vector<std::string>& postfix) {
    std::stack<double> operandStack;
    
    for (const auto& token : postfix) {
        if (isNumber(token)) {
            // Convert string to number
            std::istringstream iss(token);
            double num;
            iss >> num;
            operandStack.push(num);
        } else if (variables.find(token) != variables.end()) {
            // It's a variable, push its value
            operandStack.push(variables[token]);
        } else if (isOperator(token)) {
            // It's an operator, pop operands and apply
            if (operandStack.size() < 2) {
                std::cerr << "Error: Not enough operands for operator '" << token << "'" << std::endl;
                return 0;
            }
            
            double b = operandStack.top(); operandStack.pop();
            double a = operandStack.top(); operandStack.pop();
            
            double result = applyOperator(token, a, b);
            operandStack.push(result);
        } else {
            std::cerr << "Error: Unknown token '" << token << "'" << std::endl;
            return 0;
        }
    }
    
    if (operandStack.size() != 1) {
        std::cerr << "Error: Invalid expression - too many operands" << std::endl;
        return 0;
    }
    
    return operandStack.top();
}

double Evaluator::evaluate(const std::vector<std::string>& postfix) {
    return evaluatePostfix(postfix);
}

std::map<std::string, double> Evaluator::getVariables() {
    return variables;
}

void Evaluator::clearVariables() {
    variables.clear();
} 