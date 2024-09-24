#pragma once

#include <QWidget>
#include "ui_emotionwindow.h"

class EmotionWindow : public QWidget
{
	Q_OBJECT

public:
	EmotionWindow(QWidget *parent = Q_NULLPTR);
	~EmotionWindow();

private:
	Ui::EmotionWindow ui;
};
