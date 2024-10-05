#pragma once

#include <QWidget>
#include "talkwindowshell.h"
#include "ui_talkwindow.h"

class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent, const QString &uid/*, GroupType groupType*/);
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString &name);
	QString getTalkId();
	void setMsgLabelContent(const QString &name);

private slots:
	void onSetEmotionBtnStatus();
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
	void initControl();
	void initGroupTalkStatus();
	int getCompDepID();
	void initTalkWindow();	// 初始化群聊
	void initPtoPTalk();	// 初始化单独聊天
	void addPeopleInfo(QTreeWidgetItem *pRootGroupItem, int employeeID);

private:
	Ui::TalkWindow ui;
	QString m_talkId;
	bool m_isGroupTalk;	// 是否为群聊
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;	// 所有分组联系人姓名
};
