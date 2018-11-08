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
#include <QRgb>
#include "common_define.h"
#include "cmenuitem.h"


namespace {
    const int g_nFixedMenuItemWidth = 100;
}

class CMenuBarPrivate
{
    friend CMenuBar;
public:
    CMenuBarPrivate(CMenuBar* pMenuBar);
    ~CMenuBarPrivate();

public:
    QString productStyle(CMenuItem* pItem, const QString& strAttr, const QString& value);

public:
    void insertMenuBarItem(CMenuItem* pMenuBarItem, int nIndex = -1);

    CMenuItem* posAt(QPoint pos);
    CMenuItem* clickMenuItem();

public:
    CMenuBar*               m_pMenuBar;
    QHBoxLayout*            m_pHLayout;
    QVector<CMenuItem*>   m_vecMenusItem;
    int                     m_nCnt;
    CMenuItem*            m_pPreSelectedItem;
    bool                    m_bIsDown;

public:
    bool                    m_bMenuNormalBgSet;
    QColor                  m_colorMenuNormalBg;
    bool                    m_bMenuHoverBgSet;
    QColor                  m_colorMenuHoverBg;
    bool                    m_bMenuSelectedBgSet;
    QColor                  m_colorMenuSelectedBg;
};

CMenuBarPrivate::CMenuBarPrivate(CMenuBar* pMenuBar)
    : m_pMenuBar(pMenuBar)
    , m_pHLayout(NULL)
    , m_nCnt(0)
    , m_pPreSelectedItem(NULL)
    , m_bIsDown(false)
    , m_bMenuHoverBgSet(false)
    , m_bMenuNormalBgSet(false)
    , m_bMenuSelectedBgSet(false)
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
}

CMenuBarPrivate::~CMenuBarPrivate()
{
    m_pMenuBar = NULL;
}

//QString CMenuBarPrivate::productStyle(QPushButton* pItem, const QString& strAttr, const QString& value)
//{
//    QString strStyle;
//    if (NULL != pItem) {
//        strStyle = QString("QPushButton#%1%2%3 { background-color : #%4 }")
//            .arg(pItem->objectName()).arg((strAttr.isEmpty() ? "" : ":")).arg(strAttr).arg(value);
//    }
//    return strStyle;
//}

void CMenuBarPrivate::insertMenuBarItem(CMenuItem* pMenuBarItem, int nIndex /* = -1 */)
{
    if (NULL == pMenuBarItem) {
        return;
    }

    pMenuBarItem->setFixedWidth(g_nFixedMenuItemWidth);
    pMenuBarItem->installEventFilter(m_pMenuBar); ///////////////////////
    pMenuBarItem->menu()->installEventFilter(m_pMenuBar);

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

CMenuItem* CMenuBarPrivate::clickMenuItem()
{
    CMenuItem* pItem = NULL;
    QVector<CMenuItem*>::iterator it = m_vecMenusItem.begin();
    for (; it != m_vecMenusItem.end(); ++it) {
        if ((*it)->isDown()) {
            pItem = *it;
            break;
        }
    }

    return pItem;
}

CMenuBar::CMenuBar(QWidget* parent /*= NULL*/)
    : QWidget(parent)
    , p(*new CMenuBarPrivate(this))
{
    //// style sheet
    //QFile file(kszQssMenuBar);
    //if (file.open(QFile::ReadOnly)) {
    //    setStyleSheet(file.readAll());
    //    file.close();
    //}
}


CMenuBar::~CMenuBar()
{
    delete &p;
}

void CMenuBar::initStyle()
{
    /*QVector<QPushButton*>::iterator it = p.m_vecMenusItem.begin();
    QPushButton* pMenu;
    QString strStyle;
    for (; it != p.m_vecMenusItem.end(); ++it) {
        pMenu = *it;
        if (NULL != pMenu) {
            if (p.m_bMenuNormalBgSet) {
                strStyle += p.productStyle(pMenu, "", QString::number(p.m_colorMenuNormalBg.rgba(), 16));
            }
            if (p.m_bMenuHoverBgSet) {
                strStyle += p.productStyle(pMenu, "hover", QString::number(p.m_colorMenuHoverBg.rgba(), 16));
            }
            if (p.m_bMenuSelectedBgSet) {
                strStyle += p.productStyle(pMenu, "pressed", QString::number(p.m_colorMenuSelectedBg.rgba(), 16));
            }
            qDebug() << "-=-=-=-=- : " << strStyle;
            pMenu->setStyleSheet(strStyle);
        }
    }*/
}

void CMenuBar::addAction(const QString & strAction)
{
    if (strAction.isNull() || strAction.isEmpty()) {
        return;
    }
    QMenu* pMenu = new QMenu(tr(strAction.toLatin1()));
    addMenu(pMenu);
}

QAction * CMenuBar::addAction(const QString & text, const QObject * receiver, const char * member, const QKeySequence & shortcut)
{


    QAction *action = new QAction(text, this);
//#ifdef QT_NO_SHORTCUT
//    Q_UNUSED(shortcut);
//#else
//    action->setShortcut(shortcut);
//#endif
//    QObject::connect(action, SIGNAL(triggered(bool)), receiver, member);
//    addAction(action);
    return action;
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
    pNewItem->setObjectName("btn" + pMenu->title());
    pNewItem->setMenu(pMenu);

    p.insertMenuBarItem(pNewItem);
}

bool CMenuBar::eventFilter(QObject * watched, QEvent * event)
{
    if (NULL == watched || NULL == event) {
        return QWidget::eventFilter(watched, event);
    }

    int eType = event->type();
    if (watched->inherits("QPushButton")) {
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
            qDebug() << "==$$$$$$$$$$$$-- press : " << pPressedItem
                << ", p.m_pPreSelectedItem: " << p.m_pPreSelectedItem
                << ", isDown: " << pPressedItem->isDown();
            if (p.m_pPreSelectedItem == pPressedItem) {
                p.m_pPreSelectedItem->setDown(false);
                p.m_pPreSelectedItem = NULL;
            }
            else {
                p.m_pPreSelectedItem = static_cast<CMenuItem*>(pPressedItem);
                pPressedItem->setDown(true);
                //pPressedItem->repaint();
                QPoint globalPos = pPressedItem->mapToGlobal(QPoint(0, pPressedItem->height()));
                pPressedItem->menu()->popup(globalPos);
            }
        }
    }
    else if (watched->inherits("QMenu")) {
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
                // cursor not locate in menuitem, should set here; or set by menuitem self
                QPoint cursorPos = QCursor::pos();
                QPoint st = p.m_pPreSelectedItem->mapToGlobal(QPoint());
                QPoint en = p.m_pPreSelectedItem->mapToGlobal(QPoint(p.m_pPreSelectedItem->width(), p.m_pPreSelectedItem->height()));
                QRect rcItem(st, en);
                if (!rcItem.contains(cursorPos)) {
                    p.m_pPreSelectedItem->setDown(false);
                    p.m_pPreSelectedItem = NULL;
                }
            }
        }
    }
    else if (NULL != watched) {
        //qDebug() << "==$$$$$$$$$$$$-- : " << watched << ", event type: " << event->type();
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
    if (QEvent::StyleChange == event->type()) {
        qDebug() << "***************************** CMenuBar;";
    }

    QWidget::changeEvent(event);
}

QColor CMenuBar::menuNormalBg() const
{
    return p.m_colorMenuNormalBg;
}

void CMenuBar::setMenuNormalBg(QColor color)
{
    p.m_bMenuNormalBgSet = true;
    p.m_colorMenuNormalBg = color;
}

QColor CMenuBar::menuHoverBg() const
{
    return p.m_colorMenuHoverBg;
}

void CMenuBar::setMenuHoverBg(QColor color)
{
    p.m_bMenuHoverBgSet = true;
    p.m_colorMenuHoverBg = color;
}

QColor CMenuBar::menuSelectedBg() const
{
    return p.m_colorMenuSelectedBg;
}

void CMenuBar::setMenuSelectedBg(QColor color)
{
    qDebug() << "==---------------=======---------------- set Bg";
    p.m_bMenuSelectedBgSet = true;
    p.m_colorMenuSelectedBg = color;
    /*QVector<QPushButton*>::iterator it = p.m_vecMenusItem.begin();
    QPushButton* pMenu;
    QString strStyle;
    QString strTmp;
    for (; it != p.m_vecMenusItem.end(); ++it) {
        pMenu = *it;
        if (NULL != pMenu) {
            strStyle = "";
            if (p.m_bMenuNormalBgSet) {
                strStyle += p.productStyle(pMenu, "", QString::number(p.m_colorMenuNormalBg.rgba(), 16));
            }
            if (p.m_bMenuHoverBgSet) {
                strStyle += p.productStyle(pMenu, "hover", QString::number(p.m_colorMenuHoverBg.rgba(), 16));
            }
            if (p.m_bMenuSelectedBgSet) {
                strStyle += p.productStyle(pMenu, "pressed", QString::number(p.m_colorMenuSelectedBg.rgba(), 16));
            }
            qDebug() << "-=-=-=-=- : " << strStyle;
            pMenu->setStyleSheet(strStyle);
        }
    }*/
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
