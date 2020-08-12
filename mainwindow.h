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
    /*          SQLs           */
    const QString DRIVER = "QSQLITE";
    const QString tools_db = "Tools";
    const QString entries_db = "Entries";
    const QString consumables_db = "Consumables";
    const QString login_db = "Login";
    /*common functions */
    QLineEdit* createSectionDropDown();

    /*   PAGE  0 (entries)*/
    const QString conn_entries = "conn_entries";
    QStringList tool_names;
    QStringList consumable_names;
    QSqlDatabase up_db;
    QHash<QString, int> tools_audit;
    // funcs
    bool createEntriesConnection();
    void pullUpEntriesDB();
    void createTypeCombo(int r, int _type);
    /* PAGE 1 (tools)*/
    QString str_user;
    QStringList engineers_names;
    const QString conn_tools = "conn_tools";
    QSqlDatabase db_tools;

    bool createToolsConnection();
    void pullUpToolsDb(QString  command , QString bind_value);
    void createComboWidget(QString status_string, int r);
    void tools_autoComplete(QString t_item,int r);
    void engineers_autoComplete(QString t_item,int r);
    void saveToolsToDb(QStringList _items);
    void deleteToolsTable();
    void saveToolsTable();
    void updateToolsLabel(QString _type, QString _ref);

    /*           PAGE 2            */
    const QString conn_consumables = "conn_consumables";
    QHash<QString, int> consumables_audit;
    QHash<QString, int> cons_fromDB;
    QVector<QString>  SINs;
    bool connectConsumablesDb();
    QSqlDatabase cons_db;
    void pullUpConsumables(QString command, QStringList bind_values);
    void consumableItem(QString item,  int r);
    void consumableReceiver(QString item, int r);
    void saveConsumables();
    void saveConsumablesToDb(QStringList db_items);

    /*start-up stuff*/
    QStringList user_names;


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

    /*       Event handler         */
    void closeEvent (QCloseEvent *event);
private slots:
    /*          PAGE 0       */
    void onEngineersCellClicked(int,int);
    void onToolsCellClicked(int,int);
    void saveEntries();

    /*          PAGE 1      */
    void onToolCellClicked(int _row, int _col);
    void toolSearch();
    void restoreDisplay();
    void onPendingClicked();
    void onPrintClicked();
    void onAuditClicked();

   /*           PAGE 2         */
    void consumablesHandler();
    void onConsumableCellClicked(int , int );
    void itemSearch();
    void restoreConsDisp();
    void printCons();
    void auditCons();
};
#endif // MAINWINDOW_H
