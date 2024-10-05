#pragma once

#include <QTcpServer>

class TcpServer : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(int port);
	~TcpServer();

public:
	bool run();

protected:
	// 客户端有新的连接时
	void incomingConnection(qintptr socketDescriptor);

signals:
	void signalTcpMsgCome(QByteArray &);

private slots:
	// 处理数据
	void socketDateProcessing(QByteArray &sendData, int descriptor);
	// 断开连接 处理
	void socketDisconnected(int desriptor);

private:
	int m_port;		// 端口号
	QList<QTcpSocket*> m_tcpSocketConnectList;

};
