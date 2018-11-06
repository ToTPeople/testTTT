#pragma once
#include <qwidget.h>
#include "qtclasslibrary_global.h"


class CMenuBarPrivate;
class QTCLASSLIBRARY_EXPORT CMenuBar : public QWidget
{
public:
    CMenuBar(QWidget* parent = NULL);
    ~CMenuBar();

public:
    /*void addAction(const QString& strAction);
    void addMenu(QMenu* pMenu);*/

private:
    CMenuBarPrivate&            p;
};

