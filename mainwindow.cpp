#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startup.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    //QTimer::singleShot(0, this, SLOT(showFullScreen()));
    /*Create a database connection*/
    // 1 -> create connection(check for drivers)
    if(!createConnection()){
        qApp->quit();
    }
    /*Customize the Table Widget*/
    // 1- >set a fixed row width
    QHeaderView *verticalHeader = ui->main_tableWidget->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(38);
    // 2 -> add the column headers
    QStringList horizontalHeaderItems = {"Date","Item", "No.","Section","Name","Sign","Status", "Condition","Issued By","Received By"};
    ui->main_tableWidget->setColumnCount(horizontalHeaderItems.size());
    int i = 0;
    foreach (QString item, horizontalHeaderItems) {
        QTableWidgetItem *horizontalHeader = new QTableWidgetItem(item);
        ui->main_tableWidget->setHorizontalHeaderItem(i++,horizontalHeader);
    }
    // 3-> Pull up Db and update the table
    pullUpDb();
    // just here, get the start up window showing
    QTimer::singleShot(0, qApp,[this](){
        Startup up_run;
        up_run.setModal(true);
        up_run.exec();
        current_user = up_run.str_user;
        engineers = up_run.engineers_names;
        tools = up_run.tool_names;
    });
    // 4-> Insert Widget to status if item is clicked and is empty
    connect(ui->main_tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onCellClicked(int,int)));

    // save is manual
    connect(ui->save_pushButton,SIGNAL(clicked()),this, SLOT(mainHandler()));
}
void MainWindow::onCellClicked(int _row, int _col){
    QTableWidgetItem *item = ui->main_tableWidget->item(_row,_col);
    if(!item || item->text().isEmpty()){
        if((_col != 7) || (_col != 0) || (_col != 8) || (_col != 9)){
            if(_col == 7){
                createComboWidget("Returned", _row);
            }else if(_col == 0){
                // add today's date to column 0 of the new row
                QString today = QDate::currentDate().toString("d/M/yyyy");
                ui->main_tableWidget->setItem(_row,_col, new QTableWidgetItem(today));
                // update others
                createComboWidget("Pending", _row);
                tools_autoComplete(QString(""), _row);
                engineers_autoComplete(QString(""),_row);
            }else if (_col == 8) {
                ui->main_tableWidget->setItem(_row,_col, new QTableWidgetItem(current_user));
            }else if(_col == 9){
                ui->main_tableWidget->setItem(_row,_col, new QTableWidgetItem(current_user));
            }
        }
    }
}
bool MainWindow::createConnection(){
    const QString DRIVER("QSQLITE");
    if(!QSqlDatabase::isDriverAvailable(DRIVER)){
        QMessageBox::critical(nullptr, QObject::tr("Critical Error"),
                  QObject::tr("Unable to establish a database connection.\n"
                              "This project needs SQLite support. Please read "
                              "the Qt SQL driver documentation for information how "
                              "to build it.\n\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER,"conn_main");
    db.setDatabaseName("db.db");
    if(!db.open()){
        QMessageBox::critical(nullptr, QObject::tr("Critical Error"),
                              QObject::tr("Unable to open the database \n"
                                          "Make sure the database file is contained in the project root directory"),
                              QMessageBox::Cancel);
        return false;
    }
    return true;
}

void MainWindow::pullUpDb(){
    // clear grounds
    ui->main_tableWidget->clearContents();
    ui->main_tableWidget->setRowCount(0);

    QSqlQuery count_query(QSqlDatabase::database("conn_main"));
    // count the number of rows in the db
    count_query.prepare("SELECT COUNT(1) FROM Tools");
    int row_count = 0;
    if(count_query.exec() && count_query.seek(0)){
         row_count= count_query.value(0).toString().toInt();
    }
    ui->main_tableWidget->setRowCount((row_count + 4));// 4 Additional empty rows
    // Bring up all the data to table
    QSqlQuery pull_query(QSqlDatabase::database("conn_main"));
    pull_query.prepare("SELECT * FROM Tools");
    if(pull_query.exec()){
        int date_no = pull_query.record().indexOf("Date");
        int item_no = pull_query.record().indexOf("Item");
        int no_no = pull_query.record().indexOf("No");
        int section_no = pull_query.record().indexOf("Section");
        int name_no = pull_query.record().indexOf("Name");
        int sign_no = pull_query.record().indexOf("Sign");
        int status_no = pull_query.record().indexOf("Status");
        int condition_no = pull_query.record().indexOf("Condition");
        int issued_no = pull_query.record().indexOf("Issued_by");
        int received_no = pull_query.record().indexOf("Received_by");
        int r = 0;
        while(pull_query.next()){
            QStringList db_items;
            db_items << pull_query.value(date_no).toString();
            db_items << pull_query.value(item_no).toString();
            db_items << pull_query.value(no_no).toString();
            db_items << pull_query.value(section_no).toString();
            db_items << pull_query.value(name_no).toString();
            db_items << pull_query.value(sign_no).toString();
            db_items << pull_query.value(status_no).toString();
            db_items << pull_query.value(condition_no).toString();
            db_items << pull_query.value(issued_no).toString();
            db_items << pull_query.value(received_no).toString();

            for(int c = 0; c < ui->main_tableWidget->columnCount(); c++){
                if(c == 6){
                    // insert the widget
                    createComboWidget(db_items[c],r);
                }else if(c == 1){
                    tools_autoComplete(db_items[c], r);
                }else if(c == 4){
                    engineers_autoComplete(db_items[c], r);
                }else{
                    ui->main_tableWidget->setItem(r,c, new QTableWidgetItem(db_items[c]));
                }
            }
            r++;
        }
    }
}
void MainWindow::createComboWidget(QString status_string, int r){
    QComboBox *combo = new QComboBox();
    if(status_string == "Returned"){
        combo->addItem(status_string);
        combo->addItem("Pending");
    }else{
        combo->addItem(status_string);
        combo->addItem("Returned");
    }
    ui->main_tableWidget->setCellWidget(r,6,combo);
}

void MainWindow::tools_autoComplete(QString t_item,int r)
{
    QLineEdit *tools_edit = new QLineEdit();
    QCompleter *tool_completer = new QCompleter(tools);
    tool_completer->setCaseSensitivity(Qt::CaseInsensitive);
    tool_completer->setFilterMode(Qt::MatchContains);
    tools_edit->setCompleter(tool_completer);
    tools_edit->setText(t_item);
    ui->main_tableWidget->setCellWidget(r,1,tools_edit);
}

void MainWindow::engineers_autoComplete(QString t_item,int r)
{
    QLineEdit *engineer_edit = new QLineEdit();
    QCompleter *engineer_completer = new QCompleter(engineers);
    engineer_completer->setCaseSensitivity(Qt::CaseInsensitive);
    engineer_completer->setFilterMode(Qt::MatchContains);
    engineer_edit->setCompleter(engineer_completer);
    engineer_edit->setText(t_item);
    ui->main_tableWidget->setCellWidget(r,4,engineer_edit);
}

void MainWindow::mainHandler(){
    deleteDbTable();// delete db table before saving new
    saveTable();// saves the table
    pullUpDb();// update the table again
}

void MainWindow::saveTable(){
    bool _break = false;
    for(int r = 0; r < ui->main_tableWidget->rowCount(); r++){
        QStringList db_items;
        for(int c = 0; c < ui->main_tableWidget->columnCount(); c++){
            QTableWidgetItem *item = ui->main_tableWidget->item(r,c);
            QString  item_string;
            if(c == 0 && (!item || item->text().isEmpty())){
                _break = true;
                break;
            }
            if(c == 6){
                QComboBox *myCB = qobject_cast<QComboBox*>(ui->main_tableWidget->cellWidget(r,6));
                item_string  = myCB->currentText();
                db_items << item_string;
            }else if(c == 1){
                QLineEdit *tool_item = qobject_cast<QLineEdit*>(ui->main_tableWidget->cellWidget(r,1));
                item_string = tool_item->text();
                db_items << item_string;
            }else if(c == 4){
                QLineEdit *engineer_item = qobject_cast<QLineEdit*>(ui->main_tableWidget->cellWidget(r,4));
                item_string = engineer_item->text();
                db_items << item_string;
            }else{
                if(!item || item->text().isEmpty())
                    ui->main_tableWidget->setItem(r,c,new QTableWidgetItem(QString("")));
                item_string = ui->main_tableWidget->item(r,c)->text();
                db_items << item_string;
            }
        }
        if(_break)
            break;
        saveToDb(db_items);
    }
}
void MainWindow::saveToDb(QStringList _items){
     QSqlQuery entry_query(QSqlDatabase::database("conn_main"));
     QString command = "INSERT INTO Tools VALUES(?,?,?,?,?,?,?,?,?,?)";
     entry_query.prepare(command);
     // binding
     for(int i = 0; i < _items.size(); i++){
         entry_query.bindValue(i,_items[i]);
     }
     if(entry_query.exec()){
         updateLabel("Success", "save=success");
     }else{
         updateLabel("Error",entry_query.lastError().text());
         }
}
void MainWindow::updateLabel(QString _type, QString _ref)
{
    QTime ct = QTime::currentTime();
    QString _time = ct.toString("hh:mm:ss.zzz");
    QString _lastSave = "last saved";
    ui->statusbar->showMessage(_ref);

    QString lbl_text = _time + " : " + _lastSave+ "<"+_type+"> ;(StatusBar)";
    ui->save_label->setText(lbl_text);
}
void MainWindow::deleteDbTable(){
           QSqlQuery  del_query(QSqlDatabase::database("conn_main"));
           del_query.exec("DELETE FROM Tools");
}

void MainWindow::resizeEvent(QResizeEvent *event){
    int size = 10;// No of the header items
    ui->main_tableWidget->setColumnWidth(0, this->width()/size);
    ui->main_tableWidget->setColumnWidth(1, this->width()/size);
    ui->main_tableWidget->setColumnWidth(2, this->width()/size);
    ui->main_tableWidget->setColumnWidth(3, this->width()/size);
    ui->main_tableWidget->setColumnWidth(4, this->width()/size);
    ui->main_tableWidget->setColumnWidth(5, this->width()/size);
    ui->main_tableWidget->setColumnWidth(6, this->width()/size);
    ui->main_tableWidget->setColumnWidth(7, this->width()/size);
    ui->main_tableWidget->setColumnWidth(8, this->width()/size);
    ui->main_tableWidget->setColumnWidth(9, this->width()/size);

    QMainWindow::resizeEvent(event);
}
void MainWindow::closeEvent (QCloseEvent *event){
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Boito Tools",
                                                                tr("Are you sure you want to close the application?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        deleteDbTable();// delete ...
        saveTable();// and save table before exit
        event->accept();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

