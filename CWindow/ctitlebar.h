#pragma once
#include <qwidget.h>
#include "qtclasslibrary_global.h"


class CTitleBarPrivate;
class QTCLASSLIBRARY_EXPORT CTitleBar : public QWidget
{
    Q_OBJECT
public:
    CTitleBar(QWidget* parent = NULL);
    ~CTitleBar();

public:
    void setTitle(const QString& strTitle);
    QString getTitle();

    void setMaximizeIconVisiable(bool bVisiable = true);

    virtual void retranslateUi();

public:
    virtual void changeEvent(QEvent *event);

signals:
    void sigOnMinimizeBtnClick();
    void sigOnMaximizeBtnClick();
    void sigOnExitBtnClick();

protected:
    CTitleBarPrivate&           p;
};

