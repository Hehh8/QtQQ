#include "userlogin.h"
#include "CCMainWindow.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

QString gLoginEmployeeID;	// ��¼��QQ��

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

	// �������ݿ�
	if (!connectMysql())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("�������ݿ�ʧ��!"));

		close();
	}
}

bool UserLogin::connectMysql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
	db.setDatabaseName("qtqq");		// ���ݿ�����
	db.setHostName("localhost");	// ������
	db.setUserName("root");			// �û���
	db.setPassword("rootpasswd");	// ����
	db.setPort(3306);				// �˿ں�

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

	// �û�����¼
	QString strSqlCode = "SELECT code FROM tab_accounts WHERE employeeID = :employeeID";
	QSqlQuery queryEmployeeID;
	queryEmployeeID.prepare(strSqlCode);
	queryEmployeeID.bindValue(":employeeID", strAccountInput);

	if (!queryEmployeeID.exec()) {
		qDebug() << "Query execution error:" << queryEmployeeID.lastError().text();
	}

	if (queryEmployeeID.first())	// ָ��������һ��
	{
		// ���ݿ���QQ�Ŷ�Ӧ����
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

	// �˺ŵ�¼
	strSqlCode = "SELECT code, employeeID FROM tab_accounts WHERE account = :account";
	QSqlQuery queryAccount;
	queryAccount.prepare(strSqlCode);
	queryAccount.bindValue(":account", strAccountInput);

	if (!queryAccount.exec()) {
		qDebug() << "Query execution error:" << queryAccount.lastError().text();
	}

	if (queryAccount.first())	// ָ��������һ��
	{
		// ���ݿ���QQ�Ŷ�Ӧ����
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
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("��������˺Ż���������,����������!"));
		// ui.editUserAccount->setText("");
		ui.editPassword->setText("");
		return;
	}

	close();
	CCMainWindow *mainwindow = new CCMainWindow(strAccount, isAccountLogin);
	mainwindow->show();

}
