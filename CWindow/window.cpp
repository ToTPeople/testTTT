#include "window.h"
#include <QMouseEvent>
#include <QDebug>
#include <QFile>
#include <QPalette>
#include "ui_window.h"
#include "common_define.h"


class CWindowPrivate
{
    friend CWindow;
public:
    CWindowPrivate(CWindow * parent = NULL);
    ~CWindowPrivate();

public:
    int setCursorStyle(const QPoint& curPoint);    // 当前位置设置鼠标样式

    void replaceWidget(QWidget* pOldWgt, QWidget* pNew, int nIndex, int nFixedHeight);
    void init();

public:
    enum tagCursorCtrlStyle {
        eCursorNormal = 0,    // 普通鼠标
        eCursorHor,           // 水平拉伸
        eCursorVer,           // 垂直拉伸
        eCursorHorVer         // 水平和垂直拉伸
    };

public:
    QPoint m_MousePressPos;                         // 鼠标点击位置
    QPoint m_MouseMovePos;                          // 鼠标移动位置
    bool   m_bMousePressed;                         // 鼠标是否按下
    int    m_nMouseResize;                          // 鼠标设置大小
    bool   m_bMouseResizePressed;                   // 设置大小的按下
    QPoint m_ResizePressPos;                        // 设置大小鼠标按下的点

    CWindow*        m_pWindow;
    Ui::Window      m_ui;
    QWidget*        m_pTitleBar;
    QWidget*        m_pMenuBar;
    QWidget*        m_pCenterWidget;
    QWidget*        m_pStatusBar;
};

CWindowPrivate::CWindowPrivate(CWindow * parent /* = NULL */)
    : m_pWindow(parent)
    , m_bMousePressed(false)
    , m_bMouseResizePressed(false)
    , m_nMouseResize(eCursorNormal)
    , m_pTitleBar(NULL)
    , m_pStatusBar(NULL)
    , m_pMenuBar(NULL)
    , m_pCenterWidget(NULL)
{
    init();
}

CWindowPrivate::~CWindowPrivate()
{

}

int CWindowPrivate::setCursorStyle(const QPoint& curPoint)
{
    if (NULL == m_pWindow) {
        return eCursorNormal;
    }

    int nCurWidth = m_pWindow->width();
    int nCurHeight = m_pWindow->height();
    int nRes = eCursorNormal;

    if ((nCurWidth - curPoint.x() <= 3) && (nCurHeight - curPoint.y() <= 3)) {
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

void CWindowPrivate::replaceWidget(QWidget* pOldWgt, QWidget* pNew, int nIndex, int nFixedHeight)
{
    if (NULL == pNew) {
        return;
    }

    m_ui.vLayoutWindow->removeWidget(pOldWgt);
    DELETE(pOldWgt);
    if (nFixedHeight > 0) {
        pNew->setFixedHeight(nFixedHeight);
    }
    m_ui.vLayoutWindow->insertWidget(nIndex, pNew);
}

void CWindowPrivate::init()
{
    if (NULL == m_pWindow) {
        return;
    }

    m_ui.setupUi(m_pWindow);
#ifdef Q_OS_CYGWIN
    if (NULL != m_ui.wgtMacTitleBar) {
        if (NULL != m_ui.btnMacExit) {
            QObject::connect(m_ui.btnMacExit, SIGNAL(clicked()), m_pWindow, SLOT(slotOnExitClick()));
        }
        if (NULL != m_ui.btnMacMin) {
            QObject::connect(m_ui.btnMacMin, SIGNAL(clicked()), m_pWindow, SLOT(slotOnMinimizeClick()));
        }
        if (NULL != m_ui.btnMacMax) {
            QObject::connect(m_ui.btnMacMax, SIGNAL(clicked()), m_pWindow, SLOT(slotOnMaximizeClick()));
        }
        // test -------------------------------
        m_ui.wgtMacTitleBar->setAutoFillBackground(true);
        QPalette pal = m_ui.wgtMacTitleBar->palette();
        pal.setBrush(QPalette::Background, QBrush(Qt::yellow));
        m_ui.wgtMacTitleBar->setPalette(pal);
        // test -------------------------------
    }
    if (NULL != m_ui.wgtTitleBar) {
        m_ui.vLayoutWindow->removeWidget(m_ui.wgtTitleBar);
        DELETE(m_ui.wgtTitleBar);
    }
    m_pTitleBar = m_ui.wgtMacTitleBar;
#else
    if (NULL != m_ui.wgtTitleBar) {
        if (NULL != m_ui.btnExit) {
            QObject::connect(m_ui.btnExit, SIGNAL(clicked()), m_pWindow, SLOT(slotOnExitClick()));
        }
        if (NULL != m_ui.btnMin) {
            QObject::connect(m_ui.btnMin, SIGNAL(clicked()), m_pWindow, SLOT(slotOnMinimizeClick()));
        }
        if (NULL != m_ui.btnMax) {
            QObject::connect(m_ui.btnMax, SIGNAL(clicked()), m_pWindow, SLOT(slotOnMaximizeClick()));
        }
        // test -------------------------------
        m_ui.labelTitle->setText("lkjalkdjsfka");
        m_ui.wgtTitleBar->setAutoFillBackground(true);
        QPalette pal = m_ui.wgtTitleBar->palette();
        pal.setBrush(QPalette::Background, QBrush(Qt::red));
        m_ui.wgtTitleBar->setPalette(pal);
        // test -------------------------------
    }
    if (NULL != m_ui.wgtMacTitleBar) {
        m_ui.vLayoutWindow->removeWidget(m_ui.wgtMacTitleBar);
        DELETE(m_ui.wgtMacTitleBar);
    }
    m_pTitleBar = m_ui.wgtTitleBar;
#endif
    m_pMenuBar = m_ui.wgtMenuBar;
    m_pCenterWidget = m_ui.wgtCenterWidget;
    m_pStatusBar = m_ui.wgtStatusBar;
    // test ----------------------------------------
    {
        m_ui.wgtMenuBar->setAutoFillBackground(true);
        QPalette pal = m_ui.wgtMenuBar->palette();
        pal.setBrush(QPalette::Background, QBrush(Qt::blue));
        m_ui.wgtMenuBar->setPalette(pal);
    }
    {
        m_pCenterWidget->setAutoFillBackground(true);
        QPalette pal = m_pCenterWidget->palette();
        pal.setBrush(QPalette::Background, QBrush(Qt::black));
        m_pCenterWidget->setPalette(pal);
    }
    {
        m_ui.wgtStatusBar->setAutoFillBackground(true);
        QPalette pal = m_ui.wgtStatusBar->palette();
        pal.setBrush(QPalette::Background, QBrush(Qt::green));
        m_ui.wgtStatusBar->setPalette(pal);
    }
    // test ----------------------------------------
}


CWindow::CWindow(QWidget* parent /* = NULL */)
    : QWidget(parent)
    , p(*new CWindowPrivate(this))
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

CWindow::~CWindow()
{
    delete &p;
}

void CWindow::setTitleBar(QWidget * pTitleBar)
{
    // title bar must have
    if (NULL == pTitleBar) {
        return;
    }

    p.replaceWidget(p.m_pTitleBar, pTitleBar, TITLEBAR_INDEX, FIXED_TITLEBAR_HEIGHT);
    p.m_pTitleBar = pTitleBar;
}

QWidget * CWindow::getTitleBar()
{
    return p.m_pTitleBar;
}

void CWindow::setMenuBar(QWidget * pMenuBar)
{
    p.replaceWidget(p.m_pMenuBar, pMenuBar, MENUBAR_INDEX, FIXED_MENUBAR_HEIGHT);
    p.m_pMenuBar = pMenuBar;
}

QWidget * CWindow::getMenuBar()
{
    return p.m_pMenuBar;
}

void CWindow::setCenterWidget(QWidget * pWgt)
{
    p.replaceWidget(p.m_pCenterWidget, pWgt, CENTERWIDGET_INDEX, -1);
    p.m_pCenterWidget = pWgt;
}

QWidget * CWindow::getCenterWidget()
{
    return p.m_pCenterWidget;
}

void CWindow::setStatusBar(QWidget * pStatusBar)
{
    p.replaceWidget(p.m_pStatusBar, pStatusBar, STATUSBAR_INDEX, FIXED_STATUSBAR_HEIGHT);
    p.m_pStatusBar = pStatusBar;
}

QWidget * CWindow::getStatusBar()
{
    return p.m_pStatusBar;
}

void CWindow::slotOnExitClick()
{
    close();
}

void CWindow::slotOnMinimizeClick()
{
    // minimize
    showMinimized();
}

void CWindow::slotOnMaximizeClick()
{
    // maximize or normal
    if (Qt::WindowNoState == windowState()) {
        showMaximized();
    }
    else if (Qt::WindowMaximized == windowState()) {
        showNormal();
    }
}

void CWindow::mousePressEvent(QMouseEvent * event)
{
    if (Qt::WindowMaximized == windowState()) {
        return QWidget::mouseMoveEvent(event);
    }

    QWidget* pTitleBar = getTitleBar();
    if (event->button() == Qt::LeftButton && NULL != pTitleBar &&
        pTitleBar->rect().contains(event->globalPos() - this->frameGeometry().topLeft())) {
        p.m_MousePressPos = event->globalPos();
        p.m_bMousePressed = true;
    }

    if (CWindowPrivate::eCursorNormal != p.m_nMouseResize) {
        p.m_bMouseResizePressed = true;
        p.m_ResizePressPos = event->pos();
    }
    event->ignore();    //表示继续向下传递事件，其他的控件还可以去获取
}

void CWindow::mouseReleaseEvent(QMouseEvent * event)
{
    if (Qt::WindowMaximized == windowState()) {
        return QWidget::mouseMoveEvent(event);
    }

    if (event->button() == Qt::LeftButton)
    {
        p.m_bMousePressed = false;
        p.m_bMouseResizePressed = false;
        p.m_ResizePressPos.setX(0);
        p.m_ResizePressPos.setY(0);
        p.m_nMouseResize = p.setCursorStyle(event->pos());
    }
    event->ignore();
}

void CWindow::mouseMoveEvent(QMouseEvent * event)
{
    if (Qt::WindowMaximized == windowState()) {
        return QWidget::mouseMoveEvent(event);
    }

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
        qDebug() << "curPoint: " << curPoint;
        p.m_nMouseResize = p.setCursorStyle(curPoint);
    }

    if (p.m_bMouseResizePressed && !p.m_ResizePressPos.isNull())
    {
        switch (p.m_nMouseResize)
        {
        case CWindowPrivate::eCursorHor:
            this->resize(curPoint.x(), this->height());
            break;
        case CWindowPrivate::eCursorVer:
            this->resize(this->width(), curPoint.y());
            break;
        case CWindowPrivate::eCursorHorVer:
            this->resize(curPoint.x(), curPoint.y());
            break;
        default:
            break;
        }
    }

    event->ignore();
}
