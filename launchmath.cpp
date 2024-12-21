#include <QString>
#include <QStack>
#include <QChar>
#include <cmath>

// Helper function to check if character is an operator
bool isOperator(QChar c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Helper function to get operator precedence
int precedence(QChar op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

// Helper function to apply operator
double applyOp(double a, double b, QChar op) {
    switch (op.toLatin1()) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
        case '^': return pow(a, b);
    }
    return 0;
}

// Main function to evaluate arithmetic expression
double evaluateExpression(const QString& expr) {
    QStack<double> values;
    QStack<QChar> ops;

    QString sanitized = expr.simplified();

    for (int i = 0; i < sanitized.length(); i++) {
        if (sanitized[i] == ' ') continue;

        // If current char is a digit or decimal point
        if (sanitized[i].isDigit() || sanitized[i] == '.') {
            QString numStr;
            while (i < sanitized.length() &&
                   (sanitized[i].isDigit() || sanitized[i] == '.')) {
                numStr += sanitized[i];
                i++;
            }
            i--;
            values.push(numStr.toDouble());
        }

        // If current char is opening brace
        else if (sanitized[i] == '(') {
            ops.push(sanitized[i]);
        }

        // If current char is closing brace
        else if (sanitized[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.pop();
                double val1 = values.pop();
                QChar op = ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            if (!ops.empty()) {
                ops.pop(); // Remove opening brace
            }
        }

        // If current char is an operator
        else if (isOperator(sanitized[i])) {
            while (!ops.empty() && precedence(ops.top()) >= precedence(sanitized[i])) {
                double val2 = values.pop();
                double val1 = values.pop();
                QChar op = ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(sanitized[i]);
        }
    }

    // Process remaining operators
    while (!ops.empty()) {
        double val2 = values.pop();
        double val1 = values.pop();
        QChar op = ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    return values.empty() ? 0 : values.top();
}
