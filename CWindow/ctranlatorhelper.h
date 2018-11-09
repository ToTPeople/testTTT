#pragma once

#include <QList>

class QObject;
class QMenu;
class QAction;
class QPushButton;
//////////////////// 暂未覆盖同源不同翻译场景
class CTranlatorHelper
{
public:
    static CTranlatorHelper* getInstance();

public:
    typedef struct {
        QMenu*      pMenu;
        QString     strText;
    } MenuTrans_t;

    typedef struct {
        QAction*    pAction;
        QString     strText;
    } ActionTrans_t;

    typedef struct {
        QPushButton*    pButton;
        QString         strText;
    } ButtonTrans_t;

public:
    void addMenuTransText(QMenu* pMenu, const QString& strText, bool bRecursiveAction = false);
    void removeMenuTransText(QMenu* pMenu);

    void addActionTransText(QAction* pAction, const QString& strText);
    void removeActionTransText(QAction* pAction);

    void addButtonTransText(QPushButton* pButton, const QString& strText);
    void removeButtonTransText(QPushButton* pButton);

    void retranslateUi();

private:
    bool isValid(QObject* pObj, const QString& strText);

private:
    CTranlatorHelper();
    ~CTranlatorHelper();

private:
    QList<MenuTrans_t*>               m_listMenuTrans;
    QList<ActionTrans_t*>             m_listActionTrans;
    QList<ButtonTrans_t*>             m_listButtonTrans;

private:
    static CTranlatorHelper*    s_pInstance;
};

#define g_pTranlatorHelper     (CTranlatorHelper::getInstance())
