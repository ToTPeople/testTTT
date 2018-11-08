#pragma once
#include <qwidget.h>
#include "qtclasslibrary_global.h"


class QMenu;
class CMenuBarPrivate;
class QTCLASSLIBRARY_EXPORT CMenuBar : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor menuNormalBg READ menuNormalBg WRITE setMenuNormalBg)
    Q_PROPERTY(QColor menuHoverBg READ menuHoverBg WRITE setMenuHoverBg)
    Q_PROPERTY(QColor menuSelectedBg READ menuSelectedBg WRITE setMenuSelectedBg)
    //Q_PROPERTY(QColor menuNormalBg READ menuNormalBg WRITE setMenuNormalBg)
public:
    CMenuBar(QWidget* parent = NULL);
    ~CMenuBar();

public:
    void initStyle();

    void addAction(const QString& strAction);
    QAction* addAction(const QString &text, const QObject *receiver, const char* member, const QKeySequence &shortcut);
    void addMenu(QMenu* pMenu);

public:
    virtual bool eventFilter(QObject *watched, QEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void changeEvent(QEvent *event);

    bool event(QEvent *event) Q_DECL_OVERRIDE;

public:
    QColor menuNormalBg() const;
    void setMenuNormalBg(QColor color);

    QColor menuHoverBg() const;
    void setMenuHoverBg(QColor color);

    QColor menuSelectedBg() const;
    void setMenuSelectedBg(QColor color);

private:
    CMenuBarPrivate&            p;
};

