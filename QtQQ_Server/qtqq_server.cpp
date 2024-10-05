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
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("连接数据库失败!"));

		close();
		return;
	}

	m_quertInfoModel.setQuery("SELECT * FROM tab_employee");
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// 初始化查询公司群所有员工信息
	m_depID = getCompDepID();
	m_compDepID = m_depID;

	setDepNameMap();
	setStatusMap();
	setOnlineMap();

	initComboBoxData();

	updateTableData();

	// 定时刷新数据
	m_timer = new QTimer(this);
	m_timer->setInterval(200);
	m_timer->start();
	connect(m_timer, &QTimer::timeout, this, &QtQQ_Server::onRefresh);

	initTcpSocket();
}

void QtQQ_Server::initComboBoxData()
{
	QString itemText;	// 组合框的文本

	// 获取公司总的部门数
	QSqlQueryModel queryDepModel;
	queryDepModel.setQuery("SELECT * FROM tab_department");
	int depCounts = queryDepModel.rowCount() - 1;	// 公司群不属于公司部门

	for (int i = 0; i < depCounts; i++)
	{
		itemText = ui.employeeDepBox->itemText(i);
		QSqlQuery queryDepID;
		QString strSql = "SELECT departmentID FROM tab_department WHERE department_name = :department_name";
		queryDepID.prepare(strSql);
		queryDepID.bindValue(":department_name", itemText);
		queryDepID.exec();
		queryDepID.next();

		// 设置员工所属部门组合框的数据为相应的部门QQ号
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

		// 设置部门组合框的数据为相应的部门QQ号
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());
	}

	// 多一个"公司群"部门
	for (int i = 0; i < depCounts + 1; i++)
	{
		itemText = ui.departmentBox->itemText(i);
		QSqlQuery queryDepID;
		QString strSql = "SELECT departmentID FROM tab_department WHERE department_name = :department_name";
		queryDepID.prepare(strSql);
		queryDepID.bindValue(":department_name", itemText);
		queryDepID.exec();
		queryDepID.next();

		// 设置部门组合框的数据为相应的部门QQ号
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

void QtQQ_Server::setDepNameMap()
{
	m_depNameMap.insert(QStringLiteral("2001"), QStringLiteral("人事部"));
	m_depNameMap.insert(QStringLiteral("2002"), QStringLiteral("研发部"));
	m_depNameMap.insert(QStringLiteral("2003"), QStringLiteral("市场部"));
}

void QtQQ_Server::setStatusMap()
{
	m_statusMap.insert(QStringLiteral("1"), QStringLiteral("有效"));
	m_statusMap.insert(QStringLiteral("0"), QStringLiteral("无效"));
}

void QtQQ_Server::setOnlineMap()
{
	m_onlineMap.insert(QStringLiteral("1"), QStringLiteral("离线"));
	m_onlineMap.insert(QStringLiteral("2"), QStringLiteral("在线"));
	m_onlineMap.insert(QStringLiteral("3"), QStringLiteral("隐身"));
}

void QtQQ_Server::updateTableData(int depID, int employeeID)
{
	ui.tableWidget->clear();

	if (depID && depID != m_compDepID)	// 查询部门
	{
		m_quertInfoModel.setQuery(QString("SELECT * FROM tab_employee WHERE departmentID = %1").arg(depID));
	}
	else if (employeeID)	//	精确查找
	{
		m_quertInfoModel.setQuery(QString("SELECT * FROM tab_employee WHERE employeeID = %1").arg(employeeID));
	}
	else  // 查询所有
	{
		m_quertInfoModel.setQuery(QString("SELECT * FROM tab_employee"));
	}
	int rows = m_quertInfoModel.rowCount();
	int columns = m_quertInfoModel.columnCount();
	QModelIndex index;	// 模型索引
	//设置表格的行数、列数
	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);

	// 设置表头
	QStringList hearders;
	hearders << QStringLiteral("部门") << QStringLiteral("工号") << QStringLiteral("姓名") << QStringLiteral("签名") << QStringLiteral("员工状态") << QStringLiteral("员工头像") << QStringLiteral("在线状态");
	ui.tableWidget->setHorizontalHeaderLabels(hearders);

	// 设置列等宽
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			index = m_quertInfoModel.index(i, j);
			QString strData = m_quertInfoModel.data(index).toString();

			// 获取字段名称
			QSqlRecord record = m_quertInfoModel.record(i);		// 当前行的记录
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

	// 检测员工QQ号是否输入
	if (!ui.queryIDLineEdit->text().length())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("请输入员工QQ号"));
		ui.queryIDLineEdit->setFocus();
		return;
	}

	// 获取用户输入的QQ号
	int employeeID = ui.queryIDLineEdit->text().toInt();

	QSqlQuery queryInfo;
	QString strSql = "SELECT * FROM tab_employee WHERE employeeID = :employeeID";
	queryInfo.prepare(strSql);
	queryInfo.bindValue(":employeeID", employeeID);
	queryInfo.exec();
	if (!queryInfo.next())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("员工QQ号不存在"));
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
	queryCompDepID.bindValue(":department_name", QString::fromLocal8Bit("公司群"));
	queryCompDepID.exec();
	queryCompDepID.first();
	return queryCompDepID.value(0).toInt();
}

void QtQQ_Server::onUdpBroadMsg(QByteArray & btData)
{
	updateTableData(m_depID, m_employeeID);
}
