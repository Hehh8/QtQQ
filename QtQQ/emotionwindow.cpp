#include "emotionwindow.h"
#include "CommonUtils.h"
#include "emotionlabelitem.h"

#include <QStyleOption>
#include <QPainter>

const int emotionRow = 12;
const int emotionCol = 14;

EmotionWindow::EmotionWindow(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_DeleteOnClose);

	ui.setupUi(this);

	initControl();
}

EmotionWindow::~EmotionWindow()
{
}

void EmotionWindow::initControl()
{
	CommonUtils::loadStyleSheet(this, "EmotionWindow");

	for (int row = 0; row < emotionRow; row++)
	{
		for (int col = 0; col < emotionCol; col++)
		{
			EmotionLabelItem *label = new EmotionLabelItem(this);
			label->setEmotionName(row *emotionCol + col);

			connect(label, &EmotionLabelItem::emotionClicked, this, &EmotionWindow::addEmotion);
			ui.gridLayout->addWidget(label, row, col);
		}
	}
}

void EmotionWindow::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.init(this);

	QPainter painter;

	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

	__super::paintEvent(event);
}

void EmotionWindow::addEmotion(int emotionNum)
{
	hide();
	emit signalEmotionWindowHide();
	emit signalEmotionItemClicked(emotionNum);
}
