#include <string>
#include <stack>
#include <cmath>
#include <cctype>

// Helper function to check if character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Helper function to get operator precedence
int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

// Helper function to apply operator
double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
        case '^': return pow(a, b);
    }
    return 0;
}

// Main function to evaluate arithmetic expression
double evaluateExpression(const std::string& expr) {
    std::stack<double> values;
    std::stack<char> ops;

    // Remove whitespace (simplified version)
    std::string sanitized;
    for (char c : expr) {
        if (!std::isspace(c)) sanitized += c;
    }

    for (size_t i = 0; i < sanitized.length(); i++) {
        // If current char is a digit or decimal point
        if (std::isdigit(sanitized[i]) || sanitized[i] == '.') {
            std::string numStr;
            while (i < sanitized.length() &&
                   (std::isdigit(sanitized[i]) || sanitized[i] == '.')) {
                numStr += sanitized[i];
                i++;
            }
            i--;
            values.push(std::stod(numStr));
        }

        // If current char is opening brace
        else if (sanitized[i] == '(') {
            ops.push(sanitized[i]);
        }

        // If current char is closing brace
        else if (sanitized[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            if (!ops.empty()) {
                ops.pop(); // Remove opening brace
            }
        }

        // If current char is an operator
        else if (isOperator(sanitized[i])) {
            while (!ops.empty() && precedence(ops.top()) >= precedence(sanitized[i])) {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(sanitized[i]);
        }
    }

    // Process remaining operators
    while (!ops.empty()) {
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    return values.empty() ? 0 : values.top();
}
