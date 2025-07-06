#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/evaluator.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cctype>

class ArithmeticEvaluator {
private:
    Lexer lexer;
    Parser parser;
    Evaluator evaluator;
    std::vector<std::string> lines;

    struct ExpressionInfo {
        std::string type;
        std::string var;
        std::string expr;
        std::string infix;
        std::string postfix;
        std::string result;
    };

public:
    ArithmeticEvaluator(const std::string& source) : lexer(source) {
        std::istringstream iss(source);
        std::string line;
        while (std::getline(iss, line)) {
            lines.push_back(line);
        }
    }

    void process() {
        std::cout << "\n=== Mini Arithmetic Expression Evaluator ===\n\n";
        
        // Step 1: Lexical Analysis
        std::cout << "Step 1: Lexical Analysis\n";
        std::cout << "========================\n";
        std::vector<Token> tokens = lexer.tokenize();
        
        std::cout << std::left << std::setw(20) << "Input Expression" 
                  << std::setw(15) << "Token Name" 
                  << std::setw(10) << "Line" << "\n";
        std::cout << std::string(45, '-') << "\n";
        
        for (const auto& token : tokens) {
            if (token.type == TOKEN_EOF) break;
            
            std::cout << std::left << std::setw(20) << token.value
                      << std::setw(15) << lexer.getTokenTypeName(token.type)
                      << std::setw(10) << token.line << "\n";
        }
        
        // Step 2: Parsing and Postfix Conversion
        std::cout << "\nStep 2: Parsing and Postfix Conversion\n";
        std::cout << "======================================\n";
        
        parser = Parser(tokens);
        std::vector<std::string> expression = parser.parse();
        std::string postfix = parser.getPostfixExpression();
        
        std::cout << "Infix Expression: ";
        for (const auto& token : expression) {
            std::cout << token << " ";
        }
        std::cout << "\n";
        
        std::cout << "Postfix Expression: " << postfix << "\n";
        
        // Step 3: Variable Analysis
        std::cout << "\nStep 3: Variable Analysis\n";
        std::cout << "=========================\n";
        
        auto variables = parser.getVariables();
        if (!variables.empty()) {
            std::cout << std::left << std::setw(15) << "Variable" 
                      << std::setw(15) << "Value" << "\n";
            std::cout << std::string(30, '-') << "\n";
            
            for (const auto& var : variables) {
                std::cout << std::left << std::setw(15) << var.first
                          << std::setw(15) << var.second << "\n";
            }
        } else {
            std::cout << "No variables found in the expression.\n";
        }
        
        // Step 4: Expression Evaluation
        std::cout << "\nStep 4: Expression Evaluation\n";
        std::cout << "==============================\n";
        
        // Process all expressions found in the input
        std::vector<std::string> allExpressions;
        std::istringstream inputStream(input);
        std::string line;
        while (std::getline(inputStream, line)) {
            if (line.find("=") != std::string::npos && 
                (line.find("int ") != std::string::npos || 
                 line.find("float ") != std::string::npos || 
                 line.find("double ") != std::string::npos || 
                 line.find("string ") != std::string::npos || 
                 line.find("char ") != std::string::npos)) {
                allExpressions.push_back(line);
            }
        }
        
        if (!allExpressions.empty()) {
            std::cout << "All Expressions Found:\n";
            std::cout << std::string(30, '-') << "\n";
            
            for (const auto& expr : allExpressions) {
                std::cout << expr << "\n";
            }
            
            std::cout << "\nExpression Results:\n";
            std::cout << std::string(30, '-') << "\n";
            
            // Set variables in evaluator
            for (const auto& var : variables) {
                try {
                    double value = std::stod(var.second);
                    evaluator.setVariable(var.first, value);
                } catch (const std::exception& e) {
                    // For string and char variables, display them as strings
                    if (var.second.find("\"") != std::string::npos || var.second.find("'") != std::string::npos) {
                        std::cout << var.first << " = " << var.second << " (string/char)\n";
                    } else {
                        std::cout << var.first << " = " << var.second << " (variable)\n";
                    }
                }
            }
            
            // Show arithmetic results
            if (!expression.empty()) {
                // Convert postfix string back to vector
                std::vector<std::string> postfixTokens;
                std::istringstream iss(postfix);
                std::string token;
                while (iss >> token) {
                    postfixTokens.push_back(token);
                }
                
                try {
                    double result = evaluator.evaluate(postfixTokens);
                    std::cout << "Arithmetic Result: " << result << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Expression: " << postfix << " (cannot evaluate)\n";
                }
            }
        } else {
            std::cout << "No expressions found to evaluate.\n";
        }
        
        std::cout << "\n" << std::string(50, '=') << "\n";
    }
};

int main() {
    std::cout << "Mini Arithmetic Expression Evaluator using C++\n";
    std::cout << "==============================================\n\n";
    
    std::cout << "Enter your C/C++ code (press Ctrl+D or Ctrl+Z when done):\n";
    std::cout << "Example:\n";
    std::cout << "int a = 5;\n";
    std::cout << "int b = 10;\n";
    std::cout << "float f = 2.5;\n";
    std::cout << "double d = 35.735;\n";
    std::cout << "string s = \"Hello\";\n";
    std::cout << "char c = 'A';\n";
    std::cout << "int sum = a + b * 2;\n\n";
    
    std::string input;
    std::string line;
    
    while (std::getline(std::cin, line)) {
        input += line + "\n";
    }
    
    if (input.empty()) {
        // Use default example if no input
        input = "int a = 5;\nint b = 10;\nfloat f = 2.5;\ndouble d = 35.735;\nstring s = \"Hello\";\nchar c = 'A';\nint sum = a + b;\nfloat product = f * a;\ndouble total = d + f + b;\nstring greet = s + \" World\";\ncout << sum << endl;\ncout << product << endl;\ncout << total << endl;\ncout << greet << endl;";
        std::cout << "Using default example:\n" << input << "\n";
    }
    
    try {
        ArithmeticEvaluator evaluator(input);
        evaluator.process();
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 