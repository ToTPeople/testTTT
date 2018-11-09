#pragma once
#include <qwidget.h>
#include "qtclasslibrary_global.h"


class QMenu;
class CMenuBarPrivate;
class QTCLASSLIBRARY_EXPORT CMenuBar : public QWidget
{
    Q_OBJECT
    // menu item space
    Q_PROPERTY(int menuItemSpace READ menuItemSpace WRITE setMenuItemSpace)
public:
    CMenuBar(QWidget* parent = NULL);
    ~CMenuBar();

public:
    void addAction(const QString& strAction);
    void addAction(const QString &text, const QObject *receiver, const char* member, const QKeySequence &shortcut);
    void addMenu(QMenu* pMenu);

public:
    virtual bool eventFilter(QObject *watched, QEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void changeEvent(QEvent *event);

    bool event(QEvent *event) Q_DECL_OVERRIDE; // ≤‚ ‘”√£¨ø……æ≥˝

public slots:
    void slotShortcut();            // ≤‚ ‘”√£¨ø……æ≥˝

public:
    int menuItemSpace();
    void setMenuItemSpace(int nSpace);

protected:
    CMenuBarPrivate&            p;
};

