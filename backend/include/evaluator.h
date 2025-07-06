#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <string>
#include <stack>
#include <map>
#include <vector>

class Evaluator {
private:
    std::map<std::string, double> variables;
    std::stack<double> operandStack;
    
    bool isOperator(const std::string& token);
    bool isNumber(const std::string& token);
    double applyOperator(const std::string& op, double a, double b);
    double evaluatePostfix(const std::vector<std::string>& postfix);

public:
    Evaluator();
    void setVariable(const std::string& name, double value);
    double evaluate(const std::vector<std::string>& postfix);
    std::map<std::string, double> getVariables();
    void clearVariables();
};

#endif 