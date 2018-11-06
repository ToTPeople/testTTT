/********************************************************************************
** Form generated from reading UI file 'CWindowDemo.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CWINDOWDEMO_H
#define UI_CWINDOWDEMO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CWindowDemoClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CWindowDemoClass)
    {
        if (CWindowDemoClass->objectName().isEmpty())
            CWindowDemoClass->setObjectName(QStringLiteral("CWindowDemoClass"));
        CWindowDemoClass->resize(600, 400);
        menuBar = new QMenuBar(CWindowDemoClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        CWindowDemoClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CWindowDemoClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CWindowDemoClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(CWindowDemoClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        CWindowDemoClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(CWindowDemoClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CWindowDemoClass->setStatusBar(statusBar);

        retranslateUi(CWindowDemoClass);

        QMetaObject::connectSlotsByName(CWindowDemoClass);
    } // setupUi

    void retranslateUi(QMainWindow *CWindowDemoClass)
    {
        CWindowDemoClass->setWindowTitle(QApplication::translate("CWindowDemoClass", "CWindowDemo", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CWindowDemoClass: public Ui_CWindowDemoClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CWINDOWDEMO_H
