#pragma once

#include "basicwindow.h"
#include "ui_talkwindowshell.h"

#include <QMap>
#include <QTcpSocket>

class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

const int gtcpPort = 8888;

class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

public:
	// 添加新的聊天窗口
	void addTalkWindow(TalkWindow *talkWindow, TalkWindowItem *talkWindowItem, const QString uid /*GroupType grouptype*/);

	// 设置当前聊天窗口
	void setCurrentWidget(QWidget *widget);

	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap() const;

private:
	void initControl();
	void initTcpSocket();	// 初始化TCP
	void getEmployeeID(QStringList& employeeList);	// 获取所有员工QQ号
	bool createJSFile(QStringList &employeeList);

public slots:
	void onEmotionBtnClicked(bool);	// 表情按钮执行后的槽函数
	void updateSendTcpMsg(QString &strData, int &msgType, QString fileName = "");

private slots:
	void onTalkWindowItemClicked(QListWidgetItem *item);	// 左侧列表点击后执行的槽函数
	void onEmotionItemClicked(int emotionNum);

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkWindowItemMap;	//打开的聊天窗口
	EmotionWindow *m_emotionWindow;

private: 
	QTcpSocket *m_tcpClientSocket;	//Tcp客户端
};
