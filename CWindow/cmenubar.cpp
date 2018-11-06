#include "cmenubar.h"
#include <QMenu>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QVector>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include "common_define.h"


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
    void insertMenuBarItem(QPushButton* pMenuBarItem, int nIndex = -1);

    QPushButton* posAt(QPoint pos);

public:
    CMenuBar*               m_pMenuBar;
    QHBoxLayout*            m_pHLayout;
    QVector<QPushButton*>   m_vecMenusItem;
    int                     m_nCnt;
    QPushButton*            m_pPreSelectedItem;
    bool                    m_bIsDown;
};

CMenuBarPrivate::CMenuBarPrivate(CMenuBar* pMenuBar)
    : m_pMenuBar(pMenuBar)
    , m_pHLayout(NULL)
    , m_nCnt(0)
    , m_pPreSelectedItem(NULL)
    , m_bIsDown(false)
{
    if (NULL != m_pMenuBar) {
        m_pHLayout = new QHBoxLayout(m_pMenuBar);
        m_pHLayout->setObjectName(QStringLiteral("menubarHLayout"));
        QSpacerItem* horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        m_pHLayout->addItem(horizontalSpacer);
        m_pHLayout->setSpacing(0);
        qDebug() << "==0000------==== space: " << m_pHLayout->spacing();
    }
    m_vecMenusItem.clear();
}

CMenuBarPrivate::~CMenuBarPrivate()
{
    m_pMenuBar = NULL;
}

void CMenuBarPrivate::insertMenuBarItem(QPushButton* pMenuBarItem, int nIndex /* = -1 */)
{
    if (NULL == pMenuBarItem) {
        return;
    }

    pMenuBarItem->setFixedWidth(g_nFixedMenuItemWidth);
    pMenuBarItem->installEventFilter(m_pMenuBar); ///////////////////////
    pMenuBarItem->menu()->installEventFilter(m_pMenuBar);

    if (nIndex <= 0 || nIndex >= m_nCnt) {
        //m_vecMenusItem[m_nCnt++] = pMenuBarItem;
        m_vecMenusItem.push_back(pMenuBarItem);
        //++m_nCnt;
        //m_pHLayout->addWidget(pMenuBarItem);
        m_pHLayout->insertWidget(m_nCnt, pMenuBarItem);
        ++m_nCnt;
    }
    else {
        int nPreIndex = -1;
        if (NULL != m_vecMenusItem[nIndex]) {
            QPushButton* pItem = m_vecMenusItem[nIndex];
            nPreIndex = m_pHLayout->indexOf(pItem);
            m_pHLayout->removeWidget(pItem);
            m_vecMenusItem.remove(nIndex);
            DELETE(pItem);
        }

        m_vecMenusItem[nIndex] = pMenuBarItem;
        if (nPreIndex < 0) {
            //m_pHLayout->addWidget(pMenuBarItem);
            m_pHLayout->insertWidget(m_nCnt, pMenuBarItem);
            ++m_nCnt;
        }
        else {
            m_pHLayout->insertWidget(nPreIndex, pMenuBarItem);
        }
    }
}

QPushButton* CMenuBarPrivate::posAt(QPoint pos)
{
    QPushButton* pTarget = NULL;
    QVector<QPushButton*>::iterator it = m_vecMenusItem.begin();
    for (; it != m_vecMenusItem.end(); ++it) {
        QPoint st = (*it)->mapToGlobal(QPoint(0,0));
        QPoint en = (*it)->mapToGlobal(QPoint((*it)->width(), (*it)->height()));
        QRect rc = QRect(st, en);
        qDebug() << "widget: " << *it
            << ", geo: " << (*it)->geometry()
            << ", \nmouse global pos: " << pos
            << ", st: " << st << ", en: " << en;
        /*if ((*it)->geometry().contains(pos)) {
            pTarget = *it;
            break;
        }*/
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
    /*QPushButton* pNewItem = new QPushButton(this);
    if (NULL == pNewItem) {
        return;
    }

    QMenu* pMenu = new QMenu();*/
}

void CMenuBar::addMenu(QMenu * pMenu)
{
    if (NULL == pMenu) {
        return;
    }

    QPushButton* pNewItem = new QPushButton(this);
    if (NULL == pNewItem) {
        return;
    }
    pNewItem->setText(tr(pMenu->title().toLatin1()));
    pNewItem->setMenu(pMenu);

    p.insertMenuBarItem(pNewItem);
}

bool CMenuBar::eventFilter(QObject * watched, QEvent * event)
{
    int eType = event->type();
    if (watched->inherits("QPushButton")) {
        QPushButton* pBtn = static_cast<QPushButton*>(watched);
        qDebug() << "================ button: " << pBtn << ", event type: " << event->type();
    }
    else if (watched->inherits("QMenu")) {
        QMenu* pBtn = static_cast<QMenu*>(watched);
        qDebug() << "###########-- menu: " << pBtn << ", event type: " << event->type();
        if (QEvent::MouseMove == event->type()) {
            if (watched != p.m_pPreSelectedItem) {
                QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(event);
                if (NULL != pMouseEvent) {
                    qDebug() << "btn geo: " << pBtn->geometry()
                        << ", mouse pos: " << pMouseEvent->pos()
                        << ", mouse global pos: " << pMouseEvent->globalPos();
                }
                QPushButton* pNew = p.posAt(pMouseEvent->globalPos());
                if (NULL != pNew && pNew != p.m_pPreSelectedItem) {
                    if (NULL != p.m_pPreSelectedItem) {
                        p.m_pPreSelectedItem->menu()->hide();
                        p.m_pPreSelectedItem->setDown(false);
                        //p.m_pPreSelectedItem->pressed();
                        //p.m_pPreSelectedItem->click();
                    }
                    p.m_pPreSelectedItem = pNew;
                    if (!pNew->isDown()) {
                        //pNew->setFocus();
                        //pNew->setDown(true);
                        pNew->showMenu();
                        //pNew->click();
                    }
                    return true;
                }
            }
        }
    }
    else if (NULL != watched) {
        //qDebug() << "==$$$$$$$$$$$$-- : " << watched << ", event type: " << event->type();
    }
    return QWidget::eventFilter(watched, event);
}

bool CMenuBar::event(QEvent *event)
{
    int eType = event->type();
    if (1) {
        //qDebug() << "==^^^^^^^^^^^^^^^^^^^^^--  event type: " << event->type();
    }

    return QWidget::event(event);
}
