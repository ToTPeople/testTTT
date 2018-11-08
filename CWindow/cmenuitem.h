#pragma once
#include <qpushbutton.h>

class CMenuItem : public QPushButton
{
    Q_OBJECT
public:
    CMenuItem(QWidget *parent = NULL);
    ~CMenuItem();
};
