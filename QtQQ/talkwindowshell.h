#pragma once

#include "basicwindow.h"
#include "ui_talkwindowshell.h"

#include <QMap>

class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

enum GroupType {
	COMPANY = 0,		// 公司群
	PERSONELGROUP,		// 人事部
	DEVELOPMENTGROUP,	// 研发部
	MARKETGROUP,		// 市场部
	PTOP				// 单独聊天
};

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

	QMap<QListWidgetItem*, QWidget*> getTalkWindowItemMap() const;

public slots:
	void onEmotionBtnClicked(bool);	// 表情按钮执行后的槽函数

private:
	void initControl();

private slots:
	void onTalkWindowItemClicked(QListWidgetItem *item);	// 左侧列表点击后执行的槽函数
	void onEmotionItemClicked(int emotionNum);

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkWindowItemMap;	//打开的聊天窗口
	EmotionWindow *m_emotionWindow;
};
