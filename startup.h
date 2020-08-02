#ifndef STARTUP_H
#define STARTUP_H

#include <QDialog>
/*sql database*/
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QMessageBox>
#include <QDebug>
#include <QCloseEvent>
#include <QTimer>
#include <QCompleter>

namespace Ui {
class Startup;
}

class Startup : public QDialog
{
    Q_OBJECT

public:
    explicit Startup(QWidget *parent = nullptr);
    QString str_user;
    QStringList tool_names;
    QStringList engineers_names;

    QHash<QString, int> tools_audit;
    ~Startup();

private:
    Ui::Startup *ui;
    QStringList user_names;
    void pullUpStartDB();
    bool createMainConnection();
    void closeEvent (QCloseEvent *event);
    void makeAvailable();
private slots:
    void onEngineersCellClicked(int,int);
    void onToolsCellClicked(int,int);
    void saveData();
    void quitMain();
};

#endif // STARTUP_H
