#include "CWindowDemo.h"
#include <QtWidgets/QApplication>
#include <QMenuBar>
#include <QMenu>
#include "cbasewindow.h"
#include "window.h"
#include "cmenubar.h"

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
    //QWidget* pWgt = new QWidget(&tt);
    //tt.setTitleBar(pWgt);
    //tt.setMenuBar(pWgt);
    //tt.setCenterWidget(pWgt);
    //tt.setStatusBar(pWgt);
    /*QMenuBar* pBar = new QMenuBar();
    pBar->addAction("File");*/
    QMenu* pMenu = new QMenu("About");
    pMenu->addAction("VS 2017");
    pMenu->addAction("Qt");
    //pBar->addMenu(pMenu);

    //tt.setMenuBar(pBar);

    QMenu* m2 = new QMenu("File");
    m2->addAction("New");
    m2->addAction("Create");
    m2->addSeparator();
    m2->addAction("Exit");

    CMenuBar* pMenuBar = new CMenuBar(&tt);
    pMenuBar->addMenu(m2);
    pMenuBar->addMenu(pMenu);

    tt.setMenuBar(pMenuBar);
#endif

    tt.show();


    return a.exec();
}
