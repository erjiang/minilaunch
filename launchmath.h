#ifndef LAUNCHMATH_H
#define LAUNCHMATH_H

#include <QString>

// Helper function declarations
bool isOperator(QChar c);
int precedence(QChar op);
double applyOp(double a, double b, QChar op);

// Main expression evaluation function
double evaluateExpression(const QString& expr);

#endif // LAUNCHMATH_H
