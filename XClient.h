#pragma once


#include "CoreNetworkGlobal.h"

#include <QString>
#include <QFuture>
#include <QList>
#include <QtConcurrent>
#include <QTcpServer>
#include <QHostAddress>

#include "XService.h"

namespace CoreNetwork
{

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    23.10.2022
// Context: DefaultNamespace
// Class:   XClient
// Method:  XClient
// Description: The XClient class is defined within the CoreNetwork namespace.
// It inherits from the XService class and is essentially an implementation of
// an object communicating via TCP (Transmission Control Protocol). It provides
// a variety of public static call and action methods that allow users to connect
// to a remote host and obtain information by sending TCP packets (data) to
// the remote host. It consists of constructors, which can accept either a
// host address or a hostname with a corresponding port number, and a destructor.
// The available call and action methods allow for variation in data formats
// that is dependent on the design of the remote host, allowing the user
// to send data that follows the same design as that of their remote host,
// providing customizability and flexibility. The class also has a few private
// slots that handle socket errors such as disconnection.
//----------------------------------------------------------------------------- 
class CORENETWORK_EXPORT XClient :public XService
{
    Q_OBJECT
public:
    XClient(QObject* parent=Q_NULLPTR);
    XClient(const QString& hostname, qint32 port);
    XClient(const QHostAddress& host, qint32 port);
    virtual ~XClient();

	static QByteArray call(const QString& hostname, qint32 port, const QByteArray& data);
    static QByteArray call(const QHostAddress& host, qint32 port, const QByteArray& data);
    static QMap<QString, QVariant> action(const QString& hostname, qint32 port, const QString& action, const QMap<QString, QVariant>& values = QMap<QString, QVariant>(),const QByteArray&rawdata=QByteArray());
    static QMap<QString, QVariant> action(const QHostAddress& host, qint32 port, const QString& action, const QMap<QString, QVariant>& values = QMap<QString, QVariant>(), const QByteArray& rawdata = QByteArray());

    static void silent(bool s=true) { m_Silent = s; }
protected:
    bool open(const QString& hostname, qint32 port);
    bool open(const QHostAddress& host, qint32 port);
    QByteArray call(const QByteArray& head,const QByteArray& data);
    QMap<QString, QVariant> callAction(const QString& action, const QMap<QString, QVariant>& values = QMap<QString, QVariant>(), const QByteArray& rawdata = QByteArray());
public slots:
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);
private:
    QTcpSocket* m_Socket;
    static bool m_Silent;
};


};
using namespace CoreNetwork;

