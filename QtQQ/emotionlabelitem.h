#pragma once

#include "qclicklabel.h"

class QMovie;

class EmotionLabelItem : public QClickLabel
{
	Q_OBJECT

public:
	EmotionLabelItem(QWidget *parent);
	~EmotionLabelItem();

public:
	void setEmotionName(int emotionName);

private:
	void initControl();

signals:
	void emotionClicked(int emotionName);

private:
	int m_emotionName;
	QMovie *m_apngMovie;
};
