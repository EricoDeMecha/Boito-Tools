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
#include <QSqlQueryModel>
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
    void createComboWidget(QString status_string, int r);
    void saveTable();
    void closeEvent (QCloseEvent *event);

private slots:
    void mainHandler();
    void onCellClicked(int _row, int _col);
};
#endif // MAINWINDOW_H