#include "userlogin.h"
#include "CCMainWindow.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

QString gLoginEmployeeID;	// 登录者QQ号

UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("UserLogin");
	initControl();
}

UserLogin::~UserLogin()
{
}

void UserLogin::initControl()
{
	QLabel *headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/app/logo.ico"), pix, headlabel->size()));
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);

	// 连接数据库
	if (!connectMysql())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("连接数据库失败!"));

		close();
	}
}

bool UserLogin::connectMysql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
	db.setDatabaseName("qtqq");		// 数据库名称
	db.setHostName("localhost");	// 主机名
	db.setUserName("root");			// 用户名
	db.setPassword("rootpasswd");	// 密码
	db.setPort(3306);				// 端口号

	if (db.open())
	{
		return true;
	}
	else
	{
		return false;
	}	
}

bool UserLogin::verifyAccountCode(bool &isAccountLogin, QString &strAccount)
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();

	// 用户名登录
	QString strSqlCode = "SELECT code FROM tab_accounts WHERE employeeID = :employeeID";
	QSqlQuery queryEmployeeID;
	queryEmployeeID.prepare(strSqlCode);
	queryEmployeeID.bindValue(":employeeID", strAccountInput);

	if (!queryEmployeeID.exec()) {
		qDebug() << "Query execution error:" << queryEmployeeID.lastError().text();
	}

	if (queryEmployeeID.first())	// 指向结果集第一条
	{
		// 数据库中QQ号对应密码
		QString strCode = queryEmployeeID.value(0).toString();

		if (strCode == strCodeInput)
		{
			gLoginEmployeeID = strAccountInput;

			strAccount = strCodeInput;
			isAccountLogin = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	// 账号登录
	strSqlCode = "SELECT code, employeeID FROM tab_accounts WHERE account = :account";
	QSqlQuery queryAccount;
	queryAccount.prepare(strSqlCode);
	queryAccount.bindValue(":account", strAccountInput);

	if (!queryAccount.exec()) {
		qDebug() << "Query execution error:" << queryAccount.lastError().text();
	}

	if (queryAccount.first())	// 指向结果集第一条
	{
		// 数据库中QQ号对应密码
		QString strCode = queryAccount.value(0).toString();

		if (strCode == strCodeInput)
		{
			gLoginEmployeeID = queryAccount.value(1).toString();

			isAccountLogin = true;
			strAccount = strCodeInput;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void UserLogin::onLoginBtnClicked()
{
	bool isAccountLogin;
	QString strAccount;

	if (!verifyAccountCode(isAccountLogin, strAccount))
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("您输入的账号或密码有误,请重新输入!"));
		// ui.editUserAccount->setText("");
		ui.editPassword->setText("");
		return;
	}

	close();
	CCMainWindow *mainwindow = new CCMainWindow(strAccount, isAccountLogin);
	mainwindow->show();

}
