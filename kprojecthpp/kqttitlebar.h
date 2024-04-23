/*****************************************************************//**
 * \file   kqttitlebar.h
 * \brief  自定义qt标题栏
 * 
 * \author jiang
 * \date   2023.10.20
 *********************************************************************/
#pragma once
#ifndef KQTTITLEBAR_H
#define KQTTITLEBAR_H

#include "cstandard.h"
#include "qstandard.h"
 //调用WIN API需要用到的头文件与库
#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif
#include "stylesheet.h"

class KQtTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit KQtTitleBar(QWidget* parent = nullptr, QWidget* child = nullptr)
        : QWidget(parent)
    {
        setAttribute(Qt::WA_StyledBackground);
        setFixedHeight(40);

        setStyleSheet(titlebar_stylesheet.c_str());
        this->setContentsMargins(0, 0, 0, 0);

        //给成员变量申请内存
        m_pIconLabel      = new QLabel(this);
        m_pTitleLabel     = new QLabel(this);
        m_pCustomerLabel  = new QLabel(this);
        m_pMinimizeButton = new QPushButton(this);
        m_pMaximizeButton = new QPushButton(this);
        m_pCloseButton    = new QPushButton(this);

        //初始化图标Label
        m_pIconLabel->setFixedSize(30, 26);
        //m_pIconLabel->setScaledContents(true);
        m_pIconLabel->setStyleSheet(transparent_stylesheet.c_str());
        m_pIconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_pCustomerLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        m_pCustomerLabel->setFixedHeight(38);
        m_pCustomerLabel->setStyleSheet(transparent_stylesheet.c_str());
        if (child)
        {
            AddChildWidget(child);
        }
        //设置按钮的固定大小、图片、取消边框
        m_pMinimizeButton->setFixedSize(45, 38);
        m_pMinimizeButton->setIconSize(QSize(27, 22));
        //m_pMinimizeButton->setIcon(QIcon(":/kcommon/kqttitlebar_image/min.png"));
        m_pMinimizeButton->setFlat(true);
        m_pMinimizeButton->setStyleSheet((titlebar_button_stylesheet + min_button_stylesheet).c_str());
        //--
        m_pMaximizeButton->setFixedSize(45, 38);
        m_pMaximizeButton->setIconSize(QSize(27, 22));
        //m_pMaximizeButton->setIcon(QIcon(":/kcommon/kqttitlebar_image/max.png"));
        m_pMaximizeButton->setFlat(true);
        m_pMaximizeButton->setCheckable(true);
        m_pMaximizeButton->setStyleSheet((titlebar_button_stylesheet + max_button_stylesheet).c_str());
        //--
        m_pCloseButton->setFixedSize(45, 38);
        m_pCloseButton->setIconSize(QSize(27, 22));
        //m_pCloseButton->setIcon(QIcon(":/kcommon/kqttitlebar_image/close.png"));
        m_pCloseButton->setFlat(true);
        m_pCloseButton->setStyleSheet((titlebar_button_stylesheet + close_button_stylesheet).c_str());

        //设置窗口部件的名称
        m_pTitleLabel->setObjectName("whiteLabel");
        m_pMinimizeButton->setObjectName("minimizeButton");
        m_pMaximizeButton->setObjectName("maximizeButton");
        m_pCloseButton->setObjectName("closeButton");


        //给按钮设置静态tooltip，当鼠标移上去时显示tooltip
        m_pMinimizeButton->setToolTip("Minimize");
        m_pMaximizeButton->setToolTip("Maximize");
        m_pCloseButton->setToolTip("Close");

        //标题栏布局
        QHBoxLayout* pLayout = new QHBoxLayout(this);
        pLayout->setSpacing(0);
        pLayout->setMargin(0);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pLayout->addSpacing(5);
        pLayout->addWidget(m_pIconLabel);        
        pLayout->addWidget(m_pTitleLabel);
        m_pTitleLabel->hide();
        pLayout->addSpacing(5);
        //pLayout->addStretch();
        pLayout->addWidget(m_pCustomerLabel);
        //pLayout->addStretch();
        pLayout->addSpacing(80);
        pLayout->addWidget(m_pMinimizeButton);
        pLayout->addWidget(m_pMaximizeButton);
        pLayout->addWidget(m_pCloseButton);

        setLayout(pLayout);

        //btnLoad->setStyleSheet("background:transparent;border-width:0;border-style:outset");

        //连接三个按钮的信号槽
        connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
        connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
        connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    }
    ~KQtTitleBar()
    {

    }

private:
    void AddChildWidget(QWidget* widget)
    {
        QHBoxLayout* pLayout = new QHBoxLayout(m_pCustomerLabel);
        pLayout->setMargin(0);    
        pLayout->setContentsMargins(0, 0, 0, 0);
        pLayout->addWidget(widget);
        m_pCustomerLabel->setLayout(pLayout);
    }

protected:

    //双击标题栏进行界面的最大化/还原
    virtual void mouseDoubleClickEvent(QMouseEvent* event)
    {
        Q_UNUSED(event); //没有实质性的作用，只是用来允许event可以不使用，用来避免编译器警告

        emit m_pMaximizeButton->clicked();
    }


    //进行界面的拖动 //进行界面的拖动  [一般情况下，是界面随着标题栏的移动而移动，所以我们将事件写在标题栏中比较合理]
    virtual void mousePressEvent(QMouseEvent* event)
    {
#ifdef Q_OS_WIN
        if (ReleaseCapture())
        {
            QWidget* pWindow = this->window();
            if (pWindow->isTopLevel())
            {
                SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
            }
        }
        event->ignore();
#else
#endif
    }

    //设置界面标题与图标 //使用事件过滤器监听标题栏所在的窗体，所以当窗体标题、图标等信息发生改变时，标题栏也应该随之改变
    virtual bool eventFilter(QObject* obj, QEvent* event)
    {
        switch (event->type()) //判断发生事件的类型
        {
        case QEvent::WindowTitleChange: //窗口标题改变事件
        {
            QWidget* pWidget = qobject_cast<QWidget*>(obj); //获得发生事件的窗口对象
            if (pWidget)
            {
                //窗体标题改变，则标题栏标题也随之改变
                m_pTitleLabel->setText(pWidget->windowTitle());
                return true;
            }
        }
        break;

        case QEvent::WindowIconChange: //窗口图标改变事件
        {
            QWidget* pWidget = qobject_cast<QWidget*>(obj);
            if (pWidget)
            {
                //窗体图标改变，则标题栏图标也随之改变
                QIcon icon = pWidget->windowIcon();
                m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
                return true;
            }
        }
        break;

        case QEvent::Resize:
            updateMaximize(); //最大化/还原
            return true;

        default:
            return QWidget::eventFilter(obj, event);
        }

        return QWidget::eventFilter(obj, event);
    }

private slots:

    //进行最小化、最大化/还原、关闭操作
    void onClicked()
    {
        //QObject::Sender()返回发送信号的对象的指针，返回类型为QObject *
        QPushButton* pButton = qobject_cast<QPushButton*>(sender());

        QWidget* pWindow = this->window(); //获得标题栏所在的窗口

        if (pWindow->isTopLevel())
        {
            //判断发送信号的对象使哪个按钮
            if (pButton == m_pMinimizeButton)
            {
                pWindow->showMinimized(); //窗口最小化显示
            }
            else if (pButton == m_pMaximizeButton)
            {
                pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();  //窗口最大化/还原显示
                if (pWindow->isMaximized())
                {
                    //Widget::showNormal();//还原事件
                    //m_pMaximizeButton->setIcon(QIcon(":/kcommon/kqttitlebar_image/restore.png"));
                    m_pMaximizeButton->setChecked(true);
                }
                else
                {
                    //Widget::showMaximized();//最大化事件
                    //m_pMaximizeButton->setIcon(QIcon(":/kcommon/kqttitlebar_image/max.png"));
                    m_pMaximizeButton->setChecked(false);
                }
            }
            else if (pButton == m_pCloseButton)
            {
                pWindow->close(); //窗口关闭
            }
        }
    }

private:

    //最大化/还原 //最大化/还原
    void updateMaximize()
    {
        QWidget* pWindow = this->window(); //获得标题栏所在的窗口

        if (pWindow->isTopLevel())
        {
            bool bMaximize = pWindow->isMaximized(); //判断窗口是不是最大化状态，是则返回true，否则返回false
            if (bMaximize)
            {
                //目前窗口是最大化状态，则最大化/还原的toolTip设置为"Restore"
                m_pMaximizeButton->setToolTip(tr("Restore"));
                //设置按钮的属性名为"maximizeProperty"
                m_pMaximizeButton->setProperty("maximizeProperty", "restore");
                m_pMaximizeButton->setChecked(true);
            }
            else
            {
                //目前窗口是还原状态，则最大化/还原的toolTip设置为"Maximize"
                m_pMaximizeButton->setToolTip(tr("Maximize"));
                //设置按钮的属性名为"maximizeProperty"
                m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
                m_pMaximizeButton->setChecked(false);
            }

            m_pMaximizeButton->setStyle(QApplication::style());
        }
    }

private:
    QLabel* m_pIconLabel; //标题栏图标
    QLabel* m_pTitleLabel; //标题栏标题
    QLabel* m_pCustomerLabel; //标题栏用户控件
    QPushButton* m_pMinimizeButton; //最小化按钮
    QPushButton* m_pMaximizeButton; //最大化/还原按钮
    QPushButton* m_pCloseButton; //关闭按钮
};

#endif // KQTTITLEBAR_H



