#include "talkwindow.h"
#include "rootcontactitem.h"
#include "contactitem.h"
#include "CommonUtils.h"
#include "windowmanager.h"

TalkWindow::TalkWindow(QWidget *parent, const QString &uid, GroupType groupType)
	: QWidget(parent)
	, m_talkId(uid)
	, m_groupType(groupType)
{
	ui.setupUi(this);
	WindowManager::getInstance()->addWindowName(m_talkId, this);
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
}

TalkWindow::~TalkWindow()
{
	WindowManager::getInstance()->deleteWindowName(m_talkId);
}

void TalkWindow::addEmotionImage(int emotionNum)
{
}

void TalkWindow::setWindowName(const QString & name)
{
	ui.nameLabel->setText(name);
}

void TalkWindow::onSendBtnClicked(bool)
{
}


void TalkWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild)
	{
		QString strPeopleName = m_groupPeopleMap.value(item);
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), PTOP, strPeopleName);
	}
}

void TalkWindow::setMsgLabelContent(const QString & name)
{
}

void TalkWindow::initControl()
{
	QList<int> rightWidgetSize;
	rightWidgetSize << 600 << 138;
	ui.bodySplitter->setSizes(rightWidgetSize);

	ui.textEdit->setFontPointSize(10);
	ui.textEdit->setFocus();

	connect(ui.sysmin, SIGNAL(clicked(bool)), parent(), SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));

	connect(ui.faceBtn, SIGNAL(clicked(bool)), parent(), SLOT(onEmotionBtnClicked(bool)));
	connect(ui.sendBtn, SIGNAL(clicked(bool)), this, SLOT(onSendBtnClicked(bool)));

	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

	switch (m_groupType)
	{
	case COMPANY:
	{
		initCompanyTalk();
	}
		break;
	case PERSONELGROUP:
	{
		initPersonTalk();
	}
		break;
	case DEVELOPMENTGROUP:
	{
		initDevelopTalk();
	}
		break;
	case MARKETGROUP:
	{
		initMarkTalk();
	}
		break;
	case PTOP:
	{
		initPtoPTalk();
	}
		break;
	default:
	{
	}
		break;
	}
}

void TalkWindow::initCompanyTalk()
{
	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactitem *pItemName = new RootContactitem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);

	int nEmployeeNum = 50;
	QString qsGroupName = QString::fromLocal8Bit("公司群%1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	// 展开
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; ++i)
	{
		addPeopleInfo(pRootItem);
	}
}

void TalkWindow::initPersonTalk()
{
	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactitem *pItemName = new RootContactitem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);

	int nEmployeeNum = 5;
	QString qsGroupName = QString::fromLocal8Bit("人事部%1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	// 展开
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; ++i)
	{
		addPeopleInfo(pRootItem);
	}
}

void TalkWindow::initDevelopTalk()
{
	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactitem *pItemName = new RootContactitem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);

	int nEmployeeNum = 32;
	QString qsGroupName = QString::fromLocal8Bit("研发部%1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	// 展开
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; ++i)
	{
		addPeopleInfo(pRootItem);
	}
}

void TalkWindow::initMarkTalk()
{
	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactitem *pItemName = new RootContactitem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);

	int nEmployeeNum = 8;
	QString qsGroupName = QString::fromLocal8Bit("市场部%1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	// 展开
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; ++i)
	{
		addPeopleInfo(pRootItem);
	}
}

void TalkWindow::initPtoPTalk()
{
	QPixmap pixSkin;
	pixSkin.load(":/Resources/MainWindow/skin.png");

	ui.widget->setFixedSize(pixSkin.size());
	QLabel *skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());
}

void TalkWindow::addPeopleInfo(QTreeWidgetItem *pRootGroupItem)
{
	QTreeWidgetItem *pChild = new QTreeWidgetItem();

	QPixmap pixMask;
	pixMask.load(":/Resources/MainWindow/head_mask.png");
	QPixmap pixSrc(":/Resources/MainWindow/girl.png");

	// 添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, QString::number((int)pChild));
	ContactItem *pContactItem = new ContactItem(ui.treeWidget);

	static int i = 0;
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(pixSrc, pixMask, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(QString::fromLocal8Bit("齐齐%1").arg(i));
	pContactItem->setSignName(QString::fromLocal8Bit("个性签名%1").arg(i++));

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);

}

void TalkWindow::onSetEmotionBtnStatus()
{
}
