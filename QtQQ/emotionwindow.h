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
	void initControl();

private slots:
	void addEmotion(int emotionNum);

signals:
	void signalEmotionWindowHide();
	void signalEmotionItemClicked(int emotionNum);

protected:
	void paintEvent(QPaintEvent *event) override; 

private:
	Ui::EmotionWindow ui;
};
