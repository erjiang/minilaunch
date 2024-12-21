#include <QtTest>
#include "../units.h"

class UnitsTest : public QObject
{
    Q_OBJECT

private slots:
    void testIsUnitsConversion_ValidInput();
    void testIsUnitsConversion_InvalidInput();
    void testConvertUnits_BasicConversions();
};

void UnitsTest::testIsUnitsConversion_ValidInput()
{
    QVERIFY(isUnitsConversion("5km in miles"));
    QVERIFY(isUnitsConversion("10.5 meters in feet"));
    QVERIFY(isUnitsConversion("1.0kg in pounds"));
}

void UnitsTest::testIsUnitsConversion_InvalidInput()
{
    QVERIFY(!isUnitsConversion("hello world"));
    QVERIFY(!isUnitsConversion("5"));
    QVERIFY(!isUnitsConversion("km in miles"));
    QVERIFY(!isUnitsConversion("5km to miles"));
}

void UnitsTest::testConvertUnits_BasicConversions()
{
    // Test meter to feet conversion (approximately 3.28084 feet per meter)
    double result = convertUnits("1meter", "feet");
    QVERIFY(qAbs(result - 3.28084) < 0.0001);

    // Test kilometer to mile conversion (approximately 0.621371 miles per km)
    result = convertUnits("1km", "mile");
    QVERIFY(qAbs(result - 0.621371) < 0.0001);

    // Test kilogram to pound conversion (approximately 2.20462 pounds per kg)
    result = convertUnits("1kg", "pounds");
    QVERIFY(qAbs(result - 2.20462) < 0.0001);
}

QTEST_MAIN(UnitsTest)
#include "units_test.moc"
