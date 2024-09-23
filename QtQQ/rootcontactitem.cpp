#include "rootcontactitem.h"

#include <QPainter>

RootContactitem::RootContactitem(bool hasArrow, QWidget *parent)
	: QLabel(parent)
	, m_rotation(0)
	, m_hasArrow(hasArrow)
{
	setFixedHeight(32);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// 属性动画
	m_animation = new QPropertyAnimation(this, "rotation");
	m_animation->setDuration(50);	// 设置单次动画时长50ms
	m_animation->setEasingCurve(QEasingCurve::InQuad);	// 动画缓和曲线类型t^2从0加速
}

RootContactitem::~RootContactitem()
{
}

void RootContactitem::setText(const QString & title)
{
	m_titleText = title;
	update();
}

void RootContactitem::setExpanded(bool expand)
{
	if (expand)
	{
		m_animation->setEndValue(90);	// 设置属性动画的结束值
	}
	else
	{
		m_animation->setEndValue(0);
	}
	
	m_animation->start();
}

int RootContactitem::getrotation()
{
	return m_rotation;
}

void RootContactitem::setRotation(int rotation)
{
	m_rotation = rotation;
	update();
}

void RootContactitem::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::TextAntialiasing, true);

	QFont font;
	font.setPointSize(10);
	painter.setFont(font);
	painter.drawText(24, 0, width() - 24, height(), Qt::AlignLeft | Qt::AlignVCenter, m_titleText);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.save();		// 保存画家设置

	if (m_hasArrow)
	{
		QPixmap pixmap;
		pixmap.load(";/Resources/MainWindow/arrow.png");

		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);

		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform, true);

		// 坐标系偏移(x方向便宜,y方向偏移)
		p.translate(pixmap.width() / 2, pixmap.height() / 2);
		// 旋转坐标系(顺时针)
		p.rotate(m_rotation);
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);
		painter.restore();	// 恢复画家设置
	}

	QLabel::paintEvent(event);
}
