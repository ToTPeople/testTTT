#include "cbasewindow.h"
#include <QMouseEvent>
#include <QDebug>
#include <QFile>
#include "cmenubar.h"
#include "ctitlebar.h"
#include "common_define.h"
#include <QPalette>

class CBaseWindowPrivate
{
    friend CBaseWindow;
public:
    CBaseWindowPrivate(CBaseWindow * parent = NULL);
    ~CBaseWindowPrivate();

public:
    int setCursorStyle(const QPoint& curPoint);    // 当前位置设置鼠标样式

public:
    enum tagCursorCtrlStyle {
        eCursorNormal = 0,    // 普通鼠标
        eCursorHor,           // 水平拉伸
        eCursorVer,           // 垂直拉伸
        eCursorHorVer         // 水平和垂直拉伸
    };

public:
    CMenuBar*           m_pMenuBar;
    CTitleBar*          m_pTitleBar;
    QWidget*            m_pCenterWidget;
    QWidget*            m_pStatusBar;
public:
    bool                m_bFirstCreateShow;         // 首次创建显示
    int                 m_nPreWidth;
    int                 m_nPreHeight;

    QPoint m_MousePressPos;                         // 鼠标点击位置
    QPoint m_MouseMovePos;                          // 鼠标移动位置
    bool   m_bMousePressed;                         // 鼠标是否按下
    int    m_nMouseResize;                          // 鼠标设置大小
    bool   m_bMouseResizePressed;                   // 设置大小的按下
    QPoint m_ResizePressPos;                        // 设置大小鼠标按下的点

    CBaseWindow*        m_pWindow;
};

CBaseWindowPrivate::CBaseWindowPrivate(CBaseWindow * parent /* = NULL */)
    : m_pCenterWidget(NULL)
    , m_pMenuBar(NULL)
    , m_pStatusBar(NULL)
    , m_pTitleBar(NULL)
    , m_bFirstCreateShow(true)
    , m_nPreWidth(0)
    , m_nPreHeight(0)
    , m_bMousePressed(false)
    , m_bMouseResizePressed(false)
    , m_nMouseResize(eCursorNormal)
    , m_pWindow(parent)
{
    m_pTitleBar = new CTitleBar(parent);
    if (NULL != m_pTitleBar) {
        if (NULL != m_pWindow) {
            QObject::connect(m_pTitleBar, SIGNAL(sigOnExitBtnClick()), m_pWindow, SLOT(slotOnExitClick()));
            QObject::connect(m_pTitleBar, SIGNAL(sigOnMinimizeBtnClick()), m_pWindow, SLOT(slotOnMinimizeClick()));
            QObject::connect(m_pTitleBar, SIGNAL(sigOnMaximizeBtnClick()), m_pWindow, SLOT(slotOnMaximizeClick()));
        }
        // test -------------------------------
        m_pTitleBar->setAutoFillBackground(true);
        QPalette pal = m_pTitleBar->palette();
        pal.setBrush(QPalette::Background, QBrush(Qt::red));
        m_pTitleBar->setPalette(pal);
        // test -------------------------------
    }
    // test ----------------------------------------
    {
        m_pMenuBar = new CMenuBar(parent);
        m_pMenuBar->setAutoFillBackground(true);
        QPalette pal = m_pMenuBar->palette();
        pal.setBrush(QPalette::Background, QBrush(Qt::blue));
        m_pMenuBar->setPalette(pal);
    }
    {
        m_pStatusBar = new QWidget(parent);
        m_pStatusBar->setAutoFillBackground(true);
        QPalette pal = m_pStatusBar->palette();
        pal.setBrush(QPalette::Background, QBrush(Qt::green));
        m_pStatusBar->setPalette(pal);
    }
    // test ----------------------------------------
}

CBaseWindowPrivate::~CBaseWindowPrivate()
{
    DELETE(m_pTitleBar);
    DELETE(m_pStatusBar);
    DELETE(m_pMenuBar);
    DELETE(m_pCenterWidget);
}

int CBaseWindowPrivate::setCursorStyle(const QPoint& curPoint)
{
    if (NULL == m_pWindow) {
        return eCursorNormal;
    }

    int nCurWidth = m_pWindow->width();
    int nCurHeight = m_pWindow->height();
    int nRes = eCursorNormal;

    if ( (nCurWidth - curPoint.x() <= 3) && (nCurHeight - curPoint.y() <= 3) ) {
        m_pWindow->setCursor(Qt::SizeFDiagCursor);
        nRes = eCursorHorVer;
    }
    else if (nCurWidth - curPoint.x() <= 3) {
        m_pWindow->setCursor(Qt::SizeHorCursor);
        nRes = eCursorHor;
    }
    else if (nCurHeight - curPoint.y() <= 3) {
        m_pWindow->setCursor(Qt::SizeVerCursor);
        nRes = eCursorVer;
    }
    else {
        m_pWindow->setCursor(Qt::ArrowCursor);
        nRes = eCursorNormal;
    }

    return nRes;
}

CBaseWindow::CBaseWindow(QWidget * parent /*= NULL*/)
    : QWidget(parent)
    , p(*new CBaseWindowPrivate(this))
{
    setWindowFlags(Qt::FramelessWindowHint);// | Qt::Tool);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);    // 设置尺寸属性
    setMouseTracking(true);

    // style sheet
    QFile file(kszQssWindows);
    if (file.open(QFile::ReadOnly)) {
        setStyleSheet(file.readAll());
        file.close();
    }
}

CBaseWindow::~CBaseWindow()
{
    delete &p;
}

void CBaseWindow::setTitleBar(CTitleBar * pTitleBar)
{
    // title bar must have
    if (NULL == pTitleBar) {
        return;
    }

    DELETE(p.m_pTitleBar);
    p.m_pTitleBar = pTitleBar;

    // ---------------------------------------
}

CTitleBar * CBaseWindow::getTitleBar()
{
    return p.m_pTitleBar;
}

void CBaseWindow::setMenuBar(CMenuBar * pMenuBar)
{
    DELETE(p.m_pMenuBar);
    p.m_pMenuBar = pMenuBar;
}

CMenuBar * CBaseWindow::getMenuBar()
{
    return p.m_pMenuBar;
}

void CBaseWindow::setCenterWidget(QWidget * pWgt)
{
    DELETE(p.m_pCenterWidget);
    p.m_pCenterWidget = pWgt;
}

QWidget * CBaseWindow::getCenterWidget()
{
    return p.m_pCenterWidget;
}

void CBaseWindow::setStatusBar(QWidget * pStatusBar)
{
    DELETE(p.m_pStatusBar);
    p.m_pStatusBar = pStatusBar;
}

QWidget * CBaseWindow::getStatusBar()
{
    return p.m_pStatusBar;
}

void CBaseWindow::slotOnExitClick()
{
    close();
}

void CBaseWindow::slotOnMinimizeClick()
{
    // minimize
    showMinimized();
}

void CBaseWindow::slotOnMaximizeClick()
{
    // maximize or normal
    if (Qt::WindowNoState == windowState()) {
        showMaximized();
    }
    else if (Qt::WindowMaximized == windowState()) {
        showNormal();
    }
}

void CBaseWindow::showEvent(QShowEvent * event)
{
    if (p.m_bFirstCreateShow) {
        p.m_bFirstCreateShow = false;

        // Window min width and height
        int nMinWidth = 50;  // Title bar icon width----------------------------- tmp
        int nMinHeight = FIXED_TITLEBAR_HEIGHT + 1;
        if (NULL != p.m_pMenuBar) {
            nMinHeight += FIXED_MENUBAR_HEIGHT + 1;
        }
        if (NULL != p.m_pStatusBar) {
            nMinHeight += FIXED_STATUSBAR_HEIGHT + 1;
        }

        setMinimumSize(nMinWidth, nMinHeight);

        // 这边更新位置，可能会错误（与上面逻辑冲突）
        p.m_nPreWidth = width();
        p.m_nPreHeight = height();

        // adjust titlebar menubar statusbar location
        int nY = 0;
        if (NULL != p.m_pTitleBar) {
            p.m_pTitleBar->setGeometry(0, nY, p.m_nPreWidth, FIXED_TITLEBAR_HEIGHT);
            nY += FIXED_TITLEBAR_HEIGHT + 1;
            p.m_pTitleBar->show();
        }
        if (NULL != p.m_pMenuBar) {
            p.m_pMenuBar->setGeometry(0, nY, p.m_nPreWidth, FIXED_MENUBAR_HEIGHT);
            nY += FIXED_MENUBAR_HEIGHT + 1;
        }
        if (NULL != p.m_pCenterWidget) {
            int nCenterWgtHeight = p.m_nPreHeight - nY - 1;
            if (NULL != p.m_pStatusBar) {
                nCenterWgtHeight -= FIXED_STATUSBAR_HEIGHT;
            }
            p.m_pCenterWidget->setGeometry(0, nY, p.m_nPreWidth, nCenterWgtHeight);
            nY += nCenterWgtHeight + 1;
        }
        if (NULL != p.m_pStatusBar) {
            p.m_pStatusBar->setGeometry(0, nY, p.m_nPreWidth, FIXED_STATUSBAR_HEIGHT);
        }
    }
}

void CBaseWindow::resizeEvent(QResizeEvent * event)
{
    // adjust titlebar menubar statusbar location
    int nWidth = width();
    int nHeight = height();

    if (nWidth != p.m_nPreWidth || nHeight != p.m_nPreHeight) {
        // adjust title bar / menu bar width
        if (nWidth != p.m_nPreWidth) {
            if (NULL != p.m_pTitleBar) {
                p.m_pTitleBar->setFixedWidth(nWidth);
            }
            if (NULL != p.m_pMenuBar) {
                p.m_pMenuBar->setFixedWidth(nWidth);
            }
        }

        // adjust center widget / status bar location
        if (NULL != p.m_pCenterWidget) {
            int nNewHeight = p.m_pCenterWidget->height() + (nHeight - p.m_nPreHeight);
            p.m_pCenterWidget->setFixedSize(nWidth, nNewHeight);
        }
        if (NULL != p.m_pStatusBar) {
            int nNewY = p.m_pStatusBar->y() + (nHeight - p.m_nPreHeight);
            p.m_pStatusBar->setGeometry(0, nNewY, nWidth, FIXED_STATUSBAR_HEIGHT);
        }

        // update pre width and height
        p.m_nPreWidth = nWidth;
        p.m_nPreHeight = nHeight;
    }

    QWidget::resizeEvent(event);
}

void CBaseWindow::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton &&
        p.m_pTitleBar->rect().contains(event->globalPos() - this->frameGeometry().topLeft())) {
        p.m_MousePressPos = event->globalPos();
        p.m_bMousePressed = true;
    }

    if (CBaseWindowPrivate::eCursorNormal != p.m_nMouseResize) {
        p.m_bMouseResizePressed = true;
        p.m_ResizePressPos = event->pos();
    }
    event->ignore();    //表示继续向下传递事件，其他的控件还可以去获取
}

void CBaseWindow::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        p.m_bMousePressed = false;
        p.m_bMouseResizePressed = false;
        p.m_ResizePressPos.setX(0);
        p.m_ResizePressPos.setY(0);
        //p.m_nMouseResize = p.setCursorStyle(event->pos());
    }
    event->ignore();
}

void CBaseWindow::mouseMoveEvent(QMouseEvent * event)
{
    if (p.m_bMousePressed)
    {
        p.m_MouseMovePos = event->globalPos();
        this->move(this->pos() + p.m_MouseMovePos - p.m_MousePressPos);
        p.m_MousePressPos = p.m_MouseMovePos;
        return;
    }

    QPoint curPoint = event->pos();

    if (!p.m_bMouseResizePressed)
    {
        p.m_nMouseResize = p.setCursorStyle(curPoint);
    }

    if (p.m_bMouseResizePressed && !p.m_ResizePressPos.isNull())
    {
        switch (p.m_nMouseResize)
        {
        case CBaseWindowPrivate::eCursorHor:
            this->resize(curPoint.x(), this->height());
            break;
        case CBaseWindowPrivate::eCursorVer:
            this->resize(this->width(), curPoint.y());
            break;
        case CBaseWindowPrivate::eCursorHorVer:
            this->resize(curPoint.x(), curPoint.y());
            break;
        default:
            break;
        }
    }

    event->ignore();
}
