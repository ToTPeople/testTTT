#include "cmenubar.h"
#include <QMenu>
#include <QApplication>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QVector>
#include <QDebug>
#include <QEvent>
#include <QFile>
#include <QMouseEvent>
#include <QCursor>
#include "common_define.h"
#include "cmenuitem.h"
#include "ctranlatorhelper.h"


namespace {
    //const int g_nFixedMenuItemWidth = 100;
}

class CMenuBarPrivate
{
    friend CMenuBar;
public:
    typedef struct MenuItemTrans {
        CMenuItem*  pMenuItem;
        QString     strText;
    } s_MenuItemTrans;
    typedef struct ActionTrans {
        QAction*    pAction;
        QString     strText;
    } s_ActionTrans;
public:
    CMenuBarPrivate(CMenuBar* pMenuBar);
    ~CMenuBarPrivate();

public:
    // CMenuItem
    void insertMenuBarItem(CMenuItem* pMenuBarItem, int nIndex = -1);

    //void recordTranslorText(CMenuItem* pMenuItem, const QString& strText);
    //void retranslateUi();

    CMenuItem* posAt(QPoint pos);

public:
    CMenuBar*               m_pMenuBar;
    QHBoxLayout*            m_pHLayout;
    QVector<CMenuItem*>     m_vecMenusItem;
    int                     m_nCnt;                 // menu item count
    CMenuItem*              m_pPreSelectedItem;     // current selected menu item
    //QList<MenuItemTrans*>   m_listMenuItemTrans;        // ≥°æ∞easy£¨easy µœ÷
    //QList<s_ActionTrans*>   m_listActionTrans;
};

CMenuBarPrivate::CMenuBarPrivate(CMenuBar* pMenuBar)
    : m_pMenuBar(pMenuBar)
    , m_pHLayout(NULL)
    , m_nCnt(0)
    , m_pPreSelectedItem(NULL)
{
    if (NULL != m_pMenuBar) {
        m_pHLayout = new QHBoxLayout(m_pMenuBar);
        m_pHLayout->setObjectName(QStringLiteral("menubarHLayout"));
        QSpacerItem* horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        m_pHLayout->addItem(horizontalSpacer);
        m_pHLayout->setSpacing(0);
        m_pHLayout->setMargin(0);
    }
    m_vecMenusItem.clear();
    //m_listMenuItemTrans.clear();
    //m_listActionTrans.clear();
}

CMenuBarPrivate::~CMenuBarPrivate()
{
    m_pMenuBar = NULL;
    /*QList<s_MenuItemTrans*>::iterator it = m_listMenuItemTrans.begin();
    s_MenuItemTrans* pMenuTrans = NULL;
    for (; it != m_listMenuItemTrans.end();) {
        pMenuTrans = *it;
        it = m_listMenuItemTrans.erase(it);
        if (NULL != pMenuTrans) {
            delete pMenuTrans;
            pMenuTrans = NULL;
        }
    }
    m_listMenuItemTrans.clear();

    s_ActionTrans* pActionTrans = NULL;
    QList<s_ActionTrans*>::iterator itAction = m_listActionTrans.begin();
    for (; itAction != m_listActionTrans.end();) {
        pActionTrans = *itAction;
        itAction = m_listActionTrans.erase(itAction);
        if (NULL != pActionTrans) {
            delete pActionTrans;
            pActionTrans = NULL;
        }
    }
    m_listActionTrans.clear();*/
}

void CMenuBarPrivate::insertMenuBarItem(CMenuItem* pMenuBarItem, int nIndex /* = -1 */)
{
    if (NULL == pMenuBarItem) {
        return;
    }

    pMenuBarItem->installEventFilter(m_pMenuBar);
    pMenuBarItem->menu()->installEventFilter(m_pMenuBar);
    pMenuBarItem->menu()->menuAction()->installEventFilter(m_pMenuBar);

    if (nIndex <= 0 || nIndex >= m_nCnt) {
        m_vecMenusItem.push_back(pMenuBarItem);
        m_pHLayout->insertWidget(m_nCnt, pMenuBarItem);
        ++m_nCnt;
    }
    else {
        int nPreIndex = -1;
        if (NULL != m_vecMenusItem[nIndex]) {
            CMenuItem* pItem = m_vecMenusItem[nIndex];
            nPreIndex = m_pHLayout->indexOf(pItem);
            m_pHLayout->removeWidget(pItem);
            m_vecMenusItem.remove(nIndex);
            DELETE(pItem);
        }

        m_vecMenusItem[nIndex] = pMenuBarItem;
        if (nPreIndex < 0) {
            m_pHLayout->insertWidget(m_nCnt, pMenuBarItem);
            ++m_nCnt;
        }
        else {
            m_pHLayout->insertWidget(nPreIndex, pMenuBarItem);
        }
    }

    QObject::disconnect(pMenuBarItem, SIGNAL(pressed()), pMenuBarItem, SLOT(_q_popupPressed()));
}

//void CMenuBarPrivate::recordTranslorText(CMenuItem* pMenuItem, const QString& strText)
//{
//    if (NULL == pMenuItem) {
//        return;
//    }
//
//    s_MenuItemTrans* pMenuItemTrans = new s_MenuItemTrans();
//    if (NULL != pMenuItemTrans) {
//        pMenuItemTrans->pMenuItem = pMenuItem;
//        pMenuItemTrans->strText = strText;
//
//        m_listMenuItemTrans.push_back(pMenuItemTrans);
//    }
//    QMenu* pMenu = pMenuItem->menu();
//    if (NULL != pMenu) {
//        QList<QAction*> listActions = pMenu->actions();
//        QList<QAction*>::iterator it = listActions.begin();
//        
//        QAction* pAction = NULL;
//        for (; it != listActions.end(); ++it) {
//            pAction = *it;
//            if (NULL != pAction) {
//                s_ActionTrans* pActionTrans = new s_ActionTrans();
//                if (NULL != pActionTrans) {
//                    pActionTrans->pAction = pAction;
//                    pActionTrans->strText = pAction->text();
//
//                    m_listActionTrans.push_back(pActionTrans);
//                }
//            }
//        }
//    }
//
//    pMenuItem->setText(QApplication::translate(m_pMenuBar->objectName().toLatin1(), strText.toLatin1(), Q_NULLPTR));
//}
//
//void CMenuBarPrivate::retranslateUi()
//{
//    QList<s_MenuItemTrans*>::iterator it = m_listMenuItemTrans.begin();
//    s_MenuItemTrans* pMenuTrans = NULL;
//    for (; it != m_listMenuItemTrans.end(); ++it) {
//        pMenuTrans = *it;
//        if (NULL != pMenuTrans && NULL != pMenuTrans->pMenuItem) {
//            pMenuTrans->pMenuItem->setText(
//                QApplication::translate(m_pMenuBar->objectName().toLatin1(), pMenuTrans->strText.toLatin1(), Q_NULLPTR)
//            );
//        }
//    }
//
//    s_ActionTrans* pActionTrans = NULL;
//    QList<s_ActionTrans*>::iterator itAction = m_listActionTrans.begin();
//    for (; itAction != m_listActionTrans.end();) {
//        pActionTrans = *itAction;
//        if (NULL != pActionTrans && NULL != pActionTrans->pAction) {
//            pActionTrans->pAction->setText(
//                QApplication::translate(m_pMenuBar->objectName().toLatin1(), pActionTrans->strText.toLatin1(), Q_NULLPTR)
//            );
//        }
//    }
//}

CMenuItem* CMenuBarPrivate::posAt(QPoint pos)
{
    CMenuItem* pTarget = NULL;
    QVector<CMenuItem*>::iterator it = m_vecMenusItem.begin();
    for (; it != m_vecMenusItem.end(); ++it) {
        QPoint st = (*it)->mapToGlobal(QPoint(0,0));
        QPoint en = (*it)->mapToGlobal(QPoint((*it)->width(), (*it)->height()));
        QRect rc = QRect(st, en);
        if (rc.contains(pos)) {
            pTarget = *it;
            break;
        }
    }

    return pTarget;
}

CMenuBar::CMenuBar(QWidget* parent /*= NULL*/)
    : QWidget(parent)
    , p(*new CMenuBarPrivate(this))
{
}


CMenuBar::~CMenuBar()
{
    delete &p;
}

void CMenuBar::addAction(const QString & strAction)
{
    if (strAction.isNull() || strAction.isEmpty()) {
        return;
    }
    QMenu* pMenu = new QMenu(tr(strAction.toLatin1()));
    addMenu(pMenu);
}

void CMenuBar::addAction(const QString & text, const QObject * receiver, const char * member, const QKeySequence & shortcut)
{
    if (text.isNull() || text.isEmpty()) {
        return;
    }

    QMenu* pMenu = new QMenu(tr(text.toLatin1()));
    addMenu(pMenu);
    if (NULL == receiver && NULL == member && NULL == pMenu->menuAction()) {
        return;
    }

#ifdef QT_NO_SHORTCUT
    Q_UNUSED(shortcut);
#else
    pMenu->menuAction()->setShortcut(shortcut);
#endif
    QObject::connect(pMenu->menuAction(), SIGNAL(triggered(bool)), receiver, member);
}

void CMenuBar::addMenu(QMenu * pMenu)
{
    if (NULL == pMenu) {
        return;
    }

    CMenuItem* pNewItem = new CMenuItem(this);
    if (NULL == pNewItem) {
        return;
    }
    pNewItem->setText(tr(pMenu->title().toLatin1()));
    pNewItem->setMenu(pMenu);

    //p.recordTranslorText(pNewItem, pMenu->title());
    p.insertMenuBarItem(pNewItem);

    g_pTranlatorHelper->addButtonTransText(pNewItem, pMenu->title());
    g_pTranlatorHelper->addMenuTransText(pMenu, pMenu->title(), true);
}

bool CMenuBar::eventFilter(QObject * watched, QEvent * event)
{
    if (NULL == watched || NULL == event) {
        return QWidget::eventFilter(watched, event);
    }

    int eType = event->type();
    if (watched->inherits("QPushButton")) {
        //qDebug() << "==$$$$$$$$$$$$-- button : " << event->type();
        QPushButton* pPressedItem = static_cast<QPushButton*>(watched);
        /*if (QEvent::MouseMove != eType && QEvent::Polish != eType && QEvent::FontChange != eType
            && QEvent::PaletteChange != eType && QEvent::HoverMove != eType
            && QEvent::HoverLeave != eType && QEvent::Paint != eType
            && QEvent::WindowDeactivate != eType && QEvent::Enter != eType
            && QEvent::StyleAnimationUpdate != eType && QEvent::DynamicPropertyChange != eType
            && QEvent::WindowActivate != eType && QEvent::Enter != eType
            && QEvent::HoverEnter != eType && QEvent::InputMethodQuery != eType
            && QEvent::FocusAboutToChange != eType && QEvent::InputMethodQuery != eType
            && QEvent::InputMethodQuery != eType && QEvent::InputMethodQuery != eType) {
            qDebug() << "======= button: " << watched << ", event type: " << event->type();
        }*/
        if (QEvent::MouseButtonPress == eType && NULL != pPressedItem) {
            /*qDebug() << "==$$$$$$$$$$$$-- press : " << pPressedItem
                << ", p.m_pPreSelectedItem: " << p.m_pPreSelectedItem
                << ", isDown: " << pPressedItem->isDown();*/
            if (p.m_pPreSelectedItem == pPressedItem) {
                p.m_pPreSelectedItem->setDown(false);
                p.m_pPreSelectedItem = NULL;
            }
            else {
                p.m_pPreSelectedItem = static_cast<CMenuItem*>(pPressedItem);
                pPressedItem->setDown(true);
                QPoint globalPos = pPressedItem->mapToGlobal(QPoint(0, pPressedItem->height()));
                pPressedItem->menu()->popup(globalPos);
            }
        }
        else if (QEvent::Shortcut == eType && NULL != pPressedItem) {
            //if (pPressedItem != p.m_pPreSelectedItem) {
                //qDebug() << "==$$$$$$$$$$$$-- shortcut : " << event->type();
                // hide previout menu
                if (NULL != p.m_pPreSelectedItem) {
                    p.m_pPreSelectedItem->setDown(false);
                    p.m_pPreSelectedItem->menu()->hide();
                }
                // popup menu
                {
                    p.m_pPreSelectedItem = static_cast<CMenuItem*>(pPressedItem);
                    pPressedItem->setDown(true);
                    QPoint globalPos = pPressedItem->mapToGlobal(QPoint(0, pPressedItem->height()));
                    pPressedItem->menu()->popup(globalPos);
                    pPressedItem->setDown(true);
                }
            //}
            return true;
        }
    }
    else if (watched->inherits("QMenu")) {
        //qDebug() << "==$$$$$$$$$$$$-- menu : " << event->type();
        if (QEvent::MouseMove == event->type()) {
            if (watched != p.m_pPreSelectedItem) {
                QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(event);
                QPushButton* pNew = p.posAt(pMouseEvent->globalPos());
                if (NULL != pNew && pNew != p.m_pPreSelectedItem) {
                    if (NULL != p.m_pPreSelectedItem) {
                        p.m_pPreSelectedItem->setDown(false);
                        p.m_pPreSelectedItem->menu()->hide();
                    }
                    p.m_pPreSelectedItem = static_cast<CMenuItem*>(pNew);
                    if (!pNew->isDown()) {
                        pNew->setDown(true);
                        QPoint globalPos = pNew->mapToGlobal(QPoint(0, pNew->height()));
                        pNew->menu()->popup(globalPos);
                    }
                }
            }
        }
        else if (QEvent::Hide == event->type()) {
            if (NULL != p.m_pPreSelectedItem) {
                //qDebug() << "==$$$$$$$$$$$$-- menu hide : " << event->type();
                // cursor not locate in menuitem, should set here; or set by menuitem self
                QPoint cursorPos = QCursor::pos();
                QPoint st = p.m_pPreSelectedItem->mapToGlobal(QPoint());
                QPoint en = p.m_pPreSelectedItem->mapToGlobal(QPoint(p.m_pPreSelectedItem->width(), p.m_pPreSelectedItem->height()));
                QRect rcItem(st, en);
#if defined Q_OS_WIN
                if (!rcItem.contains(cursorPos)) {
                    p.m_pPreSelectedItem->setDown(false);
                    p.m_pPreSelectedItem = NULL;
                }
#else
                if (!rcItem.contains(cursorPos)) {
                    QEvent etLeave(QEvent::Leave);
                    QApplication::sendEvent(p.m_pPreSelectedItem, &etLeave);
                }
                p.m_pPreSelectedItem->setDown(false);
                p.m_pPreSelectedItem = NULL;
#endif
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

void CMenuBar::resizeEvent(QResizeEvent * event)
{
    QVector<CMenuItem*>::iterator it = p.m_vecMenusItem.begin();
    for (; it != p.m_vecMenusItem.end(); ++it) {
        (*it)->setFixedHeight(height());
    }
}

void CMenuBar::changeEvent(QEvent * event)
{
    if (NULL != event && QEvent::LanguageChange == event->type()) {
        g_pTranlatorHelper->retranslateUi();
        return;
    }

    QWidget::changeEvent(event);
}

int CMenuBar::menuItemSpace()
{
    return p.m_pHLayout->spacing();
}

void CMenuBar::setMenuItemSpace(int nSpace)
{
    if (nSpace < 0) {
        nSpace = 0;
    }
    p.m_pHLayout->setSpacing(nSpace);
}

void CMenuBar::slotShortcut()
{
    qDebug() << "==^^^^^^^^^^^^^^^^^^^^^ m2 shortcut";
}

bool CMenuBar::event(QEvent *event)
{
    int eType = event->type();
    if (QEvent::Shortcut == eType) {
        QShortcutEvent *se = static_cast<QShortcutEvent *>(event);
        int shortcutId = se->shortcutId();
        qDebug() << "==^^^^^^^^^^^^^^^^^^^^^--  shortcut id: " << shortcutId;
    }

    return QWidget::event(event);
}
