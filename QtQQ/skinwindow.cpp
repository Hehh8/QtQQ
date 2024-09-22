#include "skinwindow.h"
#include "qclicklabel.h"
#include "notifymanager.h"

SkinWindow::SkinWindow(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	loadStyleSheet("SkinWindow");
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
}

SkinWindow::~SkinWindow()
{
}

void SkinWindow::initControl()
{
	QList<QColor> colorList = {
		QColor(22, 154,218),QColor(40,138,221),QColor(49,166,107),QColor(218,67,68),
		QColor(177,99,158),QColor(107,81,92),QColor(89,92,160),QColor(21,159,199),
		QColor(79,169,172),QColor(155,183,154),QColor(128,77,77),QColor(240,188,189)
	};

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			QClickLabel *label = new QClickLabel(this);
			label->setCursor(Qt::PointingHandCursor);

			connect(label, &QClickLabel::clicked, [row, col, colorList]() {
				NotifyManager::getInstance()->notifyOtherWindowChangeSkin(colorList.at(row * 4 + col));
			});

			label->setFixedSize(84, 84);
			QPalette palette;
			palette.setColor(QPalette::Background, colorList.at(row * 4 + col));
			label->setAutoFillBackground(true);
			label->setPalette(palette);

			ui.gridLayout->addWidget(label, row, col);
		}
	}
	connect(ui.sysmin, &QPushButton::clicked, this, &SkinWindow::onShowMin);
	connect(ui.sysclose, &QPushButton::clicked, this, &SkinWindow::onShowClose);
}

void SkinWindow::onShowClose()
{
	close();
}
