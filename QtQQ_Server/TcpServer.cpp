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
		
		qDebug() << QString::fromLocal8Bit("服务端监听端口%1成功").arg(m_port);
		return true;
	}
	else
	{
		qDebug() << QString::fromLocal8Bit("服务端监听端口%1失败").arg(m_port);
		return false;

	}
	return false;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << QString::fromLocal8Bit("新的连接:") << socketDescriptor;

	TcpSocket *tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);
	tcpsocket->run();

	// 收到客户端数据,server进行处理
	connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray&, int)), this, SLOT(socketDateProcessing(QByteArray&, int)));
	// 收到客户端断开, server进行处理
	connect(tcpsocket, SIGNAL(signalClientDisconnect(int)), this, SLOT(socketDisconnected(int)));

	// 将socket添加到链表中
	m_tcpSocketConnectList.append(tcpsocket);

}


void TcpServer::socketDisconnected(int desriptor)
{
	for (int i = 0; i < m_tcpSocketConnectList.count(); ++i)
	{
		QTcpSocket *item = m_tcpSocketConnectList.at(i);
		int itemDescriptor = item->socketDescriptor();
		
		// 查找断开连接的socket
		if (itemDescriptor == desriptor || itemDescriptor == -1)
		{
			m_tcpSocketConnectList.removeAt(i);		// 断开的socket从链表中移除
			item->deleteLater();
			qDebug() << QString("TcpSocket断开连接") << desriptor;
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
			qDebug() << QString::fromLocal8Bit("来自IP:") << item->peerAddress().toString()
				<< QString::fromLocal8Bit("发来的数据:") << QString(sendData);

			emit signalTcpMsgCome(sendData);
		}

	}
}

