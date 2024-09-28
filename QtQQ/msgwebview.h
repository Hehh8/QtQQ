#pragma once

#include <QWebEngineView>
#include <QDomNode>
#include <QWebEnginePage>

class MsgHtmlObj : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)		// ��������Ϣ
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)		// �ҷ�����Ϣ


public:
	MsgHtmlObj(QObject *parent = nullptr);

signals:
	void signalMsgHtml(const QString &html);

private:
	void initHtmlTmpl();	// ��ʼ��������ҳ
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
	QList<QStringList> parseHtml(const QString &html);	// ����html
	// Qt������DOM�ڵ�(���ԡ�˵�����ı���)������ʹ��QDomNode��ʾ
	QList<QStringList> parseDocNode(const QDomNode &node);	// �����ڵ�

private:
	MsgHtmlObj *m_msgHtmlObj;
	
};
