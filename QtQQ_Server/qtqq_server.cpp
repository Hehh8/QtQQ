#include "qtqq_server.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QSqlRecord>
#include <QSqlQuery>

const int tcpPort = 8888;
QtQQ_Server::QtQQ_Server(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

	if (!connectMysql())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("�������ݿ�ʧ��!"));

		close();
		return;
	}

	m_quertInfoModel.setQuery("SELECT * FROM tab_employee");
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// ��ʼ����ѯ��˾Ⱥ����Ա����Ϣ
	m_depID = getCompDepID();
	m_compDepID = m_depID;

	setDepNameMap();
	setStatusMap();
	setOnlineMap();

	initComboBoxData();

	updateTableData();

	// ��ʱˢ������
	m_timer = new QTimer(this);
	m_timer->setInterval(200);
	m_timer->start();
	connect(m_timer, &QTimer::timeout, this, &QtQQ_Server::onRefresh);

	initTcpSocket();
}

void QtQQ_Server::initComboBoxData()
{
	QString itemText;	// ��Ͽ���ı�

	// ��ȡ��˾�ܵĲ�����
	QSqlQueryModel queryDepModel;
	queryDepModel.setQuery("SELECT * FROM tab_department");
	int depCounts = queryDepModel.rowCount() - 1;	// ��˾Ⱥ�����ڹ�˾����

	for (int i = 0; i < depCounts; i++)
	{
		itemText = ui.employeeDepBox->itemText(i);
		QSqlQuery queryDepID;
		QString strSql = "SELECT departmentID FROM tab_department WHERE department_name = :department_name";
		queryDepID.prepare(strSql);
		queryDepID.bindValue(":department_name", itemText);
		queryDepID.exec();
		queryDepID.next();

		// ����Ա������������Ͽ������Ϊ��Ӧ�Ĳ���QQ��
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());
	}

	for (int i = 0; i < depCounts + 1; i++)
	{
		itemText = ui.departmentBox->itemText(i);
		QSqlQuery queryDepID;
		QString strSql = "SELECT departmentID FROM tab_department WHERE department_name = :department_name";
		queryDepID.prepare(strSql);
		queryDepID.bindValue(":department_name", itemText);
		queryDepID.exec();
		queryDepID.next();

		// ���ò�����Ͽ������Ϊ��Ӧ�Ĳ���QQ��
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());
	}

	// ��һ��"��˾Ⱥ"����
	for (int i = 0; i < depCounts + 1; i++)
	{
		itemText = ui.departmentBox->itemText(i);
		QSqlQuery queryDepID;
		QString strSql = "SELECT departmentID FROM tab_department WHERE department_name = :department_name";
		queryDepID.prepare(strSql);
		queryDepID.bindValue(":department_name", itemText);
		queryDepID.exec();
		queryDepID.next();

		// ���ò�����Ͽ������Ϊ��Ӧ�Ĳ���QQ��
		ui.departmentBox->setItemData(i, queryDepID.value(0).toInt());
	}
}

void QtQQ_Server::initTcpSocket()
{
	m_tcpServer = new TcpServer(tcpPort);
	m_tcpServer->run();

	connect(m_tcpServer, &TcpServer::signalTcpMsgCome, this, &QtQQ_Server::onUdpBroadMsg);
}

bool QtQQ_Server::connectMysql()
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

void QtQQ_Server::setDepNameMap()
{
	m_depNameMap.insert(QStringLiteral("2001"), QStringLiteral("���²�"));
	m_depNameMap.insert(QStringLiteral("2002"), QStringLiteral("�з���"));
	m_depNameMap.insert(QStringLiteral("2003"), QStringLiteral("�г���"));
}

void QtQQ_Server::setStatusMap()
{
	m_statusMap.insert(QStringLiteral("1"), QStringLiteral("��Ч"));
	m_statusMap.insert(QStringLiteral("0"), QStringLiteral("��Ч"));
}

void QtQQ_Server::setOnlineMap()
{
	m_onlineMap.insert(QStringLiteral("1"), QStringLiteral("����"));
	m_onlineMap.insert(QStringLiteral("2"), QStringLiteral("����"));
	m_onlineMap.insert(QStringLiteral("3"), QStringLiteral("����"));
}

void QtQQ_Server::updateTableData(int depID, int employeeID)
{
	ui.tableWidget->clear();

	if (depID && depID != m_compDepID)	// ��ѯ����
	{
		m_quertInfoModel.setQuery(QString("SELECT * FROM tab_employee WHERE departmentID = %1").arg(depID));
	}
	else if (employeeID)	//	��ȷ����
	{
		m_quertInfoModel.setQuery(QString("SELECT * FROM tab_employee WHERE employeeID = %1").arg(employeeID));
	}
	else  // ��ѯ����
	{
		m_quertInfoModel.setQuery(QString("SELECT * FROM tab_employee"));
	}
	int rows = m_quertInfoModel.rowCount();
	int columns = m_quertInfoModel.columnCount();
	QModelIndex index;	// ģ������
	//���ñ�������������
	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);

	// ���ñ�ͷ
	QStringList hearders;
	hearders << QStringLiteral("����") << QStringLiteral("����") << QStringLiteral("����") << QStringLiteral("ǩ��") << QStringLiteral("Ա��״̬") << QStringLiteral("Ա��ͷ��") << QStringLiteral("����״̬");
	ui.tableWidget->setHorizontalHeaderLabels(hearders);

	// �����еȿ�
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			index = m_quertInfoModel.index(i, j);
			QString strData = m_quertInfoModel.data(index).toString();

			// ��ȡ�ֶ�����
			QSqlRecord record = m_quertInfoModel.record(i);		// ��ǰ�еļ�¼
			QString strRecordName = record.fieldName(j);

			if (strRecordName == QLatin1String("departmentID"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_depNameMap.value(strData)));
				continue;
			}
			else if (strRecordName == QLatin1String("status"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_statusMap.value(strData)));
				continue;
			}
			else if (strRecordName == QLatin1String("online"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_onlineMap.value(strData)));
				continue;
			}
			ui.tableWidget->setItem(i, j, new QTableWidgetItem(strData));
		}
	}
}

void QtQQ_Server::onRefresh()
{
	updateTableData(m_depID, m_employeeID);
}

void QtQQ_Server::on_queryDepartmentBtn_clicked()
{
	ui.queryIDLineEdit->clear();
	m_employeeID = 0;
	m_depID = ui.departmentBox->currentData().toInt();
	updateTableData(m_depID);
}

void QtQQ_Server::on_queryIDBtn_clicked()
{
	ui.departmentBox->setCurrentIndex(0);
	m_depID = m_compDepID;

	// ���Ա��QQ���Ƿ�����
	if (!ui.queryIDLineEdit->text().length())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("������Ա��QQ��"));
		ui.queryIDLineEdit->setFocus();
		return;
	}

	// ��ȡ�û������QQ��
	int employeeID = ui.queryIDLineEdit->text().toInt();

	QSqlQuery queryInfo;
	QString strSql = "SELECT * FROM tab_employee WHERE employeeID = :employeeID";
	queryInfo.prepare(strSql);
	queryInfo.bindValue(":employeeID", employeeID);
	queryInfo.exec();
	if (!queryInfo.next())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("Ա��QQ�Ų�����"));
		ui.queryIDLineEdit->setFocus();
		return;
	}
	else
	{
		m_employeeID = employeeID;
	}

	updateTableData(m_depID, m_employeeID);
}

int QtQQ_Server::getCompDepID()
{
	QString strSql = "SELECT departmentID FROM tab_department WHERE department_name = :department_name";
	QSqlQuery queryCompDepID;
	queryCompDepID.prepare(strSql);
	queryCompDepID.bindValue(":department_name", QString::fromLocal8Bit("��˾Ⱥ"));
	queryCompDepID.exec();
	queryCompDepID.first();
	return queryCompDepID.value(0).toInt();
}

void QtQQ_Server::onUdpBroadMsg(QByteArray & btData)
{
	updateTableData(m_depID, m_employeeID);
}
