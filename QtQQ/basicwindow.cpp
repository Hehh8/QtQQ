#include "basicwindow.h"

#include <QFile>

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
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
	QFile file(":/Resources/QSS" + sheetName + ".css");
	file.open(QFile::ReadOnly);

	if (file.isOpen())
	{
		setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());

		// 获取用户当前的皮肤RGB值
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		qsstyleSheet += QString("QWidget[titleskin=true]{"
								"background-color:rgb(%1,%2,%3);"
								"border-top-left-radius:4px;}"
								"QWidget[bottomskin=true]{"
								"border-top:1px solid rgba(%1,%2,%3,100);"
								"background-color:rgba(%1,%2,%3,50);"
								"border-bottom-left-radius:4px;"
								"border-bottom-right-radius:4px;")
								.arg(r).arg(g).arg(b);

		setStyleSheet(qsstyleSheet);
	}
	file.close();
}

QPixmap BasicWindow::getRoundImage(const QPixmap & src, QPixmap & mask, QSize makesize)
{
	return QPixmap();
}

void BasicWindow::initBackGroundColor()
{
}

void BasicWindow::paintEvent(QPaintEvent * event)
{
}

void BasicWindow::mousePressEvent(QMouseEvent * event)
{
}

void BasicWindow::mouseMoveEvent(QMouseEvent * event)
{
}

void BasicWindow::mouseReleaseEvent(QMouseEvent * event)
{
}



void BasicWindow::onShowMin(bool)
{
}

void BasicWindow::onShowHide(bool)
{
}

void BasicWindow::onShowNormal(bool)
{
}

void BasicWindow::onShowQuit(bool)
{
}



void BasicWindow::onButtonMinClicked()
{
}

void BasicWindow::onButtonRestoreClicked()
{
}

void BasicWindow::onButtonMaxClicked()
{
}

void BasicWindow::onButtonCloseClicked()
{
}
