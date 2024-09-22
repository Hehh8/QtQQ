#pragma once

#include "basicwindow.h"
#include "ui_CCMainWindow.h"

class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QWidget *parent = Q_NULLPTR);
	virtual ~CCMainWindow();

public:
	void initControl();
	void setUserName(const QString &userName);
	void setLevelPixmap(int level);
	void setGeadPixmap(const QString &headPath);

private:
    Ui::CCMainWindowClass ui;

};
