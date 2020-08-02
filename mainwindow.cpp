#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startup.h"

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
    pullUpDb(select_all, select_all_partner);
    // just here, get the start up window showing
    QTimer::singleShot(0, qApp,[this](){
        Startup up_run;
        up_run.setModal(true);
        up_run.exec();
        current_user = up_run.str_user;
        engineers = up_run.engineers_names;
        tools = up_run.tool_names;
        // search with names
                // first add auto-completion
            QCompleter *name_comp = new QCompleter(engineers);
            name_comp->setCaseSensitivity(Qt::CaseInsensitive);
            name_comp->setFilterMode(Qt::MatchContains);
            ui->Name_lineEdit->setCompleter(name_comp);
        // Search with items
            // first add auto-completion
            QCompleter *tool_comp =new QCompleter(tools);
            tool_comp->setCaseSensitivity(Qt::CaseInsensitive);
            tool_comp->setFilterMode(Qt::MatchContains);
            ui->search_lineEdit->setCompleter(tool_comp);
        // copy start_up QHash to a main QHash
            original_tools_audit = up_run.tools_audit;
    });
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
                ui->main_tableWidget->setItem(_row,_col, new QTableWidgetItem(current_user));
            }else if(_col == 9){
                ui->main_tableWidget->setItem(_row,_col, new QTableWidgetItem(current_user));
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
        if(original_tools_audit.contains(pending_item)){
             int org_val =  original_tools_audit.value(pending_item);
             original_tools_audit[pending_item] =(org_val - pending_value);
        }
    }
    // as of now the org tools have the updated list
    QHashIterator<QString, int>h_iter(original_tools_audit);
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

