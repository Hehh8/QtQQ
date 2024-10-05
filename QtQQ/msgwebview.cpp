#include "msgwebview.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>
#include "talkwindowshell.h"
#include "windowmanager.h"

MsgHtmlObj::MsgHtmlObj(QObject *parent)
	:QObject(parent)
{
	initHtmlTmpl();
}

void MsgHtmlObj::initHtmlTmpl()
{
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
}

QString MsgHtmlObj::getMsgTmplHtml(const QString &code)
{
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;
	if (file.isOpen())
	{
		strData = QLatin1String(file.readAll());
	}
	else
	{
		QMessageBox::information(nullptr, "Tips", "Failed to init html!");
		return QString("");
	}
	file.close();
	return strData;
}

bool MsgWebPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame)
{
	// ������qrc:/*.html
	if (url.scheme() == QString("qrc"))	// �ж�url����
	{
		return true;
	}
	return false;
}

// QWebEngineView��ͼ������ʾ��ҳ����
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
{
	MsgWebPage *page = new MsgWebPage(this);
	setPage(page);

	QWebChannel *channel = new QWebChannel(this);
	m_msgHtmlObj = new MsgHtmlObj(this);
	channel->registerObject("external", m_msgHtmlObj);
	this->page()->setWebChannel(channel);

	TalkWindowShell *talkWindowshell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &MsgWebView::signalSendMsg, talkWindowshell, &TalkWindowShell::updateSendTcpMsg);

	// ��ʼ������Ϣҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{
}

void MsgWebView::appendMsg(const QString & html, QString strObj)
{
	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgList = parseHtml(html);	// ����html

	int imageNum = 0;
	int msgType = 1;	// ��Ϣ����: 0���� 1�ı� 2�ļ�
	bool isImageMsg = false;
	QString strData;	//���͵�����


	for (int i = 0; i < msgList.size(); i++)
	{
		if (msgList.at(i).at(0) == "img")
		{
			QString imagePath = msgList.at(i).at(1);
			QPixmap pixmap;

			// ��ȡ�������Ƶ�λ��
			QString strEmotionPath = "qrc:/Resoucres/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			isImageMsg = true;

			// ��ȡ��������
			QString strEmotionName = imagePath.mid(pos);
			strEmotionName.replace(".png", "");
			isImageMsg = true;

			// ���ݱ������Ƶĳ��Ƚ������ñ�������,����3λ����3λ
			int emotionNameL = strEmotionName.length();
			if (emotionNameL == 1)
			{
				strData = strData + "00" + strEmotionName;
			}
			else if (emotionNameL == 2)
			{
				strData = strData + "0" + strEmotionName;
			}
			else if (emotionNameL == 3)
			{
				strData = strData + strEmotionName;
			}

			msgType = 0;	// ������Ϣ
			imageNum++;

			if (imagePath.left(3) == "qrc")
			{
				pixmap.load(imagePath.mid(3));	// ȥ��·���е�qrc
			}
			else
			{
				pixmap.load(imagePath);
			}

			// ����ͼƬhtml��ʽ�ı����
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());
			qsMsg += imgPath;
		}
		else if (msgList.at(i).at(0) == "text")
		{
			qsMsg += msgList.at(i).at(1);
			strData = qsMsg;
		}
	}

	msgObj.insert("MSG", qsMsg);

	const QString &Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);

	if (strObj == "0")	// ����Ϣ
	{
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));
		if (isImageMsg)
		{
			strData = QString::number(imageNum) + "images" + strData;
		}
		emit signalSendMsg(strData, msgType);
	}
	else  // ����Ϣ
	{
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}

}

QList<QStringList> MsgWebView::parseHtml(const QString & html)
{
	QDomDocument doc;
	doc.setContent(html);
	const QDomElement &root = doc.documentElement();	// �ڵ�Ԫ��
	const QDomNode &node = root.firstChildElement("body");	// �����ڵ�
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node)
{
	QList<QStringList> attribute;
	const QDomNodeList &list = node.childNodes();	// ���������ӽڵ�

	for (int i = 0; i < list.count(); i++)
	{
		const QDomNode &node = list.at(i);

		if (node.isElement())
		{
			// ת��Ԫ��
			const QDomElement &element = node.toElement();
			if (element.tagName() == "img")
			{
				QStringList attributeList;
				attributeList << "img" << element.attribute("src");
				attribute << attributeList;
			}
			if (element.tagName() == "span")
			{
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;
			}
			if (node.hasChildNodes())
			{
				attribute << parseDocNode(node);
			}
		}
	}

	return attribute;
}
