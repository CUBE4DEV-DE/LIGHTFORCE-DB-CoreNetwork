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
// Description: The XServer class is a subclass of XService that acts as
// a server for handling client requests. It provides functions for
// opening the server on a specified port, checking the client's IP
// address and port, and waiting for connections. It also allows
// inclusion of other servers for efficient server management. The class
// also contains slots for handling new incoming connections, reading
// data from the connections, detecting errors and managing disconnections.
// Overall, the XServer class is a useful tool when building
// network-enabled applications in Qt.
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

