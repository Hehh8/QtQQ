#pragma once

#include "basicwindow.h"
#include "ui_userlogin.h"

class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();

private slots:
	void onLoginBtnClicked();

private:
	void initControl();
	bool connectMysql();
	bool verifyAccountCode(bool &isAccountLogin, QString &strAccount);

private:
	Ui::UserLogin ui;
};
