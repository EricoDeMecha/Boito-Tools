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
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTimer>
#include <QDebug>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QTime>


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
    QString select_all = "SELECT * FROM Tools";
    QString select_all_partner = "";
    // sqlite
    bool createConnection();
    void saveToDb(QStringList _items);
    void deleteDbTable();
    void pullUpDb(QString  command , QString bind_value);
    void saveTable();
    /*main-application*/
    void closeEvent (QCloseEvent *event);
    void updateLabel(QString _type, QString _ref);
    /*From start up*/
    QString current_user;
    QStringList  engineers;
    QStringList tools;
    /*Let's create widgets*/
    void createComboWidget(QString status_string, int r);
    void tools_autoComplete(QString t_item,int r);
    void engineers_autoComplete(QString t_item,int r);

private slots:
    void mainHandler();
    void onCellClicked(int _row, int _col);
    void nameSearch();
    void toolSearch();
    void restoreDisplay();
};
#endif // MAINWINDOW_H
