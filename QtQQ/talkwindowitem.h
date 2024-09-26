#pragma once

#include <QWidget>
#include "ui_talkwindowitem.h"

class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = Q_NULLPTR);
	~TalkWindowItem();

	void setHeadPixmap(const QString &pixmap);
	void setMsgLabelContent(const QString &msg);
	QString getMsgLabelText();

private:
	void initControl();

signals:
	void signalCloseClicked();

private:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	Ui::TalkWindowItem ui;
};
