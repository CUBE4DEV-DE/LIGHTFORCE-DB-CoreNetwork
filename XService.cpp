#include "XService.h"
#include "../CoreStorage/XCrypt.h"
#include "../CoreBase/XDebug.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include "XClient.h"

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  XAction
// Params:  QObject* parent
//----------------------------------------------------------------------------- 

XService::XService(QObject *parent): QObject(parent)
{
	m_ActionActiv=true;
	m_ProcessId=QUuid::createUuid();
	m_StartDateTime=QDateTime::currentDateTime();
	m_CallCount = 0;
	m_MaxLogs=4096;
	m_AppName=QFileInfo(QCoreApplication::applicationFilePath()).fileName().toUpper();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  ~XAction
//----------------------------------------------------------------------------- 
XService::~XService()
{
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  read
// Params:  const QHostAddress& address
// Params:  qint32 port
// Params:  const QByteArray& data
//----------------------------------------------------------------------------- 

QByteArray XService::read(const QHostAddress& address, const QString& hostname, qint32 port, const QByteArray& head, const QByteArray& data)
{
	if (m_ActionActiv)
	{
		QMap<QString, QVariant> values = params(head);
		values["address"] = address.toString();
		values["hostname"] = hostname;
		values["port"] = port;
		//values["hash"]=QCryptographicHash::hash(data, QCryptographicHash::Algorithm::Md5);
		QString action = values["action"].toString();
		if (!action.isEmpty())
		{
			return call(action, values,data);
		}
	}
	return head;
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  params
// Params:  const QByteArray& data
// Params:  
//----------------------------------------------------------------------------- 

QMap<QString, QVariant> XService::params(const QByteArray& data)
{
	QMap<QString, QVariant> values;
	QDataStream dataStream(data);
	dataStream.setVersion(QDataStream::Qt_6_4);
	dataStream >> values;
	return values;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  params
// Params:  const QMap<QString, QVariant>& params
// Params:  
//----------------------------------------------------------------------------- 
QByteArray XService::params(const QMap<QString, QVariant>& params)
{
	//O_STACK(XAction, params);

	QByteArray data;
	{
		QDataStream dataStream(&data, QIODevice::WriteOnly);
		dataStream.setVersion(QDataStream::Qt_6_4);
		dataStream << params;
	}
	return data;
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  params
// Params:  const QString& name
// Params:  const QVariant& value
//----------------------------------------------------------------------------- 
QByteArray XService::params(const QString& name, const QVariant& value)
{
	QMap<QString, QVariant> map;
	map[name] = value;
	return params(map);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  params
// Params:  const QString& name
// Params:  const QVariant& value
// Params:  const QString& name2
// Params:  const QVariant& value2
//----------------------------------------------------------------------------- 
QByteArray XService::params(const QString& name, const QVariant& value, const QString& name2, const QVariant& value2)
{
	QMap<QString, QVariant> map;
	map[name] = value;
	map[name2] = value2;
	return params(map);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  params
// Params:  const QString& name
// Params:  const QVariant& value
// Params:  const QString& name2
// Params:  const QVariant& value2
// Params:  const QString& name3
// Params:  const QVariant& value3
//----------------------------------------------------------------------------- 
QByteArray XService::params(const QString& name, const QVariant& value, const QString& name2, const QVariant& value2, const QString& name3, const QVariant& value3)
{
	QMap<QString, QVariant> map;
	map[name] = value;
	map[name2] = value2;
	map[name3] = value3;
	return params(map);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  params
// Params:  const QString& name
// Params:  const QVariant& value
// Params:  const QString& name2
// Params:  const QVariant& value2
// Params:  const QString& name3
// Params:  const QVariant& value3
// Params:  const QString& name4
// Params:  const QVariant& value4
//----------------------------------------------------------------------------- 
QByteArray XService::params(const QString& name, const QVariant& value, const QString& name2, const QVariant& value2, const QString& name3, const QVariant& value3, const QString& name4, const QVariant& value4)
{
	QMap<QString, QVariant> map;
	map[name] = value;
	map[name2] = value2;
	map[name3] = value3;
	map[name4] = value4;
	return params(map);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  params
// Params:  const QString& name
// Params:  const QVariant& value
// Params:  const QString& name2
// Params:  const QVariant& value2
// Params:  const QString& name3
// Params:  const QVariant& value3
// Params:  const QString& name4
// Params:  const QVariant& value4
// Params:  const QString& name5
// Params:  const QVariant& value5
//----------------------------------------------------------------------------- 
QByteArray XService::params(const QString& name, const QVariant& value, const QString& name2, const QVariant& value2,const QString& name3, const QVariant& value3, const QString& name4, const QVariant& value4, const QString& name5,const QVariant& value5)
{
	QMap<QString, QVariant> map;
	map[name] = value;
	map[name2] = value2;
	map[name3] = value3;
	map[name4] = value4;
	map[name5] = value5;
	return params(map);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  params
// Params:  const QString& name
// Params:  const QVariant& value
// Params:  const QString& name2
// Params:  const QVariant& value2
// Params:  const QString& name3
// Params:  const QVariant& value3
// Params:  const QString& name4
// Params:  const QVariant& value4
// Params:  const QString& name5
// Params:  const QVariant& value5
// Params:  const QString& name6
// Params:  const QVariant& value6
//----------------------------------------------------------------------------- 
QByteArray XService::params(const QString& name, const QVariant& value, const QString& name2, const QVariant& value2,const QString& name3, const QVariant& value3, const QString& name4, const QVariant& value4, const QString& name5,const QVariant& value5, const QString& name6, const QVariant& value6)
{
	QMap<QString, QVariant> map;
	map[name] = value;
	map[name2] = value2;
	map[name3] = value3;
	map[name4] = value4;
	map[name5] = value5;
	map[name6] = value6;
	return params(map);
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  appIdentity
// Params:  
//----------------------------------------------------------------------------- 

QString XService::serviceIdentity()
{
	QStringList identity;
	
	if (!appName().isEmpty())
		identity << appName().toUpper().remove(".EXE").remove(".DLL");
	else
		identity << QFileInfo(QCoreApplication::applicationFilePath()).fileName().toUpper().remove(".EXE").remove(".DLL");

	if (!serviceName().isEmpty())
		identity << serviceName().toUpper();

	if (!rootPath().isEmpty())
	{
		QStringList segments;
		if (rootPath().contains("/"))
			segments = rootPath().toUpper().remove('.').split("/",Qt::SkipEmptyParts);
		else
			segments = rootPath().toUpper().remove('.').split("\\", Qt::SkipEmptyParts);

		identity << segments;
	}
		
	return identity.join('.');
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  serviceVersion
//----------------------------------------------------------------------------- 

QString XService::serviceVersion()
{
	return "0.0.0.0.0";
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XService
// Method:  processId
//----------------------------------------------------------------------------- 
QUuid XService::processId()
{
	return m_ProcessId;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  serviceStamp
// Params:  
//----------------------------------------------------------------------------- 

QString XService::serviceTimeStamp()
{
	QFileInfo executable(QCoreApplication::applicationFilePath());
	executable.refresh();
	return executable.lastModified().toString("yyyy.MM.dd.HH.mm");
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  serviceTimeStamp
// Params:  
//----------------------------------------------------------------------------- 
QDateTime XService::serviceModified()
{
	QFileInfo executable(QCoreApplication::applicationFilePath());
	executable.refresh();
	return executable.lastModified();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  serviceName
//----------------------------------------------------------------------------- 
QString XService::serviceName()
{
	return "Service"; 
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  serviceDescription
// Params:  
//----------------------------------------------------------------------------- 
QString XService::serviceDescription()
{
	return "Description";
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  servicePath
// Params:  
//----------------------------------------------------------------------------- 
QString XService::servicePath()
{
	QDir dir(rootPath());
	return dir.absolutePath();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  serviceAddress
// Params:  
//----------------------------------------------------------------------------- 
QStringList XService::serviceAddress()
{
	QStringList addresslist;


	//QHostInfo info(QHostAddress::LocalHost);
	//for(QHostAddress address: info.addresses())
	//{
	//	addresslist << address.toString();
	//}

	//QHostInfo info2(QHostAddress::LocalHostIPv6);
	//for (QHostAddress address : info.addresses())
	//{
	//	addresslist << address.toString();
	//}

	for(const QHostAddress& address:QNetworkInterface::allAddresses())
	{
		addresslist << address.toString();
	}

	//if (!QHostInfo::localHostName().isEmpty())
	//	addresslist << QHostInfo::localHostName();

	//if (!QHostInfo::localDomainName().isEmpty())
	//	addresslist << QHostInfo::localDomainName();

	return addresslist;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XService
// Method:  ping
// Params:  QString address
// Params:  qint32 port
//----------------------------------------------------------------------------- 
bool XService::ping(QString address, qint32 port)
{
	QMap<QString, QVariant> values;
	values["address"] = address;
	values["port"] = port;
	QMap<QString, QVariant> ret = XClient::action(address, port, "remotePing", values);
	return ret["return"].toBool();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.11.2022
// Context: DefaultNamespace
// Class:   XService
// Method:  serviceClose
// Params:  QString address
// Params:  qint32 port
// Params:  
//----------------------------------------------------------------------------- 

bool XService::serviceClose(QString address, qint32 port)
{
	QMap<QString, QVariant> values;
	values["address"] = address;
	values["port"] = port;
	QMap<QString, QVariant> ret = XClient::action(address, port, "remoteServiceClose", values);
	return ret["return"].toBool();
}
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  ping
// Params:  QString address
// Params:  qint32 port
//----------------------------------------------------------------------------- 
QByteArray XService::remotePing(QString address,qint32 port)
{
	return params("currentdatetime", QDateTime::currentDateTime(),"return",(bool)true,"address",address,"port",port,"called",m_CallCount);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.11.2022
// Context: DefaultNamespace
// Class:   XService
// Method:  remoteServiceClose
// Params:  QString address
// Params:  qint32 port
//----------------------------------------------------------------------------- 

QByteArray XService::remoteServiceClose(QString address, qint32 port)
{
	emit ServiceClose();

	return params("currentdatetime", QDateTime::currentDateTime(), "return", (bool)true, "address", address, "port", port, "called", m_CallCount);
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    06.11.2022
// Context: DefaultNamespace
// Class:   XService
// Method:  localServiceInfo
// Params:  QMap<QString, QVariant>& map
//----------------------------------------------------------------------------- 
void XService::localServiceInfo(QMap<QString, QVariant>& map)
{
	map["hostname"] = QHostInfo::localHostName();
	map["domainname"] = QHostInfo::localDomainName();
	map["operatingsystem"] = QSysInfo::prettyProductName();
	map["machineid"] = QSysInfo::machineUniqueId();
	map["kernelversion"] = QSysInfo::kernelVersion();
	map["cpu"] = QSysInfo::currentCpuArchitecture();
	map["cpucount"] = QThread::idealThreadCount();
	map["executable"] = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
	map["datetime"] = QDateTime::currentDateTime();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  serviceInfo
// Params:  
//----------------------------------------------------------------------------- 
QByteArray XService::remoteServiceInfo(QString address, qint32 port)
{
	QMap<QString, QVariant> map;

	localServiceInfo(map);

	map["name"] = serviceName();
	map["description"] = serviceDescription();
	map["appname"] = appName();
	map["identity"] = serviceIdentity();

	map["ip"] = serviceAddress().join(';');

	map["fullpath"] = servicePath();
	map["rootpath"] = rootPath();

	map["modified"] = serviceModified();
	map["version"] = serviceVersion();
	map["timestamp"] = serviceTimeStamp();
	map["datetime"] = QDateTime::currentDateTime();

	map["startdatetime"] = m_StartDateTime;

	map["processid"] = m_ProcessId;
	map["called"] = m_CallCount;

	map["address"] = address;
	map["port"] = port;

	return params(map);
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XService
// Method:  serviceInfo
// Params:  QString address
// Params:  qint32 port
//----------------------------------------------------------------------------- 
QMap<QString, QVariant> XService::serviceInfo(QString address, qint32 port)
{
	QMap<QString, QVariant> values;
	values["address"] = address;
	values["port"] = port;
	return XClient::action(address, port, "remoteServiceInfo", values);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XService
// Method:  serviceInfo
// Params:  const QHostAddress& address
// Params:  qint32 port
// Params:  
//----------------------------------------------------------------------------- 

QMap<QString, QVariant> XService::serviceInfo(const QHostAddress& address, qint32 port)
{
	return serviceInfo(address.toString(), port);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XService
// Method:  serviceLogs
// Params:  QString address
// Params:  qint32 port
// Params:  
//----------------------------------------------------------------------------- 

QList<QMap<QString, QVariant>> XService::serviceLogs(QString address, qint32 port, int maxlogs)
{
	QMap<QString, QVariant> values;
	values["address"] = address;
	values["port"] = port;
	values["maxlogs"] = maxlogs;

	QMap<QString, QVariant> ret=XClient::action(address, port, "remoteServiceLogs", values);
	QByteArray bytelogs = (ret["logs"]).toByteArray();
	QDataStream stream(&bytelogs,QIODevice::ReadOnly);
	QList<QMap<QString, QVariant>> logs;
	stream >> logs;
	return logs;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XService
// Method:  serviceLogs
// Params:  const QHostAddress& address
// Params:  qint32 port
// Params:  
//----------------------------------------------------------------------------- 

QList<QMap<QString, QVariant>> XService::serviceLogs(const QHostAddress& address, qint32 port, int maxlogs)
{
	return serviceLogs(address.toString(), port, maxlogs);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    27.10.2022
// Context: DefaultNamespace
// Class:   XService
// Method:  remoteServiceLogs
// Params:  QString address
// Params:  qint32 port
//----------------------------------------------------------------------------- 
QByteArray XService::remoteServiceLogs(QString address, qint32 port, int maxlogs)
{
	QByteArray bytelogs;
	{
		QDataStream stream(&bytelogs, QIODevice::WriteOnly);

		if (maxlogs > m_Logs.count())
		{
			stream << m_Logs;
		}
		else
		{
			QList<QMap<QString, QVariant>> logs;
			for (qint32 i=m_Logs.count()-maxlogs;i<m_Logs.count();i++)
			{
				logs << m_Logs[i];
			}
			stream << logs;
		}
	}
	return params("logs",bytelogs);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  hash
// Params:  const QByteArray& value
//----------------------------------------------------------------------------- 
QByteArray XService::hash(const QByteArray& value)
{
	return QCryptographicHash::hash(value, QCryptographicHash::Algorithm::Md5);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  hash
// Params:  const QString& value
//----------------------------------------------------------------------------- 
QByteArray XService::hash(const QString& value)
{
	return QCryptographicHash::hash((value).toLatin1(), QCryptographicHash::Algorithm::Md5);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XAction
// Method:  call
// Params:  const QString& slotname
// Params:  const QMap<QString, QVariant>& values
//----------------------------------------------------------------------------- 
QByteArray XService::call(const QString& slotname, const QMap<QString, QVariant>& values, const QByteArray& data)
{
	//O_STACK(XAction, call);

	m_CallCount++;

	
	int slotid = -1;
	for (int i = 0; i < metaObject()->methodCount(); i++)
	{
		QMetaMethod slotmethod = metaObject()->method(i);
		if (QString(slotmethod.methodSignature()).indexOf(slotname + "(") == 0)
		{
			slotid = i;
			i = metaObject()->methodCount();
		}
	}
	if (slotid < 0)
	{
		return QByteArray();
	}

	QMetaMethod method = metaObject()->method(slotid);

	//static QMutex mutex;

	{
		//QMutexLocker lock(&mutex);

		QMetaMethodArgument arg[10];
		/*static QString paramstring[10];
		static int paramint[10];
		static bool parambool[10];
		static double paramdouble[10];
		static long paramlong[10];
		static float paramfloat[10];
		static QDate paramdate[10];
		static QTime paramtime[10];
		static QUuid paramuuid[10];*/

		QString paramstring[10];
		int paramint[10];
		bool parambool[10];
		double paramdouble[10];
		long paramlong[10];
		float paramfloat[10];
		QDate paramdate[10];
		QTime paramtime[10];
		QUuid paramuuid[10];
		QByteArray parambyte[10];

		int i = 0;
		for (; i < method.parameterNames().count(); i++)
		{
			QString paramname = QString(method.parameterNames()[i]);
			QString paramtype = QString(method.parameterTypes()[i]);
			QVariant paramvalue;

			if (values.contains(paramname.toLocal8Bit()))
			{
				paramvalue = values.value(paramname.toLocal8Bit()).toString();
			}
			else if (values.contains(paramname.toUpper().toLocal8Bit()))
			{
				paramvalue = values.value(paramname.toUpper().toLocal8Bit()).toString();
			}

			if (QString("data").contains(paramname.toLocal8Bit()))
			{
				parambyte[i] = data;
				arg[i] = Q_ARG(QByteArray, parambyte[i]);
			}
			else if (paramtype == QString("QString"))
			{
				paramstring[i] = paramvalue.toString();
				arg[i] = Q_ARG(QString, paramstring[i]);
				//O_PARAM(paramstring[i]);
			}
			else if (paramtype == QString("int"))
			{
				paramint[i] = paramvalue.toInt();
				arg[i] = Q_ARG(int, paramint[i]);
				//O_PARAM(paramint[i]);
			}
			else if (paramtype == QString("bool"))
			{
				parambool[i] = paramvalue.toString().indexOf("T", Qt::CaseInsensitive) == 0 || paramvalue.toString().indexOf("on", Qt::CaseInsensitive) == 0;
				arg[i] = Q_ARG(bool, parambool[i]);
			}
			else if (paramtype == QString("double"))
			{
				paramdouble[i] = paramvalue.toDouble();
				arg[i] = Q_ARG(double, paramdouble[i]);
			}
			else if (paramtype == QString("float"))
			{
				paramfloat[i] = paramvalue.toFloat();
				arg[i] = Q_ARG(float, paramfloat[i]);
			}
			else if (paramtype == QString("long"))
			{
				paramlong[i] = paramvalue.toString().toLong();
				arg[i] = Q_ARG(long, paramlong[i]);
			}
			else if (paramtype == QString("QDate"))
			{
				paramdate[i] = QDate::fromString(paramvalue.toString(), Qt::ISODate);
				arg[i] = Q_ARG(QDate, paramdate[i]);
			}
			else if (paramtype == QString("QTime"))
			{
				paramtime[i] = QTime::fromString(paramvalue.toString(), Qt::ISODate);
				arg[i] = Q_ARG(QTime, paramtime[i]);
			}
			else if (paramtype == QString("QUuid"))
			{
				paramuuid[i] = QUuid(paramvalue.toString());
				arg[i] = Q_ARG(QUuid, paramuuid[i]);
			}
			else if (paramtype == QString("QByteArray"))
			{
				parambyte[i] = paramvalue.toByteArray();
				arg[i] = Q_ARG(QByteArray, parambyte[i]);
			}
			else
			{
				paramstring[i] = paramvalue.toString();
				arg[i] = Q_ARG(QString, paramstring[i]);
			}
		}

		for (; i < 10; i++)
		{
			paramstring[i] = "";
			paramint[i] = 0;
			parambool[i] = false;
			paramdouble[i] = 0;
			paramlong[i] = 0;
			paramfloat[i] = 0;
			paramdate[i] = QDate();
			paramtime[i] = QTime();
			paramuuid[i] = QUuid();
			parambyte[i] = QByteArray();
		}

		QDateTime indatetime(QDateTime::currentDateTime());

		QByteArray returnValue;
		if (QMetaObject::invokeMethod(this, slotname.toLocal8Bit(), Qt::DirectConnection, Q_RETURN_ARG(QByteArray, returnValue), arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]))
		{
			QDateTime outdatetime(QDateTime::currentDateTime());
			{
				QMap<QString, QVariant> logvalues(values);
				//logvalues.remove("address");
				logvalues.remove("hash");
				//logvalues.remove("port");
				logvalues.remove("data");
				logvalues["in"] = indatetime;
				if (indatetime != outdatetime)
				{
					logvalues["out"] = outdatetime;
					logvalues["duration"] = (int)indatetime.msecsTo(outdatetime);
				}

				while (m_Logs.count() > m_MaxLogs)
					m_Logs.takeFirst();

				m_Logs << logvalues;
			}

			return returnValue;
		}

	}
	return QByteArray();
}

