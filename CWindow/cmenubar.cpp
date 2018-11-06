#include "cmenubar.h"


class CMenuBarPrivate
{
    friend CMenuBar;
public:
    CMenuBarPrivate(CMenuBar* pMenuBar);
    ~CMenuBarPrivate();

public:
    CMenuBar*           m_pMenuBar;
};

CMenuBarPrivate::CMenuBarPrivate(CMenuBar* pMenuBar)
    : m_pMenuBar(pMenuBar)
{
}

CMenuBarPrivate::~CMenuBarPrivate()
{
    m_pMenuBar = NULL;
}

CMenuBar::CMenuBar(QWidget* parent /*= NULL*/)
    : QWidget(parent)
    , p(*new CMenuBarPrivate(this))
{
}


CMenuBar::~CMenuBar()
{
    delete &p;
}
