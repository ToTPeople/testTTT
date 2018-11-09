#include "ctitlebar.h"
#include <QLabel>
#include <QPushButton>
#include "common_define.h"

#if defined Q_OS_WIN
#include "ui_titlebar.h"
#else
#include "ui_mactitlebar.h"
#endif

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

public:
#if defined Q_OS_WIN
    Ui::wgtTitleBar     m_uiTitleBar;
#else
    Ui::wgtMacTitleBar  m_uiTitleBar;
#endif
    QString             m_strTitle;
};

CTitleBarPrivate::CTitleBarPrivate(CTitleBar* parent /* = NULL */)
    : m_pTitleBar(parent)
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

    m_uiTitleBar.setupUi(m_pTitleBar);

    if (NULL != m_uiTitleBar.btnExit) {
        QObject::connect(m_uiTitleBar.btnExit, SIGNAL(clicked())
            , m_pTitleBar, SIGNAL(sigOnExitBtnClick()));
    }
    if (NULL != m_uiTitleBar.btnMin) {
        QObject::connect(m_uiTitleBar.btnMin, SIGNAL(clicked())
            , m_pTitleBar, SIGNAL(sigOnMinimizeBtnClick()));
    }
    if (NULL != m_uiTitleBar.btnMax) {
        QObject::connect(m_uiTitleBar.btnMax, SIGNAL(clicked())
            , m_pTitleBar, SIGNAL(sigOnMaximizeBtnClick()));
    }
    // test -------------------------------
    //m_uiTitleBar.labelTitle->setText(QObject::tr("Windows Title"));
    m_pTitleBar->setAutoFillBackground(true);
    QPalette pal = m_pTitleBar->palette();
    pal.setBrush(QPalette::Background, QBrush(Qt::red));
    m_pTitleBar->setPalette(pal);
    // test -------------------------------
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

void CTitleBar::setTitle(const QString & strTitle)
{
    p.m_strTitle = strTitle;
    if (NULL != p.m_uiTitleBar.labelTitle) {
        p.m_uiTitleBar.labelTitle->setText(QObject::tr(strTitle.toLatin1()));
    }
}

QString CTitleBar::getTitle()
{
    if (NULL != p.m_uiTitleBar.labelTitle) {
        return p.m_uiTitleBar.labelTitle->text();
    }
    return p.m_strTitle;
}

void CTitleBar::setMaximizeIconVisiable(bool bVisiable)
{
    if (!bVisiable && NULL != p.m_uiTitleBar.btnMax) {
        p.m_uiTitleBar.btnMax->hide();
    }
}

void CTitleBar::retranslateUi()
{
    p.m_uiTitleBar.retranslateUi(this);
    if (NULL != p.m_uiTitleBar.labelTitle) {
        p.m_uiTitleBar.labelTitle->setText(QObject::tr(p.m_strTitle.toLatin1()));
    }
}

void CTitleBar::changeEvent(QEvent * event)
{
    if (NULL != event && QEvent::LanguageChange == event->type()) {
        retranslateUi();
        return;
    }

    QWidget::changeEvent(event);
}
