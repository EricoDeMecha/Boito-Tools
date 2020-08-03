#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/**Sql Database **/
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QComboBox>
#include <QDate>
#include <QCompleter>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTimer>
#include <QDebug>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QTextDocument>
#include <QTextTableCell>
#include <QPrinter>
#include <QDesktopServices>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    /*start-up stuff*/
    QStringList user_names;
    QSqlDatabase up_db;
    QString str_user;
    QStringList tool_names;
    QStringList engineers_names;
    QHash<QString, int> tools_audit;

    bool createMainConnection();
    void pullUpStartDB();
    /*container*/
    QHash<QString, int> pending_tools_audit;
    /*Print counter*/
    int i = 0;
    /*Queries*/
    QString select_all = "SELECT * FROM Tools";
    QString select_all_partner = "";
    // sqlite
    void checkForDrivers();
    void checkForDatabases();
    bool createConnection();
    void saveToDb(QStringList _items);
    void deleteDbTable();
    void pullUpDb(QString  command , QString bind_value);
    void saveTable();
    /*main-application*/
    void closeEvent (QCloseEvent *event);
    void updateLabel(QString _type, QString _ref);
    /*Let's create widgets*/
    void createComboWidget(QString status_string, int r);
    void tools_autoComplete(QString t_item,int r);
    void engineers_autoComplete(QString t_item,int r);
    /*MainWindow construct*/
    void constructMain();
private slots:
    void mainHandler();
    void onCellClicked(int _row, int _col);
    void nameSearch();
    void toolSearch();
    void restoreDisplay();
    void onPendingClicked();
    void onPrintClicked();
    void onAuditClicked();
    void onEngineersCellClicked(int,int);
    void onToolsCellClicked(int,int);
    void saveData();
    void quitMain();
};
#endif // MAINWINDOW_H
