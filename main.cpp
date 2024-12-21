#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QProcess>
#include <QScreen>
#include <QDebug>

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

    void initUI()
    {
        setWindowTitle("Launcher");
        setFixedSize(600, 80);

        QVBoxLayout *layout = new QVBoxLayout(this);

        entry = new QLineEdit(this);
        entry->setPlaceholderText("Enter command");
        connect(entry, &QLineEdit::returnPressed, this, &Launcher::runCommand);
        layout->addWidget(entry);

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
            QProcess *process = new QProcess(this);
            process->start(cmd);
            if (!process->waitForStarted())
            {
                qDebug() << "Failed to run command:" << process->errorString();
            }
        }
        close();
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
