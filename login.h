#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
/**Sql Database **/
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QMessageBox>
#include <QDebug>
#include <QCompleter>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    QString shared_user;
    ~login();

private:
    Ui::login *ui;
    QHash<QString, QString> login_hash;
    QSqlDatabase login_db;
    const QString conn_login = "conn_login";

    bool loginDbConnection();
    void login_data();
    bool eventFilter(QObject *, QEvent *);
private slots:
    void  delete_identity();
};

#endif // LOGIN_H
