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
	void initTcpSocket();
	bool connectMysql();
	void setDepNameMap();
	void setStatusMap();
	void setOnlineMap();
	void updateTableData(int depID = 0, int employeeID = 0);

private slots:
	void onUdpBroadMsg(QByteArray &btData);
	void onRefresh();

private:
    Ui::QtQQ_ServerClass ui;

	QTimer *m_timer;	// ��ʱˢ������
	int m_compDepID;	// ��˾ȺQQ��
	int m_depID;		// ����QQ��
	int m_employeeID;	// Ա��QQ��
	QMap<QString, QString> m_statusMap;		// ״̬
	QMap<QString, QString> m_depNameMap;	// ��������
	QMap<QString, QString> m_onlineMap;		// ����״̬

	QSqlQueryModel m_quertInfoModel;	// ��ѯ����Ա������Ϣģ��
	int getCompDepId();
	TcpServer *m_tcpServer;
};
