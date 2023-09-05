#include "XServer.h"

#include <QNetworkProxy>
#include <QTcpSocket>

#include "../CoreBase/XDebug.h"

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  XServer
//----------------------------------------------------------------------------- 
XServer::XServer(QObject* parent):XService(parent)
{
    m_Server = Q_NULLPTR;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  ~XServer
//----------------------------------------------------------------------------- 
XServer::~XServer()
{
    stop();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  stop
//----------------------------------------------------------------------------- 
void XServer::stop()
{
    if (!m_Server)
        return;

	for (QTcpSocket* socket : m_Connections)
        {
            socket->close();
            socket->deleteLater();
        }
    m_Connections.clear();

    m_Server->close();
    m_Server->deleteLater();
    delete m_Server;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  open
// Params:  qint32 port
//----------------------------------------------------------------------------- 
bool XServer::open(qint32 port)
{
    stop();
    m_Server = new QTcpServer(this);
    m_Server->setProxy(QNetworkProxy::NoProxy);
    QObject::connect(m_Server, &QTcpServer::newConnection, this, &XServer::newConnection);
    if (!m_Server->listen(QHostAddress::Any, port))
    {
        stop();
        return false;
    }

	return true;
    
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  wait
// Params:  int secs
//----------------------------------------------------------------------------- 
void XServer::wait(int secs)
{
    qint32 i = 0;

    while (i++ < secs* 10)
    {
        QThread::msleep(100);
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  checkClient
// Params:  QTcpSocket* socket
//----------------------------------------------------------------------------- 
bool XServer::checkClient(const QHostAddress& address, qint32 port)
{
    if (m_IncludedServer.isEmpty())
        return true;

    QString server=address.toString();
    if (m_IncludedServer.contains(server))
        return true;

    for(const QString&include:m_IncludedServer)
    {
        if (server.contains(include))
            return true;
    }
    return true;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  newConnection
//----------------------------------------------------------------------------- 
void XServer::newConnection()
{
    while (m_Server->hasPendingConnections())
    {
        QTcpSocket* socket=m_Server->nextPendingConnection();
        if (checkClient(socket->peerAddress(), socket->localPort()))
        {
            m_Connections.append(socket);
            QObject::connect(socket, &QTcpSocket::readyRead, this, &XServer::readyRead);
            QObject::connect(socket, &QTcpSocket::disconnected, this, &XServer::disconnected);
            QObject::connect(socket, &QAbstractSocket::errorOccurred, this, &XServer::error);
        }
        else
        {
            socket->close();
            socket->deleteLater();
        }
    }
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  readSocket
//----------------------------------------------------------------------------- 
void XServer::readyRead()
{
    //O_STACK(XServer, readyRead);

    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    if (!socket)
        return;

    QByteArray head;
    QByteArray data;
    {
        QDataStream socketStream(socket);
        socketStream.setVersion(QDataStream::Qt_6_4);
        socketStream.startTransaction();
        socketStream >> head;
        socketStream >> data;
        if (!socketStream.commitTransaction())
            return;
    }

    //QtConcurrent::run([&]
      //  {
            QByteArray replydata=qCompress(XService::read(socket->peerAddress(), socket->peerName(), socket->localPort(),qUncompress(head), data));
            //QByteArray replydata = XAction::read(socket->peerAddress(), socket->localPort(), data);
            QDataStream socketStream(socket);
		    socketStream.setVersion(QDataStream::Qt_6_4);
		    socketStream << replydata;
			socket->waitForBytesWritten();
		    socket->close();
		    m_Connections.removeAll(socket);
		    socket->deleteLater();
        //});
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  discardSocket
//----------------------------------------------------------------------------- 
void XServer::disconnected()
{
    QTcpSocket* socket=reinterpret_cast<QTcpSocket*>(sender());
    if (!socket)
        return;
    {
        socket->close();
        m_Connections.removeAll(socket);
        socket->deleteLater();
    }
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XServer
// Method:  error
// Params:  QAbstractSocket::SocketError socketError
//----------------------------------------------------------------------------- 
void XServer::error(QAbstractSocket::SocketError socketError)
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    O_WARNING(QString("%1").arg(socket->errorString()));

    switch (socketError)
    {
	    case QAbstractSocket::RemoteHostClosedError: O_WARNING("RemoteHostClosedError"); break;
	    case QAbstractSocket::HostNotFoundError: O_WARNING("HostNotFoundError"); break;
	    case QAbstractSocket::ConnectionRefusedError: O_WARNING("ConnectionRefusedError"); break;
	    default: O_WARNING(QString("%1").arg(socket->errorString())); break;
    }
}
