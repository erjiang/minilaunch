#ifndef UNITS_H
#define UNITS_H

#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QProcess>
#include <QDebug>



/**
 * @brief Checks if a given text string represents a valid units conversion query
 * @param text The input string to check
 * @return true if the text represents a valid units conversion (e.g., "5kg in lbs"), false otherwise
 */
bool isUnitsConversion(const QString& text);

/**
 * @brief Converts between units using the 'units' command-line tool
 * @param fromUnit The source unit expression (e.g., "5kg")
 * @param toUnit The target unit (e.g., "lbs")
 * @return The converted value, or 0.0 if conversion fails
 */
double convertUnits(const QString& fromUnit, const QString& toUnit);

#endif // UNITS_H
