#include "CCMainWindow.h"
#include "skinwindow.h"
#include "systray.h"
#include "notifymanager.h"
#include "rootcontactitem.h"
#include "contactitem.h"

#include <QProxyStyle>
#include <QPainter>
#include <QTimer>
#include <QEvent>
#include <QTreeWidgetItem>

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

	// 设置主窗口风格
	setWindowFlags(windowFlags() | Qt::Tool);

	loadStyleSheet("CCMainWindow");

	initControl();

	initTimer();
}

CCMainWindow::~CCMainWindow()
{
}

void CCMainWindow::initTimer()
{
	QTimer *timer = new QTimer(this);
	timer->setInterval(500);
	connect(timer, &QTimer::timeout, [this]() {
		static int level = 0;
		if (level == 99)
		{
			level = 0;
		}
		level++;
		setLevelPixmap(level);
	});
	timer->start();
}

void CCMainWindow::initControl()
{
	// 树获取焦点时不绘制边框
	ui.treeWidget->setStyle(new CustomProxyStyle);

	setLevelPixmap(0);
	setHeadPixmap(":/Resources/MainWindow/girl.png");
	setStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");

	QHBoxLayout *appupLayout = new QHBoxLayout;
	appupLayout->setContentsMargins(0, 0, 0, 0);
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_7.png", "app_7"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_2.png", "app_7"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_3.png", "app_7"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_4.png", "app_7"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_5.png", "app_7"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_6.png", "app_7"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));
	appupLayout->addStretch();
	appupLayout->setSpacing(2);
	ui.appWidget->setLayout(appupLayout);

	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));
	ui.bottomLayout_up->addStretch();

	initContactTree();

	// 个性签名添加事件过滤器
	ui.lineEdit->installEventFilter(this);
	ui.searchLineEdit->installEventFilter(this);

	connect(ui.sysmin, &QPushButton::clicked, this, &CCMainWindow::onShowHide);
	connect(ui.sysclose, &QPushButton::clicked, this, &CCMainWindow::onShowClose);
	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this]() {
		updateSearchStyle();
	});

	SysTray *sysTray = new SysTray(this);

}

void CCMainWindow::setUserName(const QString & userName)
{
	ui.nameLabel->adjustSize();

	// 文本过程则进行省略...
	// fontMetrics()返回QFontMetrics类对象
	QString name = ui.nameLabel->fontMetrics().elidedText(userName, Qt::ElideRight, ui.nameLabel->width());
	ui.nameLabel->setText(name);
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
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));
}

void CCMainWindow::setStatusMenuIcon(const QString & statusPath)
{
	QPixmap statusBtnPixmap(ui.stausBtn->size());
	statusBtnPixmap.fill(Qt::transparent);

	QPainter painter(&statusBtnPixmap);
	painter.drawPixmap(4, 4, QPixmap(statusPath));

	ui.stausBtn->setIcon(statusBtnPixmap);
	ui.stausBtn->setIconSize(ui.stausBtn->size());
}

QWidget * CCMainWindow::addOtherAppExtension(const QString & appPath, const QString appName)
{
	QPushButton *btn = new QPushButton(this);
	btn->setFixedSize(20, 20);

	QPixmap pixmap(btn->size());
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	QPixmap appPixmap(appPath);
	painter.drawPixmap((btn->width() - appPixmap.width()) / 2, (btn->height() - appPixmap.height()) / 2, appPixmap);
	btn->setIcon(pixmap);
	btn->setIconSize(btn->size());
	btn->setObjectName(appName);
	btn->setProperty("hasborder", true);

	connect(btn, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);

	return btn;
}

void CCMainWindow::initContactTree()
{
	// 展开与收缩时的信号
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &CCMainWindow::onItemClicked);
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &CCMainWindow::onItemExpanded);
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &CCMainWindow::onItemCollapsed);
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &CCMainWindow::onItemDoubleClicked);

	// 根节点
	QTreeWidgetItem *pRootGroupItem = new QTreeWidgetItem;
	pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	pRootGroupItem->setData(0, Qt::UserRole, 0);	// 根项数据设为0

	RootContactitem *pItemName = new RootContactitem(true, ui.treeWidget);

	QString strGroupName = QString::fromLocal8Bit("同学");
	pItemName->setText(strGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootGroupItem);
	ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName);

	QStringList sCompDeps;
	sCompDeps << QString::fromLocal8Bit("六(4)班");
	sCompDeps << QString::fromLocal8Bit("初三(3)班");
	sCompDeps << QString::fromLocal8Bit("高三(4)班");

	for (int nIndex = 0; nIndex < sCompDeps.length(); nIndex++)
	{
		addGroupDeps(pRootGroupItem, sCompDeps.at(nIndex));
	}
}

void CCMainWindow::updateSearchStyle()
{
	ui.searchWidget->setStyleSheet(QString(
		"QWidget#searchWidget {"
		"   background-color: rgba(%1, %2, %3, 50);"
		"   border-bottom: 1px solid rgba(%1, %2, %3, 30);"
		"}"
		"QPushButton#searchBtn {"
		"   border-image: url(:/Resources/MainWindow/search/search_icon.png);"  // 按钮图片
		"}"
	).arg(m_colorBackGround.red())   // 替换 %1 为红色分量
		.arg(m_colorBackGround.green()) // 替换 %2 为绿色分量
		.arg(m_colorBackGround.blue())  // 替换 %3 为蓝色分量
	);
}

void CCMainWindow::addGroupDeps(QTreeWidgetItem * pRootGroupItem, const QString & sDeps)
{
	QTreeWidgetItem *pChild = new QTreeWidgetItem;


	// 添加子节点
	pChild->setData(0, Qt::UserRole, 1);	// 子项数据设为1
	ContactItem *pContactItem = new ContactItem(ui.treeWidget);

	pContactItem->setHeadPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"), QPixmap(":/Resources/MainWindow/head_mask.png"), pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(sDeps);

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

}

void CCMainWindow::resizeEvent(QResizeEvent * event)
{
	setUserName(QString::fromLocal8Bit("12345678900000000"));
	BasicWindow::resizeEvent(event);
}

bool CCMainWindow::eventFilter(QObject * obj, QEvent * event)
{
	if (obj == ui.searchLineEdit)
	{
		// 键盘焦点事件
		if (event->type() == QEvent::FocusIn)
		{
			// 设置 searchWidget 和按钮的样式表
			ui.searchWidget->setStyleSheet(QString(
				"QWidget#searchWidget {"
				"   background-color: rgb(255, 255, 255);"  // 白色背景
				"   border-bottom: 1px solid rgba(%1, %2, %3, 100);"  // 半透明底边框
				"}"
				"QPushButton#searchBtn {"
				"   border-image: url(:/Resources/MainWindow/search/main_search_deldown.png);"  // 按钮图片
				"}"
				"QPushButton#searchBtn:hover {"
				"   border-image: url(:/Resources/MainWindow/search/main_search_delhighlight.png);"  // 鼠标悬停
				"}"
				"QPushButton#searchBtn:pressed {"
				"   border-image: url(:/Resources/MainWindow/search/main_search_delhighdown.png);"  // 按下时的图片
				"}"
			).arg(m_colorBackGround.red())   // 替换 %1 为红色分量
				.arg(m_colorBackGround.green()) // 替换 %2 为绿色分量
				.arg(m_colorBackGround.blue())  // 替换 %3 为蓝色分量
			);
		}
		else if (event->type() == QEvent::FocusOut)
		{
			updateSearchStyle();
		}
		
	}
	return false;
}

void CCMainWindow::onItemClicked(QTreeWidgetItem * item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();

	if (!bIsChild)
	{
		item->setExpanded(!item->isExpanded());	// 未展开则展开子项
	}
}

void CCMainWindow::onItemExpanded(QTreeWidgetItem * item)
{
}

void CCMainWindow::onItemCollapsed(QTreeWidgetItem * item, int column)
{
}

void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem * item)
{
}

void CCMainWindow::onAppIconClicked()
{
	// 判断信号发送者的对象名是否是app_skin
	if (sender()->objectName() == "app_skin")
	{
		SkinWindow *skinWindow = new SkinWindow;
		skinWindow->show();
	}
}
