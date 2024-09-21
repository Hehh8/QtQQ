#pragma once

#include "basicwindow.h"
#include "ui_userlogin.h"

class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();

private:
	Ui::UserLogin ui;
};
