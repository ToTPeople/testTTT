#include "CWindowDemo.h"
#include <QtWidgets/QApplication>
#include <QMenuBar>
#include <QMenu>
#include "cbasewindow.h"
#include "window.h"

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
    QMenuBar* pBar = new QMenuBar();
    pBar->addAction("File");
    QMenu* pMenu = new QMenu("About");
    pMenu->addAction("VS 2017");
    pMenu->addAction("Qt");
    pBar->addMenu(pMenu);

    tt.setMenuBar(pBar);
#endif

    tt.show();


    return a.exec();
}
