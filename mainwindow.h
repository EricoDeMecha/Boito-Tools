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
    // sqlite
    bool createConnection();
    void saveToDb(QStringList _items);
    void deleteDbTable();
    void pullUpDb();
    void saveTable();
    void closeEvent (QCloseEvent *event);
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
};
#endif // MAINWINDOW_H
