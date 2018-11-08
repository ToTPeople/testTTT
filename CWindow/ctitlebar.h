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
    void setMaximizeIconVisiable(bool bVisiable = true);

signals:
    void sigOnMinimizeBtnClick();
    void sigOnMaximizeBtnClick();
    void sigOnExitBtnClick();

protected:
    CTitleBarPrivate&           p;
};

