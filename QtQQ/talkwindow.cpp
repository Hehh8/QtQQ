#include "talkwindow.h"
#include "rootcontactitem.h"
#include "contactitem.h"
#include "CommonUtils.h"
#include "windowmanager.h"
#include <QToolTip>
#include <QFile>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>

TalkWindow::TalkWindow(QWidget *parent, const QString &uid/*, GroupType groupType*/)
	: QWidget(parent)
	, m_talkId(uid)
	//, m_groupType(groupType)
{
	ui.setupUi(this);
	WindowManager::getInstance()->addWindowName(m_talkId, this);
	setAttribute(Qt::WA_DeleteOnClose);
	initGroupTalkStatus();
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
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString()/*, PTOP, strPeopleName*/);
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

	if (m_isGroupTalk)
	{
		initTalkWindow();
	}
	else
	{
		initPtoPTalk();
	}

	/*
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
	*/
}

void TalkWindow::initGroupTalkStatus()
{
	QSqlQueryModel querySqlDepModel; 
	QString strSql = QString("SELECT * FROM tab_department WHERE departmentID = %1").arg(m_talkId);
	querySqlDepModel.setQuery(strSql);

	int rows = querySqlDepModel.rowCount();
	if (rows == 0)	// 单独聊天
	{
		m_isGroupTalk = false;
	}
	else
	{
		m_isGroupTalk = true;
	}

}

int TalkWindow::getCompDepID()
{
	QString strSqlDepID = "SELECT departmentID FROM tab_department WHERE department_name = :department_name";
	QSqlQuery queryDepID;
	queryDepID.prepare(strSqlDepID);
	queryDepID.bindValue(":department_name", QString::fromLocal8Bit("公司群"));
	queryDepID.exec();
	queryDepID.next();
	int companyID = queryDepID.value(0).toInt();
	return companyID;
}

// void TalkWindow::initCompanyTalk()
// {
// 	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
// 	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
// 
// 	pRootItem->setData(0, Qt::UserRole, 0);
// 	RootContactitem *pItemName = new RootContactitem(false, ui.treeWidget);
// 
// 	ui.treeWidget->setFixedHeight(646);
// 
// 	int nEmployeeNum = 50;
// 	QString qsGroupName = QString::fromLocal8Bit("公司群%1/%2").arg(0).arg(nEmployeeNum);
// 	pItemName->setText(qsGroupName);
// 
// 	// 插入分组节点
// 	ui.treeWidget->addTopLevelItem(pRootItem);
// 	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
// 
// 	// 展开
// 	pRootItem->setExpanded(true);
// 
// 	for (int i = 0; i < nEmployeeNum; ++i)
// 	{
// 		addPeopleInfo(pRootItem);
// 	}
// }
// 
// void TalkWindow::initPersonTalk()
// {
// 	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
// 	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
// 
// 	pRootItem->setData(0, Qt::UserRole, 0);
// 	RootContactitem *pItemName = new RootContactitem(false, ui.treeWidget);
// 
// 	ui.treeWidget->setFixedHeight(646);
// 
// 	int nEmployeeNum = 5;
// 	QString qsGroupName = QString::fromLocal8Bit("人事部%1/%2").arg(0).arg(nEmployeeNum);
// 	pItemName->setText(qsGroupName);
// 
// 	// 插入分组节点
// 	ui.treeWidget->addTopLevelItem(pRootItem);
// 	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
// 
// 	// 展开
// 	pRootItem->setExpanded(true);
// 
// 	for (int i = 0; i < nEmployeeNum; ++i)
// 	{
// 		addPeopleInfo(pRootItem);
// 	}
// }
// 
// void TalkWindow::initDevelopTalk()
// {
// 	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
// 	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
// 
// 	pRootItem->setData(0, Qt::UserRole, 0);
// 	RootContactitem *pItemName = new RootContactitem(false, ui.treeWidget);
// 
// 	ui.treeWidget->setFixedHeight(646);
// 
// 	int nEmployeeNum = 32;
// 	QString qsGroupName = QString::fromLocal8Bit("研发部%1/%2").arg(0).arg(nEmployeeNum);
// 	pItemName->setText(qsGroupName);
// 
// 	// 插入分组节点
// 	ui.treeWidget->addTopLevelItem(pRootItem);
// 	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
// 
// 	// 展开
// 	pRootItem->setExpanded(true);
// 
// 	for (int i = 0; i < nEmployeeNum; ++i)
// 	{
// 		addPeopleInfo(pRootItem);
// 	}
// }
// 
// void TalkWindow::initMarkTalk()
// {
// 	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
// 	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
// 
// 	pRootItem->setData(0, Qt::UserRole, 0);
// 	RootContactitem *pItemName = new RootContactitem(false, ui.treeWidget);
// 
// 	ui.treeWidget->setFixedHeight(646);
// 
// 	int nEmployeeNum = 8;
// 	QString qsGroupName = QString::fromLocal8Bit("市场部%1/%2").arg(0).arg(nEmployeeNum);
// 	pItemName->setText(qsGroupName);
// 
// 	// 插入分组节点
// 	ui.treeWidget->addTopLevelItem(pRootItem);
// 	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);
// 
// 	// 展开
// 	pRootItem->setExpanded(true);
// 
// 	for (int i = 0; i < nEmployeeNum; ++i)
// 	{
// 		addPeopleInfo(pRootItem);
// 	}
// }

void TalkWindow::initTalkWindow()
{
	QTreeWidgetItem *pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

	pRootItem->setData(0, Qt::UserRole, 0);
	RootContactitem *pItemName = new RootContactitem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);

	// 当前聊天的群组名或人名
	QString strGroupName;
	QString strSqlGroupName = "SELECT department_name FROM tab_department WHERE departmentID = :departmentID";
	QSqlQuery queryGroupName;
	queryGroupName.prepare(strSqlGroupName);
	queryGroupName.bindValue(":departmentID", m_talkId);
	queryGroupName.exec();
	if (queryGroupName.first())
	{
		strGroupName = queryGroupName.value(0).toString();
	}

	// 当前群聊的人数
	QSqlQueryModel queryEmployeeModel;
	QString strSqlEmployee;
	int compDepID = getCompDepID();
	if (compDepID == m_talkId.toInt())	//公司群
	{
		queryEmployeeModel.setQuery("SELECT employeeID FROM tab_employee WHERE status = 1");
	}
	else
	{
		queryEmployeeModel.setQuery(QString("SELECT employeeID FROM tab_employee WHERE status = 1 AND departmentID = %1").arg(m_talkId));
	}
	int nEmployeeNum = queryEmployeeModel.rowCount();

	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3")
		.arg(strGroupName)
		.arg(0)
		.arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	// 插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	// 展开
	pRootItem->setExpanded(true);

	for (int i = 0; i < nEmployeeNum; ++i)
	{
		QModelIndex modeIndex = queryEmployeeModel.index(i, 0);
		int employeeID = queryEmployeeModel.data(modeIndex).toInt();

		// 添加子节点
		addPeopleInfo(pRootItem, employeeID);
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

void TalkWindow::addPeopleInfo(QTreeWidgetItem *pRootGroupItem, int employeeID)
{
	QTreeWidgetItem *pChild = new QTreeWidgetItem();

	// 添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID);
	ContactItem *pContactItem = new ContactItem(ui.treeWidget);

	// 获取用户名、签名、头像
	QString strName, strSign, strPicturePath;
	QSqlQueryModel queryInfoModel;
	queryInfoModel.setQuery(QString("SELECT employee_name, employee_sign, picture FROM tab_employee WHERE employeeID = %1").arg(employeeID));

	QModelIndex nameIndex, signIndex, pictureIndex;
	nameIndex = queryInfoModel.index(0, 0);	
	signIndex = queryInfoModel.index(0, 1);
	pictureIndex = queryInfoModel.index(0, 2);

	strName = queryInfoModel.data(nameIndex).toString();
	strSign = queryInfoModel.data(signIndex).toString();
	strPicturePath = queryInfoModel.data(pictureIndex).toString();

	QPixmap pixMask;
	pixMask.load(":/Resources/MainWindow/head_mask.png");
	QPixmap pixSrc(strPicturePath);

	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(pixSrc, pixMask, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(strName);
	pContactItem->setSignName(strSign);

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);
}

void TalkWindow::onSetEmotionBtnStatus()
{
}
