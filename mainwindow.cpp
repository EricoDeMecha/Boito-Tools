#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QFileDialog>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    /*Check for sqlite drivers and database files */
    checkForDrivers();
    checkForDatabases();

    // initialize db variables
    up_db = QSqlDatabase::addDatabase(DRIVER,conn_entries);
    up_db.setDatabaseName(entries_db);
    db_tools  = QSqlDatabase::addDatabase(DRIVER,conn_tools);
    db_tools.setDatabaseName(tools_db);
    cons_db = QSqlDatabase::addDatabase(DRIVER,conn_consumables);
    cons_db.setDatabaseName(consumables_db);

    /*###########             Page 0              #########*/

    ui->stackedWidget->setCurrentIndex(0);
    /*Do the stuff related to the first page*/

    /*Connect to the Up_db*/
    if(!createEntriesConnection()){
        qApp->quit();
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
    ui->engineers_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   /*Tools table*/
    QHeaderView *tools_verticalHeader = ui->tools_tableWidget->verticalHeader();
    tools_verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    tools_verticalHeader->setDefaultSectionSize(38);

    QStringList tools_tableItems = {"Item","Number","Type"};
    ui->tools_tableWidget->setColumnCount(tools_tableItems.size());
    int i = 0;
    foreach (QString item, tools_tableItems) {
        QTableWidgetItem *tools_horizontalHeader = new QTableWidgetItem(item);
        ui->tools_tableWidget->setHorizontalHeaderItem(i++,tools_horizontalHeader);
    }
    ui->tools_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    /*Pull the startup db*/
    pullUpEntriesDB();
    /*connect*/
    connect(ui->engineers_tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onEngineersCellClicked(int,int)));
    connect(ui->tools_tableWidget,SIGNAL(cellClicked(int, int)), this,SLOT(onToolsCellClicked(int,int)));
    connect(ui->savep0_pushButton, SIGNAL(clicked()),this,SLOT(saveEntries()));
    connect(ui->consumablep0_pushButton, &QPushButton::clicked, this, [this](){
        QMessageBox::information(nullptr, QObject::tr("Information"),QObject::tr("Please save items in this page before changing page\n"
                                                                                 "This will make them available to you later in a drop down"),
                                 QMessageBox::Ok);
        ui->stackedWidget->setCurrentIndex(2);
        if(!cons_db.isOpen()){
            if(!connectConsumablesDb()){
                qApp->quit();
            }
            QString command = "SELECT * FROM consumables";
            pullUpConsumables(command, QStringList{});
        }
    });
    connect(ui->nonconp0_pushButton, &QPushButton::clicked, this, [this](){
        QMessageBox::information(nullptr, QObject::tr("Information"),QObject::tr("Please save items in this page before changing page\n"
                                                                                 "This will make them available to you later in a drop down"),
                                 QMessageBox::Ok);
        ui->stackedWidget->setCurrentIndex(1);
        if(!db_tools.isOpen()){
            if(!createToolsConnection()){
                qApp->quit();
            }
            pullUpToolsDb(select_all, select_all_partner);
        }
    });
    /*###########             Page 1              #########*/


    /*Customize the Table Widget*/
    // 1- >set a fixed row width
    QHeaderView *verticalHeader = ui->main_tableWidget->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(38);
    // 2 -> add the column headers
    QStringList horizontalHeaderItems = {"Date","Item", "No.","Section","Name","Sign","Status", "Condition","Issued By","Received By"};
    ui->main_tableWidget->setColumnCount(horizontalHeaderItems.size());
    int _i = 0;
    foreach (QString item, horizontalHeaderItems) {
        QTableWidgetItem *horizontalHeader = new QTableWidgetItem(item);
        ui->main_tableWidget->setHorizontalHeaderItem(_i++,horizontalHeader);
    }
    // 4-> Insert Widget to status if cell is clicked and is empty
    connect(ui->main_tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onToolCellClicked(int,int)));

    // save is manual
    connect(ui->save_pushButton,&QPushButton::clicked,this, [this](){
        deleteToolsTable();
        saveToolsTable();
        pullUpToolsDb(select_all,select_all_partner);
    });
    connect(ui->restore1_pushButton,SIGNAL(clicked()),this,SLOT(restoreDisplay()));
    // search
    QStringList tool_items = {"Date","Item","Section","Name","Issued_by"};
    ui->specific_comboBox->addItems(tool_items);
    // Search with tools
    connect(ui->search_lineEdit,SIGNAL(editingFinished()),this,SLOT(toolSearch()));
    // pending button
    connect(ui->pending_pushButton, SIGNAL(clicked()), this, SLOT(onPendingClicked()));
    // print Doc
    connect(ui->print_pushButton, SIGNAL(clicked()), this, SLOT(onPrintClicked()));
    // audit tools
    connect(ui->audit_pushButton, SIGNAL(clicked()), this, SLOT(onAuditClicked()));
    // Home1 button
    connect(ui->home1_pushButton, &QPushButton::clicked, this, [this](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    // Consumables button
    connect(ui->consumable_pushButton, &QPushButton::clicked, this, [this](){
        ui->stackedWidget->setCurrentIndex(2);
        if(!cons_db.isOpen()){
            if(!connectConsumablesDb()){
                qApp->quit();
            }
            QString command = "SELECT * FROM consumables";
            pullUpConsumables(command, QStringList{});
        }
    });

    /*###########             Page 2              #########*/

    // home2 button
    connect(ui->home2_pushButton, &QPushButton::clicked, this, [this](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    // consumable table widget
    QStringList consumable_headers = {"S/N","Date","Item","No/Quantity","Section","Issued to","Sign","Issued by"};
    ui->consumable_tableWidget->setColumnCount(consumable_headers.size());
    ui->consumable_tableWidget->setHorizontalHeaderLabels(consumable_headers);
    ui->consumable_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // save  button
    connect(ui->save2_pushButton, SIGNAL(clicked()),this,SLOT(consumablesHandler()));
    // search
    QStringList fields = {"SIN","Date", "Item","Section","Issued_to","Issued_by"};
    ui->field_comboBox->addItems(fields);
    connect(ui->item_lineEdit, SIGNAL(editingFinished()), this, SLOT(itemSearch()));
    connect(ui->restore2_pushButton, SIGNAL(clicked()), this,SLOT(restoreConsDisp()));
    // non consumables
    connect(ui->noncon_pushButton, &QPushButton::clicked, this, [this](){
        ui->stackedWidget->setCurrentIndex(1);
        if(!db_tools.isOpen()){
            if(!createToolsConnection()){
                qApp->quit();
            }
            pullUpToolsDb(select_all, select_all_partner);
        }
    });
    // consumable cell click signal
    connect(ui->consumable_tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onConsumableCellClicked(int , int)));
    // consumable print clicked
    connect(ui->printcons_pushButton, SIGNAL(clicked()), this, SLOT(printCons()));
    // consumables audit clicked
    connect(ui->auditcons_pushButton, SIGNAL(clicked()), this , SLOT(auditCons()));
    /*#######################        Pop-up Window for LOGIN           ############################*/
    QTimer::singleShot(0,qApp,[this](){
        login lg;
        lg.setWindowTitle("Login");
        lg.setModal(true);
        lg.exec();
        str_user = lg.shared_user;
    });

}
/*******************            MAIN                         ************************/
void MainWindow::checkForDrivers()
{
    const QString DRIVER("QSQLITE");
    if(!QSqlDatabase::isDriverAvailable(DRIVER)){
        QMessageBox::critical(nullptr, QObject::tr("Critical Error"),
                  QObject::tr("Unable to establish a database connection.\n"
                              "This project needs SQLite support. Please read "
                              "the Qt SQL driver documentation for information how "
                              "to build it.\n\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
    }
}
void MainWindow::checkForDatabases()
{
    if(!QFile::exists(tools_db)){
        // create tools_db
        //tools_db
        QSqlDatabase tools = QSqlDatabase::addDatabase(DRIVER, conn_tools);
        tools.setDatabaseName(tools_db);
        tools.open();
        QSqlQuery q_tools(QSqlDatabase::database(conn_tools));
        q_tools.exec("CREATE TABLE Tools(Date TEXT, Item TEXT,No TEXT, Section TEXT,"
                     " Name TEXT, Sign TEXT, Status TEXT, Condition TEXT,Issued_by TEXT, Received_by TEXT)");
        tools.close();
    }
    if(!QFile::exists(entries_db)){
        // entries_db
        QSqlDatabase data_container = QSqlDatabase::addDatabase(DRIVER, conn_entries);
        data_container.setDatabaseName(entries_db);
        data_container.open();

        QStringList  entries_tables_commands = {"CREATE TABLE Engineers (Names TEXT)",
                                               "CREATE TABLE Tools (Item TEXT, Num TEXT, Type TEXT)"};
        for(int i = 0; i < entries_tables_commands.size(); i++){
         QSqlQuery q_entries(QSqlDatabase::database(conn_entries));
         q_entries.exec(entries_tables_commands[i]);
        }
        data_container.close();
    }
    if(!QFile::exists(consumables_db)){
        //consumables_db
        QSqlDatabase db_consumables = QSqlDatabase::addDatabase(DRIVER, conn_consumables);
        db_consumables.setDatabaseName(consumables_db);
        db_consumables.open();
        QSqlQuery q_consum(QSqlDatabase::database(conn_consumables));
        q_consum.exec("CREATE TABLE consumables(SIN TEXT, Date TEXT, Item TEXT,No TEXT, Section TEXT,"
                     " Issued_to TEXT, Sign TEXT,Issued_by TEXT)");
        db_consumables.close();
     }
    if(!QFile::exists(login_db)){
        // login db
        const QString conn1_login = "conn_login";
        QSqlDatabase db_login = QSqlDatabase::addDatabase(DRIVER, conn1_login);
        db_login.setDatabaseName(login_db);
        db_login.open();

        QSqlQuery q_login(QSqlDatabase::database(conn1_login));
        q_login.exec("CREATE TABLE users(username TEXT, password TEXT)");
        db_login.close();
    }
}
/****************                              PAGE 0                            ******************/
bool MainWindow::createEntriesConnection()
{
    const QString DRIVER("QSQLITE");
    if(!up_db.open()){
        QMessageBox::critical(nullptr, QObject::tr("Critical Error"),
                  QObject::tr("Could not find db file \n\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    return true;
}
void MainWindow::pullUpEntriesDB()
{
    ui->engineers_tableWidget->clearContents();
    ui->engineers_tableWidget->setRowCount(0);

    QSqlQuery engineers_query(QSqlDatabase::database(conn_entries));
    engineers_query.prepare("SELECT * FROM Engineers");
    if(engineers_query.exec()){
        int r = 0;
        while(engineers_query.next()){
            ui->engineers_tableWidget->insertRow(r);
            ui->engineers_tableWidget->setItem(r,0, new QTableWidgetItem(engineers_query.value(0).toString()));
            r++;
        }
        // add 1 more row
        ui->engineers_tableWidget->insertRow(ui->engineers_tableWidget->rowCount());
    }
    /*Tools*/
    ui->tools_tableWidget->clearContents();
    ui->tools_tableWidget->setRowCount(0);

    QSqlQuery tool_query(QSqlDatabase::database(conn_entries));
    tool_query.prepare("SELECT * FROM Tools");
    if(tool_query.exec()){
        int item_no = tool_query.record().indexOf("Item");
        int num_no = tool_query.record().indexOf("Num");
        int type_no = tool_query.record().indexOf("Type");
        int _r = 0;
        while(tool_query.next()){
            ui->tools_tableWidget->insertRow(_r);
            ui->tools_tableWidget->setItem(_r,0,new QTableWidgetItem(tool_query.value(item_no).toString()));
            ui->tools_tableWidget->setItem(_r,1,new QTableWidgetItem(tool_query.value(num_no).toString()));
            if(tool_query.value(type_no).toString() == "Non-Consumable"){
                createTypeCombo(_r, 1);
            }else{
                createTypeCombo(_r,0);
            }
            _r++;
        }
        // add 1 more row
        ui->tools_tableWidget->insertRow(ui->tools_tableWidget->rowCount());
        createTypeCombo(_r, 1);
    }
}
void MainWindow::createTypeCombo(int r, int _type){
    QStringList _types = {"Consumable", "Non-Consumable"};
    if(_type == 1){
        _types.swap(0,1);
    }
    QComboBox *combo_Types =  new QComboBox();
    combo_Types->addItems(_types);
    ui->tools_tableWidget->setCellWidget(r,2,combo_Types);
}

void MainWindow::onEngineersCellClicked(int r, int c)
{
    QTableWidgetItem *engineers_item = ui->engineers_tableWidget->item(r,c);
    if(!engineers_item ||  engineers_item->text().isEmpty()){
        int current_rowCount = ui->engineers_tableWidget->rowCount();
        ui->engineers_tableWidget->insertRow(current_rowCount);
    }
}

void MainWindow::onToolsCellClicked(int r, int c)
{
    if(c == 0){
        QTableWidgetItem *tools_item = ui->tools_tableWidget->item(r,c);
        if(!tools_item ||  tools_item->text().isEmpty()){
            int current_rowCount = ui->tools_tableWidget->rowCount();
            ui->tools_tableWidget->insertRow(current_rowCount);// row
            createTypeCombo(current_rowCount,1);
        }
    }
}

void MainWindow::saveEntries()
{
    QSqlQuery  del_query(QSqlDatabase::database(conn_entries));
    del_query.exec("DELETE FROM Tools");
    QSqlQuery  del_query_1(QSqlDatabase::database(conn_entries));
    del_query_1.exec("DELETE FROM Engineers");
    // Engineers
    for(int r = 0; r < ui->engineers_tableWidget->rowCount(); r++){
        QTableWidgetItem *item = ui->engineers_tableWidget->item(r,0);
        if(!item || item->text().isEmpty()){
            break;
        }
       QSqlQuery q_engineers(QSqlDatabase::database(conn_entries));
       q_engineers.prepare("INSERT INTO Engineers VALUES(?)");
       QString engineer_name = ui->engineers_tableWidget->item(r,0)->text();
       engineers_names << engineer_name; // Store for mainWindow
       q_engineers.bindValue(0,engineer_name);
       q_engineers.exec();
    }
    // Tools
    for(int r = 0; r < ui->tools_tableWidget->rowCount();r++){
        QTableWidgetItem *item = ui->tools_tableWidget->item(r,0);
        if(!item || item->text().isEmpty()){
            break;
        }
        QSqlQuery q_tools(QSqlDatabase::database(conn_entries));
        q_tools.prepare("INSERT INTO Tools(Item,Num,Type) VALUES(?,?,?)");
        QString tool_name = ui->tools_tableWidget->item(r,0)->text();
        QString tool_no = ui->tools_tableWidget->item(r,1)->text();
        QComboBox *item_type = qobject_cast<QComboBox*>(ui->tools_tableWidget->cellWidget(r,2));
        QString  str_item_type = item_type->currentText();
        if(tool_no.toInt()){
            int tool_noInt  = tool_no.toInt();
             if(str_item_type == "Non-Consumable"){
                 if(tools_audit.contains(tool_name)){
                     int val = tools_audit.value(tool_name);
                     tool_noInt += val;
                 }
                 tools_audit[tool_name] = tool_noInt;
             }else{
                 if(consumables_audit.contains(tool_name)){
                     int val = consumables_audit.value(tool_name);
                     tool_noInt += val;
                 }
                 consumables_audit[tool_name] = tool_noInt;
             }
        }else{
            QMessageBox::warning(nullptr,QObject::tr("Warning"),
                                 QObject::tr("\n Some of your tool numbers are not actually numbers \n"
                                             "I am gonna continue but some functionality is gonna be deducted"), QMessageBox::Cancel);
        }
        if(str_item_type == "Non-Consumable"){
            tool_names << tool_name; // store for mainWindow;
        }else{
            consumable_names << tool_name;
        }
        q_tools.bindValue(0,tool_name);
        q_tools.bindValue(1, tool_no);
        q_tools.bindValue(2, str_item_type);
        if(q_tools.exec()){
            QTime ct = QTime::currentTime();
            QString _time = ct.toString("hh:mm:ss.zzz");
            QString _lastSave = "last saved";
            ui->statusbar->showMessage("Save=Success");

            QString lbl_text = _time + " : " + _lastSave+ "<"+"success"+"> ;(StatusBar)";
            ui->savestat_label->setText(lbl_text);
        }else{
            QTime ct = QTime::currentTime();
            QString _time = ct.toString("hh:mm:ss.zzz");
            QString _lastSave = "last saved";
            ui->statusbar->showMessage(q_tools.lastError().text());

            QString lbl_text = _time + " : " + _lastSave+ "<"+"Error"+"> ;(StatusBar)";
            ui->savestat_label->setText(lbl_text);
        }
    }
}
/*********************************              PAGE 1              ************************************************/
bool MainWindow::createToolsConnection(){
    const QString DRIVER("QSQLITE");
    if(!db_tools.open()){
        QMessageBox::critical(nullptr, QObject::tr("Critical Error"),
                              QObject::tr("Unable to open the database \n"
                                          "Make sure the database file is contained in the project root directory"),
                              QMessageBox::Cancel);
        return false;
    }
    return true;
}
void MainWindow::pullUpToolsDb(QString  command , QString bind_value){
    // clear grounds
    ui->main_tableWidget->clearContents();
    ui->main_tableWidget->setRowCount(0);
    // Bring up all the data to table
    QSqlQuery pull_query(QSqlDatabase::database(conn_tools));
    pull_query.prepare(command);
    if(!bind_value.isEmpty()){
        pull_query.bindValue(0,bind_value);
    }else{
        ui->main_tableWidget->setRowCount(4);
    }
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
            ui->main_tableWidget->insertRow(ui->main_tableWidget->rowCount());//increase rows each by one
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
                }else if(c == 3){
                    QLineEdit *EditSection = createSectionDropDown();
                    ui->main_tableWidget->setCellWidget(r,3,EditSection);
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
    QCompleter *tool_completer = new QCompleter(tool_names);
    tool_completer->setCaseSensitivity(Qt::CaseInsensitive);
    tool_completer->setFilterMode(Qt::MatchContains);
    tools_edit->setCompleter(tool_completer);
    tools_edit->setText(t_item);
    ui->main_tableWidget->setCellWidget(r,1,tools_edit);
}
void MainWindow::engineers_autoComplete(QString t_item,int r)
{
    QLineEdit *engineer_edit = new QLineEdit();
    QCompleter *engineer_completer = new QCompleter(engineers_names);
    engineer_completer->setCaseSensitivity(Qt::CaseInsensitive);
    engineer_completer->setFilterMode(Qt::MatchContains);
    engineer_edit->setCompleter(engineer_completer);
    engineer_edit->setText(t_item);
    ui->main_tableWidget->setCellWidget(r,4,engineer_edit);
}
void MainWindow::onToolCellClicked(int _row, int _col){
    QTableWidgetItem *item = ui->main_tableWidget->item(_row,_col);
    if(!item || item->text().isEmpty()){
        if((_col != 7) || (_col != 0) || (_col != 8) || (_col != 9) || (_col != 3)){
            if(_col == 7){
                createComboWidget("Returned", _row);
            }else if(_col == 0){
                // add today's date to column 0 of the new row
                QString today = QDate::currentDate().toString("d/M/yyyy");
                ui->main_tableWidget->setItem(_row,_col, new QTableWidgetItem(today));
                // then add a new row
                int current_rowCount = ui->main_tableWidget->rowCount();
                ui->main_tableWidget->insertRow(current_rowCount);
                // update others
                createComboWidget("Pending", _row);
                tools_autoComplete(QString(""), _row);
                engineers_autoComplete(QString(""),_row);
                ui->main_tableWidget->setCellWidget(_row,3,createSectionDropDown());
            }else if (_col == 8) {
                ui->main_tableWidget->setItem(_row,_col, new QTableWidgetItem(str_user));
            }else if(_col == 9){
                ui->main_tableWidget->setItem(_row,_col, new QTableWidgetItem(str_user));
            }
        }
    }
}
void MainWindow::deleteToolsTable(){
           QSqlQuery  del_query(QSqlDatabase::database(conn_tools));
           del_query.exec("DELETE FROM Tools");
}
void MainWindow::saveToolsTable(){
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
            }else if(c == 3){
                QLineEdit *section = qobject_cast<QLineEdit *>(ui->main_tableWidget->cellWidget(r,3));
                item_string = section->text();
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
        saveToolsToDb(db_items);
    }
}
void MainWindow::saveToolsToDb(QStringList _items){
     QSqlQuery entry_query(QSqlDatabase::database(conn_tools));
     QString command = "INSERT INTO Tools VALUES(?,?,?,?,?,?,?,?,?,?)";
     entry_query.prepare(command);
     // binding
     for(int i = 0; i < _items.size(); i++){
         entry_query.bindValue(i,_items[i]);
     }
     if(entry_query.exec()){
         updateToolsLabel("Success", "save=success");
     }else{
         updateToolsLabel("Error",entry_query.lastError().text());
         }
}
void MainWindow::updateToolsLabel(QString _type, QString _ref)
{
    QTime ct = QTime::currentTime();
    QString _time = ct.toString("hh:mm:ss.zzz");
    QString _lastSave = "last saved";
    ui->statusbar->showMessage(_ref);

    QString lbl_text = _time + " : " + _lastSave+ "<"+_type+"> ;(StatusBar)";
    ui->save_label->setText(lbl_text);
}
void MainWindow::toolSearch()
{
     QString field_item = ui->specific_comboBox->currentText();
     QString  search_item = ui->search_lineEdit->text();
    if(!search_item.isEmpty()){
        if(ui->save_pushButton->isEnabled()){
            QString command_line = "SELECT * FROM Tools WHERE "+ field_item + " = ?";
            pullUpToolsDb(command_line, search_item);
            ui->save_pushButton->setEnabled(false);
        }else{
            QList<QTableWidgetItem *> search_results = ui->main_tableWidget->findItems(search_item,Qt::MatchEndsWith);
            foreach(QTableWidgetItem *item, search_results){
                item->setBackground(Qt::blue);
            }
        }
    }
}
void MainWindow::restoreDisplay()
{
    pullUpToolsDb(select_all,select_all_partner);
    ui->save_pushButton->setEnabled(true);
}
void MainWindow::onPendingClicked()
{
    QString bind_value = "Pending";
    QString command = "SELECT * FROM Tools WHERE Status = ? ";
    ui->save_pushButton->setEnabled(false);
    pullUpToolsDb(command, bind_value);
}

void MainWindow::onPrintClicked()
{
    const int t_columns = ui->main_tableWidget->columnCount();
    const int t_rows = ui->main_tableWidget->rowCount();
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount(1);
    tableFormat.setAlignment(Qt::AlignHCenter);
    tableFormat.setCellPadding(0);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(Qt::SolidPattern);
    tableFormat.clearColumnWidthConstraints();
    QTextTable *textTable = cursor.insertTable(t_rows +1 , t_columns, tableFormat);
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground(QColor("#DADADA"));
    for(int i = 0; i < t_columns; i++){
        QTextTableCell cell = textTable->cellAt(0,i);
        cell.setFormat(tableHeaderFormat);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText(ui->main_tableWidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString());
    }
    bool _break = false;
    for(int r = 0; r < t_rows; r++){
        QString item_string;
        for(int c = 0; c < t_columns; c++){
            QTableWidgetItem *item = ui->main_tableWidget->item(r,c);
            if(c == 0 && (!item || item->text().isEmpty())){
                _break = true;
                break;
            }
            if(c == 6){
                QComboBox *myCB = qobject_cast<QComboBox*>(ui->main_tableWidget->cellWidget(r,6));
                item_string  = myCB->currentText();
            }else if(c == 1){
                QLineEdit *tool_item = qobject_cast<QLineEdit*>(ui->main_tableWidget->cellWidget(r,1));
                item_string = tool_item->text();
            }else if(c == 3){
                QLineEdit *section = qobject_cast<QLineEdit *>(ui->main_tableWidget->cellWidget(r,3));
                item_string = section->text();
            }else if(c == 4){
                QLineEdit *engineer_item = qobject_cast<QLineEdit*>(ui->main_tableWidget->cellWidget(r,4));
                item_string = engineer_item->text();
            }else{
                if(!item || item->text().isEmpty())
                    ui->main_tableWidget->setItem(r,c,new QTableWidgetItem(QString("")));
                item_string = ui->main_tableWidget->item(r,c)->text();
            }
            QTextTableCell cell = textTable->cellAt(r+1, c);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(item_string);
        }
        if(_break)
            break;
    }
    cursor.movePosition(QTextCursor::End);
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    QString directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("Browse..."),QDir::currentPath()));
    QString  output_fileName = directory+ "/" + "Printed.pdf";
    if(QFile::exists(output_fileName)){
        i++;
        output_fileName =directory+ "/" + "Printed_"+ QString::number(i) +".pdf";
    }
    printer.setOutputFileName(output_fileName);
    doc.setDocumentMargin(0);
    doc.setTextWidth(5);
    doc.print(&printer);
    QDesktopServices::openUrl(QUrl::fromLocalFile(output_fileName));
}
void MainWindow::onAuditClicked()
{
    QString pending  = "Pending";
    QSqlQuery aq_tools(QSqlDatabase::database(conn_tools));
    aq_tools.prepare("SELECT Item, No FROM Tools WHERE Status = ? ");
    aq_tools.bindValue(0,pending);
    if(aq_tools.exec()){
        while(aq_tools.next()){
            QString item = aq_tools.value(0).toString();
            QString item_no = aq_tools.value(1).toString();
            if(item_no.toInt()){
                int tool_noInt  = item_no.toInt();
                 if(pending_tools_audit.contains(item)){
                     int val = pending_tools_audit.value(item);
                     tool_noInt += val;
                 }
                 pending_tools_audit[item] = tool_noInt;
            }else{
                QMessageBox::warning(nullptr,QObject::tr("Warning"),
                                     QObject::tr("\n Some of your tool numbers are not actually numbers \n"
                                                 "I am gonna continue but some functionality is gonna be deducted"), QMessageBox::Cancel);
            }
        }
    }
    // Set up a dialog  window
    QTableWidget *tbl = new QTableWidget();
    QStringList headerItems = {"Tool/Item","Remaining No."};
    tbl->setColumnCount(headerItems.size());
    tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tbl->setHorizontalHeaderLabels(headerItems);
    // iterate through HashTables for get items numbers differences
    QHashIterator<QString, int> hash_iterator(pending_tools_audit);
    while(hash_iterator.hasNext()){
        hash_iterator.next();
        QString pending_item = hash_iterator.key();
        int pending_value = hash_iterator.value();
        if(tools_audit.contains(pending_item)){
             int org_val =  tools_audit.value(pending_item);
             tools_audit[pending_item] =(org_val - pending_value);
        }
    }
    // as of now the org tools have the updated list
    QHashIterator<QString, int>h_iter(tools_audit);
    int _r = 0;
    while(h_iter.hasNext()){
        h_iter.next();
        tbl->insertRow(_r);
        tbl->setItem(_r,0,new QTableWidgetItem(h_iter.key()));
        int _val = h_iter.value();
        if(_val < 3){
            QTableWidgetItem *val_item = new QTableWidgetItem(QString::number(_val));
            val_item->setBackground(Qt::red);
            tbl->setItem(_r, 1 , val_item);
        }else{
            QTableWidgetItem *val_item2 = new QTableWidgetItem(QString::number(_val));
            val_item2->setBackground(Qt::green);
            tbl->setItem(_r, 1, val_item2);
        }
        _r++;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(tbl);
    QDialog *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setLayout(vbox);
    dialog->exec();
}

/*###############                           Page 2                                     #######################*/
bool MainWindow::connectConsumablesDb(){
    const QString DRIVER("QSQLITE");
    if(!cons_db.open()){
        QMessageBox::critical(nullptr, QObject::tr("Critical Error"),
                  QObject::tr("Could not find consumable DB file \n\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    return true;
}
void MainWindow::pullUpConsumables(QString command, QStringList bind_values){
    // clear  grounds
    ui->consumable_tableWidget->clearContents();
    ui->consumable_tableWidget->setRowCount(0);
    SINs.clear();
    // query
    QSqlQuery q_consum(QSqlDatabase::database(conn_consumables));
    q_consum.prepare(command);
    if(!bind_values.empty()){
        const int Num = bind_values.size();
        for(int i = 0; i < Num; i++){
            q_consum.bindValue(i,bind_values[i]);
        }
    }else{
        ui->consumable_tableWidget->setRowCount(3);
    }

    if(q_consum.exec()){
        int sin_no = q_consum.record().indexOf("SIN");
        int date_no = q_consum.record().indexOf("Date");
        int item_no = q_consum.record().indexOf("Item");
        int no_no = q_consum.record().indexOf("No");
        int section_no = q_consum.record().indexOf("Section");
        int receiver_no = q_consum.record().indexOf("Issued_to");
        int sign_no = q_consum.record().indexOf("Sign");
        int issued_no = q_consum.record().indexOf("Issued_by");
        int r = 0;
        while(q_consum.next()){
            ui->consumable_tableWidget->insertRow(ui->consumable_tableWidget->rowCount());//increase rows each by one
            QStringList db_items;
            QString  temp_sin = q_consum.value(sin_no).toString();
            SINs.push_back(temp_sin);
            db_items << temp_sin;
            db_items << q_consum.value(date_no).toString();
            db_items << q_consum.value(item_no).toString();
            db_items << q_consum.value(no_no).toString();
            db_items << q_consum.value(section_no).toString();
            db_items << q_consum.value(receiver_no).toString();
            db_items << q_consum.value(sign_no).toString();
            db_items << q_consum.value(issued_no).toString();

            for(int c = 0; c < ui->consumable_tableWidget->columnCount(); c++){
               if(c == 2){
                   consumableItem(db_items[c], r);
               }else if(c == 4){
                   QLineEdit *cellWid = createSectionDropDown();
                   cellWid->setText(db_items[c]);
                   ui->consumable_tableWidget->setCellWidget(r,4,cellWid);
               }else if(c == 5){
                   consumableReceiver(db_items[c],r);
               }else{
                   ui->consumable_tableWidget->setItem(r,c, new QTableWidgetItem(db_items[c]));
               }
            }
            r++;
        }
    }
}
void MainWindow::consumableItem(QString item, int r){
    QLineEdit *consumable_edit = new QLineEdit();
    QCompleter *consumable_completer = new QCompleter(consumable_names);
    consumable_completer->setCaseSensitivity(Qt::CaseInsensitive);
    consumable_completer->setFilterMode(Qt::MatchContains);
    consumable_edit->setCompleter(consumable_completer);
    consumable_edit->setText(item);
    ui->consumable_tableWidget->setCellWidget(r,2,consumable_edit);
}
void MainWindow::consumableReceiver(QString item, int r){
    QLineEdit *consumable_edit = new QLineEdit();
    QCompleter *consumable_completer = new QCompleter(engineers_names);
    consumable_completer->setCaseSensitivity(Qt::CaseInsensitive);
    consumable_completer->setFilterMode(Qt::MatchContains);
    consumable_edit->setCompleter(consumable_completer);
    consumable_edit->setText(item);
    ui->consumable_tableWidget->setCellWidget(r,5,consumable_edit);
}
void MainWindow::consumablesHandler(){
    saveConsumables();
    QString command = "SELECT * FROM consumables";
    pullUpConsumables(command, QStringList{});
}
void MainWindow::saveConsumables(){
    QSqlQuery q_consum_1(QSqlDatabase::database(conn_consumables));
    q_consum_1.exec("DELETE FROM consumables");

    bool _break = false;
    for(int r = 0; r < ui->consumable_tableWidget->rowCount(); r++){
        QStringList db_items;
        for(int c = 0; c < ui->consumable_tableWidget->columnCount(); c++){
            QTableWidgetItem *item = ui->consumable_tableWidget->item(r,c);
            QString  item_string;
            if((c == 1)&& (!item || item->text().isEmpty())){
                _break = true;
                break;
            }
            if(c == 2){
                QLineEdit *consumable_item = qobject_cast<QLineEdit*>(ui->consumable_tableWidget->cellWidget(r,2));
                item_string = consumable_item->text();
                db_items << item_string;
            }else if(c == 4){
                QLineEdit *section = qobject_cast<QLineEdit *>(ui->consumable_tableWidget->cellWidget(r,4));
                item_string = section->text();
                db_items << item_string;
            }else if(c == 5){
                QLineEdit *engineer_item = qobject_cast<QLineEdit*>(ui->consumable_tableWidget->cellWidget(r,5));
                item_string = engineer_item->text();
                db_items << item_string;
            }else{
                if(!item || item->text().isEmpty())
                    ui->consumable_tableWidget->setItem(r,c,new QTableWidgetItem(QString("")));
                item_string = ui->consumable_tableWidget->item(r,c)->text();
                db_items << item_string;
            }
        }
        if(_break)
            break;
        saveConsumablesToDb(db_items);
    }
}
void MainWindow::saveConsumablesToDb(QStringList db_items){
    QSqlQuery entry_query(QSqlDatabase::database(conn_consumables));
    QString command = "INSERT INTO consumables VALUES(?,?,?,?,?,?,?,?)";
    entry_query.prepare(command);
    // binding
    for(int i = 0; i < db_items.size(); i++){
        entry_query.bindValue(i,db_items[i]);
    }
    // Constants
    QTime ct = QTime::currentTime();
    QString _time = ct.toString("hh:mm:ss.zzz");
    QString _lastSave = "last saved";

    if(entry_query.exec()){
        ui->statusbar->showMessage("save=success");

        QString lbl_text = _time + " : " + _lastSave+ "<" + "Success" + "> ;(StatusBar)";
        ui->status2_label->setText(lbl_text);
    }else{
        ui->statusbar->showMessage(entry_query.lastError().text());

        QString lbl_text = _time + " : " + _lastSave+ "<" + "Error" + "> ;(StatusBar)";
        ui->status2_label->setText(lbl_text);
        }
}
void MainWindow::onConsumableCellClicked(int r, int c){
    QTableWidgetItem *item = ui->consumable_tableWidget->item(r,c);
    if(!item || item->text().isEmpty()){
        if(c == 1){
            // set other cell widgets
            // add today's date to column 0 of the new row
            QString today = QDate::currentDate().toString("d/M/yyyy");
            ui->consumable_tableWidget->setItem(r,1, new QTableWidgetItem(today));
            // item
            consumableItem(QString(""), r);
            consumableReceiver(QString(""), r);
            // insert item
            ui->consumable_tableWidget->setItem(r, 7, new QTableWidgetItem(str_user));
            //  insert section
            ui->consumable_tableWidget->setCellWidget(r,4,createSectionDropDown());
            // insert a new row
            ui->consumable_tableWidget->insertRow(ui->consumable_tableWidget->rowCount());
        }
        if(c == 0){
            QString sin_str;
            if(!SINs.isEmpty()){
                QString  temp_Sin = SINs.last();
                //S/N-001
                int sin_num = temp_Sin.right(3).toInt();
                sin_num += 1;
                sin_str = QString::number(sin_num);
                for(int i = 0; i <= (4 - sin_str.length());i++){
                    sin_str.prepend('0');
                }
                sin_str.prepend("S/N-");
            }else{
                sin_str = "S/N-001";
            }
            ui->consumable_tableWidget->setItem(r,0, new QTableWidgetItem(sin_str));
        }
    }
}
void MainWindow::itemSearch(){
    QString field_item = ui->field_comboBox->currentText();
    QString field_item_rel = ui->item_lineEdit->text();
    QStringList bind_values = {field_item_rel};

    QString command = "SELECT * FROM consumables WHERE " + field_item + "= ?";
    pullUpConsumables(command, bind_values);
    ui->save2_pushButton->setEnabled(false);
}
void MainWindow::restoreConsDisp(){
    QString command = "SELECT * FROM consumables";
    pullUpConsumables(command, QStringList{});
    ui->save2_pushButton->setEnabled(true);
}
void MainWindow::printCons(){
    const int t_columns = ui->consumable_tableWidget->columnCount();
    const int t_rows = ui->consumable_tableWidget->rowCount();
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount(1);
    tableFormat.setAlignment(Qt::AlignHCenter);
    tableFormat.setCellPadding(0);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(Qt::SolidPattern);
    tableFormat.clearColumnWidthConstraints();
    QTextTable *textTable = cursor.insertTable(t_rows +1 , t_columns, tableFormat);
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground(QColor("#DADADA"));
    for(int i = 0; i < t_columns; i++){
        QTextTableCell cell = textTable->cellAt(0,i);
        cell.setFormat(tableHeaderFormat);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText(ui->consumable_tableWidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString());
    }
    bool _break = false;
    for(int r = 0; r < t_rows; r++){
        QString item_string;
        for(int c = 0; c < t_columns; c++){
            QTableWidgetItem *item = ui->consumable_tableWidget->item(r,c);
            if(c == 0 && (!item || item->text().isEmpty())){
                _break = true;
                break;
            }
            if(c == 2){
                QLineEdit *consumable_item = qobject_cast<QLineEdit*>(ui->consumable_tableWidget->cellWidget(r,2));
                item_string = consumable_item->text();
            }else if(c == 4){
                QLineEdit *section = qobject_cast<QLineEdit *>(ui->consumable_tableWidget->cellWidget(r,4));
                item_string = section->text();
            }else if(c == 5){
                QLineEdit *engineer_item = qobject_cast<QLineEdit*>(ui->consumable_tableWidget->cellWidget(r,5));
                item_string = engineer_item->text();
            }else{
                if(!item || item->text().isEmpty())
                    ui->consumable_tableWidget->setItem(r,c,new QTableWidgetItem(QString("")));
                item_string = ui->consumable_tableWidget->item(r,c)->text();
            }
            QTextTableCell cell = textTable->cellAt(r+1, c);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(item_string);
        }
        if(_break)
            break;
    }
    cursor.movePosition(QTextCursor::End);
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    QString directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("Browse..."),QDir::currentPath()));
    QString  output_fileName = directory+ "/" + "PrintedCons.pdf";
    if(QFile::exists(output_fileName)){
        i++;
        output_fileName =directory+ "/" + "PrintedCons_"+ QString::number(i) +".pdf";
    }
    printer.setOutputFileName(output_fileName);
    doc.setDocumentMargin(0);
    doc.setTextWidth(5);
    doc.print(&printer);
    QDesktopServices::openUrl(QUrl::fromLocalFile(output_fileName));
}
void MainWindow::auditCons(){
    QSqlQuery q_auditCons(QSqlDatabase::database(conn_consumables));
    q_auditCons.prepare("SELECT Item, No FROM consumables");
    if(q_auditCons.exec()){
        while(q_auditCons.next()){
            QString item = q_auditCons.value(0).toString();
            QString item_no = q_auditCons.value(1).toString();
            if(item_no.toInt()){
                int tool_noInt  = item_no.toInt();
                 if(cons_fromDB.contains(item)){
                     int val = cons_fromDB.value(item);
                     tool_noInt += val;
                 }
                 cons_fromDB[item] = tool_noInt;
            }else{
                QMessageBox::warning(nullptr,QObject::tr("Warning"),
                                     QObject::tr("\n Some of your tool numbers are not actually numbers \n"
                                                 "I am gonna continue but some functionality is gonna be deducted"), QMessageBox::Cancel);
            }
        }
    }
    // Set up a dialog  window
    QTableWidget *tbl = new QTableWidget();
    QStringList headerItems = {"Consumable Item","Remaining No."};
    tbl->setColumnCount(headerItems.size());
    tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tbl->setHorizontalHeaderLabels(headerItems);
    // iterate through HashTables for get items numbers differences
    QHashIterator<QString, int> hash_iterator(cons_fromDB);
    while(hash_iterator.hasNext()){
        hash_iterator.next();
        QString pending_item = hash_iterator.key();
        int pending_value = hash_iterator.value();
        if(consumables_audit.contains(pending_item)){
             int org_val =  consumables_audit.value(pending_item);
             consumables_audit[pending_item] =(org_val - pending_value);
        }
    }
    // as of now the org tools have the updated list
    QHashIterator<QString, int>h_iter(consumables_audit);
    int _r = 0;
    while(h_iter.hasNext()){
        h_iter.next();
        tbl->insertRow(_r);
        tbl->setItem(_r,0,new QTableWidgetItem(h_iter.key()));
        int _val = h_iter.value();
        if(_val < 3){
            QTableWidgetItem *val_item = new QTableWidgetItem(QString::number(_val));
            val_item->setBackground(Qt::red);
            tbl->setItem(_r, 1 , val_item);
        }else{
            QTableWidgetItem *val_item2 = new QTableWidgetItem(QString::number(_val));
            val_item2->setBackground(Qt::green);
            tbl->setItem(_r, 1, val_item2);
        }
        _r++;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(tbl);
    QDialog *dialog = new QDialog();
    dialog->setModal(true);
    dialog->setLayout(vbox);
    dialog->exec();
}
QLineEdit* MainWindow::createSectionDropDown(){
    QStringList sections = {"Withering","CTC","CFU","Driers","Sorting","Packing","Boiler","Pallets","Automotive","Power Room","Green Leaf","Water plant","Workshop"};
    QCompleter *comp = new QCompleter(sections);
    comp->setCaseSensitivity(Qt::CaseInsensitive);
    comp->setFilterMode(Qt::MatchContains);
    QLineEdit *autoCompleteSection = new QLineEdit();
    autoCompleteSection->setCompleter(comp);
    return autoCompleteSection;
}
/************************                           Clean Up & Event handlers                   *******************/
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
        // save page 0 data
        if(!up_db.isOpen()){
            up_db.open();
        }
        saveEntries();
        up_db.close();
        // save page 1
        if(!db_tools.isOpen()){
            db_tools.open();
        }
        deleteToolsTable();
        saveToolsTable();
        db_tools.close();
        if(!cons_db.isOpen()){
            cons_db.open();
        }
        saveConsumables();
        db_tools.close();
        event->accept();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
