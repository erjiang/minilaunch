#include <QtTest>
#include "../launchmath.h"

class LaunchMathTest : public QObject
{
    Q_OBJECT

private slots:
    void testBasicOperations()
    {
        QCOMPARE(evaluateExpression("2 + 2"), 4.0);
        QCOMPARE(evaluateExpression("3 - 1"), 2.0);
        QCOMPARE(evaluateExpression("4 * 5"), 20.0);
        QCOMPARE(evaluateExpression("8 / 2"), 4.0);
        QCOMPARE(evaluateExpression("2 ^ 3"), 8.0);
    }

    void testComplexExpressions()
    {
        QCOMPARE(evaluateExpression("2 + 3 * 4"), 14.0);
        QCOMPARE(evaluateExpression("(2 + 3) * 4"), 20.0);
        QCOMPARE(evaluateExpression("2 * 3 + 4 * 5"), 26.0);
        QCOMPARE(evaluateExpression("(2 + 3 * 4) / 2"), 7.0);
    }

    void testDecimalNumbers()
    {
        QCOMPARE(evaluateExpression("1.5 + 2.5"), 4.0);
        QCOMPARE(evaluateExpression("3.14 * 2"), 6.28);
        QCOMPARE(evaluateExpression("10 / 2.5"), 4.0);
    }

    void testEdgeCases()
    {
        // Empty expression
        QCOMPARE(evaluateExpression(""), 0.0);

        // Single number
        QCOMPARE(evaluateExpression("42"), 42.0);

        // Division by zero
        QCOMPARE(evaluateExpression("5 / 0"), 0.0);

        // Extra spaces
        QCOMPARE(evaluateExpression("  2   +   2  "), 4.0);
    }

    void testNestedParentheses()
    {
        QCOMPARE(evaluateExpression("((2 + 3) * (4 + 5))"), 45.0);
        QCOMPARE(evaluateExpression("(1 + (2 * (3 + 4)))"), 15.0);
    }
};

QTEST_MAIN(LaunchMathTest)
#include "launchmath_test.moc"
