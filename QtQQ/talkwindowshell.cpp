#include "talkwindowshell.h"
#include "emotionwindow.h"
#include "talkwindow.h"
#include "CommonUtils.h"
#include "talkwindowitem.h"
#include <QListWidget>
#include <QSplitter>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFile>

extern QString gLoginEmployeeID;

TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
	initTcpSocket();

	QFile file("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size())
	{
		QStringList employeeIDList;
		getEmployeeID(employeeIDList);
		if (!createJSFile(employeeIDList))
		{
			QMessageBox::information(this,
				QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("����js�ļ�����ʧ��"));
		}
	}
}

TalkWindowShell::~TalkWindowShell()
{
	delete m_emotionWindow;
	m_emotionWindow = nullptr;
	
}

void TalkWindowShell::addTalkWindow(TalkWindow * talkWindow, TalkWindowItem * talkWindowItem, const QString uid/*,GroupType grouptype*/)
{
	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()),
		talkWindow, SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem *aItem = new QListWidgetItem(ui.listWidget);
	m_talkWindowItemMap.insert(aItem, talkWindow);

	aItem->setSelected(true);

	// �ж�Ⱥ�Ļ��ǵ���
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(uid);
	sqlDepModel.setQuery(strQuery);
	int rows = sqlDepModel.rowCount();

	if (rows == 0)	// ����
	{
		strQuery = QString("SELECT picture FROM tab_employee WHERE employeeID = %1").arg(uid);
		sqlDepModel.setQuery(strQuery);
	}

	QModelIndex index;
	index = sqlDepModel.index(0, 0);
	QImage img;
	img.load(sqlDepModel.data(index).toString());

	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));	// ����ͷ��
	ui.listWidget->addItem(aItem);

	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked, [talkWindowItem, talkWindow, aItem, this]() {
		m_talkWindowItemMap.remove(aItem);
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));
		delete talkWindowItem;
		ui.rightStackedWidget->removeWidget(talkWindow);
		if (ui.rightStackedWidget->count() < 1)
		{
			close();
		}
	});
}

void TalkWindowShell::setCurrentWidget(QWidget * widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::getTalkWindowItemMap() const
{
	return m_talkWindowItemMap;
}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("���촰��"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();	// ���ر��鴰��

	QList<int> leftWidgetSize;
	leftWidgetSize << 153 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}

void TalkWindowShell::initTcpSocket()
{
	m_tcpClientSocket = new QTcpSocket(this);
	m_tcpClientSocket->connectToHost("127.0.0.1", gtcpPort);
}

void TalkWindowShell::getEmployeeID(QStringList& employeeList)
{
	QSqlQueryModel queryModel;
	queryModel.setQuery("SELECT employeeID FROM tab_employee WHERE status = 1");
	
	int employeeNum = queryModel.rowCount();
	QModelIndex index;
	for (int i = 0; i < employeeNum; ++i)
	{
		index = queryModel.index(i, 0);
		employeeList << queryModel.data(index).toString();
	}

}

bool TalkWindowShell::createJSFile(QStringList & employeeList)
{
	QString strFileTxt = "Resources/MainWindow/MsgHtml/msgtmpl.txt";
	QFile fileRead(strFileTxt);
	QString strFile;

	if (fileRead.open(QIODevice::ReadOnly))
	{
		strFile = fileRead.readAll();
		fileRead.close();
	}
	else
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("��ȡmsgtmpl.txtʧ��"));

		return false;
	}

	// �滻(external0, appendHtml0�����Լ�����Ϣʹ��)
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		// ���¿�ֵ
		QString strSourceInitNull = "var external = null;";
		QString strSourceInit = "external = channel.objects.external;";
		QString strSourceNew = "new QWebChannel(qt.webChannelTransport,"
			"function(channel) {"
			"external = channel.objects.external;"
			"}); ";

		// �ű�����˫�����޷�ֱ�Ӹ�ֵ,���ö��ļ���ʽ
		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly))
		{
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		}
		else
		{
			QMessageBox::information(this,
				QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("��ȡrecvHtml.txtʧ��"));

			return false;
		}

		// �����滻��Ľű�
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;

		for (int i = 0; i < employeeList.length(); i++)
		{
			// �༭�滻��Ŀ�ֵ
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeeList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";

			// �༭�滻��ĳ�ʼֵ
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeeList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";

			// �༭�滻���newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeeList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";

			// �༭�滻���recvHtml
			QString strRecvHtml = strSourceRecvHtml;
			strRecvHtml.replace("external", QString("external_%1").arg(employeeList.at(i)));
			strRecvHtml.replace("recvHtml", QString("recvHtml_%1").arg(employeeList.at(i)));
			strReplaceRecvHtml += strRecvHtml;
			strReplaceRecvHtml += "\n";
		}

		strFile.replace(strSourceInitNull, strReplaceInitNull);
		strFile.replace(strSourceInit, strReplaceInit);
		strFile.replace(strSourceNew, strReplaceNew);
		strFile.replace(strSourceRecvHtml, strReplaceRecvHtml);

		QTextStream stream(&fileWrite);
		stream << strFile;
		fileWrite.close();

		return true;
	}
	else
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("дmsgtmpl.jsʧ��"));

		return false;
	}
	return false;
}

void TalkWindowShell::onEmotionBtnClicked(bool)
{
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));	// ����ǰ�ؼ������λ��ת��Ϊ��Ļ�ľ���λ��

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 220);
	m_emotionWindow->move(emotionPoint);
}

// �ı����ݰ���ʽ:Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�� + ��Ϣ���� + ���ݳ��� + ����
// �������ݰ���ʽ:Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�� + ��Ϣ���� + ������� + images + ����
void TalkWindowShell::updateSendTcpMsg(QString & strData, int & msgType, QString fileName)
{
	// ��ȡ��ǰ����촰��
	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	QString talkId = curTalkWindow->getTalkId();

	QString strGroupFlag;
	QString strSend;
	if (talkId.length() == 4)	// ȺQQ�ĳ���
	{
		strGroupFlag = "1";
	}
	else
	{
		strGroupFlag = "0";
	}

	int sourceDataLength = strData.length();
	int dataLength = QString::number(sourceDataLength).length();
	QString strDataLength;
	if (msgType == 1)	// �����ı���Ϣ
	{
		// �ı���Ϣ����Լ��Ϊ5
		if (dataLength == 1)
		{
			strDataLength = "0000" + QString::number(sourceDataLength);
		}
		else if (dataLength == 2)
		{
			strDataLength = "000" + QString::number(sourceDataLength);
		}
		else if (dataLength == 3)
		{
			strDataLength = "00" + QString::number(sourceDataLength);
		}
		else if (dataLength == 4)
		{
			strDataLength = "0" + QString::number(sourceDataLength);
		}
		else if (dataLength == 5)
		{
			strDataLength = QString::number(sourceDataLength);
		}
		else
		{
			QMessageBox::information(this,
				QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("����������ݳ���!"));
		}

		// �ı����ݰ���ʽ:Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�� + ��Ϣ���� + ���ݳ��� + ����
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strDataLength + strData;
	}
	else if (msgType == 0)	// ������Ϣ
	{
		// �������ݰ���ʽ:Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�� + ��Ϣ���� + ������� + images + ����
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData;
	}
	else if (msgType == 2)	// �ļ���Ϣ
	{
		// �ļ����ݸ�ʽ:Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ�� + ��Ϣ���� + �ļ����� + "bytes" + �ļ����� + "data_begin" + �ļ�����
		QByteArray bt = strData.toUtf8();
		QString strLength = QString::number(bt.length());
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2" + strLength + "bytes" + fileName + "data_begin" + strData;
	}

	QByteArray dataBt;
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8();
	m_tcpClientSocket->write(dataBt);

}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem * item)
{
	QWidget *talkWindowWidget = m_talkWindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkWindowWidget);
}

void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow*> (ui.rightStackedWidget->currentWidget());
	if (curTalkWindow)
	{
		curTalkWindow->addEmotionImage(emotionNum);
	}
}
