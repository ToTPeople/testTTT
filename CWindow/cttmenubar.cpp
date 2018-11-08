#include "cttmenubar.h"


#include <qstyle.h>
#include <qlayout.h>
#include <qapplication.h>
#include <qdesktopwidget.h>
#ifndef QT_NO_ACCESSIBILITY
# include <qaccessible.h>
#endif
#include <qpainter.h>
#include <qstylepainter.h>
#include <qevent.h>
#if QT_CONFIG(mainwindow)
#include <qmainwindow.h>
#endif
#include <qtoolbar.h>
#if QT_CONFIG(toolbutton)
#include <qtoolbutton.h>
#endif
#if QT_CONFIG(whatsthis)
#include <qwhatsthis.h>
#endif

#include <qtoolbutton.h>

//#include <qpa/qplatformtheme.h>
//#include "private/qguiapplication_p.h"
//#include "qpa/qplatformintegration.h"
//
//#include "qmenu_p.h"
//#include "qmenubar_p.h"
#include "qdebug.h"

class CMenuBarExtension : public QToolButton
{
public:
    explicit CMenuBarExtension(QWidget *parent);

    QSize sizeHint() const Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
};

CMenuBarExtension::CMenuBarExtension(QWidget *parent)
    : QToolButton(parent)
{
    setObjectName(QLatin1String("qt_menubar_ext_button"));
    setAutoRaise(true);
#if QT_CONFIG(menu)
    setPopupMode(QToolButton::InstantPopup);
#endif
    setIcon(style()->standardIcon(QStyle::SP_ToolBarHorizontalExtensionButton, 0, parentWidget()));
}

void CMenuBarExtension::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    // We do not need to draw both extension arrows
    opt.features &= ~QStyleOptionToolButton::HasMenu;
    p.drawComplexControl(QStyle::CC_ToolButton, opt);
}


QSize CMenuBarExtension::sizeHint() const
{
    int ext = style()->pixelMetric(QStyle::PM_ToolBarExtensionExtent, 0, parentWidget());
    return QSize(ext, ext);
}

////////////////////////////////////////////////////////////////////////////////
class CMenuBarExtension;
class CTTMenuBarPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(CTTMenuBar)
public:
    CTTMenuBarPrivate() : itemsDirty(0), currentAction(0), mouseDown(0),
        closePopupMode(0), defaultPopDown(1), popupState(0), keyboardState(0), altPressed(0),
        doChildEffects(false), platformMenuBar(0)
    { }

    ~CTTMenuBarPrivate()
    {
        delete platformMenuBar;
    }

    void init();
    QAction *getNextAction(const int start, const int increment) const;

    //item calculations
    uint itemsDirty : 1;

    QVector<int> shortcutIndexMap;
    mutable QVector<QRect> actionRects;
    void calcActionRects(int max_width, int start) const;
    QRect actionRect(QAction *) const;
    void updateGeometries();

    //selection
    QPointer<QAction>currentAction;
    uint mouseDown : 1, closePopupMode : 1, defaultPopDown;
    QAction *actionAt(QPoint p) const;
    void setCurrentAction(QAction *, bool = false, bool = false);
    void popupAction(QAction *, bool);

    //active popup state
    uint popupState : 1;
    QPointer<QMenu> activeMenu;

    //keyboard mode for keyboard navigation
    void focusFirstAction();
    void setKeyboardMode(bool);
    uint keyboardState : 1, altPressed : 1;
    QPointer<QWidget> keyboardFocusWidget;

    //firing of events
    void activateAction(QAction *, QAction::ActionEvent);

    void _q_actionTriggered();
    void _q_actionHovered();
    void _q_internalShortcutActivated(int);
    void _q_updateLayout();

    //extra widgets in the menubar
    QPointer<QWidget> leftWidget, rightWidget;
    CMenuBarExtension *extension;
    bool isVisible(QAction *action);

    //menu fading/scrolling effects
    bool doChildEffects;

    QRect menuRect(bool) const;

    // reparenting
    void handleReparent();
    QVector<QPointer<QWidget> > oldParents;

    QList<QAction*> hiddenActions;
    //default action
    QPointer<QAction> defaultAction;

    QBasicTimer autoReleaseTimer;
    QPlatformMenuBar *platformMenuBar;
    QPlatformMenu *getPlatformMenu(const QAction *action);
    QPlatformMenu *findInsertionPlatformMenu(const QAction *action);
    void copyActionToPlatformMenu(const QAction *e, QPlatformMenu *menu);

    inline int indexOf(QAction *act) const { return q_func()->actions().indexOf(act); }
};
////////////////////////////////////////////////////////////////////////////////

/*!
    \internal
*/
QAction *CTTMenuBarPrivate::actionAt(QPoint p) const
{
    for (int i = 0; i < actions.size(); ++i) {
        if (actionRect(actions.at(i)).contains(p))
            return actions.at(i);
    }
    return 0;
}

QRect CTTMenuBarPrivate::menuRect(bool extVisible) const
{
    Q_Q(const QMenuBar);

    int hmargin = q->style()->pixelMetric(QStyle::PM_MenuBarPanelWidth, 0, q);
    QRect result = q->rect();
    result.adjust(hmargin, 0, -hmargin, 0);

    if (extVisible) {
        if (q->isRightToLeft())
            result.setLeft(result.left() + extension->sizeHint().width());
        else
            result.setWidth(result.width() - extension->sizeHint().width());
    }

    if (leftWidget && leftWidget->isVisible()) {
        QSize sz = leftWidget->sizeHint();
        if (q->isRightToLeft())
            result.setRight(result.right() - sz.width());
        else
            result.setLeft(result.left() + sz.width());
    }

    if (rightWidget && rightWidget->isVisible()) {
        QSize sz = rightWidget->sizeHint();
        if (q->isRightToLeft())
            result.setLeft(result.left() + sz.width());
        else
            result.setRight(result.right() - sz.width());
    }

    return result;
}

bool CTTMenuBarPrivate::isVisible(QAction *action)
{
    return !hiddenActions.contains(action);
}

void CTTMenuBarPrivate::updateGeometries()
{
    Q_Q(QMenuBar);
    if (!itemsDirty)
        return;
    int q_width = q->width() - (q->style()->pixelMetric(QStyle::PM_MenuBarPanelWidth, 0, q) * 2);
    int q_start = -1;
    if (leftWidget || rightWidget) {
        int vmargin = q->style()->pixelMetric(QStyle::PM_MenuBarVMargin, 0, q)
            + q->style()->pixelMetric(QStyle::PM_MenuBarPanelWidth, 0, q);
        int hmargin = q->style()->pixelMetric(QStyle::PM_MenuBarHMargin, 0, q)
            + q->style()->pixelMetric(QStyle::PM_MenuBarPanelWidth, 0, q);
        if (leftWidget && leftWidget->isVisible()) {
            QSize sz = leftWidget->sizeHint();
            q_width -= sz.width();
            q_start = sz.width();
            QPoint pos(hmargin, (q->height() - leftWidget->height()) / 2);
            QRect vRect = QStyle::visualRect(q->layoutDirection(), q->rect(), QRect(pos, sz));
            leftWidget->setGeometry(vRect);
        }
        if (rightWidget && rightWidget->isVisible()) {
            QSize sz = rightWidget->sizeHint();
            q_width -= sz.width();
            QPoint pos(q->width() - sz.width() - hmargin, vmargin);
            QRect vRect = QStyle::visualRect(q->layoutDirection(), q->rect(), QRect(pos, sz));
            rightWidget->setGeometry(vRect);
        }
    }

#ifdef Q_OS_MAC
    if (q->isNativeMenuBar()) {//nothing to see here folks, move along..
        itemsDirty = false;
        return;
    }
#endif
    calcActionRects(q_width, q_start);
    currentAction = 0;
#ifndef QT_NO_SHORTCUT
    if (itemsDirty) {
        for (int j = 0; j < shortcutIndexMap.size(); ++j)
            q->releaseShortcut(shortcutIndexMap.value(j));
        shortcutIndexMap.clear();
        const int actionsCount = actions.count();
        shortcutIndexMap.reserve(actionsCount);
        for (int i = 0; i < actionsCount; i++)
            shortcutIndexMap.append(q->grabShortcut(QKeySequence::mnemonic(actions.at(i)->text())));
    }
#endif
    itemsDirty = false;

    hiddenActions.clear();
    //this is the menu rectangle without any extension
    QRect menuRect = this->menuRect(false);

    //we try to see if the actions will fit there
    bool hasHiddenActions = false;
    for (int i = 0; i < actions.count(); ++i) {
        const QRect &rect = actionRects.at(i);
        if (rect.isValid() && !menuRect.contains(rect)) {
            hasHiddenActions = true;
            break;
        }
    }

    //...and if not, determine the ones that fit on the menu with the extension visible
    if (hasHiddenActions) {
        menuRect = this->menuRect(true);
        for (int i = 0; i < actions.count(); ++i) {
            const QRect &rect = actionRects.at(i);
            if (rect.isValid() && !menuRect.contains(rect)) {
                hiddenActions.append(actions.at(i));
            }
        }
    }

    if (hiddenActions.count() > 0) {
        QMenu *pop = extension->menu();
        if (!pop) {
            pop = new QMenu(q);
            extension->setMenu(pop);
        }
        pop->clear();
        pop->addActions(hiddenActions);

        int vmargin = q->style()->pixelMetric(QStyle::PM_MenuBarVMargin, 0, q);
        int x = q->isRightToLeft()
            ? menuRect.left() - extension->sizeHint().width() + 1
            : menuRect.right();
        extension->setGeometry(x, vmargin, extension->sizeHint().width(), menuRect.height() - vmargin * 2);
        extension->show();
    }
    else {
        extension->hide();
    }
    q->updateGeometry();
}

QRect CTTMenuBarPrivate::actionRect(QAction *act) const
{
    const int index = actions.indexOf(act);

    //makes sure the geometries are up-to-date
    const_cast<CTTMenuBarPrivate*>(this)->updateGeometries();

    if (index < 0 || index >= actionRects.count())
        return QRect(); // that can happen in case of native menubar

    return actionRects.at(index);
}

void CTTMenuBarPrivate::focusFirstAction()
{
    if (!currentAction) {
        updateGeometries();
        int index = 0;
        while (index < actions.count() && actionRects.at(index).isNull()) ++index;
        if (index < actions.count())
            setCurrentAction(actions.at(index));
    }
}

void CTTMenuBarPrivate::setKeyboardMode(bool b)
{
    Q_Q(QMenuBar);
    if (b && !q->style()->styleHint(QStyle::SH_MenuBar_AltKeyNavigation, 0, q)) {
        setCurrentAction(0);
        return;
    }
    keyboardState = b;
    if (b) {
        QWidget *fw = QApplication::focusWidget();
        if (fw && fw != q && fw->window() != QApplication::activePopupWidget())
            keyboardFocusWidget = fw;
        focusFirstAction();
        q->setFocus(Qt::MenuBarFocusReason);
    }
    else {
        if (!popupState)
            setCurrentAction(0);
        if (keyboardFocusWidget) {
            if (QApplication::focusWidget() == q)
                keyboardFocusWidget->setFocus(Qt::MenuBarFocusReason);
            keyboardFocusWidget = 0;
        }
    }
    q->update();
}

void CTTMenuBarPrivate::popupAction(QAction *action, bool activateFirst)
{
    Q_Q(QMenuBar);
    if (!action || !action->menu() || closePopupMode)
        return;
    popupState = true;
    if (action->isEnabled() && action->menu()->isEnabled()) {
        closePopupMode = 0;
        activeMenu = action->menu();
        activeMenu->d_func()->causedPopup.widget = q;
        activeMenu->d_func()->causedPopup.action = action;

        QRect adjustedActionRect = actionRect(action);
        QPoint pos(q->mapToGlobal(QPoint(adjustedActionRect.left(), adjustedActionRect.bottom() + 1)));
        QSize popup_size = activeMenu->sizeHint();

        //we put the popup menu on the screen containing the bottom-center of the action rect
        QRect screenRect = QApplication::desktop()->screenGeometry(pos + QPoint(adjustedActionRect.width() / 2, 0));
        pos = QPoint(qMax(pos.x(), screenRect.x()), qMax(pos.y(), screenRect.y()));

        const bool fitUp = (q->mapToGlobal(adjustedActionRect.topLeft()).y() >= popup_size.height());
        const bool fitDown = (pos.y() + popup_size.height() <= screenRect.bottom());
        const bool rtl = q->isRightToLeft();
        const int actionWidth = adjustedActionRect.width();

        if (!fitUp && !fitDown) { //we should shift the menu
            bool shouldShiftToRight = !rtl;
            if (rtl && popup_size.width() > pos.x())
                shouldShiftToRight = true;
            else if (actionWidth + popup_size.width() + pos.x() > screenRect.right())
                shouldShiftToRight = false;

            if (shouldShiftToRight) {
                pos.rx() += actionWidth + (rtl ? popup_size.width() : 0);
            }
            else {
                //shift to left
                if (!rtl)
                    pos.rx() -= popup_size.width();
            }
        }
        else if (rtl) {
            pos.rx() += actionWidth;
        }

        if (!defaultPopDown || (fitUp && !fitDown))
            pos.setY(qMax(screenRect.y(), q->mapToGlobal(QPoint(0, adjustedActionRect.top() - popup_size.height())).y()));
        activeMenu->popup(pos);
        if (activateFirst)
            activeMenu->d_func()->setFirstActionActive();
    }
    q->update(actionRect(action));
}

void CTTMenuBarPrivate::setCurrentAction(QAction *action, bool popup, bool activateFirst)
{
    if (currentAction == action && popup == popupState)
        return;

    autoReleaseTimer.stop();

    doChildEffects = (popup && !activeMenu);
    Q_Q(QMenuBar);
    QWidget *fw = 0;
    if (QMenu *menu = activeMenu) {
        activeMenu = 0;
        if (popup) {
            fw = q->window()->focusWidget();
            q->setFocus(Qt::NoFocusReason);
        }
        menu->hide();
    }

    if (currentAction)
        q->update(actionRect(currentAction));

    popupState = popup;
#if QT_CONFIG(statustip)
    QAction *previousAction = currentAction;
#endif
    currentAction = action;
    if (action && action->isEnabled()) {
        activateAction(action, QAction::Hover);
        if (popup)
            popupAction(action, activateFirst);
        q->update(actionRect(action));
#if QT_CONFIG(statustip)
    }
    else if (previousAction) {
        QString empty;
        QStatusTipEvent tip(empty);
        QApplication::sendEvent(q, &tip);
#endif
    }
    if (fw)
        fw->setFocus(Qt::NoFocusReason);
}

void CTTMenuBarPrivate::calcActionRects(int max_width, int start) const
{
    Q_Q(const QMenuBar);

    if (!itemsDirty)
        return;

    //let's reinitialize the buffer
    actionRects.resize(actions.count());
    actionRects.fill(QRect());

    const QStyle *style = q->style();

    const int itemSpacing = style->pixelMetric(QStyle::PM_MenuBarItemSpacing, 0, q);
    int max_item_height = 0, separator = -1, separator_start = 0, separator_len = 0;

    //calculate size
    const QFontMetrics fm = q->fontMetrics();
    const int hmargin = style->pixelMetric(QStyle::PM_MenuBarHMargin, 0, q),
        vmargin = style->pixelMetric(QStyle::PM_MenuBarVMargin, 0, q),
        icone = style->pixelMetric(QStyle::PM_SmallIconSize, 0, q);
    for (int i = 0; i < actions.count(); i++) {
        QAction *action = actions.at(i);
        if (!action->isVisible())
            continue;

        QSize sz;

        //calc what I think the size is..
        if (action->isSeparator()) {
            if (style->styleHint(QStyle::SH_DrawMenuBarSeparator, 0, q))
                separator = i;
            continue; //we don't really position these!
        }
        else {
            const QString s = action->text();
            QIcon is = action->icon();
            // If an icon is set, only the icon is visible
            if (!is.isNull())
                sz = sz.expandedTo(QSize(icone, icone));
            else if (!s.isEmpty())
                sz = fm.size(Qt::TextShowMnemonic, s);
        }

        //let the style modify the above size..
        QStyleOptionMenuItem opt;
        q->initStyleOption(&opt, action);
        sz = q->style()->sizeFromContents(QStyle::CT_MenuBarItem, &opt, sz, q);

        if (!sz.isEmpty()) {
            { //update the separator state
                int iWidth = sz.width() + itemSpacing;
                if (separator == -1)
                    separator_start += iWidth;
                else
                    separator_len += iWidth;
            }
            //maximum height
            max_item_height = qMax(max_item_height, sz.height());
            //append
            actionRects[i] = QRect(0, 0, sz.width(), sz.height());
        }
    }

    //calculate position
    const int fw = q->style()->pixelMetric(QStyle::PM_MenuBarPanelWidth, 0, q);
    int x = fw + ((start == -1) ? hmargin : start) + itemSpacing;
    int y = fw + vmargin;
    for (int i = 0; i < actions.count(); i++) {
        QRect &rect = actionRects[i];
        if (rect.isNull())
            continue;

        //resize
        rect.setHeight(max_item_height);

        //move
        if (separator != -1 && i >= separator) { //after the separator
            int left = (max_width - separator_len - hmargin - itemSpacing) + (x - separator_start - hmargin);
            if (left < separator_start) { //wrap
                separator_start = x = hmargin;
                y += max_item_height;
            }
            rect.moveLeft(left);
        }
        else {
            rect.moveLeft(x);
        }
        rect.moveTop(y);

        //keep moving along..
        x += rect.width() + itemSpacing;

        //make sure we follow the layout direction
        rect = QStyle::visualRect(q->layoutDirection(), q->rect(), rect);
    }
}

void CTTMenuBarPrivate::activateAction(QAction *action, QAction::ActionEvent action_e)
{
    Q_Q(QMenuBar);
    if (!action || !action->isEnabled())
        return;
    action->activate(action_e);
    if (action_e == QAction::Hover)
        action->showStatusText(q);

    //     if(action_e == QAction::Trigger)
    //         emit q->activated(action);
    //     else if(action_e == QAction::Hover)
    //         emit q->highlighted(action);
}


void CTTMenuBarPrivate::_q_actionTriggered()
{
    Q_Q(QMenuBar);
    if (QAction *action = qobject_cast<QAction *>(q->sender())) {
        emit q->triggered(action);
    }
}

void CTTMenuBarPrivate::_q_actionHovered()
{
    Q_Q(QMenuBar);
    if (QAction *action = qobject_cast<QAction *>(q->sender())) {
        emit q->hovered(action);
#ifndef QT_NO_ACCESSIBILITY
        if (QAccessible::isActive()) {
            int actionIndex = actions.indexOf(action);
            QAccessibleEvent focusEvent(q, QAccessible::Focus);
            focusEvent.setChild(actionIndex);
            QAccessible::updateAccessibility(&focusEvent);
        }
#endif //QT_NO_ACCESSIBILITY
    }
}

/*!
    Initialize \a option with the values from the menu bar and information from \a action. This method
    is useful for subclasses when they need a QStyleOptionMenuItem, but don't want
    to fill in all the information themselves.

    \sa QStyleOption::initFrom(), QMenu::initStyleOption()
*/
void CTTMenuBar::initStyleOption(QStyleOptionMenuItem *option, const QAction *action) const
{
    if (!option || !action)
        return;
    Q_D(const QMenuBar);
    option->palette = palette();
    option->state = QStyle::State_None;
    if (isEnabled() && action->isEnabled())
        option->state |= QStyle::State_Enabled;
    else
        option->palette.setCurrentColorGroup(QPalette::Disabled);
    option->fontMetrics = fontMetrics();
    if (d->currentAction && d->currentAction == action) {
        option->state |= QStyle::State_Selected;
        if (d->popupState && !d->closePopupMode)
            option->state |= QStyle::State_Sunken;
    }
    if (hasFocus() || d->currentAction)
        option->state |= QStyle::State_HasFocus;
    option->menuRect = rect();
    option->menuItemType = QStyleOptionMenuItem::Normal;
    option->checkType = QStyleOptionMenuItem::NotCheckable;
    option->text = action->text();
    option->icon = action->icon();
}

void CTTMenuBarPrivate::init()
{
    Q_Q(QMenuBar);
    q->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    q->setAttribute(Qt::WA_CustomWhatsThis);

    if (!QApplication::instance()->testAttribute(Qt::AA_DontUseNativeMenuBar))
        platformMenuBar = QGuiApplicationPrivate::platformTheme()->createPlatformMenuBar();

    if (platformMenuBar)
        q->hide();
    q->setBackgroundRole(QPalette::Button);
    handleReparent();
    q->setMouseTracking(q->style()->styleHint(QStyle::SH_MenuBar_MouseTracking, 0, q));

    extension = new CMenuBarExtension(q);
    extension->setFocusPolicy(Qt::NoFocus);
    extension->hide();
}

//Gets the next action for keyboard navigation
QAction *CTTMenuBarPrivate::getNextAction(const int _start, const int increment) const
{
    Q_Q(const QMenuBar);
    const_cast<CTTMenuBarPrivate*>(this)->updateGeometries();
    bool allowActiveAndDisabled = q->style()->styleHint(QStyle::SH_Menu_AllowActiveAndDisabled, 0, q);
    const int start = (_start == -1 && increment == -1) ? actions.count() : _start;
    const int end = increment == -1 ? 0 : actions.count() - 1;

    for (int i = start; i != end;) {
        i += increment;
        QAction *current = actions.at(i);
        if (!actionRects.at(i).isNull() && (allowActiveAndDisabled || current->isEnabled()))
            return current;
    }

    if (_start != -1) //let's try from the beginning or the end
        return getNextAction(-1, increment);

    return 0;
}

/*!
    Constructs a menu bar with parent \a parent.
*/
CTTMenuBar::QMenuBar(QWidget *parent) : QWidget(*new CTTMenuBarPrivate, parent, 0)
{
    Q_D(QMenuBar);
    d->init();
}


/*!
    Destroys the menu bar.
*/
CTTMenuBar::~QMenuBar()
{
    Q_D(QMenuBar);
    delete d->platformMenuBar;
    d->platformMenuBar = 0;
}

/*!
    \overload

    This convenience function creates a new action with \a text.
    The function adds the newly created action to the menu's
    list of actions, and returns it.

    \sa QWidget::addAction(), QWidget::actions()
*/
QAction *CTTMenuBar::addAction(const QString &text)
{
    QAction *ret = new QAction(text, this);
    addAction(ret);
    return ret;
}

/*!
    \overload

    This convenience function creates a new action with the given \a
    text. The action's triggered() signal is connected to the \a
    receiver's \a member slot. The function adds the newly created
    action to the menu's list of actions and returns it.

    \sa QWidget::addAction(), QWidget::actions()
*/
QAction *CTTMenuBar::addAction(const QString &text, const QObject *receiver, const char* member)
{
    QAction *ret = new QAction(text, this);
    QObject::connect(ret, SIGNAL(triggered(bool)), receiver, member);
    addAction(ret);
    return ret;
}

/*!
  Appends a new QMenu with \a title to the menu bar. The menu bar
  takes ownership of the menu. Returns the new menu.

  \sa QWidget::addAction(), QMenu::menuAction()
*/
QMenu *CTTMenuBar::addMenu(const QString &title)
{
    QMenu *menu = new QMenu(title, this);
    addAction(menu->menuAction());
    return menu;
}

/*!
  Appends a new QMenu with \a icon and \a title to the menu bar. The menu bar
  takes ownership of the menu. Returns the new menu.

  \sa QWidget::addAction(), QMenu::menuAction()
*/
QMenu *CTTMenuBar::addMenu(const QIcon &icon, const QString &title)
{
    QMenu *menu = new QMenu(title, this);
    menu->setIcon(icon);
    addAction(menu->menuAction());
    return menu;
}

/*!
    Appends \a menu to the menu bar. Returns the menu's menuAction().

    \note The returned QAction object can be used to hide the corresponding
    menu.

    \sa QWidget::addAction(), QMenu::menuAction()
*/
QAction *CTTMenuBar::addMenu(QMenu *menu)
{
    QAction *action = menu->menuAction();
    addAction(action);
    return action;
}

/*!
  Appends a separator to the menu.
*/
QAction *CTTMenuBar::addSeparator()
{
    QAction *ret = new QAction(this);
    ret->setSeparator(true);
    addAction(ret);
    return ret;
}

/*!
    This convenience function creates a new separator action, i.e. an
    action with QAction::isSeparator() returning true. The function inserts
    the newly created action into this menu bar's list of actions before
    action \a before and returns it.

    \sa QWidget::insertAction(), addSeparator()
*/
QAction *CTTMenuBar::insertSeparator(QAction *before)
{
    QAction *action = new QAction(this);
    action->setSeparator(true);
    insertAction(before, action);
    return action;
}

/*!
  This convenience function inserts \a menu before action \a before
  and returns the menus menuAction().

  \sa QWidget::insertAction(), addMenu()
*/
QAction *CTTMenuBar::insertMenu(QAction *before, QMenu *menu)
{
    QAction *action = menu->menuAction();
    insertAction(before, action);
    return action;
}

/*!
  Returns the QAction that is currently highlighted. A null pointer
  will be returned if no action is currently selected.
*/
QAction *CTTMenuBar::activeAction() const
{
    Q_D(const QMenuBar);
    return d->currentAction;
}

/*!
    \since 4.1

    Sets the currently highlighted action to \a act.
*/
void CTTMenuBar::setActiveAction(QAction *act)
{
    Q_D(QMenuBar);
    d->setCurrentAction(act, true, false);
}


/*!
    Removes all the actions from the menu bar.

    \note On \macos, menu items that have been merged to the system
    menu bar are not removed by this function. One way to handle this
    would be to remove the extra actions yourself. You can set the
    \l{QAction::MenuRole}{menu role} on the different menus, so that
    you know ahead of time which menu items get merged and which do
    not. Then decide what to recreate or remove yourself.

    \sa removeAction()
*/
void CTTMenuBar::clear()
{
    QList<QAction*> acts = actions();
    for (int i = 0; i < acts.size(); i++)
        removeAction(acts[i]);
}

/*!
    \property CTTMenuBar::defaultUp
    \brief the popup orientation

    The default popup orientation. By default, menus pop "down" the
    screen. By setting the property to true, the menu will pop "up".
    You might call this for menus that are \e below the document to
    which they refer.

    If the menu would not fit on the screen, the other direction is
    used automatically.
*/
void CTTMenuBar::setDefaultUp(bool b)
{
    Q_D(QMenuBar);
    d->defaultPopDown = !b;
}

bool CTTMenuBar::isDefaultUp() const
{
    Q_D(const QMenuBar);
    return !d->defaultPopDown;
}

/*!
  \reimp
*/
void CTTMenuBar::resizeEvent(QResizeEvent *)
{
    Q_D(QMenuBar);
    d->itemsDirty = true;
    d->updateGeometries();
}

/*!
  \reimp
*/
void CTTMenuBar::paintEvent(QPaintEvent *e)
{
    Q_D(QMenuBar);
    QPainter p(this);
    QRegion emptyArea(rect());

    //draw the items
    for (int i = 0; i < d->actions.count(); ++i) {
        QAction *action = d->actions.at(i);
        QRect adjustedActionRect = d->actionRect(action);
        if (adjustedActionRect.isEmpty() || !d->isVisible(action))
            continue;
        if (!e->rect().intersects(adjustedActionRect))
            continue;

        emptyArea -= adjustedActionRect;
        QStyleOptionMenuItem opt;
        initStyleOption(&opt, action);
        opt.rect = adjustedActionRect;
        p.setClipRect(adjustedActionRect);
        style()->drawControl(QStyle::CE_MenuBarItem, &opt, &p, this);
    }
    //draw border
    if (int fw = style()->pixelMetric(QStyle::PM_MenuBarPanelWidth, 0, this)) {
        QRegion borderReg;
        borderReg += QRect(0, 0, fw, height()); //left
        borderReg += QRect(width() - fw, 0, fw, height()); //right
        borderReg += QRect(0, 0, width(), fw); //top
        borderReg += QRect(0, height() - fw, width(), fw); //bottom
        p.setClipRegion(borderReg);
        emptyArea -= borderReg;
        QStyleOptionFrame frame;
        frame.rect = rect();
        frame.palette = palette();
        frame.state = QStyle::State_None;
        frame.lineWidth = style()->pixelMetric(QStyle::PM_MenuBarPanelWidth);
        frame.midLineWidth = 0;
        style()->drawPrimitive(QStyle::PE_PanelMenuBar, &frame, &p, this);
    }
    p.setClipRegion(emptyArea);
    QStyleOptionMenuItem menuOpt;
    menuOpt.palette = palette();
    menuOpt.state = QStyle::State_None;
    menuOpt.menuItemType = QStyleOptionMenuItem::EmptyArea;
    menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
    menuOpt.rect = rect();
    menuOpt.menuRect = rect();
    style()->drawControl(QStyle::CE_MenuBarEmptyArea, &menuOpt, &p, this);
}

/*!
  \reimp
*/
void CTTMenuBar::setVisible(bool visible)
{
    if (isNativeMenuBar()) {
        if (!visible)
            QWidget::setVisible(false);
        return;
    }
    QWidget::setVisible(visible);
}

/*!
  \reimp
*/
void CTTMenuBar::mousePressEvent(QMouseEvent *e)
{
    Q_D(QMenuBar);
    if (e->button() != Qt::LeftButton)
        return;

    d->mouseDown = true;

    QAction *action = d->actionAt(e->pos());
    if (!action || !d->isVisible(action) || !action->isEnabled()) {
        d->setCurrentAction(0);
#if QT_CONFIG(whatsthis)
        if (QWhatsThis::inWhatsThisMode())
            QWhatsThis::showText(e->globalPos(), d->whatsThis, this);
#endif
        return;
    }

    if (d->currentAction == action && d->popupState) {
        if (QMenu *menu = d->activeMenu) {
            d->activeMenu = 0;
            menu->setAttribute(Qt::WA_NoMouseReplay);
            menu->hide();
        }
    }
    else {
        d->setCurrentAction(action, true);
    }
}

/*!
  \reimp
*/
void CTTMenuBar::mouseReleaseEvent(QMouseEvent *e)
{
    Q_D(QMenuBar);
    if (e->button() != Qt::LeftButton || !d->mouseDown)
        return;

    d->mouseDown = false;
    QAction *action = d->actionAt(e->pos());
    if ((d->closePopupMode && action == d->currentAction) || !action || !action->menu()) {
        //we set the current action before activating
        //so that we let the leave event set the current back to 0
        d->setCurrentAction(action, false);
        if (action)
            d->activateAction(action, QAction::Trigger);
    }
    d->closePopupMode = 0;
}

/*!
  \reimp
*/
void CTTMenuBar::keyPressEvent(QKeyEvent *e)
{
    Q_D(QMenuBar);
    d->updateGeometries();
    int key = e->key();
    if (isRightToLeft()) {  // in reverse mode open/close key for submenues are reversed
        if (key == Qt::Key_Left)
            key = Qt::Key_Right;
        else if (key == Qt::Key_Right)
            key = Qt::Key_Left;
    }
    if (key == Qt::Key_Tab) //means right
        key = Qt::Key_Right;
    else if (key == Qt::Key_Backtab) //means left
        key = Qt::Key_Left;

    bool key_consumed = false;
    switch (key) {
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Enter:
    case Qt::Key_Space:
    case Qt::Key_Return: {
        if (!style()->styleHint(QStyle::SH_MenuBar_AltKeyNavigation, 0, this) || !d->currentAction)
            break;
        if (d->currentAction->menu()) {
            d->popupAction(d->currentAction, true);
        }
        else if (key == Qt::Key_Enter || key == Qt::Key_Return || key == Qt::Key_Space) {
            d->activateAction(d->currentAction, QAction::Trigger);
            d->setCurrentAction(d->currentAction, false);
            d->setKeyboardMode(false);
        }
        key_consumed = true;
        break; }

    case Qt::Key_Right:
    case Qt::Key_Left: {
        if (d->currentAction) {
            int index = d->actions.indexOf(d->currentAction);
            if (QAction *nextAction = d->getNextAction(index, key == Qt::Key_Left ? -1 : +1)) {
                d->setCurrentAction(nextAction, d->popupState, true);
                key_consumed = true;
            }
        }
        break; }

    default:
        key_consumed = false;
    }

#ifndef QT_NO_SHORTCUT
    if (!key_consumed && e->matches(QKeySequence::Cancel)) {
        d->setCurrentAction(0);
        d->setKeyboardMode(false);
        key_consumed = true;
    }
#endif

    if (!key_consumed &&
        (!e->modifiers() ||
        (e->modifiers()&(Qt::MetaModifier | Qt::AltModifier))) && e->text().length() == 1 && !d->popupState) {
        int clashCount = 0;
        QAction *first = 0, *currentSelected = 0, *firstAfterCurrent = 0;
        {
            const QChar c = e->text().at(0).toUpper();
            for (int i = 0; i < d->actions.size(); ++i) {
                if (d->actionRects.at(i).isNull())
                    continue;
                QAction *act = d->actions.at(i);
                QString s = act->text();
                if (!s.isEmpty()) {
                    int ampersand = s.indexOf(QLatin1Char('&'));
                    if (ampersand >= 0) {
                        if (s[ampersand + 1].toUpper() == c) {
                            clashCount++;
                            if (!first)
                                first = act;
                            if (act == d->currentAction)
                                currentSelected = act;
                            else if (!firstAfterCurrent && currentSelected)
                                firstAfterCurrent = act;
                        }
                    }
                }
            }
        }
        QAction *next_action = 0;
        if (clashCount >= 1) {
            if (clashCount == 1 || !d->currentAction || (currentSelected && !firstAfterCurrent))
                next_action = first;
            else
                next_action = firstAfterCurrent;
        }
        if (next_action) {
            key_consumed = true;
            d->setCurrentAction(next_action, true, true);
        }
    }
    if (key_consumed)
        e->accept();
    else
        e->ignore();
}

/*!
  \reimp
*/
void CTTMenuBar::mouseMoveEvent(QMouseEvent *e)
{
    Q_D(QMenuBar);
    if (!(e->buttons() & Qt::LeftButton))
        d->mouseDown = false;
    bool popupState = d->popupState || d->mouseDown;
    QAction *action = d->actionAt(e->pos());
    if ((action && d->isVisible(action)) || !popupState)
        d->setCurrentAction(action, popupState);
}

/*!
  \reimp
*/
void CTTMenuBar::leaveEvent(QEvent *)
{
    Q_D(QMenuBar);
    if ((!hasFocus() && !d->popupState) ||
        (d->currentAction && d->currentAction->menu() == 0))
        d->setCurrentAction(0);
}

QPlatformMenu *CTTMenuBarPrivate::getPlatformMenu(const QAction *action)
{
    if (!action || !action->menu())
        return 0;

    QPlatformMenu *platformMenu = action->menu()->platformMenu();
    if (!platformMenu && platformMenuBar) {
        platformMenu = platformMenuBar->createMenu();
        if (platformMenu)
            action->menu()->setPlatformMenu(platformMenu);
    }

    return platformMenu;
}

QPlatformMenu *CTTMenuBarPrivate::findInsertionPlatformMenu(const QAction *action)
{
    Q_Q(QMenuBar);
    QPlatformMenu *beforeMenu = nullptr;
    for (int beforeIndex = indexOf(const_cast<QAction *>(action)) + 1;
        !beforeMenu && (beforeIndex < q->actions().size());
        ++beforeIndex) {
        beforeMenu = getPlatformMenu(q->actions().at(beforeIndex));
    }

    return beforeMenu;
}

void CTTMenuBarPrivate::copyActionToPlatformMenu(const QAction *action, QPlatformMenu *menu)
{
    const auto tag = reinterpret_cast<quintptr>(action);
    if (menu->tag() != tag)
        menu->setTag(tag);
    menu->setText(action->text());
    menu->setVisible(action->isVisible());
    menu->setEnabled(action->isEnabled());
}

/*!
  \reimp
*/
void CTTMenuBar::actionEvent(QActionEvent *e)
{
    Q_D(QMenuBar);
    d->itemsDirty = true;

    if (d->platformMenuBar) {
        QPlatformMenuBar *nativeMenuBar = d->platformMenuBar;
        if (!nativeMenuBar)
            return;

        if (e->type() == QEvent::ActionAdded) {
            QPlatformMenu *menu = d->getPlatformMenu(e->action());
            if (menu) {
                d->copyActionToPlatformMenu(e->action(), menu);

                QPlatformMenu *beforeMenu = d->findInsertionPlatformMenu(e->action());
                d->platformMenuBar->insertMenu(menu, beforeMenu);
            }
        }
        else if (e->type() == QEvent::ActionRemoved) {
            QPlatformMenu *menu = d->getPlatformMenu(e->action());
            if (menu)
                d->platformMenuBar->removeMenu(menu);
        }
        else if (e->type() == QEvent::ActionChanged) {
            QPlatformMenu *cur = d->platformMenuBar->menuForTag(reinterpret_cast<quintptr>(e->action()));
            QPlatformMenu *menu = d->getPlatformMenu(e->action());

            // the menu associated with the action can change, need to
            // remove and/or insert the new platform menu
            if (menu != cur) {
                if (cur)
                    d->platformMenuBar->removeMenu(cur);
                if (menu) {
                    d->copyActionToPlatformMenu(e->action(), menu);

                    QPlatformMenu *beforeMenu = d->findInsertionPlatformMenu(e->action());
                    d->platformMenuBar->insertMenu(menu, beforeMenu);
                }
            }
            else if (menu) {
                d->copyActionToPlatformMenu(e->action(), menu);
                d->platformMenuBar->syncMenu(menu);
            }
        }
    }

    if (e->type() == QEvent::ActionAdded) {
        connect(e->action(), SIGNAL(triggered()), this, SLOT(_q_actionTriggered()));
        connect(e->action(), SIGNAL(hovered()), this, SLOT(_q_actionHovered()));
    }
    else if (e->type() == QEvent::ActionRemoved) {
        e->action()->disconnect(this);
    }
    // updateGeometries() is also needed for native menu bars because
    // it updates shortcutIndexMap
    if (isVisible() || isNativeMenuBar())
        d->updateGeometries();
    if (isVisible())
        update();
}

/*!
  \reimp
*/
void CTTMenuBar::focusInEvent(QFocusEvent *)
{
    Q_D(QMenuBar);
    if (d->keyboardState)
        d->focusFirstAction();
}

/*!
  \reimp
*/
void CTTMenuBar::focusOutEvent(QFocusEvent *)
{
    Q_D(QMenuBar);
    if (!d->popupState) {
        d->setCurrentAction(0);
        d->setKeyboardMode(false);
    }
}

/*!
  \reimp
 */
void CTTMenuBar::timerEvent(QTimerEvent *e)
{
    Q_D(QMenuBar);
    if (e->timerId() == d->autoReleaseTimer.timerId()) {
        d->autoReleaseTimer.stop();
        d->setCurrentAction(0);
    }
    QWidget::timerEvent(e);
}


void CTTMenuBarPrivate::handleReparent()
{
    Q_Q(QMenuBar);
    QWidget *newParent = q->parentWidget();

    //Note: if parent is reparented, then window may change even if parent doesn't.
    // We need to install an avent filter on each parent up to the parent that is
    // also a window (for shortcuts)
    QWidget *newWindow = newParent ? newParent->window() : Q_NULLPTR;

    QVector<QPointer<QWidget> > newParents;
    // Remove event filters on ex-parents, keep them on still-parents
    // The parents are always ordered in the vector
    foreach(const QPointer<QWidget> &w, oldParents) {
        if (w) {
            if (newParent == w) {
                newParents.append(w);
                if (newParent != newWindow) //stop at the window
                    newParent = newParent->parentWidget();
            }
            else {
                w->removeEventFilter(q);
            }
        }
    }

    // At this point, newParent is the next one to be added to newParents
    while (newParent && newParent != newWindow) {
        //install event filters all the way up to (excluding) the window
        newParents.append(newParent);
        newParent->installEventFilter(q);
        newParent = newParent->parentWidget();
    }

    if (newParent && newWindow) {
        // Install the event filter on the window
        newParents.append(newParent);
        newParent->installEventFilter(q);
    }
    oldParents = newParents;

    if (platformMenuBar) {
        if (newWindow) {
            // force the underlying platform window to be created, since
            // the platform menubar needs it (and we have no other way to
            // discover when the platform window is created)
            newWindow->createWinId();
            platformMenuBar->handleReparent(newWindow->windowHandle());
        }
        else {
            platformMenuBar->handleReparent(0);
        }
    }
}

/*!
  \reimp
*/
void CTTMenuBar::changeEvent(QEvent *e)
{
    Q_D(QMenuBar);
    if (e->type() == QEvent::StyleChange) {
        d->itemsDirty = true;
        setMouseTracking(style()->styleHint(QStyle::SH_MenuBar_MouseTracking, 0, this));
        if (parentWidget())
            resize(parentWidget()->width(), heightForWidth(parentWidget()->width()));
        d->updateGeometries();
    }
    else if (e->type() == QEvent::ParentChange) {
        d->handleReparent();
    }
    else if (e->type() == QEvent::FontChange
        || e->type() == QEvent::ApplicationFontChange) {
        d->itemsDirty = true;
        d->updateGeometries();
    }

    QWidget::changeEvent(e);
}

/*!
  \reimp
*/
bool CTTMenuBar::event(QEvent *e)
{
    Q_D(QMenuBar);
    switch (e->type()) {
    case QEvent::KeyPress: {
        QKeyEvent *ke = (QKeyEvent*)e;
#if 0
        if (!d->keyboardState) { //all keypresses..
            d->setCurrentAction(0);
            return;
        }
#endif
        if (ke->key() == Qt::Key_Tab || ke->key() == Qt::Key_Backtab) {
            keyPressEvent(ke);
            return true;
        }

    } break;
#ifndef QT_NO_SHORTCUT
    case QEvent::Shortcut: {
        QShortcutEvent *se = static_cast<QShortcutEvent *>(e);
        int shortcutId = se->shortcutId();
        for (int j = 0; j < d->shortcutIndexMap.size(); ++j) {
            if (shortcutId == d->shortcutIndexMap.value(j))
                d->_q_internalShortcutActivated(j);
        }
    } break;
#endif
    case QEvent::Show:
        d->_q_updateLayout();
        break;
#ifndef QT_NO_SHORTCUT
    case QEvent::ShortcutOverride: {
        QKeyEvent *kev = static_cast<QKeyEvent*>(e);
        //we only filter out escape if there is a current action
        if (kev->matches(QKeySequence::Cancel) && d->currentAction) {
            e->accept();
            return true;
        }
    }
                                   break;
#endif
#if QT_CONFIG(whatsthis)
    case QEvent::QueryWhatsThis:
        e->setAccepted(d->whatsThis.size());
        if (QAction *action = d->actionAt(static_cast<QHelpEvent*>(e)->pos())) {
            if (action->whatsThis().size() || action->menu())
                e->accept();
        }
        return true;
#endif
    case QEvent::LayoutDirectionChange:
        d->_q_updateLayout();
        break;
    default:
        break;
    }
    return QWidget::event(e);
}

/*!
  \reimp
*/
bool CTTMenuBar::eventFilter(QObject *object, QEvent *event)
{
    Q_D(QMenuBar);
    if (object && (event->type() == QEvent::ParentChange)) //GrandparentChange
        d->handleReparent();

    if (object == d->leftWidget || object == d->rightWidget) {
        switch (event->type()) {
        case QEvent::ShowToParent:
        case QEvent::HideToParent:
            d->_q_updateLayout();
            break;
        default:
            break;
        }
    }

    if (isNativeMenuBar() && event->type() == QEvent::ShowToParent) {
        // On some desktops like Unity, the D-Bus menu bar is unregistered
        // when the window is hidden. So when the window is shown, we need
        // to forcefully re-register it. The only way to force re-registering
        // with D-Bus menu is the handleReparent method.
        QWidget *widget = qobject_cast<QWidget *>(object);
        QWindow *handle = widget ? widget->windowHandle() : nullptr;
        if (handle != nullptr)
            d->platformMenuBar->handleReparent(handle);
    }

    if (style()->styleHint(QStyle::SH_MenuBar_AltKeyNavigation, 0, this)) {
        if (d->altPressed) {
            switch (event->type()) {
            case QEvent::KeyPress:
            case QEvent::KeyRelease:
            {
                QKeyEvent *kev = static_cast<QKeyEvent*>(event);
                if (kev->key() == Qt::Key_Alt || kev->key() == Qt::Key_Meta) {
                    if (event->type() == QEvent::KeyPress) // Alt-press does not interest us, we have the shortcut-override event
                        break;
                    d->setKeyboardMode(!d->keyboardState);
                }
            }
            // fall through
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonRelease:
            case QEvent::MouseMove:
            case QEvent::FocusIn:
            case QEvent::FocusOut:
            case QEvent::ActivationChange:
            case QEvent::Shortcut:
                d->altPressed = false;
                qApp->removeEventFilter(this);
                break;
            default:
                break;
            }
        }
        else if (isVisible()) {
            if (event->type() == QEvent::ShortcutOverride) {
                QKeyEvent *kev = static_cast<QKeyEvent*>(event);
                if ((kev->key() == Qt::Key_Alt || kev->key() == Qt::Key_Meta)
                    && kev->modifiers() == Qt::AltModifier) {
                    d->altPressed = true;
                    qApp->installEventFilter(this);
                }
            }
        }
    }

    return false;
}

/*!
  Returns the QAction at \a pt. Returns 0 if there is no action at \a pt or if
the location has a separator.

    \sa addAction(), addSeparator()
*/
QAction *CTTMenuBar::actionAt(const QPoint &pt) const
{
    Q_D(const QMenuBar);
    return d->actionAt(pt);
}

/*!
  Returns the geometry of action \a act as a QRect.

    \sa actionAt()
*/
QRect CTTMenuBar::actionGeometry(QAction *act) const
{
    Q_D(const QMenuBar);
    return d->actionRect(act);
}

/*!
  \reimp
*/
QSize CTTMenuBar::minimumSizeHint() const
{
    Q_D(const QMenuBar);
    const bool as_gui_menubar = !isNativeMenuBar();

    ensurePolished();
    QSize ret(0, 0);
    const_cast<CTTMenuBarPrivate*>(d)->updateGeometries();
    const int hmargin = style()->pixelMetric(QStyle::PM_MenuBarHMargin, 0, this);
    const int vmargin = style()->pixelMetric(QStyle::PM_MenuBarVMargin, 0, this);
    int fw = style()->pixelMetric(QStyle::PM_MenuBarPanelWidth, 0, this);
    int spaceBelowMenuBar = style()->styleHint(QStyle::SH_MainWindow_SpaceBelowMenuBar, 0, this);
    if (as_gui_menubar) {
        int w = parentWidget() ? parentWidget()->width() : QApplication::desktop()->width();
        d->calcActionRects(w - (2 * fw), 0);
        for (int i = 0; ret.isNull() && i < d->actions.count(); ++i)
            ret = d->actionRects.at(i).size();
        if (!d->extension->isHidden())
            ret += QSize(d->extension->sizeHint().width(), 0);
        ret += QSize(2 * fw + hmargin, 2 * fw + vmargin);
    }
    int margin = 2 * vmargin + 2 * fw + spaceBelowMenuBar;
    if (d->leftWidget) {
        QSize sz = d->leftWidget->minimumSizeHint();
        ret.setWidth(ret.width() + sz.width());
        if (sz.height() + margin > ret.height())
            ret.setHeight(sz.height() + margin);
    }
    if (d->rightWidget) {
        QSize sz = d->rightWidget->minimumSizeHint();
        ret.setWidth(ret.width() + sz.width());
        if (sz.height() + margin > ret.height())
            ret.setHeight(sz.height() + margin);
    }
    if (as_gui_menubar) {
        QStyleOptionMenuItem opt;
        opt.rect = rect();
        opt.menuRect = rect();
        opt.state = QStyle::State_None;
        opt.menuItemType = QStyleOptionMenuItem::Normal;
        opt.checkType = QStyleOptionMenuItem::NotCheckable;
        opt.palette = palette();
        return (style()->sizeFromContents(QStyle::CT_MenuBar, &opt,
            ret.expandedTo(QApplication::globalStrut()),
            this));
    }
    return ret;
}

/*!
  \reimp
*/
QSize CTTMenuBar::sizeHint() const
{
    Q_D(const QMenuBar);
    const bool as_gui_menubar = !isNativeMenuBar();

    ensurePolished();
    QSize ret(0, 0);
    const_cast<CTTMenuBarPrivate*>(d)->updateGeometries();
    const int hmargin = style()->pixelMetric(QStyle::PM_MenuBarHMargin, 0, this);
    const int vmargin = style()->pixelMetric(QStyle::PM_MenuBarVMargin, 0, this);
    int fw = style()->pixelMetric(QStyle::PM_MenuBarPanelWidth, 0, this);
    int spaceBelowMenuBar = style()->styleHint(QStyle::SH_MainWindow_SpaceBelowMenuBar, 0, this);
    if (as_gui_menubar) {
        const int w = parentWidget() ? parentWidget()->width() : QApplication::desktop()->width();
        d->calcActionRects(w - (2 * fw), 0);
        for (int i = 0; i < d->actionRects.count(); ++i) {
            const QRect &actionRect = d->actionRects.at(i);
            ret = ret.expandedTo(QSize(actionRect.x() + actionRect.width(), actionRect.y() + actionRect.height()));
        }
        //the action geometries already contain the top and left
        //margins. So we only need to add those from right and bottom.
        ret += QSize(fw + hmargin, fw + vmargin);
    }
    int margin = 2 * vmargin + 2 * fw + spaceBelowMenuBar;
    if (d->leftWidget) {
        QSize sz = d->leftWidget->sizeHint();
        sz.rheight() += margin;
        ret = ret.expandedTo(sz);
    }
    if (d->rightWidget) {
        QSize sz = d->rightWidget->sizeHint();
        ret.setWidth(ret.width() + sz.width());
        if (sz.height() + margin > ret.height())
            ret.setHeight(sz.height() + margin);
    }
    if (as_gui_menubar) {
        QStyleOptionMenuItem opt;
        opt.rect = rect();
        opt.menuRect = rect();
        opt.state = QStyle::State_None;
        opt.menuItemType = QStyleOptionMenuItem::Normal;
        opt.checkType = QStyleOptionMenuItem::NotCheckable;
        opt.palette = palette();
        return (style()->sizeFromContents(QStyle::CT_MenuBar, &opt,
            ret.expandedTo(QApplication::globalStrut()),
            this));
    }
    return ret;
}

/*!
  \reimp
*/
int CTTMenuBar::heightForWidth(int) const
{
    Q_D(const QMenuBar);
    const bool as_gui_menubar = !isNativeMenuBar();

    const_cast<CTTMenuBarPrivate*>(d)->updateGeometries();
    int height = 0;
    const int vmargin = style()->pixelMetric(QStyle::PM_MenuBarVMargin, 0, this);
    int fw = style()->pixelMetric(QStyle::PM_MenuBarPanelWidth, 0, this);
    int spaceBelowMenuBar = style()->styleHint(QStyle::SH_MainWindow_SpaceBelowMenuBar, 0, this);
    if (as_gui_menubar) {
        for (int i = 0; i < d->actionRects.count(); ++i)
            height = qMax(height, d->actionRects.at(i).height());
        if (height) //there is at least one non-null item
            height += spaceBelowMenuBar;
        height += 2 * fw;
        height += 2 * vmargin;
    }
    int margin = 2 * vmargin + 2 * fw + spaceBelowMenuBar;
    if (d->leftWidget)
        height = qMax(d->leftWidget->sizeHint().height() + margin, height);
    if (d->rightWidget)
        height = qMax(d->rightWidget->sizeHint().height() + margin, height);
    if (as_gui_menubar) {
        QStyleOptionMenuItem opt;
        opt.init(this);
        opt.menuRect = rect();
        opt.state = QStyle::State_None;
        opt.menuItemType = QStyleOptionMenuItem::Normal;
        opt.checkType = QStyleOptionMenuItem::NotCheckable;
        return style()->sizeFromContents(QStyle::CT_MenuBar, &opt, QSize(0, height), this).height(); //not pretty..
    }
    return height;
}

/*!
  \internal
*/
void CTTMenuBarPrivate::_q_internalShortcutActivated(int id)
{
    Q_Q(QMenuBar);
    QAction *act = actions.at(id);
    if (act && act->menu()) {
        if (QPlatformMenu *platformMenu = act->menu()->platformMenu()) {
            platformMenu->showPopup(q->windowHandle(), actionRects.at(id), Q_NULLPTR);
            return;
        }
    }

    keyboardFocusWidget = QApplication::focusWidget();
    setCurrentAction(act, true, true);
    if (act && !act->menu()) {
        activateAction(act, QAction::Trigger);
        //100 is the same as the default value in QPushButton::animateClick
        autoReleaseTimer.start(100, q);
    }
    else if (act && q->style()->styleHint(QStyle::SH_MenuBar_AltKeyNavigation, 0, q)) {
        // When we open a menu using a shortcut, we should end up in keyboard state
        setKeyboardMode(true);
    }
}

void CTTMenuBarPrivate::_q_updateLayout()
{
    Q_Q(QMenuBar);
    itemsDirty = true;
    if (q->isVisible()) {
        updateGeometries();
        q->update();
    }
}

/*!
    \fn void CTTMenuBar::setCornerWidget(QWidget *widget, Qt::Corner corner)

    This sets the given \a widget to be shown directly on the left of the first
    menu item, or on the right of the last menu item, depending on \a corner.

    The menu bar takes ownership of \a widget, reparenting it into the menu bar.
    However, if the \a corner already contains a widget, this previous widget
    will no longer be managed and will still be a visible child of the menu bar.

   \note Using a corner other than Qt::TopRightCorner or Qt::TopLeftCorner
    will result in a warning.
*/
void CTTMenuBar::setCornerWidget(QWidget *w, Qt::Corner corner)
{
    Q_D(QMenuBar);
    switch (corner) {
    case Qt::TopLeftCorner:
        if (d->leftWidget)
            d->leftWidget->removeEventFilter(this);
        d->leftWidget = w;
        break;
    case Qt::TopRightCorner:
        if (d->rightWidget)
            d->rightWidget->removeEventFilter(this);
        d->rightWidget = w;
        break;
    default:
        qWarning("CTTMenuBar::setCornerWidget: Only TopLeftCorner and TopRightCorner are supported");
        return;
    }

    if (w) {
        w->setParent(this);
        w->installEventFilter(this);
    }

    d->_q_updateLayout();
}

/*!
    Returns the widget on the left of the first or on the right of the last menu
    item, depending on \a corner.

   \note Using a corner other than Qt::TopRightCorner or Qt::TopLeftCorner
    will result in a warning.
*/
QWidget *CTTMenuBar::cornerWidget(Qt::Corner corner) const
{
    Q_D(const QMenuBar);
    QWidget *w = 0;
    switch (corner) {
    case Qt::TopLeftCorner:
        w = d->leftWidget;
        break;
    case Qt::TopRightCorner:
        w = d->rightWidget;
        break;
    default:
        qWarning("CTTMenuBar::cornerWidget: Only TopLeftCorner and TopRightCorner are supported");
        break;
    }

    return w;
}

/*!
    \property CTTMenuBar::nativeMenuBar
    \brief Whether or not a menubar will be used as a native menubar on platforms that support it
    \since 4.6

    This property specifies whether or not the menubar should be used as a native menubar on
    platforms that support it. The currently supported platforms are \macos, and
    Linux desktops which use the com.canonical.dbusmenu D-Bus interface (such as Ubuntu Unity).
    If this property is \c true, the menubar is used in the native menubar and is not in the window of
    its parent; if \c false the menubar remains in the window. On other platforms,
    setting this attribute has no effect, and reading this attribute will always return \c false.

    The default is to follow whether the Qt::AA_DontUseNativeMenuBar attribute
    is set for the application. Explicitly setting this property overrides
    the presence (or absence) of the attribute.
*/

void CTTMenuBar::setNativeMenuBar(bool nativeMenuBar)
{
    Q_D(QMenuBar);
    if (nativeMenuBar != bool(d->platformMenuBar)) {
        if (!nativeMenuBar) {
            delete d->platformMenuBar;
            d->platformMenuBar = 0;
        }
        else {
            if (!d->platformMenuBar)
                d->platformMenuBar = QGuiApplicationPrivate::platformTheme()->createPlatformMenuBar();
        }

        updateGeometry();
        if (!nativeMenuBar && parentWidget())
            setVisible(true);
    }
}

bool CTTMenuBar::isNativeMenuBar() const
{
    Q_D(const QMenuBar);
    return bool(d->platformMenuBar);
}

/*!
    \internal
*/
QPlatformMenuBar *CTTMenuBar::platformMenuBar()
{
    Q_D(const QMenuBar);
    return d->platformMenuBar;
}

/*!
    \fn void CTTMenuBar::triggered(QAction *action)

    This signal is emitted when an action in a menu belonging to this menubar
    is triggered as a result of a mouse click; \a action is the action that
    caused the signal to be emitted.

    \note QMenuBar has to have ownership of the QMenu in order this signal to work.

    Normally, you connect each menu action to a single slot using
    QAction::triggered(), but sometimes you will want to connect
    several items to a single slot (most often if the user selects
    from an array). This signal is useful in such cases.

    \sa hovered(), QAction::triggered()
*/

/*!
    \fn void CTTMenuBar::hovered(QAction *action)

    This signal is emitted when a menu action is highlighted; \a action
    is the action that caused the event to be sent.

    Often this is used to update status information.

    \sa triggered(), QAction::hovered()
*/

// for private slots



