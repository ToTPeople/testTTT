#pragma once
#include <QWidget>
#include "ui_CenterWidget.h"

class QTranslator;
class QApplication;
class CWindowDemo : public QWidget
{
    Q_OBJECT

public:
    CWindowDemo(QApplication* pApp, QWidget *parent = Q_NULLPTR);


public slots:
    void slotOnEnglishClick();
    void slotOnChineseClick();

private:
    Ui::wgtCenter       ui;
    QTranslator*        m_pCurTranslator;
    QApplication*       m_pApp;
};
