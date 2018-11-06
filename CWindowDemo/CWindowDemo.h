#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CWindowDemo.h"

class CWindowDemo : public QMainWindow
{
    Q_OBJECT

public:
    CWindowDemo(QWidget *parent = Q_NULLPTR);

private:
    Ui::CWindowDemoClass ui;
};
