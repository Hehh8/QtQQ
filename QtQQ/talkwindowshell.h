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
	// 添加新的聊天窗口
	void addTalkWindow(TalkWindow *talkWindow, TalkWindowItem *talkWindowItem);

	// 设置当前聊天窗口
	void setCurrentWidget(QWidget *widget);

public slots:
	void a();
	void onEmotionClicked(bool);	// 表情按钮执行后的槽函数

private:
	void initControl();

private:
	void onTalkWindowItemClicked(QListWidgetItem *item);	// 左侧列表点击后执行的槽函数
	void onEmotionItemClicked(int emotionNum);

private:
	Ui::TalkWindowShell ui;
	QMap<QListWidgetItem*, QWidget*> m_talkWindowItemMap;	//打开的聊天窗口
	EmotionWindow *m_emotionWindow;
};
