#pragma once

#include "basicwindow.h"
#include "ui_talkwindowshell.h"

#include <QMap>

class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

enum GroupType {
	COMPANY = 0,		// ��˾Ⱥ
	PERSONELGROUP,		// ���²�
	DEVELOPMENTGROUP,	// �з���
	MARKETGROUP,		// �г���
	PTOP				// ��������
};

class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

public:
	// ����µ����촰��
	void addTalkWindow(TalkWindow *talkWindow, TalkWindowItem *talkWindowItem, const QString uid /*GroupType grouptype*/);

	// ���õ�ǰ���촰��
	void setCurrentWidget(QWidget *widget);

	QMap<QListWidgetItem*, QWidget*> getTalkWindowItemMap() const;

public slots:
	void onEmotionBtnClicked(bool);	// ���鰴ťִ�к�Ĳۺ���

private:
	void initControl();

private slots:
	void onTalkWindowItemClicked(QListWidgetItem *item);	// ����б�����ִ�еĲۺ���
	void onEmotionItemClicked(int emotionNum);

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkWindowItemMap;	//�򿪵����촰��
	EmotionWindow *m_emotionWindow;
};
