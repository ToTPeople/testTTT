#pragma once
#include <QWidget>
#include "ui_CenterWidget.h"

class CWindowDemo : public QWidget
{
    Q_OBJECT

public:
    CWindowDemo(QWidget *parent = Q_NULLPTR);

private:
    Ui::wgtCenter       ui;
};
