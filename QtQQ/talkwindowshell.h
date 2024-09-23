#pragma once

#include <QWidget>
#include "ui_talkwindowshell.h"

class TalkWindowShell : public QWidget
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

private:
	Ui::TalkWindowShell ui;
};
