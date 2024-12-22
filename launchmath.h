#ifndef LAUNCHMATH_H
#define LAUNCHMATH_H

#include <string>
#include <optional>

// Helper function declarations
bool isOperator(char c);
int precedence(char op);
double applyOp(double a, double b, char op);

// Main expression evaluation function
std::optional<double> evaluateExpression(const std::string& expression);

#endif // LAUNCHMATH_H
