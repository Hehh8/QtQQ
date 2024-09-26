#pragma once

#include <QWidget>
#include "talkwindowshell.h"
#include "ui_talkwindow.h"

class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent, const QString &uid, GroupType groupType);
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString &name);
	void setMsgLabelContent(const QString &name);

public slots:
	void onSetEmotionBtnStatus();
	void onSendBtnClicked(bool);
	void onItemDoubleClicked();

private:
	void initControl();
	void initCompanyTalk();	// 初始化公司群聊天
	void initPersonTalk();	// 初始化人事部聊天
	void initDevelopTalk();	// 初始化研发部聊天
	void initMarkTalk();	// 初始化研发部聊天
	void initPtoPTalk();	// 初始化单独聊天
	void addPeopleInfo(QTreeWidgetItem *pRootGroupItem);

private:
	Ui::TalkWindow ui;
	QString m_talkId;
	GroupType m_groupType;
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;	// 所有分组联系人姓名
};
