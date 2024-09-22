#include "basicwindow.h"
#include "CommonUtils.h"
#include "notifymanager.h"

#include <QDesktopWidget>
#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	m_colorBackGround = CommonUtils::getDefaultSkinColor();
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, this, &BasicWindow::onSignalSkinChanged);
/*	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)), this, SLOT(onSignalSkinChanged(const QColor&)));*/
}

BasicWindow::~BasicWindow()
{
}

void BasicWindow::onSignalSkinChanged(const QColor & color)
{
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);
}

void BasicWindow::setTitleBarTitle(const QString title, const QString & icon)
{
	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

void BasicWindow::initTitleBar(ButtonType buttontype)
{
	_titleBar = new TitleBar(this);
	_titleBar->setButtonType(buttontype);
	_titleBar->move(0, 0);

	connect(_titleBar, &TitleBar::signalButtonMinClicked, this, &BasicWindow::onButtonMinClicked);
	connect(_titleBar, &TitleBar::signalButtonRestoreClicked, this, &BasicWindow::onButtonRestoreClicked);
	connect(_titleBar, &TitleBar::signalButtonMaxClicked, this, &BasicWindow::onButtonMaxClicked);
	connect(_titleBar, &TitleBar::signalButtonCloseClicked, this, &BasicWindow::onButtonCloseClicked);
}

void BasicWindow::loadStyleSheet(const QString & sheetName)
{
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);

	if (file.isOpen())
	{
		setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());

		// 获取用户当前的皮肤RGB值
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		qDebug() << "r = " << r << "  g = " << g << "  b = " << b;

		qsstyleSheet += QString("QWidget[titleskin=true]{"
								"background-color:rgb(%1,%2,%3);"
								"border-top-left-radius:4px;}"
								"QWidget[bottomskin=true]{"
								"border-top:1px solid rgba(%1,%2,%3,100);"
								"background-color:rgba(%1,%2,%3,50);"
								"border-bottom-left-radius:4px;"
								"border-bottom-right-radius:4px;}")
								.arg(r).arg(g).arg(b);

		setStyleSheet(qsstyleSheet);
		file.close();
	}
}

// 背景图
void BasicWindow::initBackGroundColor()
{
	QStyleOption opt;
	opt.init(this);

	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

// 子类初始化部时,需要重写绘图事件设置背景图
void BasicWindow::paintEvent(QPaintEvent * event)
{
	initBackGroundColor();
	QDialog::paintEvent(event);
}

// 头像转圆头像
QPixmap BasicWindow::getRoundImage(const QPixmap & src, QPixmap & mask, QSize maskSize)
{
	if (maskSize == QSize(0, 0))
	{
		maskSize = mask.size();
	}
	else
	{
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	// 保存转换后的图像
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();

	return QPixmap::fromImage(resultImage);
}

void BasicWindow::onShowClose(bool)
{
	close();
}

void BasicWindow::onShowMin(bool)
{
	showMinimized();
}

void BasicWindow::onShowHide(bool)
{
	hide();
}

void BasicWindow::onShowNormal(bool)
{
	show();
	activateWindow();
}

void BasicWindow::onShowQuit(bool)
{
	QApplication::quit();
}

// 鼠标按下事件
void BasicWindow::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_mousePressed = true;
		m_mousePoint = event->globalPos() - pos();
		event->accept();
	}
}

// 鼠标移动事件
void BasicWindow::mouseMoveEvent(QMouseEvent * event)
{
	if (m_mousePressed && (event->button() == Qt::LeftButton))
	{
		move(event->globalPos() - m_mousePoint);
		event->accept();
	}
}

// 鼠标释放事件
void BasicWindow::mouseReleaseEvent(QMouseEvent * event)
{
	m_mousePressed = false;
}

void BasicWindow::onButtonMinClicked()
{
	if (Qt::Tool == (windowFlags() & Qt::Tool))
	{
		hide();
	}
	else
	{
		showMinimized();
	}
}

void BasicWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;
	_titleBar->getRestoreInfo(windowPos, windowSize);
	setGeometry(QRect(windowPos, windowSize));
}

void BasicWindow::onButtonMaxClicked()
{
	_titleBar->setRestoreInfo(pos(), QSize(width(), height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(factRect);
}

void BasicWindow::onButtonCloseClicked()
{
	close();
}
