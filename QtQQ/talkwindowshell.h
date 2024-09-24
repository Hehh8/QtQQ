#pragma once

#include "basicwindow.h"
#include "ui_talkwindowshell.h"

#include <QMap>

class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

public:
	// ����µ����촰��
	void addTalkWindow(TalkWindow *talkWindow, TalkWindowItem *talkWindowItem);

	// ���õ�ǰ���촰��
	void setCurrentWidget(QWidget *widget);

public slots:
	void a();
	void onEmotionClicked(bool);	// ���鰴ťִ�к�Ĳۺ���

private:
	void initControl();

private:
	void onTalkWindowItemClicked(QListWidgetItem *item);	// ����б�����ִ�еĲۺ���
	void onEmotionItemClicked(int emotionNum);

private:
	Ui::TalkWindowShell ui;
	QMap<QListWidgetItem*, QWidget*> m_talkWindowItemMap;	//�򿪵����촰��
	EmotionWindow *m_emotionWindow;
};
