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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *search_label;
    QLineEdit *Name_lineEdit;
    QLineEdit *search_lineEdit;
    QTableWidget *main_tableWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *save_label;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *save_pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(727, 458);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        search_label = new QLabel(centralwidget);
        search_label->setObjectName(QString::fromUtf8("search_label"));

        horizontalLayout->addWidget(search_label);

        Name_lineEdit = new QLineEdit(centralwidget);
        Name_lineEdit->setObjectName(QString::fromUtf8("Name_lineEdit"));

        horizontalLayout->addWidget(Name_lineEdit);

        search_lineEdit = new QLineEdit(centralwidget);
        search_lineEdit->setObjectName(QString::fromUtf8("search_lineEdit"));

        horizontalLayout->addWidget(search_lineEdit);


        verticalLayout->addLayout(horizontalLayout);

        main_tableWidget = new QTableWidget(centralwidget);
        main_tableWidget->setObjectName(QString::fromUtf8("main_tableWidget"));

        verticalLayout->addWidget(main_tableWidget);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, -1, 0);
        save_label = new QLabel(centralwidget);
        save_label->setObjectName(QString::fromUtf8("save_label"));

        horizontalLayout_2->addWidget(save_label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        save_pushButton = new QPushButton(centralwidget);
        save_pushButton->setObjectName(QString::fromUtf8("save_pushButton"));
        QFont font;
        font.setBold(true);
        font.setUnderline(true);
        font.setWeight(75);
        save_pushButton->setFont(font);

        horizontalLayout_2->addWidget(save_pushButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 727, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Boito Tools Inventory", nullptr));
        search_label->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
#if QT_CONFIG(tooltip)
        Name_lineEdit->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        Name_lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter engineer's name", nullptr));
        search_lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Enter tool's name", nullptr));
        save_label->setText(QString());
        save_pushButton->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
