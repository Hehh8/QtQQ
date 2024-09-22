#pragma once

#include <QLabel>

class QClickLabel : public QLabel
{
	Q_OBJECT

public:
	QClickLabel(QWidget *parent);
	~QClickLabel();

signals:
	void clicked();

protected:
	void mousePressEvent(QMouseEvent *event);

};
