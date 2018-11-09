#pragma once
#include <QWidget>
#include "qtclasslibrary_global.h"


class CTitleBar;
class CWindowPrivate;
class QTCLASSLIBRARY_EXPORT CWindow : public QWidget
{
    Q_OBJECT
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
