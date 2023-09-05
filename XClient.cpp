#include "XClient.h"

#include <QTcpSocket>
#include "../CoreBase/XDebug.h"

bool XClient::m_Silent;

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  XClient
// Params:  QObject* parent
//----------------------------------------------------------------------------- 

XClient::XClient(QObject *parent): XService(parent)
{
    m_Socket=new QTcpSocket(this);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  XClient
// Params:  const QString& hostname
// Params:  qint32 port
//----------------------------------------------------------------------------- 

XClient::XClient(const QString& hostname, qint32 port)
{
    m_Socket = new QTcpSocket(this);
    open(hostname, port);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  XClient
// Params:  const QHostAddress& host
// Params:  qint32 port
//----------------------------------------------------------------------------- 

XClient::XClient(const QHostAddress& host, qint32 port)
{
    m_Socket = new QTcpSocket(this);
    open(host, port);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  ~XClient
//----------------------------------------------------------------------------- 
XClient::~XClient()
{
    if (m_Socket)
    {
        if (m_Socket->isOpen())
            m_Socket->close();
        if (m_Socket)
            m_Socket->deleteLater();
    }
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  open
// Params:  const QString& hostname
// Params:  qint32 port
//----------------------------------------------------------------------------- 
bool XClient::open(const QString& hostname, qint32 port)
{
    return open(QHostAddress(hostname), port);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  open
// Params:  const QString& hostname
// Params:  qint32 port
//----------------------------------------------------------------------------- 
bool XClient::open(const QHostAddress& host, qint32 port)
{
    //QObject::connect(m_Socket, &QTcpSocket::readyRead, this, &XClient::readyRead);
    QObject::connect(m_Socket, &QTcpSocket::disconnected, this, &XClient::disconnected);
    QObject::connect(m_Socket, &QAbstractSocket::errorOccurred, this, &XClient::error);

    m_Socket->connectToHost(host, port);

    if (m_Silent)
    {
        if (m_Socket->waitForConnected(50))
            return true;
    }
    else
    {
        for (qint32 i = 0; i < 4; i++)
        {
            if (m_Socket->waitForConnected())
            {
                QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
                return true;
            }

            QThread::msleep(10);
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }
    }

    if (!m_Silent)
    {
        O_WARNING("Open Failed");
        O_PARAM(host.toString());
        O_PARAM(port);
    }
    return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  call
// Params:  const QByteArray& data
//----------------------------------------------------------------------------- 

QByteArray XClient::call(const QByteArray& head,const QByteArray& data)
{
    {
        QDataStream socketStream(m_Socket);
        socketStream.setVersion(QDataStream::Qt_6_4);
        socketStream << qCompress(head);
        socketStream << data;
        //socketStream << data;
        
    }
    QByteArray reply;
    {
        bool commit = false;
        int i = 0;
        m_Socket->waitForReadyRead();
    	while (!commit&&i++<30)
        {
            QDataStream socketStream(m_Socket);
            socketStream.setVersion(QDataStream::Qt_6_4);
			socketStream.startTransaction();
			socketStream >> reply;
            commit = socketStream.commitTransaction();
            if (!commit)
                m_Socket->waitForReadyRead();
		} 
    }
    return qUncompress(reply);
    //return reply;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  call
// Params:  const QString& action
// Params:  const QMap<QString, QVariant>& values
// Params:  
//----------------------------------------------------------------------------- 

QMap<QString, QVariant> XClient::callAction(const QString& action, const QMap<QString, QVariant>& values, const QByteArray& rawdata)
{
    QMap<QString, QVariant> callparams(values);
    callparams["action"] = action;
    QByteArray result = call(params(callparams),rawdata);
    return params(result);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  call
// Params:  const QString& hostname
// Params:  qint32 port
// Params:  const QByteArray& data
//----------------------------------------------------------------------------- 

QByteArray XClient::call(const QString& hostname, qint32 port, const QByteArray& data)
{
    return call(QHostAddress(hostname), port, data);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  call
// Params:  const QHostAddress& address
// Params:  qint32 port
// Params:  const QByteArray& data
// Params:  
//----------------------------------------------------------------------------- 
QByteArray XClient::call(const QHostAddress& address, qint32 port, const QByteArray& data)
{
    XClient client;
	if (!client.open(address, port))
        return QByteArray();

    return client.call(data,QByteArray("XXXX"));
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  action
// Params:  const QString& hostname
// Params:  qint32 port
// Params:  const QString& action
// Params:  const QMap<QString, QVariant>& values
//----------------------------------------------------------------------------- 

QMap<QString, QVariant> XClient::action(const QString& hostname, qint32 port, const QString& action,const QMap<QString, QVariant>& values, const QByteArray& rawdata)
{
    XClient client;
    if (!client.open(hostname, port))
        return QMap<QString, QVariant>();

    return client.callAction(action, values,rawdata);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  action
// Params:  const QHostAddress& host
// Params:  qint32 port
// Params:  const QString& action
// Params:  const QMap<QString, QVariant>& values
//----------------------------------------------------------------------------- 

QMap<QString, QVariant> XClient::action(const QHostAddress& host, qint32 port, const QString& action,const QMap<QString, QVariant>& values, const QByteArray& rawdata)
{
    XClient client;
    if (!client.open(host, port))
        return QMap<QString, QVariant>();

    return client.callAction(action,values,rawdata);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  discardSocket
//----------------------------------------------------------------------------- 
void XClient::disconnected()
{
    if (m_Socket)
    {
        m_Socket->deleteLater();
        m_Socket = Q_NULLPTR;
    }
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  displayError
// Params:  QAbstractSocket::SocketError socketError
//----------------------------------------------------------------------------- 
void XClient::error(QAbstractSocket::SocketError socketError)
{
    if (m_Silent)
        return;

    switch (socketError)
	{
	    case QAbstractSocket::RemoteHostClosedError: O_WARNING("RemoteHostClosedError"); break;
	    case QAbstractSocket::HostNotFoundError: O_WARNING("HostNotFoundError"); break;
	    case QAbstractSocket::ConnectionRefusedError: O_WARNING("ConnectionRefusedError"); break;
    }

    O_WARNING(QString("%1").arg(m_Socket->errorString()));

}
