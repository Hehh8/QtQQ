#include "talkwindowshell.h"
#include "CommonUtils.h"

#include <QListWidget>

TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
}

TalkWindowShell::~TalkWindowShell()
{
	delete m_emotionWindow;
	m_emotionWindow = nullptr;
	
}

void TalkWindowShell::addTalkWindow(TalkWindow * talkWindow, TalkWindowItem * talkWindowItem)
{
}

void TalkWindowShell::setCurrentWidget(QWidget * widget)
{
}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("聊天窗口"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();	// 隐藏表情窗口

	QList<int> leftWidgetSize;
	leftWidgetSize << 153 << width() - 154;
	ui.splitter->setSize(leftWidgetSize);

	ui.listWidget->setSharable(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionClicked(int)));
}
