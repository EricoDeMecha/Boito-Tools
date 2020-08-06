#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    if(!loginDbConnection()){
        qApp->quit();
    }
    // prepare data
    login_data();
    // connect
    QStringList completerData = login_hash.keys();
    QCompleter *user_comp = new QCompleter(completerData);
    user_comp->setCaseSensitivity(Qt::CaseInsensitive);
    user_comp->setFilterMode(Qt::MatchContains);
    ui->username_lineEdit->setCompleter(user_comp);

    connect(ui->login_pushButton, &QPushButton::clicked,this,[this](){
        QString username =ui->username_lineEdit->text();
        QString password = ui->password_lineEdit->text();
        if(username.isEmpty() || password.isEmpty()){
            QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Username/Password field is empty"), QMessageBox::Retry);
        }else{
            if(login_hash.contains(username)){
                QString temp_password = login_hash.value(username);
                if(temp_password == password){
                    shared_user = username;
                    this->close();
                }else{
                    QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Incorrect password"), QMessageBox::Retry);
                    ui->password_lineEdit->clear();
                }
            }else{
                QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Incorrect username"), QMessageBox::Retry);
                ui->username_lineEdit->clear();
             }
        }
    });
    connect(ui->register_pushButton,&QPushButton::clicked,this,[this](){
        QString username =ui->username_lineEdit->text();
        QString password = ui->password_lineEdit->text();
        if(username.isEmpty() || password.isEmpty()){
            QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Username/Password field is empty"), QMessageBox::Retry);
        }else{
            if(login_hash.contains(username)){
                QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Username already exists"), QMessageBox::Retry);
                ui->username_lineEdit->clear();
            }else{
                QSqlQuery q_insert(QSqlDatabase::database(conn_login));
                q_insert.prepare("INSERT INTO users(username, password)VALUES(?,?)");
                q_insert.bindValue(0, username);
                q_insert.bindValue(1,password);
                if(q_insert.exec()){
                    shared_user = username;
                    this->close();
                }else{
                    QMessageBox::critical(nullptr,QObject::tr("Critical Error"),QObject::tr("Error inserting"), QMessageBox::Retry);
                }
            }
        }
    });
    /*connect(this, &QDialog::rejected,this,[](){
        qApp->exit();
    });*/
}
bool login::loginDbConnection(){
    const QString DRIVER("QSQLITE");
    login_db = QSqlDatabase::addDatabase(DRIVER,conn_login);
    login_db.setDatabaseName("Login");
    if(!login_db.open()){
        QMessageBox::critical(nullptr, QObject::tr("Critical Error"),
                  QObject::tr("Could not find login DB file \n\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    return true;
}
void login::login_data(){
        QSqlQuery q_login(QSqlDatabase::database(conn_login));
        q_login.prepare("SELECT username, password FROM users");
        if(q_login.exec()){
            while(q_login.next()){
                QString username = q_login.value(0).toString();
                QString password = q_login.value(1).toString();
                login_hash[username] = password;
            }
        }
}
login::~login()
{
    login_db.close();
    delete ui;
}
