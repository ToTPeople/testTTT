#include "CWindowDemo.h"
#include <QTranslator>
#include <QApplication>


namespace {
    const char* const kszEnglishTranslator = "./cwindowdemo_en.qm";
    const char* const kszChineseTranslator = "./cwindowdemo_zh.qm";
}

CWindowDemo::CWindowDemo(QApplication* pApp, QWidget *parent)
    : QWidget(parent)
    , m_pCurTranslator(NULL)
    , m_pApp(pApp)
{
    ui.setupUi(this);

    ui.btnCenterExit->setShortcut(Qt::Key_Alt | Qt::Key_C);

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
