/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_8;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_8;
    QTableWidget *engineers_tableWidget;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_10;
    QTableWidget *tools_tableWidget;
    QHBoxLayout *horizontalLayout_10;
    QLabel *savestat_label;
    QPushButton *consumablep0_pushButton;
    QPushButton *nonconp0_pushButton;
    QSpacerItem *horizontalSpacer_13;
    QPushButton *savep0_pushButton;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_11;
    QLabel *search_label_2;
    QLineEdit *Name_lineEdit;
    QLineEdit *search_lineEdit;
    QPushButton *restore1_pushButton;
    QTableWidget *main_tableWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *save_label;
    QPushButton *home1_pushButton;
    QPushButton *consumable_pushButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *audit_pushButton;
    QPushButton *print_pushButton;
    QPushButton *pending_pushButton;
    QPushButton *save_pushButton;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_10;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *search_label;
    QComboBox *field_comboBox;
    QLineEdit *item_lineEdit;
    QPushButton *restore2_pushButton;
    QVBoxLayout *verticalLayout_9;
    QTableWidget *consumable_tableWidget;
    QHBoxLayout *horizontalLayout_9;
    QLabel *status2_label;
    QPushButton *home2_pushButton;
    QPushButton *noncon_pushButton;
    QSpacerItem *horizontalSpacer_12;
    QPushButton *save2_pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(727, 458);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow{\n"
"	background-color: rgb(78, 154, 6);\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setStyleSheet(QString::fromUtf8("centralwidget{\n"
"	background-color: rgb(115, 210, 22);\n"
"}"));
        verticalLayout_8 = new QVBoxLayout(centralwidget);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_5 = new QVBoxLayout(page);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, -1, -1, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setItalic(true);
        font.setUnderline(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout_3->addWidget(label);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, -1, 0, -1);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, -1, -1, 0);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);

        label_3 = new QLabel(page);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font1;
        font1.setPointSize(11);
        label_3->setFont(font1);

        horizontalLayout_6->addWidget(label_3);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_8);


        verticalLayout_3->addLayout(horizontalLayout_6);

        engineers_tableWidget = new QTableWidget(page);
        engineers_tableWidget->setObjectName(QString::fromUtf8("engineers_tableWidget"));

        verticalLayout_3->addWidget(engineers_tableWidget);


        horizontalLayout_4->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(-1, -1, 0, 0);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(-1, -1, 45, -1);
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_9);

        label_4 = new QLabel(page);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font1);

        horizontalLayout_7->addWidget(label_4);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_10);


        verticalLayout_4->addLayout(horizontalLayout_7);

        tools_tableWidget = new QTableWidget(page);
        tools_tableWidget->setObjectName(QString::fromUtf8("tools_tableWidget"));

        verticalLayout_4->addWidget(tools_tableWidget);


        horizontalLayout_4->addLayout(verticalLayout_4);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(-1, -1, -1, 0);
        savestat_label = new QLabel(page);
        savestat_label->setObjectName(QString::fromUtf8("savestat_label"));

        horizontalLayout_10->addWidget(savestat_label);

        consumablep0_pushButton = new QPushButton(page);
        consumablep0_pushButton->setObjectName(QString::fromUtf8("consumablep0_pushButton"));
        consumablep0_pushButton->setFont(font1);

        horizontalLayout_10->addWidget(consumablep0_pushButton);

        nonconp0_pushButton = new QPushButton(page);
        nonconp0_pushButton->setObjectName(QString::fromUtf8("nonconp0_pushButton"));
        nonconp0_pushButton->setFont(font1);

        horizontalLayout_10->addWidget(nonconp0_pushButton);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_13);

        savep0_pushButton = new QPushButton(page);
        savep0_pushButton->setObjectName(QString::fromUtf8("savep0_pushButton"));
        savep0_pushButton->setFont(font1);

        horizontalLayout_10->addWidget(savep0_pushButton);


        verticalLayout_2->addLayout(horizontalLayout_10);


        verticalLayout_5->addLayout(verticalLayout_2);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_7 = new QVBoxLayout(page_2);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(-1, -1, -1, 0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_11);

        search_label_2 = new QLabel(page_2);
        search_label_2->setObjectName(QString::fromUtf8("search_label_2"));
        search_label_2->setFont(font1);

        horizontalLayout_8->addWidget(search_label_2);

        Name_lineEdit = new QLineEdit(page_2);
        Name_lineEdit->setObjectName(QString::fromUtf8("Name_lineEdit"));

        horizontalLayout_8->addWidget(Name_lineEdit);

        search_lineEdit = new QLineEdit(page_2);
        search_lineEdit->setObjectName(QString::fromUtf8("search_lineEdit"));

        horizontalLayout_8->addWidget(search_lineEdit);

        restore1_pushButton = new QPushButton(page_2);
        restore1_pushButton->setObjectName(QString::fromUtf8("restore1_pushButton"));
        restore1_pushButton->setFont(font1);

        horizontalLayout_8->addWidget(restore1_pushButton);


        verticalLayout_6->addLayout(horizontalLayout_8);

        main_tableWidget = new QTableWidget(page_2);
        main_tableWidget->setObjectName(QString::fromUtf8("main_tableWidget"));

        verticalLayout_6->addWidget(main_tableWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, -1, 0);
        save_label = new QLabel(page_2);
        save_label->setObjectName(QString::fromUtf8("save_label"));

        horizontalLayout_2->addWidget(save_label);

        home1_pushButton = new QPushButton(page_2);
        home1_pushButton->setObjectName(QString::fromUtf8("home1_pushButton"));
        home1_pushButton->setFont(font1);

        horizontalLayout_2->addWidget(home1_pushButton);

        consumable_pushButton = new QPushButton(page_2);
        consumable_pushButton->setObjectName(QString::fromUtf8("consumable_pushButton"));
        consumable_pushButton->setFont(font1);

        horizontalLayout_2->addWidget(consumable_pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        audit_pushButton = new QPushButton(page_2);
        audit_pushButton->setObjectName(QString::fromUtf8("audit_pushButton"));
        audit_pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(191, 64, 172);\n"
"}"));

        horizontalLayout_2->addWidget(audit_pushButton);

        print_pushButton = new QPushButton(page_2);
        print_pushButton->setObjectName(QString::fromUtf8("print_pushButton"));
        print_pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(193, 125, 17)\n"
"}"));

        horizontalLayout_2->addWidget(print_pushButton);

        pending_pushButton = new QPushButton(page_2);
        pending_pushButton->setObjectName(QString::fromUtf8("pending_pushButton"));
        QFont font2;
        font2.setBold(false);
        font2.setUnderline(false);
        font2.setWeight(50);
        pending_pushButton->setFont(font2);
        pending_pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:rgb(237, 212, 0);\n"
"}"));

        horizontalLayout_2->addWidget(pending_pushButton);

        save_pushButton = new QPushButton(page_2);
        save_pushButton->setObjectName(QString::fromUtf8("save_pushButton"));
        QFont font3;
        font3.setPointSize(11);
        font3.setBold(true);
        font3.setUnderline(true);
        font3.setWeight(75);
        save_pushButton->setFont(font3);
        save_pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgb(52, 101, 164);\n"
"}"));

        horizontalLayout_2->addWidget(save_pushButton);


        verticalLayout_6->addLayout(horizontalLayout_2);


        verticalLayout_7->addLayout(verticalLayout_6);

        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        verticalLayout_10 = new QVBoxLayout(page_3);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        search_label = new QLabel(page_3);
        search_label->setObjectName(QString::fromUtf8("search_label"));

        horizontalLayout->addWidget(search_label);

        field_comboBox = new QComboBox(page_3);
        field_comboBox->setObjectName(QString::fromUtf8("field_comboBox"));

        horizontalLayout->addWidget(field_comboBox);

        item_lineEdit = new QLineEdit(page_3);
        item_lineEdit->setObjectName(QString::fromUtf8("item_lineEdit"));

        horizontalLayout->addWidget(item_lineEdit);

        restore2_pushButton = new QPushButton(page_3);
        restore2_pushButton->setObjectName(QString::fromUtf8("restore2_pushButton"));

        horizontalLayout->addWidget(restore2_pushButton);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        consumable_tableWidget = new QTableWidget(page_3);
        consumable_tableWidget->setObjectName(QString::fromUtf8("consumable_tableWidget"));

        verticalLayout_9->addWidget(consumable_tableWidget);


        verticalLayout->addLayout(verticalLayout_9);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        status2_label = new QLabel(page_3);
        status2_label->setObjectName(QString::fromUtf8("status2_label"));

        horizontalLayout_9->addWidget(status2_label);

        home2_pushButton = new QPushButton(page_3);
        home2_pushButton->setObjectName(QString::fromUtf8("home2_pushButton"));

        horizontalLayout_9->addWidget(home2_pushButton);

        noncon_pushButton = new QPushButton(page_3);
        noncon_pushButton->setObjectName(QString::fromUtf8("noncon_pushButton"));

        horizontalLayout_9->addWidget(noncon_pushButton);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_12);

        save2_pushButton = new QPushButton(page_3);
        save2_pushButton->setObjectName(QString::fromUtf8("save2_pushButton"));

        horizontalLayout_9->addWidget(save2_pushButton);


        verticalLayout->addLayout(horizontalLayout_9);


        verticalLayout_10->addLayout(verticalLayout);

        stackedWidget->addWidget(page_3);

        verticalLayout_8->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 727, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Boito Tools Inventory", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Boito Tools Inventory", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Engineers", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Tools", nullptr));
        savestat_label->setText(QString());
        consumablep0_pushButton->setText(QCoreApplication::translate("MainWindow", "Consumables ", nullptr));
        nonconp0_pushButton->setText(QCoreApplication::translate("MainWindow", "Non-Consumable", nullptr));
        savep0_pushButton->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        search_label_2->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
#if QT_CONFIG(tooltip)
        Name_lineEdit->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        Name_lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter engineer's name", nullptr));
        search_lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter specifying item.", nullptr));
        restore1_pushButton->setText(QCoreApplication::translate("MainWindow", "Restore", nullptr));
        save_label->setText(QString());
        home1_pushButton->setText(QCoreApplication::translate("MainWindow", "Home", nullptr));
        consumable_pushButton->setText(QCoreApplication::translate("MainWindow", "Consumables ", nullptr));
        audit_pushButton->setText(QCoreApplication::translate("MainWindow", "Audit Tools", nullptr));
        print_pushButton->setText(QCoreApplication::translate("MainWindow", "Print Doc", nullptr));
        pending_pushButton->setText(QCoreApplication::translate("MainWindow", "Pending", nullptr));
        save_pushButton->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        search_label->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        restore2_pushButton->setText(QCoreApplication::translate("MainWindow", "Restore", nullptr));
        status2_label->setText(QString());
        home2_pushButton->setText(QCoreApplication::translate("MainWindow", "Home", nullptr));
        noncon_pushButton->setText(QCoreApplication::translate("MainWindow", "Non-Consumables", nullptr));
        save2_pushButton->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
