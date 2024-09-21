#include "titlebar.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#define BUTTON_HEIGHT 27	// ��ť�߶�
#define BUTTON_WIDTH 27		// ��ť�߶�
#define TITLE_HEIGHT 27		// �������߶�

TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
	, m_isPressed(false)
	,m_buttonType(MIN_MAX_BUTTON)
{
	initControl();
	initConnections();
	loadStyleSheet("Title");
}

TitleBar::~TitleBar()
{
}

// ��ʼ���ؼ�
void TitleBar::initControl()
{
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonMin = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);

	// ��ť���ù̶���С
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	// ���ö�����
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonClose->setObjectName("ButtonClose");

	// ���ò���
	QHBoxLayout *mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);

	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);

	mylayout->setContentsMargins(5, 0, 0, 0);	// ���ò��ֵļ�϶
	mylayout->setSpacing(0);					// ���ò����ﲿ����ļ�϶

	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setFixedHeight(TITLE_HEIGHT);				// ���ù̶��߶�
	setWindowFlags(Qt::FramelessWindowHint);	// �ޱ߿�
}

// �ź���۵�����
void TitleBar::initConnections()
{
	connect(m_pButtonMin, &QPushButton::clicked, this, &TitleBar::onButtonMinClicked);
	connect(m_pButtonRestore, &QPushButton::clicked, this, &TitleBar::onButtonRestoreClicked);
	connect(m_pButtonMax, &QPushButton::clicked, this, &TitleBar::onButtonMaxClicked);
	connect(m_pButtonClose, &QPushButton::clicked, this, &TitleBar::onButtonCloseClicked);
}

void TitleBar::setTitleIcon(QString & filePath)
{
}

void TitleBar::setTitleContent(QString & titleContent)
{
}

void TitleBar::setTitleWidth(int width)
{
}

void TitleBar::setButtonType(ButtonType buttonType)
{
}

void TitleBar::setRestoreInfo(const QPoint & point, const QSize & size)
{
}

void TitleBar::getRestoreInfo(QPoint & point, QSize & size)
{
}

void TitleBar::paintEvent(QPaintEvent * event)
{
}

void TitleBar::loadStyleSheet(const QString & sheetName)
{
}
