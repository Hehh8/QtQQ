#pragma once

#include <QWebEngineView>
#include <QDomNode>
#include <QWebEnginePage>

class MsgHtmlObj : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)		// 发来的信息
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)		// 我发的信息


public:
	MsgHtmlObj(QObject *parent = nullptr);

signals:
	void signalMsgHtml(const QString &html);

private:
	void initHtmlTmpl();	// 初始化聊天网页
	QString getMsgTmplHtml(const QString &code);

private:
	QString m_msgLHtmlTmpl;
	QString m_msgRHtmlTmpl;

};

class MsgWebPage : public QWebEnginePage
{
	Q_OBJECT
public:
	MsgWebPage(QObject *parent = nullptr) : QWebEnginePage(parent) {}
	
protected:
	bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);

};

class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
	void appendMsg(const QString & html);

private:
	QList<QStringList> parseHtml(const QString &html);	// 解析html
	// Qt中所有DOM节点(属性、说明、文本等)都可以使用QDomNode表示
	QList<QStringList> parseDocNode(const QDomNode &node);	// 解析节点

private:
	MsgHtmlObj *m_msgHtmlObj;
	
};
