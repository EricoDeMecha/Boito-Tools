#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    /*show first page*/
    ui->stackedWidget->setCurrentIndex(0);
    /*Do the stuff related to the first page*/
    ui->stackedWidget->setWindowTitle("Start-Up Window");
    /*Connect to the Up_db*/
    if(!createMainConnection()){
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

    QStringList tools_tableItems = {"Item","Number"};
    ui->tools_tableWidget->setColumnCount(tools_tableItems.size());
    int i = 0;
    foreach (QString item, tools_tableItems) {
        QTableWidgetItem *tools_horizontalHeader = new QTableWidgetItem(item);
        ui->tools_tableWidget->setHorizontalHeaderItem(i++,tools_horizontalHeader);
    }
    ui->tools_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
    connect(ui->ok_pushButton, SIGNAL(clicked()),this,SLOT(saveData()));
    connect(ui->cancel_pushButton,SIGNAL(clicked()), this,SLOT(quitMain()));
    /*************************************************************************************************************************/
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
    // search with names
            // first add auto-completion
        QCompleter *name_comp = new QCompleter(engineers_names);
        name_comp->setCaseSensitivity(Qt::CaseInsensitive);
        name_comp->setFilterMode(Qt::MatchContains);
        ui->Name_lineEdit->setCompleter(name_comp);
    // Search with items
        // first add auto-completion
        QCompleter *tool_comp =new QCompleter(tool_names);
        tool_comp->setCaseSensitivity(Qt::CaseInsensitive);
        tool_comp->setFilterMode(Qt::MatchContains);
        ui->search_lineEdit->setCompleter(tool_comp);

    // 4-> Insert Widget to status if cell is clicked and is empty
    connect(ui->main_tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onCellClicked(int,int)));

    // save is manual
    connect(ui->save_pushButton,SIGNAL(clicked()),this, SLOT(mainHandler()));
    // search with names
    connect(ui->Name_lineEdit, SIGNAL(editingFinished()),this,SLOT(nameSearch()));
    connect(ui->Name_lineEdit,SIGNAL(returnPressed()),this,SLOT(restoreDisplay()));
    // Search with tools
    connect(ui->search_lineEdit,SIGNAL(editingFinished()),this,SLOT(toolSearch()));
    // pending button
    connect(ui->pending_pushButton, SIGNAL(clicked()), this, SLOT(onPendingClicked()));
    // print Doc
    connect(ui->print_pushButton, SIGNAL(clicked()), this, SLOT(onPrintClicked()));
    // audit tools
    connect(ui->audit_pushButton, SIGNAL(clicked()), this, SLOT(onAuditClicked()));
}
bool MainWindow::createMainConnection()
{
    const QString DRIVER("QSQLITE");
    up_db = QSqlDatabase::addDatabase(DRIVER,"conn_start");
    up_db.setDatabaseName("up_db");
    if(!up_db.open()){
        QMessageBox::critical(nullptr, QObject::tr("Critical Error"),
                  QObject::tr("Could not find db file \n\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    return true;
}
void MainWindow::pullUpStartDB()
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
    }
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
        }
    }
}

void MainWindow::saveData()
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
        QSqlQuery q_tools(QSqlDatabase::database("conn_start"));
        q_tools.prepare("INSERT INTO MainTools VALUES(?,?)");
        QString tool_name = ui->tools_tableWidget->item(r,0)->text();
        QString tool_no = ui->tools_tableWidget->item(r,1)->text();
        if(tool_no.toInt()){
            int tool_noInt  = tool_no.toInt();
             if(tools_audit.contains(tool_name)){
                 int val = tools_audit.value(tool_name);
                 tool_noInt += val;
             }
             tools_audit[tool_name] = tool_noInt;
        }else{
            QMessageBox::warning(nullptr,QObject::tr("Warning"),
                                 QObject::tr("\n Some of your tool numbers are not actually numbers \n"
                                             "I am gonna continue but some functionality is gonna be deducted"), QMessageBox::Cancel);
        }
        tool_names << tool_name; // store for mainWindow;
        q_tools.bindValue(0,tool_name);
        q_tools.bindValue(1, tool_no);
        q_tools.exec();
    }
    // Users Line Edit
    str_user = ui->user_lineEdit->text();
    if(str_user.isEmpty()){
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("User Line edit is empty\n"
                                                                          "Your changes are obsolete"), QMessageBox::Cancel);
        qApp->quit();
    }
    up_db.close();
    /*Switch page to 1 */
    ui->stackedWidget->setCurrentIndex(1);
    constructMain();
}
void MainWindow::quitMain(){
    qApp->quit();
}
/***************************************************************************************************************************/
void MainWindow::constructMain(){
    /*Create a database connection*/
    // 1 -> create connection(check for drivers)
    if(!createConnection()){
        qApp->quit();
    }
    // 3-> Pull up Db and update the table
    pullUpDb(select_all, select_all_partner);
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
                // then add a new row
                int current_rowCount = ui->main_tableWidget->rowCount();
                ui->main_tableWidget->insertRow(current_rowCount);
                // update others
                createComboWidget("Pending", _row);
                tools_autoComplete(QString(""), _row);
                engineers_autoComplete(QString(""),_row);
            }else if (_col == 8) {
                ui->main_tableWidget->setItem(_row,_col, new QTableWidgetItem(str_user));
            }else if(_col == 9){
                ui->main_tableWidget->setItem(_row,_col, new QTableWidgetItem(str_user));
            }
        }
    }
}
bool MainWindow::createConnection(){
    const QString DRIVER("QSQLITE");
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

void MainWindow::pullUpDb(QString  command , QString bind_value){
    // clear grounds
    ui->main_tableWidget->clearContents();
    ui->main_tableWidget->setRowCount(0);
    // Bring up all the data to table
    QSqlQuery pull_query(QSqlDatabase::database("conn_main"));
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

void MainWindow::mainHandler(){
    deleteDbTable();// delete db table before saving new
    saveTable();// saves the table
    pullUpDb(select_all, select_all_partner);// update the table again
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
        if(ui->save_pushButton->isEnabled()){
            deleteDbTable();// delete ...
            saveTable();// and save table before exit
        }else{
            restoreDisplay();
            deleteDbTable();
            saveTable();
        }
        event->accept();
    }
}
void MainWindow::nameSearch()
{
    QString search_item = ui->Name_lineEdit->text();
    if(!search_item.isEmpty()){
        ui->save_pushButton->setEnabled(false);
        QString command_line = "SELECT * FROM Tools WHERE Name = ? ";
        pullUpDb(command_line, search_item);
    }
}

void MainWindow::toolSearch()
{
    QString  search_item = ui->search_lineEdit->text();
    if(!search_item.isEmpty()){
         ui->save_pushButton->setEnabled(false);
        QList<QTableWidgetItem *> search_results = ui->main_tableWidget->findItems(search_item,Qt::MatchEndsWith);
        foreach(QTableWidgetItem *item, search_results){
            item->setBackground(Qt::blue);
        }
    }
}

void MainWindow::restoreDisplay()
{
    pullUpDb(select_all,select_all_partner);
    ui->save_pushButton->setEnabled(true);
}

void MainWindow::onPendingClicked()
{
    QString bind_value = "Pending";
    QString command = "SELECT * FROM Tools WHERE Status = ? ";
    ui->save_pushButton->setEnabled(false);
    pullUpDb(command, bind_value);
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
    QSqlQuery aq_tools(QSqlDatabase::database("conn_main"));
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
MainWindow::~MainWindow()
{
    delete ui;
}

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
    const QString DRIVER = "QSQLITE";
    const QString tools_db = "db.db";
    const QString start_up = "up_db";
    if(QFile::exists(tools_db) && QFile::exists(start_up)){
        return;
    }else{
        // Create databases
        //tools_db
        const QString tools_conn = "tools_conn";
        QSqlDatabase tools = QSqlDatabase::addDatabase(DRIVER, tools_conn);
        tools.setDatabaseName(tools_db);
        tools.open();
        QSqlQuery q_tools(QSqlDatabase::database(tools_conn));
        q_tools.exec("CREATE TABLE Tools(Date TEXT, Item TEXT,No TEXT, Section TEXT,"
                     " Name TEXT, Sign TEXT, Status TEXT, Condition TEXT,Issued_by TEXT, Received_by TEXT)");
        tools.close();

        //data_db
        const QString data_conn = "data_conn";
        QSqlDatabase data_container = QSqlDatabase::addDatabase(DRIVER, data_conn);
        data_container.setDatabaseName(start_up);
        data_container.open();

        QSqlQuery q_user(QSqlDatabase::database(data_conn));
        q_user.prepare("CREATE TABLE Users(Names TEXT)");
        if(q_user.exec()){
            QStringList  data_items = {"Tonui","Alan"};
            for(int i = 0; i < data_items.size(); i++){
                QSqlQuery insert_user(QSqlDatabase::database(data_conn));
                insert_user.prepare("INSERT INTO Users VALUES(?)");
                insert_user.bindValue(0,data_items[i]);
                if(!insert_user.exec()){
                    break;
                }
            }
        }

        QStringList sql_commands = {"CREATE TABLE Engineers (Names TEXT)","CREATE TABLE MainTools (Items TEXT, Num TEXT)"};
        for(int i = 0; i < sql_commands.size(); i++){
            QSqlQuery create_query(QSqlDatabase::database(data_conn));
            create_query.exec(sql_commands[i]);
        }
        data_container.close();
    }
}

