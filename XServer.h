#pragma once


#include "CoreNetworkGlobal.h"

#include <QString>
#include <QFuture>
#include <QList>
#include <QtConcurrent>
#include <QTcpServer>

#include "XService.h"

namespace CoreNetwork
{
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  XServer
//----------------------------------------------------------------------------- 
class CORENETWORK_EXPORT XServer :public XService
{
	Q_OBJECT
public:
	XServer(QObject*parent=Q_NULLPTR);
	void stop();
	virtual ~XServer();
	bool open(qint32 port);
	void wait(int secs=10);

	virtual bool checkClient(const QHostAddress& address, qint32 port);
	void inlcudeServer(const QString& server) { if (server.isEmpty())  { m_IncludedServer.clear(); return; } m_IncludedServer << server; }
	
public slots:
    void newConnection();
    void readyRead();
	void error(QAbstractSocket::SocketError socketError);
	void disconnected();
private:
	QTcpServer* m_Server;
	QList<QTcpSocket*> m_Connections;
	QList<QString> m_IncludedServer;
};


};
using namespace CoreNetwork;

