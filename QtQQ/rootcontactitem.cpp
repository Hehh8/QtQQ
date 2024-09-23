#include "rootcontactitem.h"

#include <QPainter>

RootContactitem::RootContactitem(bool hasArrow, QWidget *parent)
	: QLabel(parent)
	, m_rotation(0)
	, m_hasArrow(hasArrow)
{
	setFixedHeight(32);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// ���Զ���
	m_animation = new QPropertyAnimation(this, "rotation");
	m_animation->setDuration(50);	// ���õ��ζ���ʱ��50ms
	m_animation->setEasingCurve(QEasingCurve::InQuad);	// ����������������t^2��0����
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
		m_animation->setEndValue(90);	// �������Զ����Ľ���ֵ
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
	painter.save();		// ���滭������

	if (m_hasArrow)
	{
		QPixmap pixmap;
		pixmap.load(";/Resources/MainWindow/arrow.png");

		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);

		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform, true);

		// ����ϵƫ��(x�������,y����ƫ��)
		p.translate(pixmap.width() / 2, pixmap.height() / 2);
		// ��ת����ϵ(˳ʱ��)
		p.rotate(m_rotation);
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);
		painter.restore();	// �ָ���������
	}

	QLabel::paintEvent(event);
}
