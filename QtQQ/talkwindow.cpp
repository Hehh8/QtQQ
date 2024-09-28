#include "talkwindow.h"
#include "rootcontactitem.h"
#include "contactitem.h"
#include "CommonUtils.h"
#include "windowmanager.h"
#include <QToolTip>
#include <QFile>
#include <QMessageBox>

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
	ui.textEdit->setFocus();
	ui.textEdit->addEmotionUrl(emotionNum);
}

void TalkWindow::setWindowName(const QString & name)
{
	ui.nameLabel->setText(name);
}

void TalkWindow::onSendBtnClicked(bool)
{
	if (ui.textEdit->toPlainText().isEmpty())
	{
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)), 
			QString::fromLocal8Bit("发送的信息不能为空!"),
			this, QRect(0, 0, 120, 100), 2000);
		return;
	}

	QString &html = ui.textEdit->document()->toHtml();

	// 文本html如果没有字体则添加字体
	if (!html.contains(".png") && !html.contains("</span>"))
	{
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");

		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			fontHtml.replace("%1", text);
			file.close();
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("msgFont.txt不存在"));
			return;
		}

		if (!html.contains(fontHtml))
		{
			html.replace(text, fontHtml);
		}
	}

	ui.textEdit->clear();
	ui.textEdit->deleteAllEmotionImage();

	ui.msgWidget->appendMsg(html);

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
		initPtoPTalk();
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
