#pragma once
#include <QMenu>
#include "qtclasslibrary_global.h"

QT_REQUIRE_CONFIG(menubar);


class CTTMenuBarPrivate;
class QStyleOptionMenuItem;
class QWindowsStyle;
class QPlatformMenuBar;

class QTCLASSLIBRARY_EXPORT CTTMenuBar : public QWidget
{
    Q_OBJECT

        Q_PROPERTY(bool defaultUp READ isDefaultUp WRITE setDefaultUp)
        Q_PROPERTY(bool nativeMenuBar READ isNativeMenuBar WRITE setNativeMenuBar)

public:
    explicit CTTMenuBar(QWidget *parent = Q_NULLPTR);
    ~CTTMenuBar();

    using QWidget::addAction;
    QAction *addAction(const QString &text);
    QAction *addAction(const QString &text, const QObject *receiver, const char* member);

    QAction *addMenu(QMenu *menu);
    QMenu *addMenu(const QString &title);
    QMenu *addMenu(const QIcon &icon, const QString &title);


    QAction *addSeparator();
    QAction *insertSeparator(QAction *before);

    QAction *insertMenu(QAction *before, QMenu *menu);

    void clear();

    QAction *activeAction() const;
    void setActiveAction(QAction *action);

    void setDefaultUp(bool);
    bool isDefaultUp() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    int heightForWidth(int) const override;

    QRect actionGeometry(QAction *) const;
    QAction *actionAt(const QPoint &) const;

    void setCornerWidget(QWidget *w, Qt::Corner corner = Qt::TopRightCorner);
    QWidget *cornerWidget(Qt::Corner corner = Qt::TopRightCorner) const;

    bool isNativeMenuBar() const;
    void setNativeMenuBar(bool nativeMenuBar);
    QPlatformMenuBar *platformMenuBar();
public Q_SLOTS:
    void setVisible(bool visible) override;

Q_SIGNALS:
    void triggered(QAction *action);
    void hovered(QAction *action);

protected:
    void changeEvent(QEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void actionEvent(QActionEvent *) override;
    void focusOutEvent(QFocusEvent *) override;
    void focusInEvent(QFocusEvent *) override;
    void timerEvent(QTimerEvent *) override;
    bool eventFilter(QObject *, QEvent *) override;
    bool event(QEvent *) override;
    void initStyleOption(QStyleOptionMenuItem *option, const QAction *action) const;

private:
    Q_DECLARE_PRIVATE(QMenuBar)
        Q_DISABLE_COPY(QMenuBar)
        Q_PRIVATE_SLOT(d_func(), void _q_actionTriggered())
        Q_PRIVATE_SLOT(d_func(), void _q_actionHovered())
        Q_PRIVATE_SLOT(d_func(), void _q_internalShortcutActivated(int))
        Q_PRIVATE_SLOT(d_func(), void _q_updateLayout())

        friend class QMenu;
    friend class QMenuPrivate;
    friend class QWindowsStyle;
};

