#include "talkwindowshell.h"
#include "emotionwindow.h"
#include "talkwindow.h"
#include "CommonUtils.h"
#include "talkwindowitem.h"
#include <QListWidget>
#include <QSplitter>

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

void TalkWindowShell::addTalkWindow(TalkWindow * talkWindow, TalkWindowItem * talkWindowItem, GroupType grouptype)
{
	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()),
		talkWindow, SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem *aItem = new QListWidgetItem(ui.listWidget);
	m_talkWindowItemMap.insert(aItem, talkWindow);

	aItem->setSelected(true);

	const QPixmap pix(":/Resources/MainWindow/girl.png");
	talkWindowItem->setHeadPixmap("");	// ����ͷ��
	ui.listWidget->addItem(aItem);

	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked, [talkWindowItem, talkWindow, aItem, this]() {
		m_talkWindowItemMap.remove(aItem);
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));
		delete talkWindowItem;
		ui.rightStackedWidget->removeWidget(talkWindow);
		if (ui.rightStackedWidget->count() < 1)
		{
			close();
		}
	});
}

void TalkWindowShell::setCurrentWidget(QWidget * widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("���촰��"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();	// ���ر��鴰��

	QList<int> leftWidgetSize;
	leftWidgetSize << 153 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionClicked(int)));
}

void TalkWindowShell::onEmotionBtnClicked(bool)
{
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));	// ����ǰ�ؼ������λ��ת��Ϊ��Ļ�ľ���λ��

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 220);
	m_emotionWindow->move(emotionPoint);
}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem * item)
{
	QWidget *talkWindowWidget = m_talkWindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkWindowWidget);
}

void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	TalkWindow *curTalkWindow = dynamic_cast<TalkWindow*> (ui.rightStackedWidget->currentWidget());
	if (curTalkWindow)
	{
		curTalkWindow->addEmotionImage(emotionNum);
	}
}
