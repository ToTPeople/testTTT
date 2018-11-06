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

signals:
    void sigOnMinimizeBtnClick();
    void sigOnMaximizeBtnClick();
    void sigOnExitBtnClick();

private:
    CTitleBarPrivate&           p;
};

