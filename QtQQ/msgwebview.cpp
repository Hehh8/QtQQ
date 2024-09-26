#include "msgwebview.h"
#include <QFile>
#include <QMessageBox>

MsgHtmlObj::MsgHtmlObj(QObject *parent)
	:QObject(parent)
{
	initHtmlTmpl();
}

void MsgHtmlObj::initHtmlTmpl()
{
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgLHtmlTmpl = getMsgTmplHtml("msgreftTmpl");
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
	// 仅接受qrc:/*.html
	if (url.scheme() == QString("qrc"))	// 判断url类型
	{
		return true;
	}
	return false;
}

// QWebEngineView视图用于显示网页内容
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
{
}

MsgWebView::~MsgWebView()
{
}

void MsgWebView::appendMsg(const QString & html)
{
}

QList<QStringList> MsgWebView::parseHtml(const QString & html)
{
	return QList<QStringList>();
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node)
{
	return QList<QStringList>();
}
