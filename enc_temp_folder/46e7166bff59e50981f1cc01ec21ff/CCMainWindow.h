#pragma once

#include "basicwindow.h"
#include "ui_CCMainWindow.h"

class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QWidget *parent = nullptr);
	virtual ~CCMainWindow();

public:
	void initControl();
	void setUserName(const QString &userName);		// �����û���
	void setLevelPixmap(int level);					// ���õȼ�
	void setHeadPixmap(const QString &headPath);	// ����ͷ��
	void setStatusMenuIcon(const QString &statusPath);	// ����״̬
	// ���Ӧ�ò���(appͼƬ·��,app���ֶ�����)
	QWidget *addOtherAppExtension(const QString &appPath, const QString appName);

private:
    Ui::CCMainWindowClass ui;

};
