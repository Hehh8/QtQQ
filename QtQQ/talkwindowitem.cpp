#include "talkwindowitem.h"

#include "CommonUtils.h"

TalkWindowItem::TalkWindowItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initControl();
}

TalkWindowItem::~TalkWindowItem()
{
}

void TalkWindowItem::initControl()
{
	ui.tclosebtn->setVisible(false);
	connect(ui.tclosebtn, SIGNAL(clicked(bool)), this, SIGNAL(signalCloseClicked()));
}

void TalkWindowItem::enterEvent(QEvent * event)
{
	ui.tclosebtn->setVisible(true);
	__super::enterEvent(event);
}

void TalkWindowItem::leaveEvent(QEvent * event)
{
	ui.tclosebtn->setVisible(false);
	__super::leaveEvent(event);
}

void TalkWindowItem::resizeEvent(QResizeEvent * event)
{
	__super::resizeEvent(event);
}

void TalkWindowItem::setHeadPixmap(const QString & pixmap)
{
	QPixmap mask = QPixmap(":/Resources/MainWindow/head_mask.png");
	QPixmap head = QPixmap(":/Resources/MainWindow/girl.png");
	const QPixmap &headPixmap = CommonUtils::getRoundImage(head, mask, ui.headlabel->size());
	ui.headlabel->setPixmap(headPixmap);
}

void TalkWindowItem::setMsgLabelContent(const QString & msg)
{
	ui.msgLabel->setText(msg);
}

QString TalkWindowItem::getMsgLabelText()
{
	return ui.msgLabel->text();
}


