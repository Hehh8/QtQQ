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
	// �ͻ������µ�����ʱ
	void incomingConnection(qintptr socketDescriptor);

signals:
	void signalTcpMsgCome(QByteArray &);

private slots:
	// ��������
	void socketDateProcessing(QByteArray &sendData, int descriptor);
	// �Ͽ����� ����
	void socketDisconnected(int desriptor);

private:
	int m_port;		// �˿ں�
	QList<QTcpSocket*> m_tcpSocketConnectList;

};
