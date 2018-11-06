/********************************************************************************
** Form generated from reading UI file 'window.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Window
{
public:
    QVBoxLayout *vLayoutWindow;
    QWidget *wgtTitleBar;
    QHBoxLayout *hLayoutWinTitleBar;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnMin;
    QPushButton *btnMax;
    QPushButton *btnExit;
    QWidget *wgtMacTitleBar;
    QHBoxLayout *hLayoutMacTitleBar;
    QPushButton *btnMacExit;
    QPushButton *btnMacMin;
    QPushButton *btnMacMax;
    QSpacerItem *horizontalSpacer_2;
    QWidget *wgtMenuBar;
    QHBoxLayout *hLayoutMenuBar;
    QWidget *wgtCenterWidget;
    QWidget *wgtStatusBar;

    void setupUi(QWidget *Window)
    {
        if (Window->objectName().isEmpty())
            Window->setObjectName(QStringLiteral("Window"));
        Window->resize(464, 377);
        vLayoutWindow = new QVBoxLayout(Window);
        vLayoutWindow->setSpacing(1);
        vLayoutWindow->setObjectName(QStringLiteral("vLayoutWindow"));
        vLayoutWindow->setContentsMargins(5, 5, 5, 5);
        wgtTitleBar = new QWidget(Window);
        wgtTitleBar->setObjectName(QStringLiteral("wgtTitleBar"));
        wgtTitleBar->setMinimumSize(QSize(0, 45));
        wgtTitleBar->setMaximumSize(QSize(16777215, 45));
        hLayoutWinTitleBar = new QHBoxLayout(wgtTitleBar);
        hLayoutWinTitleBar->setObjectName(QStringLiteral("hLayoutWinTitleBar"));
        labelTitle = new QLabel(wgtTitleBar);
        labelTitle->setObjectName(QStringLiteral("labelTitle"));

        hLayoutWinTitleBar->addWidget(labelTitle);

        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayoutWinTitleBar->addItem(horizontalSpacer);

        btnMin = new QPushButton(wgtTitleBar);
        btnMin->setObjectName(QStringLiteral("btnMin"));
        btnMin->setMinimumSize(QSize(32, 32));
        btnMin->setMaximumSize(QSize(32, 32));

        hLayoutWinTitleBar->addWidget(btnMin);

        btnMax = new QPushButton(wgtTitleBar);
        btnMax->setObjectName(QStringLiteral("btnMax"));
        btnMax->setMinimumSize(QSize(32, 32));
        btnMax->setMaximumSize(QSize(32, 32));

        hLayoutWinTitleBar->addWidget(btnMax);

        btnExit = new QPushButton(wgtTitleBar);
        btnExit->setObjectName(QStringLiteral("btnExit"));
        btnExit->setMinimumSize(QSize(32, 32));
        btnExit->setMaximumSize(QSize(32, 32));

        hLayoutWinTitleBar->addWidget(btnExit);


        vLayoutWindow->addWidget(wgtTitleBar);

        wgtMacTitleBar = new QWidget(Window);
        wgtMacTitleBar->setObjectName(QStringLiteral("wgtMacTitleBar"));
        wgtMacTitleBar->setMinimumSize(QSize(0, 45));
        wgtMacTitleBar->setMaximumSize(QSize(16777215, 45));
        hLayoutMacTitleBar = new QHBoxLayout(wgtMacTitleBar);
        hLayoutMacTitleBar->setObjectName(QStringLiteral("hLayoutMacTitleBar"));
        btnMacExit = new QPushButton(wgtMacTitleBar);
        btnMacExit->setObjectName(QStringLiteral("btnMacExit"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnMacExit->sizePolicy().hasHeightForWidth());
        btnMacExit->setSizePolicy(sizePolicy);
        btnMacExit->setMinimumSize(QSize(16, 16));
        btnMacExit->setMaximumSize(QSize(16, 16));

        hLayoutMacTitleBar->addWidget(btnMacExit);

        btnMacMin = new QPushButton(wgtMacTitleBar);
        btnMacMin->setObjectName(QStringLiteral("btnMacMin"));
        btnMacMin->setMinimumSize(QSize(16, 16));
        btnMacMin->setMaximumSize(QSize(16, 16));

        hLayoutMacTitleBar->addWidget(btnMacMin);

        btnMacMax = new QPushButton(wgtMacTitleBar);
        btnMacMax->setObjectName(QStringLiteral("btnMacMax"));
        btnMacMax->setMinimumSize(QSize(16, 16));
        btnMacMax->setMaximumSize(QSize(16, 16));

        hLayoutMacTitleBar->addWidget(btnMacMax);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayoutMacTitleBar->addItem(horizontalSpacer_2);


        vLayoutWindow->addWidget(wgtMacTitleBar);

        wgtMenuBar = new QWidget(Window);
        wgtMenuBar->setObjectName(QStringLiteral("wgtMenuBar"));
        wgtMenuBar->setMinimumSize(QSize(0, 45));
        wgtMenuBar->setMaximumSize(QSize(16777215, 45));
        hLayoutMenuBar = new QHBoxLayout(wgtMenuBar);
        hLayoutMenuBar->setObjectName(QStringLiteral("hLayoutMenuBar"));

        vLayoutWindow->addWidget(wgtMenuBar);

        wgtCenterWidget = new QWidget(Window);
        wgtCenterWidget->setObjectName(QStringLiteral("wgtCenterWidget"));

        vLayoutWindow->addWidget(wgtCenterWidget);

        wgtStatusBar = new QWidget(Window);
        wgtStatusBar->setObjectName(QStringLiteral("wgtStatusBar"));
        wgtStatusBar->setMinimumSize(QSize(0, 60));
        wgtStatusBar->setMaximumSize(QSize(16777215, 60));

        vLayoutWindow->addWidget(wgtStatusBar);


        retranslateUi(Window);

        QMetaObject::connectSlotsByName(Window);
    } // setupUi

    void retranslateUi(QWidget *Window)
    {
        Window->setWindowTitle(QApplication::translate("Window", "Window", Q_NULLPTR));
        labelTitle->setText(QString());
        btnMin->setText(QString());
        btnMax->setText(QString());
        btnExit->setText(QString());
        btnMacExit->setText(QApplication::translate("Window", "PushButton", Q_NULLPTR));
        btnMacMin->setText(QApplication::translate("Window", "PushButton", Q_NULLPTR));
        btnMacMax->setText(QApplication::translate("Window", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Window: public Ui_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW_H
