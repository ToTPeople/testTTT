#pragma once
#include <QWidget>
#include "qtclasslibrary_global.h"


class CTitleBar;
class CWindowPrivate;
class QTCLASSLIBRARY_EXPORT CWindow : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int titleBarHeight READ titleBarHeight WRITE setTitleBarHeight)
    Q_PROPERTY(int menuBarHeight READ menuBarHeight WRITE setMenuBarHeight)
    Q_PROPERTY(int statusBarHeight READ statusBarHeight WRITE setStatusBarHeight)
    Q_PROPERTY(int widgetSpace READ widgetSpace WRITE setWidgetSpace)
public:
    CWindow(QWidget* parent = NULL);
    ~CWindow();

public:
    void setTitleBar(CTitleBar* pTitleBar);
    CTitleBar* getTitleBar();

    void setMenuBar(QWidget* pMenuBar);
    QWidget* getMenuBar();

    void setCenterWidget(QWidget* pWgt);
    QWidget* getCenterWidget();

    void setStatusBar(QWidget* pStatusBar);
    QWidget* getStatusBar();

    void setTitle(const QString& strTitle);
    QString getTitle();

public:
    int titleBarHeight();
    void setTitleBarHeight(int nTitleBarHeight);

    int menuBarHeight();
    void setMenuBarHeight(int nMenuBarHeight);

    int statusBarHeight();
    void setStatusBarHeight(int nStatusBarHeight);

    int widgetSpace();
    void setWidgetSpace(int nWidgetSpace);

public:
    virtual void retranslateUi();

public slots:
    void slotOnExitClick();
    void slotOnMinimizeClick();
    void slotOnMaximizeClick();

public:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void changeEvent(QEvent *event);

private:
    CWindowPrivate&         p;
};
