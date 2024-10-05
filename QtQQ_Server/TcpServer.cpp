#include "TcpServer.h"
#include <QDebug>
#include <QTcpSocket>
#include "TcpSocket.h"

TcpServer::TcpServer(int port)
	: m_port(port)
{
}

TcpServer::~TcpServer()
{
}

bool TcpServer::run()
{
	if (this->listen(QHostAddress::Any, m_port))
	{
		
		qDebug() << QString::fromLocal8Bit("����˼����˿�%1�ɹ�").arg(m_port);
		return true;
	}
	else
	{
		qDebug() << QString::fromLocal8Bit("����˼����˿�%1ʧ��").arg(m_port);
		return false;

	}
	return false;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << QString::fromLocal8Bit("�µ�����:") << socketDescriptor;

	TcpSocket *tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);
	tcpsocket->run();

	// �յ��ͻ�������,server���д���
	connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray&, int)), this, SLOT(socketDateProcessing(QByteArray&, int)));
	// �յ��ͻ��˶Ͽ�, server���д���
	connect(tcpsocket, SIGNAL(signalClientDisconnect(int)), this, SLOT(socketDisconnected(int)));

	// ��socket��ӵ�������
	m_tcpSocketConnectList.append(tcpsocket);

}


void TcpServer::socketDisconnected(int desriptor)
{
	for (int i = 0; i < m_tcpSocketConnectList.count(); ++i)
	{
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int itemDescriptor = item->socketDescriptor();
		
		// ���ҶϿ����ӵ�socket
		if (itemDescriptor == desriptor || itemDescriptor == -1)
		{
			m_tcpSocketConnectList.removeAt(i);		// �Ͽ���socket���������Ƴ�
			item->deleteLater();
			qDebug() << QString("TcpSocket�Ͽ�����") << desriptor;
			return;
		}
	}
}

void TcpServer::socketDateProcessing(QByteArray &sendData, int descriptor)
{
	for (int i = 0; i < m_tcpSocketConnectList.count(); ++i)
	{
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		if (item->socketDescriptor() == descriptor)
		{
			qDebug() << QString::fromLocal8Bit("����IP:") << item->peerAddress().toString()
				<< QString::fromLocal8Bit("����������:") << QString(sendData);

			emit signalTcpMsgCome(sendData);
		}

	}
}

