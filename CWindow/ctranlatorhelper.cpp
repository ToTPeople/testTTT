#include "ctranlatorhelper.h"
#include <QAction>
#include <QMenu>
#include <QPushButton>
#include <QApplication>


CTranlatorHelper* CTranlatorHelper::s_pInstance = NULL;
CTranlatorHelper* CTranlatorHelper::getInstance()
{
    if (NULL == s_pInstance) {
        s_pInstance = new CTranlatorHelper();
    }
    return s_pInstance;
}

void CTranlatorHelper::addMenuTransText(QMenu * pMenu, const QString & strText, bool bRecursiveAction)
{
    if (!isValid(pMenu, strText)) {
        return;
    }

    MenuTrans_t* pMenuTrans = new MenuTrans_t();
    if (NULL == pMenuTrans) {
        return;
    }

    pMenuTrans->pMenu = pMenu;
    pMenuTrans->strText = strText;
    m_listMenuTrans.push_back(pMenuTrans);

    if (!bRecursiveAction) {
        return;
    }

    // 遍历获取Action
    QList<QAction*> listActions = pMenu->actions();
    QList<QAction*>::iterator it = listActions.begin();
    for (; it != listActions.end(); ++it) {
        addActionTransText(*it, (*it)->text());
    }
}

void CTranlatorHelper::removeMenuTransText(QMenu * pMenu)
{
    if (NULL == pMenu) {
        return;
    }

    QList<MenuTrans_t*>::iterator it = m_listMenuTrans.begin();
    for (; it != m_listMenuTrans.end(); ++it) {
        if (NULL != *it && pMenu == (*it)->pMenu) {
            m_listMenuTrans.erase(it++);
            break;
        }
    }
}

void CTranlatorHelper::addActionTransText(QAction * pAction, const QString & strText)
{
    if (!isValid(pAction, strText)) {
        return;
    }

    ActionTrans_t* pActionTrans = new ActionTrans_t();
    if (NULL == pActionTrans) {
        return;
    }

    pActionTrans->pAction = pAction;
    pActionTrans->strText = strText;

    m_listActionTrans.push_back(pActionTrans);
}

void CTranlatorHelper::removeActionTransText(QAction * pAction)
{
    if (NULL == pAction) {
        return;
    }

    QList<ActionTrans_t*>::iterator it = m_listActionTrans.begin();
    for (; it != m_listActionTrans.end(); ++it) {
        if (NULL != *it && pAction == (*it)->pAction) {
            m_listActionTrans.erase(it++);
            break;
        }
    }
}

void CTranlatorHelper::addButtonTransText(QPushButton * pButton, const QString & strText)
{
    if (!isValid(pButton, strText)) {
        return;
    }

    ButtonTrans_t* pButtonTrans = new ButtonTrans_t();
    if (NULL == pButtonTrans) {
        return;
    }

    pButtonTrans->pButton = pButton;
    pButtonTrans->strText = strText;

    m_listButtonTrans.push_back(pButtonTrans);
}

void CTranlatorHelper::removeButtonTransText(QPushButton * pButton)
{
    if (NULL == pButton) {
        return;
    }

    QList<ButtonTrans_t*>::iterator it = m_listButtonTrans.begin();
    for (; it != m_listButtonTrans.end(); ++it) {
        if (NULL != *it && pButton == (*it)->pButton) {
            m_listButtonTrans.erase(it++);
            break;
        }
    }
}

void CTranlatorHelper::retranslateUi()
{
    {
        // menu
        QList<MenuTrans_t*>::iterator it = m_listMenuTrans.begin();
        MenuTrans_t* pTrans = NULL;
        for (; it != m_listMenuTrans.end(); ++it) {
            pTrans = *it;
            if (NULL != pTrans && NULL != pTrans->pMenu) {
                // object name暂时放空
                pTrans->pMenu->setTitle(QApplication::translate("QObject", pTrans->strText.toLatin1(), Q_NULLPTR));
            }
        }
    }
    {
        // action
        QList<ActionTrans_t*>::iterator it = m_listActionTrans.begin();
        ActionTrans_t* pTrans = NULL;
        for (; it != m_listActionTrans.end(); ++it) {
            pTrans = *it;
            if (NULL != pTrans && NULL != pTrans->pAction) {
                // object name暂时放空
                pTrans->pAction->setText(QApplication::translate("QObject", pTrans->strText.toLatin1(), Q_NULLPTR));
            }
        }
    }
    {
        // button
        QList<ButtonTrans_t*>::iterator it = m_listButtonTrans.begin();
        ButtonTrans_t* pTrans = NULL;
        for (; it != m_listButtonTrans.end(); ++it) {
            pTrans = *it;
            if (NULL != pTrans && NULL != pTrans->pButton) {
                // object name暂时放空
                pTrans->pButton->setText(QApplication::translate("QObject", pTrans->strText.toLatin1(), Q_NULLPTR));
            }
        }
    }
}

bool CTranlatorHelper::isValid(QObject * pObj, const QString & strText)
{
    if (NULL == pObj || strText.isNull() || strText.isEmpty()) {
        return false;
    }
    return true;
}

CTranlatorHelper::CTranlatorHelper()
{
}


CTranlatorHelper::~CTranlatorHelper()
{
    m_listMenuTrans.clear();
    m_listActionTrans.clear();
    m_listButtonTrans.clear();
}
