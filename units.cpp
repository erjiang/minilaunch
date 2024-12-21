#include "units.h"
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QProcess>
#include <QDebug>

// Define the function with the correct return type
bool isUnitsConversion(const QString& text)
{
    if (!text.contains(" in ", Qt::CaseInsensitive)) {
        return false;
    }

    QString leftPart = text.split(" in ", Qt::SkipEmptyParts).first().trimmed();
    QRegExp numberPattern("^\\s*[-+]?\\d*\\.?\\d+\\s*[a-zA-Z]+$");
    return numberPattern.exactMatch(leftPart);
}

// Define the function with the correct return type
double convertUnits(const QString& fromUnit, const QString& toUnit)
{
    QProcess process;
    QStringList arguments;
    arguments << fromUnit << toUnit;

    process.start("units", arguments);
    if (!process.waitForFinished()) {
        qDebug() << "Failed to run units command:" << process.errorString();
        return 0.0;
    }

    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    QString firstLine = output.split('\n').first().trimmed();

    if (!firstLine.startsWith('*')) {
        qDebug() << "Unexpected output from units command:" << firstLine;
        return 0.0;
    }

    bool ok;
    double value = firstLine.mid(1).trimmed().toDouble(&ok);
    return ok ? value : 0.0;
}
