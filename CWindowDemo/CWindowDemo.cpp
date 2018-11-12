#include "CWindowDemo.h"
#include <QTranslator>
#include <QApplication>


namespace {
    const char* const kszEnglishTranslator = ":/language/English.qm";
    const char* const kszChineseTranslator = ":/language/Chinese.qm";
}

CWindowDemo::CWindowDemo(QApplication* pApp, QWidget *parent)
    : QWidget(parent)
    , m_pCurTranslator(NULL)
    , m_pApp(pApp)
{
    ui.setupUi(this);

    connect(ui.btnEnglish, SIGNAL(clicked()), this, SLOT(slotOnEnglishClick()));
    connect(ui.btnChinese, SIGNAL(clicked()), this, SLOT(slotOnChineseClick()));
}

void CWindowDemo::slotOnEnglishClick()
{
    if (NULL == m_pCurTranslator) {
        m_pCurTranslator = new QTranslator();
    }
    else {
        m_pApp->removeTranslator(m_pCurTranslator);
    }

    m_pCurTranslator->load(kszEnglishTranslator);
    m_pApp->installTranslator(m_pCurTranslator);
}

void CWindowDemo::slotOnChineseClick()
{
    if (NULL == m_pCurTranslator) {
        m_pCurTranslator = new QTranslator();
    }
    else {
        m_pApp->removeTranslator(m_pCurTranslator);
    }

    m_pCurTranslator->load(kszChineseTranslator);
    m_pApp->installTranslator(m_pCurTranslator);
}

void CWindowDemo::changeEvent(QEvent * event)
{
    if (NULL != event && QEvent::LanguageChange == event->type()) {
        ui.retranslateUi(this);
    }

    QWidget::changeEvent(event);
}
