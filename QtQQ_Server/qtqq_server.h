#pragma once

#include <QtWidgets/QDialog>
#include "ui_qtqq_server.h"
#include "TcpServer.h"
#include <QSqlQueryModel>
#include <QTimer>

class QtQQ_Server : public QDialog
{
    Q_OBJECT

public:
    QtQQ_Server(QWidget *parent = Q_NULLPTR);

private:
	void initComboBoxData();	// 初始化组合框的数据
	void initTcpSocket();
	bool connectMysql();
	void setDepNameMap();
	void setStatusMap();
	void setOnlineMap();
	void updateTableData(int depID = 0, int employeeID = 0);

private slots:
	void onUdpBroadMsg(QByteArray &btData);
	void onRefresh();
	void on_queryDepartmentBtn_clicked();	// 根据群QQ号查找员工(点击信号与槽函数自动连接)
	void on_queryIDBtn_clicked();			// 根据员工QQ号查找
	void on_logoutBtn_clicked();			// 注销员工QQ号

private:
    Ui::QtQQ_ServerClass ui;

	QTimer *m_timer;	// 定时刷新数据
	int m_compDepID;	// 公司群QQ号
	int m_depID;		// 部门QQ号
	int m_employeeID;	// 员工QQ号
	QString m_pixPath;	// 头像路径
	QMap<QString, QString> m_statusMap;		// 状态
	QMap<QString, QString> m_depNameMap;	// 部门名称
	QMap<QString, QString> m_onlineMap;		// 在线状态

	QSqlQueryModel m_quertInfoModel;	// 查询所有员工的信息模型
	int getCompDepID();
	TcpServer *m_tcpServer;
};
