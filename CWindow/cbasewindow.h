#pragma once

#include <QWidget>
#include "qtclasslibrary_global.h"

class CTitleBar;
class CMenuBar;
class QMouseEvent;
class CBaseWindowPrivate;

class QTCLASSLIBRARY_EXPORT CBaseWindow : public QWidget
{
    Q_OBJECT
public:
    CBaseWindow(QWidget* parent = NULL);
    ~CBaseWindow();

public:
    void setTitleBar(CTitleBar* pTitleBar);
    CTitleBar* getTitleBar();

    void setMenuBar(CMenuBar* pMenuBar);
    CMenuBar* getMenuBar();

    void setCenterWidget(QWidget* pWgt);
    QWidget* getCenterWidget();

    void setStatusBar(QWidget* pStatusBar);
    QWidget* getStatusBar();

public slots:
    void slotOnExitClick();
    void slotOnMinimizeClick();
    void slotOnMaximizeClick();

public:
    virtual void showEvent(QShowEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);


private:
    CBaseWindowPrivate&         p;
};
