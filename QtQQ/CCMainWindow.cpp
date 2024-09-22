#include "CCMainWindow.h"

#include <QProxyStyle>
#include <QPainter>

class CustomProxyStyle :public QProxyStyle
{
public:
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const
	{
		if (element == PE_FrameFocusRect)
		{
			return;
		}
		else
		{
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};

CCMainWindow::CCMainWindow(QWidget *parent)
    : BasicWindow(parent)
{
    ui.setupUi(this);

	setWindowFlags(windowFlags() | Qt::Tool);
	loadStyleSheet("CCMainWindow");

	initControl();
}

CCMainWindow::~CCMainWindow()
{
}

void CCMainWindow::initControl()
{
	// 树获取焦点时不绘制边框
	ui.treeWidget->setStyle(new CustomProxyStyle);

	setLevelPixmap(0);
}

void CCMainWindow::setUserName(const QString & userName)
{
}

void CCMainWindow::setLevelPixmap(int level)
{
	QPixmap levelPixmap(ui.levelBtn->size());
	levelPixmap.fill(Qt::transparent);

	QPainter painter(&levelPixmap);
	painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

	int unitNum = level % 10;	// 个位数
	int tenNum = level / 10;	// 十位数

	// 十位,截取图片中的部分进行绘制
	// drawPixmap(绘制点x, 绘制点y, 图片, 图片左上角x,图片左上角y,拷贝的宽度,拷贝的高度)
	painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), tenNum * 6, 0, 6, 7);

	// 个位
	painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unitNum * 6, 0, 6, 7);

	ui.levelBtn->setIcon(levelPixmap);
	ui.levelBtn->setIconSize(ui.levelBtn->size());
}

void CCMainWindow::setHeadPixmap(const QString & headPath)
{

}
