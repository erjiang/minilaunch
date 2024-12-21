#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QProcess>
#include <QScreen>
#include <QDebug>
#include <QLabel>
#include "launchmath.h"
#include "units.h"

class Launcher : public QWidget
{
    Q_OBJECT

public:
    Launcher(QWidget *parent = nullptr) : QWidget(parent)
    {
        initUI();
    }

private:
    QLineEdit *entry;
    QLabel *statusLabel;

    void initUI()
    {
        setWindowTitle("Launcher");
        setFixedSize(600, 80);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setAlignment(Qt::AlignTop);

        entry = new QLineEdit(this);
        entry->setPlaceholderText("Enter command or math expression");
        connect(entry, &QLineEdit::returnPressed, this, &Launcher::runCommand);
        connect(entry, &QLineEdit::textChanged, this, &Launcher::evaluateMath);
        layout->addWidget(entry);

        statusLabel = new QLabel(this);
        statusLabel->setText("Ready");
        statusLabel->hide();
        layout->addWidget(statusLabel);

        setLayout(layout);

        // Center the window on the screen
        QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }

    void runCommand()
    {
        QString cmd = entry->text().trimmed();
        if (!cmd.isEmpty())
        {
            QProcess *process = new QProcess(nullptr);
            process->start(cmd, QStringList());
            if (!process->waitForStarted())
            {
                qDebug() << "Failed to run command:" << process->errorString();
                delete process;
            }
        }
        close();
    }

    void evaluateMath(const QString& text)
    {
        if (text.isEmpty()) {
            statusLabel->hide();
            return;
        }

        // Check for units conversion
        if (isUnitsConversion(text)) {
            QStringList parts = text.split(" in ", Qt::SkipEmptyParts);
            QString fromUnit = parts.first().trimmed();
            QString toUnit = parts.last().trimmed();

            double conversionResult = convertUnits(fromUnit, toUnit);
            if (conversionResult != 0.0) {
                statusLabel->setText(QString::number(conversionResult));
                statusLabel->show();
                return;
            }
        }

        // Existing math evaluation logic
        bool hasOperator = text.contains(QRegExp("[+\\-*/^()]"));
        if (!hasOperator && !text.contains('.')) {
            statusLabel->hide();
            return;
        }

        double result = evaluateExpression(text);
        if (result != 0 || text == "0") {
            statusLabel->setText(QString::number(result));
            statusLabel->show();
        } else {
            statusLabel->hide();
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Launcher launcher;
    launcher.show();
    return app.exec();
}

#include "main.moc"
