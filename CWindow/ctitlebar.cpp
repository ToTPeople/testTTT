#include "ctitlebar.h"
#include <QLabel>
#include <QPushButton>
#include "common_define.h"

class CTitleBarPrivate
{
    friend CTitleBar;
public:
    CTitleBarPrivate(CTitleBar* parent = NULL);
    ~CTitleBarPrivate();

public:
    void init();

public:
    CTitleBar*          m_pTitleBar;

    QLabel*             m_pLabelTitle;
    QPushButton*        m_pBtnMinimize;
    QPushButton*        m_pBtnMaximize;
    QPushButton*        m_pBtnExit;
};

CTitleBarPrivate::CTitleBarPrivate(CTitleBar* parent /* = NULL */)
    : m_pTitleBar(parent)
    , m_pLabelTitle(NULL)
    , m_pBtnExit(NULL)
    , m_pBtnMaximize(NULL)
    , m_pBtnMinimize(NULL)
{
    init();
}

CTitleBarPrivate::~CTitleBarPrivate()
{
    m_pTitleBar = NULL;
}

void CTitleBarPrivate::init()
{
    if (NULL == m_pTitleBar) {
        return;
    }

    m_pLabelTitle = new QLabel(m_pTitleBar);
    if (NULL != m_pLabelTitle) {
        m_pLabelTitle->setObjectName(kszWindowTitle);
    }

    m_pBtnMinimize = new QPushButton(m_pTitleBar);
    if (NULL != m_pBtnMinimize) {
        m_pBtnMinimize->setObjectName(kszWindowMinimize);
        QObject::connect(m_pBtnMinimize, SIGNAL(clicked()), m_pTitleBar, SIGNAL(sigOnMinimizeBtnClick()));
    }

    m_pBtnMaximize = new QPushButton(m_pTitleBar);
    if (NULL != m_pBtnMaximize) {
        m_pBtnMaximize->setObjectName(kszWindowMaximize);
        QObject::connect(m_pBtnMaximize, SIGNAL(clicked()), m_pTitleBar, SIGNAL(sigOnMaximizeBtnClick()));
    }

    m_pBtnExit = new QPushButton(m_pTitleBar);
    if (NULL != m_pBtnExit) {
        m_pBtnExit->setObjectName(kszWindowExit);
        QObject::connect(m_pBtnExit, SIGNAL(clicked()), m_pTitleBar, SIGNAL(sigOnExitBtnClick()));
    }

    // test -------------------------------------
    m_pLabelTitle->setText("Test");
    m_pBtnExit->move(50, 0);
    m_pBtnMinimize->move(100, 0);
    m_pBtnMaximize->move(200, 0);
}

CTitleBar::CTitleBar(QWidget* parent /*= NULL*/)
    : QWidget(parent)
    , p(*new CTitleBarPrivate(this))
{
}


CTitleBar::~CTitleBar()
{
    delete &p;
}
