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

