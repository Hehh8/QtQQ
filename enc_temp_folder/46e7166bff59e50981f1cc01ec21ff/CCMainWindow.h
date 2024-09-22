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
	void setUserName(const QString &userName);		// 设置用户名
	void setLevelPixmap(int level);					// 设置等级
	void setHeadPixmap(const QString &headPath);	// 设置头像
	void setStatusMenuIcon(const QString &statusPath);	// 设置状态
	// 添加应用部件(app图片路径,app布局对象名)
	QWidget *addOtherAppExtension(const QString &appPath, const QString appName);

private:
    Ui::CCMainWindowClass ui;

};
