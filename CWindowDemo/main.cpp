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
    /*CWindowDemo w;
    w.show();*/

    /*CBaseWindow ss;
    ss.show();*/
    CWindow tt;

#ifdef CUSTOMER_TITLEBAR
    ///////////////////////////// menubar //////////////////////////////////
    tt.setTitle(QObject::tr("Windows Title"));
    //QWidget* pWgt = new QWidget(&tt);
    //tt.setTitleBar(pWgt);
    //tt.setMenuBar(pWgt);
    //tt.setCenterWidget(pWgt);
    //tt.setStatusBar(pWgt);
    /*QMenuBar* pBar = new QMenuBar();
    pBar->addAction("File");*/
    QMenu* pMenu = new QMenu(QObject::tr("About"));
    pMenu->addAction(QObject::tr("VS 2017"));
    pMenu->addAction(QObject::tr("Qt"));
    pMenu->setShortcutEnabled(Qt::ALT | Qt::Key_J, true);
    //pBar->addMenu(pMenu);

    //tt.setMenuBar(pBar);

    QMenu* m2 = new QMenu(QObject::tr("&File"));
    //QMenu* m2 = new QMenu("File(&F)");
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

    ///////
    pMenuBar->setAutoFillBackground(true);
    QPalette pal = pMenuBar->palette();
    pal.setBrush(QPalette::Background, QBrush(Qt::darkCyan));
    pMenuBar->setPalette(pal);
    ///////

    tt.setMenuBar(pMenuBar);
    //pMenuBar->adjustSize();
#endif

    CWindowDemo centerWidget(&a);
    tt.setCenterWidget(&centerWidget);

    tt.show();


    return a.exec();
}
