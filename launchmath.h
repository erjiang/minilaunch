#ifndef LAUNCHMATH_H
#define LAUNCHMATH_H

#include <string>

// Helper function declarations
bool isOperator(char c);
int precedence(char op);
double applyOp(double a, double b, char op);

// Main expression evaluation function
double evaluateExpression(const std::string& expr);

#endif // LAUNCHMATH_H
