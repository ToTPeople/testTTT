#pragma once
#include <QWidget>
#include "qtclasslibrary_global.h"


class CWindowPrivate;
class  CWindow : public QWidget
{
    Q_OBJECT
public:
    CWindow(QWidget* parent = NULL);
    ~CWindow();

public:
    void setTitleBar(QWidget* pTitleBar);
    QWidget* getTitleBar();

    void setMenuBar(QWidget* pMenuBar);
    QWidget* getMenuBar();

    void setCenterWidget(QWidget* pWgt);
    QWidget* getCenterWidget();

    void setStatusBar(QWidget* pStatusBar);
    QWidget* getStatusBar();

public slots:
    void slotOnExitClick();
    void slotOnMinimizeClick();
    void slotOnMaximizeClick();

public:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);


private:
    CWindowPrivate&         p;
};
