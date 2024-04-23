/*****************************************************************//**
 * \file   kqtdialog.h
 * \brief  包含KQtTitleBar的窗口
 *
 * \author jiang
 * \date   2023.10.20
 *********************************************************************/
 /*************************************************************************************
  * 功能：实现自定义窗体的无边框与移动,以及自定义标题栏-用来显示窗体的图标、标题，以及控制窗体最小化、最大化/还原、关闭、缩放（仅支持windows平台）。
 *************************************************************************************/

#pragma once
#ifndef KQTDIALOG_H
#define KQTDIALOG_H

#include "cstandard.h"
#include "qstandard.h"
#include "kqttitlebar.h"
 //调用WIN API需要用到的头文件与库 [实现缩放]
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#endif

#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <Windows.h>
#pragma comment(lib, "dwmapi")
#pragma comment(lib,"user32.lib")
#include <dwmapi.h>
#include <windowsx.h>


class KQtDialog : public QDialog
{
	Q_OBJECT

public:
	explicit KQtDialog(QWidget* parent = nullptr) : QDialog(parent)
	{
#if 0
		//Qt::FramelessWindowHint设置窗口标志为无边框，而Qt::WindowStaysOnTopHint使窗口位于所有界面之上
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		//背景透明
		//setAttribute(Qt::WA_TranslucentBackground, true);

		//定义自定义标题栏对象
		m_titlebar = new KQtTitleBar(this);
		installEventFilter(m_titlebar);

		resize(400, 300);
		setWindowTitle("Custom Window"); //设置窗口名称，会发生窗口标题栏改变事件，随之自定义标题栏的标题会更新
		setWindowIcon(QIcon("://Images/icon.png")); //设置窗口图标，会发生窗口图标改变事件，随之自定义标题栏的图标会更新

		//使用调色板设置窗口的背景色
		QPalette pal(palette());
		pal.setColor(QPalette::Background, QColor(150, 150, 150));
		setAutoFillBackground(true);
		setPalette(pal);

		//窗口布局中加入标题栏
		QVBoxLayout* pLayout = new QVBoxLayout();
		pLayout->addWidget(pTitleBar);
		pLayout->addStretch();
		pLayout->setSpacing(0);
		pLayout->setContentsMargins(0, 0, 0, 0);
		setLayout(pLayout);
#endif
		//setWindowFlags(Qt::FramelessWindowHint/* | windowFlags()*/);
		setWindowFlags(Qt::FramelessWindowHint/* | windowFlags()*/);
		setMouseTracking(true);
		//setAttribute(Qt::WA_Hover);
		//
		//m_titlebar = new KQtTitleBar(this);
		//m_titlebar->setObjectName("titlebar");
		//m_titlebar->setStyleSheet(
		//	"QWidget{background-color: rgba(59, 68, 83, 255);}"
		//	"QWidget{font-size:20px; color:rgb(217, 217, 217); }"
		//	"QWidget{border:0px solid; border-color:rgba(34, 41, 51, 255);}"
		//);
		////m_titlebar->setFixedHeight(TOOLBAR_HEIGHT);
		//installEventFilter(m_titlebar);
		//m_nBorder表示鼠标位于边框缩放范围的宽度，可以设置为5
		m_nBorderWidth = 5;

		//m_work_panel = new QWidget();
		////m_work_panel->hide();
		//m_work_panel->setStyleSheet("QWidget{border:1px solid red;}");
		//QVBoxLayout* window_layout = new QVBoxLayout;	
		//this->setLayout(window_layout);
		//window_layout->setMargin(0);
		//window_layout->setContentsMargins(0, 0, 0, 0);
		////hostwidget_layout->setContentsMargins(5, 5, 5, 5);
		//window_layout->setSpacing(0);
		//window_layout->addWidget(m_titlebar);
		//window_layout->addWidget(m_work_panel);
	}
	~KQtDialog()
	{
	}

	LRESULT OnTestBorder(const QPoint& pt)	
	{
		if (::IsZoomed((HWND)this->winId()))
		{
			return HTCLIENT;
		}
		int borderSize = 4;
		int cx = this->size().width();
		int cy = this->size().height();
		QRect rectTopLeft(0, 0, borderSize, borderSize);
		if (rectTopLeft.contains(pt))
		{
			return HTTOPLEFT;
		}
		QRect rectLeft(0, borderSize, borderSize, cy - borderSize * 2);
		if (rectLeft.contains(pt))
		{
			return HTLEFT;
		}
		QRect rectTopRight(cx - borderSize, 0, borderSize, borderSize);
		if (rectTopRight.contains(pt))
		{
			return HTTOPRIGHT;
		}
		QRect rectRight(cx - borderSize, borderSize, borderSize, cy - borderSize * 2);
		if (rectRight.contains(pt))
		{
			return HTRIGHT;
		}
		QRect rectTop(borderSize, 0, cx - borderSize * 2, borderSize);
		if (rectTop.contains(pt))
		{
			return HTTOP;
		}
		QRect rectBottomLeft(0, cy - borderSize, borderSize, borderSize);
		if (rectBottomLeft.contains(pt))
		{
			return HTBOTTOMLEFT;
		}
		QRect rectBottomRight(cx - borderSize, cy - borderSize, borderSize, borderSize);
		if (rectBottomRight.contains(pt))
		{
			return HTBOTTOMRIGHT;
		}
		QRect rectBottom(borderSize, cy - borderSize, cx - borderSize * 2, borderSize);
		if (rectBottom.contains(pt))
		{
			return HTBOTTOM;
		}
		return HTCLIENT;
	}
	
#if 0
		//nativeEvent主要用于进程间通信-消息传递，使用这种方式后来实现窗体的缩放 [加上了这函数，窗口也能移动了]
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override
	{
		Q_UNUSED(eventType)

			MSG* param = static_cast<MSG*>(message);

		switch (param->message)
		{
		case WM_NCHITTEST:
		{
			int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
			int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

#if 1
			// 如果鼠标位于子控件上，则不进行处理
			if (childAt(nX, nY) != nullptr)
				return QWidget::nativeEvent(eventType, message, result);
#endif

			* result = HTCAPTION;

			// 鼠标区域位于窗体边框，进行缩放
			if ((nX > 0) && (nX < m_nBorderWidth))
				*result = HTLEFT;

			if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
				*result = HTRIGHT;

			if ((nY > 0) && (nY < m_nBorderWidth))
				*result = HTTOP;

			if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
				*result = HTBOTTOM;

			if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
				&& (nY < m_nBorderWidth))
				*result = HTTOPLEFT;

			if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
				&& (nY > 0) && (nY < m_nBorderWidth))
				*result = HTTOPRIGHT;

			if ((nX > 0) && (nX < m_nBorderWidth)
				&& (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
				*result = HTBOTTOMLEFT;

			if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
				&& (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
				*result = HTBOTTOMRIGHT;

			return true;
		}
		}

		return QWidget::nativeEvent(eventType, message, result);
	}
#endif

#if 0
	//nativeEvent主要用于进程间通信-消息传递，使用这种方式后来实现窗体的缩放 [加上了这函数，窗口也能移动了]
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override
	{
		Q_UNUSED(eventType)

			MSG* param = static_cast<MSG*>(message);

		switch (param->message)
		{
#if 1
		case WM_NCHITTEST:
		{
			int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
			int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

			// 如果鼠标位于子控件上，则不进行处理
			if (childAt(nX, nY) != nullptr)
				return QDialog::nativeEvent(eventType, message, result);

			*result = HTCAPTION;

			// 鼠标区域位于窗体边框，进行缩放
			if ((nX > 0) && (nX < m_nBorderWidth))
				*result = HTLEFT;

			if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
				*result = HTRIGHT;

			if ((nY > 0) && (nY < m_nBorderWidth))
				*result = HTTOP;

			if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
				*result = HTBOTTOM;

			if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
				&& (nY < m_nBorderWidth))
				*result = HTTOPLEFT;

			if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
				&& (nY > 0) && (nY < m_nBorderWidth))
				*result = HTTOPRIGHT;

			if ((nX > 0) && (nX < m_nBorderWidth)
				&& (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
				*result = HTBOTTOMLEFT;

			if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
				&& (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
				*result = HTBOTTOMRIGHT;

			return true;
		}
#endif
#if 0
		case WM_NCHITTEST:
		{
			//处理resize
			//标记只处理resize
			bool isResize = false;

			int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
			int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();
			//鼠标点击的坐标
			POINT ptMouse = { GET_X_LPARAM(param->lParam), GET_Y_LPARAM(param->lParam) };
			//窗口矩形
			RECT rcWindow;
			GetWindowRect(param->hwnd, &rcWindow);
			RECT rcFrame = { 0,0,0,0 };
			AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);
			USHORT uRow = 1;
			USHORT uCol = 1;
			bool fOnResizeBorder = false;

			//确认鼠标指针是否在top或者bottom
			if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + 1)
			{
				fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
				uRow = 0;
				isResize = true;
			}
			else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - 5)
			{
				uRow = 2;
				isResize = true;
			}
			//确认鼠标指针是否在left或者right
			if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + 5)
			{
				uCol = 0; // left side
				isResize = true;
			}
			else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - 5)
			{
				uCol = 2; // right side
				isResize = true;
			}
			if (ptMouse.x >= rcWindow.left && ptMouse.x <= rcWindow.right - 135 && ptMouse.y > rcWindow.top + 3 && ptMouse.y <= rcWindow.top + 30)
			{
				*result = HTCAPTION;
				return true;
			}

			LRESULT hitTests[3][3] =
			{
				{ HTTOPLEFT,    fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
				{ HTLEFT,       HTNOWHERE,     HTRIGHT },
				{ HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
			};

			if (isResize == true)
			{
				*result = hitTests[uRow][uCol];
				return true;
			}
			else
			{
				return false;
			}

		}
#endif
		}

		return QDialog::nativeEvent(eventType, message, result);
	}
#endif

#if 0
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override
	{
#ifdef Q_OS_WIN
		if (eventType != "windows_generic_MSG")
			return false;

		MSG* msg = static_cast<MSG*>(message);
		QWidget* widget = QWidget::find(reinterpret_cast<WId>(msg->hwnd));
		if (!widget)
			return false;

		switch (msg->message) {

		case WM_NCCALCSIZE: {
			*result = 0;
			return true;
		}

		case WM_NCHITTEST:
		{
			int x = GET_X_LPARAM(msg->lParam);
			int y = GET_Y_LPARAM(msg->lParam);
			QPoint pt = mapFromGlobal(QPoint(x, y));
			*result = OnTestBorder(pt);
			if (*result == HTCLIENT)
			{
				QWidget* tempWidget = this->childAt(pt.x(), pt.y());
				if (tempWidget == NULL)
				{
					*result = HTCAPTION;
				}
			}
			return true;
		}

		case WM_GETMINMAXINFO: {
			if (::IsZoomed(msg->hwnd)) {

				RECT frame = { 0, 0, 0, 0 };
				AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
				frame.left = abs(frame.left);
				frame.top = abs(frame.bottom);
				widget->setContentsMargins(frame.left, frame.top, frame.right, frame.bottom);
			}
			else {
				widget->setContentsMargins(0, 0, 0, 0);
			}

			*result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
			return true;
		}
							 break;

		default:
			break;
		}

#endif

		return QWidget::nativeEvent(eventType, message, result);
	}
#endif


#if 1
	//过滤掉消息返回true，否则返回false
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override
	{
		MSG* pMsg = reinterpret_cast<MSG*>(message);

		switch (pMsg->message)
		{
			//去掉边框
		case WM_NCCALCSIZE:
		{
			*result = 0;
			return true;
			break;
		}
#if 0
		//阴影
		case WM_ACTIVATE:
		{
			MARGINS margins = { 1,1,1,1 };
			HRESULT hr = S_OK;
			hr = DwmExtendFrameIntoClientArea(pMsg->hwnd, &margins);
			*result = hr;
			return true;
		}
#endif
		case WM_NCHITTEST:
		{
			//处理resize
			//标记只处理resize
			bool isResize = false;

			//鼠标点击的坐标
			POINT ptMouse = { GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam) };
			//窗口矩形
			RECT rcWindow;
			GetWindowRect(pMsg->hwnd, &rcWindow);
			RECT rcFrame = { 0,0,0,0 };
			AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);
			USHORT uRow = 1;
			USHORT uCol = 1;
			bool fOnResizeBorder = false;

			//确认鼠标指针是否在top或者bottom，顺带说一下屏幕坐标原点是左上角，窗体坐标原点也是左上角
			if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + m_nBorderWidth)
			{
				fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
				uRow = 0;
				isResize = true;
			}
			else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - m_nBorderWidth)
			{
				uRow = 2;
				isResize = true;
			}
			//确认鼠标指针是否在left或者right
			if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + m_nBorderWidth)
			{
				uCol = 0; // left side
				isResize = true;
			}
			else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - m_nBorderWidth)
			{
				uCol = 2; // right side
				isResize = true;
			}
#if 0
			//检测是不是在标题栏上，右边预留出了45*3 = 135的宽度，是留给关闭按钮、最大化、最小化的。
			if (ptMouse.x >= rcWindow.left && ptMouse.x <= rcWindow.right - 135 && ptMouse.y > rcWindow.top + 3 && ptMouse.y <= rcWindow.top + 30)
			{
				*result = HTCAPTION;
				return true;
			}
#endif

			LRESULT hitTests[3][3] =
			{
				{ HTTOPLEFT,    fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
				{ HTLEFT,       HTNOWHERE,     HTRIGHT },
				{ HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
			};

			if (isResize == true)
			{
				*result = hitTests[uRow][uCol];
				return true;
			}
			else
			{
#if 1
				// 如果鼠标位于子控件上，则不进行处理
				if (childAt(ptMouse.x, ptMouse.y) != nullptr)
				{
					return QWidget::nativeEvent(eventType, message, result);
				}					
#endif
				return false;
			}

		}
		}

		//这里一定要返回false，否则是屏蔽所有消息了
		return false;
	}
#endif 

public:
	//KQtTitleBar* m_titlebar = nullptr;
	//QWidget*   m_work_panel = nullptr;
	int m_nBorderWidth = 5; //m_nBorder表示鼠标位于边框缩放范围的宽度
};

#endif // KQTDIALOG_H




