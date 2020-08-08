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
    ui->username_lineEdit->installEventFilter(this);
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
                    this->accept();
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
                    this->accept();
                }else{
                    QMessageBox::critical(nullptr,QObject::tr("Critical Error"),QObject::tr("Error inserting"), QMessageBox::Retry);
                }
            }
        }
    });
    connect(this, &QDialog::rejected,this,[](){
        qApp->exit();
    });
    connect(ui->delete_pushButton, SIGNAL(clicked()), this, SLOT(delete_identity()));
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

bool login::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == ui->username_lineEdit){
        if(ev->type() == QEvent::MouseButtonPress ||  ev->type() == QEvent::FocusIn){
            QStringList completerData = login_hash.keys();
            QCompleter *user_comp = new QCompleter(completerData);
            user_comp->setCaseSensitivity(Qt::CaseInsensitive);
            user_comp->setFilterMode(Qt::MatchContains);
            ui->username_lineEdit->setCompleter(user_comp);
        }
    }
    return false;
}

void login::delete_identity()
{
    QString username =ui->username_lineEdit->text();
    QString password = ui->password_lineEdit->text();
    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Username/Password field is empty"), QMessageBox::Retry);
    }else{
        if(login_hash.contains(username)){
            if(login_hash.value(username) == password){
                QSqlQuery q_delete(QSqlDatabase::database(conn_login));
                q_delete.prepare("DELETE FROM users WHERE username = ? AND password = ?");
                q_delete.bindValue(0,username);
                q_delete.bindValue(1, password);
                if(q_delete.exec()){
                    ui->username_lineEdit->clear();
                    ui->password_lineEdit->clear();
                    // also remove from hash
                    login_hash.remove(username);
                    return;
                }
            }else{
                QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Incorrect password"), QMessageBox::Retry);
                ui->password_lineEdit->clear();
            }
        }else{
            QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Username do not exist in the system.\n"
                                                                              "Register first to delete yourself"),QMessageBox::Retry);
        }
    }
}
login::~login()
{
    login_db.close();
    delete ui;
}
