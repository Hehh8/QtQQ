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

private:
	Ui::TalkWindow ui;
};
