#pragma once
#include <qwidget.h>
#include "qtclasslibrary_global.h"


class QMenu;
class CMenuBarPrivate;
class QTCLASSLIBRARY_EXPORT CMenuBar : public QWidget
{
public:
    CMenuBar(QWidget* parent = NULL);
    ~CMenuBar();

public:
    void addAction(const QString& strAction);
    void addMenu(QMenu* pMenu);

public:
    virtual bool eventFilter(QObject *watched, QEvent *event);

    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    CMenuBarPrivate&            p;
};

