#pragma once

#include <QSystemTrayIcon>
#include <QWidget>

class SysTray : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget *parent = nullptr);
	~SysTray();

private:
	void initSystemTray();
	void addSystrayMenu();

public slots:
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	QWidget *m_parent;
};
