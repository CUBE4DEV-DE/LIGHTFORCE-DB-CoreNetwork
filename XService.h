#pragma once

#include "CoreNetworkGlobal.h"

#include <QString>
#include <QFuture>
#include <QList>
#include <QtConcurrent>
#include <QTcpServer>

namespace CoreNetwork
{
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  XAction
//----------------------------------------------------------------------------- 
class CORENETWORK_EXPORT XService  : public QObject
{
	Q_OBJECT
public:
	XService(QObject *parent=Q_NULLPTR);
	virtual ~XService();

	void ActionActiv(bool action = true) { m_ActionActiv = action; }
	QByteArray read(const QHostAddress& address, const QString& hostname, qint32 port, const QByteArray& head, const QByteArray& data);

	// Service Values
	void setRootPath(const QString& rootpath) { m_RootPath = rootpath; }
	const QString& rootPath() { return m_RootPath; }
	QString rootPath(const QString& path) { if (m_RootPath.isEmpty()) return path;  return m_RootPath + QDir::separator() + path; }
	QString servicePath();
	 
	void setAppName(const QString& rootpath) { m_AppName = rootpath; }
	const QString& appName() { return m_AppName; }
	QString serviceIdentity();
	QString serviceTimeStamp();
	QDateTime serviceModified();
	QUuid processId();
	QStringList serviceAddress();

	virtual QString serviceName();
	virtual QString serviceDescription();
	virtual QString serviceVersion();

	// Live Access
	static bool ping(QString address, qint32 port);
	static bool serviceClose(QString address, qint32 port);

	static void localServiceInfo(QMap<QString, QVariant>& map);
	static QMap<QString, QVariant> serviceInfo(QString address, qint32 port);
	static QMap<QString, QVariant> serviceInfo(const QHostAddress& address, qint32 port);

	static QList<QMap<QString,QVariant>> serviceLogs(QString address, qint32 port,int maxlogs=128);
	static QList<QMap<QString,QVariant>>  serviceLogs(const QHostAddress& address, qint32 port, int maxlogs=128);

	const QList< QMap<QString, QVariant> >& logs() { return m_Logs; }
	void clearLogs() { m_Logs.clear(); }

signals:
	void ServiceClose();
public slots:
	QByteArray remotePing(QString address,qint32 port);
	QByteArray remoteServiceClose(QString address, qint32 port);
	QByteArray remoteServiceInfo(QString address, qint32 port);
	QByteArray remoteServiceLogs(QString address, qint32 port, int maxlogs);

protected:
	static QByteArray hash(const QByteArray& value);
	static QByteArray hash(const QString& value);

	QByteArray call(const QString& slotname,const QMap<QString,QVariant>& values=QMap<QString,QVariant>(), const QByteArray& data=QByteArray());
	QMap<QString,QVariant> params(const QByteArray&data);

	QByteArray params(const QMap<QString, QVariant>& params);
	QByteArray params(const QString& name, const QVariant& value);
	QByteArray params(const QString& name, const QVariant& value, const QString& name2, const QVariant& value2);
	QByteArray params(const QString& name, const QVariant& value, const QString& name2, const QVariant& value2, const QString& name3, const QVariant& value3);
	QByteArray params(const QString& name, const QVariant& value, const QString& name2, const QVariant& value2, const QString& name3, const QVariant& value3, const QString& name4, const QVariant& value4);
	QByteArray params(const QString& name, const QVariant& value, const QString& name2, const QVariant& value2, const QString& name3, const QVariant& value3, const QString& name4, const QVariant& value4, const QString& name5, const QVariant& value5);
	QByteArray params(const QString& name, const QVariant& value, const QString& name2, const QVariant& value2, const QString& name3, const QVariant& value3, const QString& name4, const QVariant& value4, const QString& name5, const QVariant& value5, const QString& name6, const QVariant& value6);
private:
	bool m_ActionActiv;
	QString m_RootPath;
	QString m_AppName;
	QUuid m_ProcessId;
	QDateTime m_StartDateTime;
	qint32 m_CallCount;

	QList< QMap<QString, QVariant> > m_Logs;
	qint32 m_MaxLogs;

	QString m_ServiceVersion;
	QString m_ServiceDescr;
};

};
using namespace CoreNetwork;

