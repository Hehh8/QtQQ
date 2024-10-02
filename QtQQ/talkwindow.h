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
	void setMsgLabelContent(const QString &name);

private slots:
	void onSetEmotionBtnStatus();
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
	void initControl();
	void initGroupTalkStatus();
	int getCompDepID();
// 	void initCompanyTalk();	// ��ʼ����˾Ⱥ����
// 	void initPersonTalk();	// ��ʼ�����²�����
// 	void initDevelopTalk();	// ��ʼ���з�������
// 	void initMarkTalk();	// ��ʼ���з�������
	void initTalkWindow();	// ��ʼ��Ⱥ��
	void initPtoPTalk();	// ��ʼ����������
	void addPeopleInfo(QTreeWidgetItem *pRootGroupItem, int employeeID);

private:
	Ui::TalkWindow ui;
	QString m_talkId;
	bool m_isGroupTalk;	// �Ƿ�ΪȺ��
	GroupType m_groupType;
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;	// ���з�����ϵ������
};
