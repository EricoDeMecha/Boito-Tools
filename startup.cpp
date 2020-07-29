#include "startup.h"
#include "ui_startup.h"


Startup::Startup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Startup)
{
    ui->setupUi(this);
    this->setWindowTitle("Start up Window");
    /*Create a DB connection*/
    if(!createMainConnection()){
        QApplication::quit();
    }
    /*Customize the engineers table*/
    // 1- >set a fixed row width
    /*Engineers table*/
    QHeaderView *eng_verticalHeader = ui->engineers_tableWidget->verticalHeader();
    eng_verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    eng_verticalHeader->setDefaultSectionSize(38);

    QTableWidgetItem *engineers_horizontalHeader = new QTableWidgetItem("Names");
    ui->engineers_tableWidget->setColumnCount(1);
    ui->engineers_tableWidget->setHorizontalHeaderItem(0,engineers_horizontalHeader);
   /*Tools table*/
    QHeaderView *tools_verticalHeader = ui->tools_tableWidget->verticalHeader();
    tools_verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    tools_verticalHeader->setDefaultSectionSize(38);

    QStringList tools_tableItems = {"Item","Number"};
    ui->tools_tableWidget->setColumnCount(tools_tableItems.size());
    int i = 0;
    foreach (QString item, tools_tableItems) {
        QTableWidgetItem *tools_horizontalHeader = new QTableWidgetItem(item);
        ui->tools_tableWidget->setHorizontalHeaderItem(i++,tools_horizontalHeader);
    }

    /*Pull the startup db*/
    pullUpStartDB();
    /*Add the autocompletion to user's lineEdi*/
    QCompleter *user_completer = new QCompleter(user_names);
    user_completer->setCaseSensitivity(Qt::CaseInsensitive);
    user_completer->setFilterMode(Qt::MatchContains);
    ui->user_lineEdit->setCompleter(user_completer);
    /*connect*/
    connect(ui->engineers_tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onEngineersCellClicked(int,int)));
    connect(ui->tools_tableWidget,SIGNAL(cellClicked(int, int)), this,SLOT(onToolsCellClicked(int,int)));

    connect(ui->buttonBox, SIGNAL(accepted()),this,SLOT(saveData()));
    connect(ui->buttonBox,SIGNAL(rejected()), this,SLOT(quitMain()));
}

bool Startup::createMainConnection()
{
    const QString DRIVER("QSQLITE");
    QSqlDatabase up_db = QSqlDatabase::addDatabase(DRIVER,"conn_start");
    up_db.setDatabaseName("up_db");
    if(!up_db.open()){
        QMessageBox::critical(nullptr, QObject::tr("Critical Error"),
                  QObject::tr("Could not find db file \n\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    return true;
}

void Startup::pullUpStartDB()
{
    ui->engineers_tableWidget->clearContents();
    ui->engineers_tableWidget->setRowCount(0);

    QSqlQuery engineers_count_query(QSqlDatabase::database("conn_start"));
    // count the number of rows in the db
    engineers_count_query.prepare("SELECT COUNT(1) FROM Engineers");
    int engineers_row_count = 0;
    if(engineers_count_query.exec() && engineers_count_query.seek(0)){
         engineers_row_count= engineers_count_query.value(0).toString().toInt();
    }
    ui->engineers_tableWidget->setRowCount((engineers_row_count + 1));// 4 Additional empty rows
    QSqlQuery engineers_query(QSqlDatabase::database("conn_start"));
    engineers_query.prepare("SELECT * FROM Engineers");
    if(engineers_query.exec()){
        int r = 0, c = 0;
        while(engineers_query.next()){
            ui->engineers_tableWidget->setItem(r,c, new QTableWidgetItem(engineers_query.value(0).toString()));
            r++;
        }
    }
    /*Tools*/
    ui->tools_tableWidget->clearContents();
    ui->tools_tableWidget->setRowCount(0);

    QSqlQuery tools_count_query(QSqlDatabase::database("conn_start"));
    // count the number of rows in the db
    tools_count_query.prepare("SELECT COUNT(1) FROM MainTools");
    int tools_row_count = 0;
    if(tools_count_query.exec() && tools_count_query.seek(0)){
         tools_row_count= tools_count_query.value(0).toString().toInt();
    }
    ui->tools_tableWidget->setRowCount((tools_row_count + 2));// 4 Additional empty rows

    QSqlQuery tool_query(QSqlDatabase::database("conn_start"));
    tool_query.prepare("SELECT * FROM MainTools");
    if(tool_query.exec()){
        int item_no = tool_query.record().indexOf("Items");
        int no_no = tool_query.record().indexOf("Num");
        int _r = 0 , _c0 = 0, _c1 = 1;
        while(tool_query.next()){
            ui->tools_tableWidget->setItem(_r,_c0,new QTableWidgetItem(tool_query.value(item_no).toString()));
            ui->tools_tableWidget->setItem(_r,_c1,new QTableWidgetItem(tool_query.value(no_no).toString()));
            _r++;
        }
    }
    /*Users*/
    QSqlQuery q_users(QSqlDatabase::database("conn_start"));
    q_users.prepare("SELECT Names FROM Users");
    if(q_users.exec()){
        while(q_users.next()){
            user_names << q_users.value(0).toString();
        }
        qDebug() << user_names;
    }
}

void Startup::onEngineersCellClicked(int r, int c)
{
    QTableWidgetItem *engineers_item = ui->engineers_tableWidget->item(r,c);
    if(!engineers_item ||  engineers_item->text().isEmpty()){
        int current_rowCount = ui->engineers_tableWidget->rowCount();
        ui->engineers_tableWidget->insertRow(current_rowCount);
    }
}

void Startup::onToolsCellClicked(int r, int c)
{
    if(c == 0){
        QTableWidgetItem *tools_item = ui->tools_tableWidget->item(r,c);
        if(!tools_item ||  tools_item->text().isEmpty()){
            int current_rowCount = ui->tools_tableWidget->rowCount();
            ui->tools_tableWidget->insertRow(current_rowCount);// row
        }
    }
}

void Startup::saveData()
{
    QSqlQuery  del_query(QSqlDatabase::database("conn_start"));
    del_query.exec("DELETE FROM MainTools");
    QSqlQuery  del_query_1(QSqlDatabase::database("conn_start"));
    del_query_1.exec("DELETE FROM Engineers");

    // Engineers
    for(int r = 0; r < ui->engineers_tableWidget->rowCount(); r++){
        QTableWidgetItem *item = ui->engineers_tableWidget->item(r,0);
        if(!item || item->text().isEmpty()){
            break;
        }
       QSqlQuery q_engineers(QSqlDatabase::database("conn_start"));
       q_engineers.prepare("INSERT INTO Engineers VALUES(?)");
       q_engineers.bindValue(0,ui->engineers_tableWidget->item(r,0)->text());
       q_engineers.exec();
    }

    // Tools
    for(int r = 0; r < ui->tools_tableWidget->rowCount();r++){
        QTableWidgetItem *item = ui->tools_tableWidget->item(r,0);
        if(!item || item->text().isEmpty()){
            break;
        }
        QSqlQuery q_tools(QSqlDatabase::database("conn_start"));
        q_tools.prepare("INSERT INTO MainTools VALUES(?,?)");
        q_tools.bindValue(0,ui->tools_tableWidget->item(r,0)->text());
        q_tools.bindValue(1, ui->tools_tableWidget->item(r,1)->text());
        q_tools.exec();
    }
    // Users Line Edit
    str_user = ui->user_lineEdit->text();
    if(str_user.isEmpty()){
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("User Line edit is empty"), QMessageBox::Cancel);
    }
}
void Startup::quitMain(){
    qApp->quit();
}
void Startup::closeEvent(QCloseEvent *event)
{
    event->accept();
    qApp->quit();
}
Startup::~Startup()
{
    delete ui;
}
