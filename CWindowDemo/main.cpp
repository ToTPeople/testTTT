#include "CWindowDemo.h"
#include <QtWidgets/QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QShortcut>
#include <QKeySequence>
#include <QTranslator>
#include "cbasewindow.h"
#include "window.h"
#include "cmenubar.h"
#include "ctitlebar.h"

namespace {
#define CUSTOMER_TITLEBAR
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CWindow tt;

#ifdef CUSTOMER_TITLEBAR
    ///////////////////////////// titlebar //////////////////////////////////
    {
        CTitleBar* pTitleBar = tt.getTitleBar();
        if (NULL != pTitleBar) {
            pTitleBar->setTitle(QObject::tr("Windows Title"));
            pTitleBar->setAutoFillBackground(true);
            QPalette pal = pTitleBar->palette();
            pal.setBrush(QPalette::Background, QBrush(Qt::red));
            pTitleBar->setPalette(pal);
        }
    }
    ///////////////////////////// menubar //////////////////////////////////
    QMenu* pMenu = new QMenu(QObject::tr("About"));
    pMenu->addAction(QObject::tr("VS 2017"));
    pMenu->addAction(QObject::tr("Qt"));
    pMenu->setShortcutEnabled(Qt::ALT | Qt::Key_J, true);

    QMenu* m2 = new QMenu(QObject::tr("&File"));
    m2->addAction(QObject::tr("New"));
    m2->addAction(QObject::tr("Create"));
    m2->addSeparator();
    m2->addAction(QObject::tr("Exit"));
    QAction* pA = new QAction(QObject::tr("T&T"));
    pA->setShortcut(Qt::Key_X);
    m2->addAction(pA);


    CMenuBar* pMenuBar = new CMenuBar(&tt);
    pMenuBar->setObjectName(QStringLiteral("myMenuBar"));

    QShortcut* shortCut = new QShortcut(QKeySequence(QObject::tr("Ctrl+E")), pMenuBar);
    //QShortcut* shortCut = new QShortcut(QKeySequence(Qt::Key_E), pMenuBar);
    //QShortcut *shortCut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left), pMenuBar);
    QObject::connect(shortCut, SIGNAL(activated()), pMenuBar, SLOT(slotShortcut()));
    //QObject::connect(m2, SIGNAL(activated()), pMenuBar, SLOT(slotShortcut()));

    QObject::connect(pA, SIGNAL(triggered(bool)), pMenuBar, SLOT(slotShortcut()));

    pMenuBar->addMenu(m2);
    pMenuBar->addMenu(pMenu);
    pMenuBar->addAction(QObject::tr("Test"));

    /////// background
    pMenuBar->setAutoFillBackground(true);
    QPalette pal = pMenuBar->palette();
    pal.setBrush(QPalette::Background, QBrush(Qt::darkCyan));
    pMenuBar->setPalette(pal);
    ///////

    tt.setMenuBar(pMenuBar);

    ///////////////////////////// Center //////////////////////////////////
    CWindowDemo centerWidget(&a);
    tt.setCenterWidget(&centerWidget);
    ///////////////////////////// StatusBar //////////////////////////////////
    {
        QWidget* pStatusBar = new QWidget(&tt);
        if (NULL != pStatusBar) {
            pStatusBar->setAutoFillBackground(true);
            QPalette pal = pStatusBar->palette();
            pal.setBrush(QPalette::Background, QBrush(Qt::green));
            pStatusBar->setPalette(pal);

            tt.setStatusBar(pStatusBar);
        }
    }
#endif

    tt.show();


    return a.exec();
}
