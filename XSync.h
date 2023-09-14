#pragma once

#include <QObject>

#include "XServer.h"

namespace CoreNetwork
{

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XFileTransfer
// Method:  XFileTransfer
// Description: The XSync class is a subclass of XServer. It represents a
//   network file synchronization service that is used for uploading, downloading,
//   removing, and managing files and directories between a local and remote system.
//   XSync has methods like sync, upload, download, remove, uploadDirectory,
//   downloadDirectory, removeDirectory, files, tree, and treeInfo, which serve
//   various functionalities mentioned above. Additionally, XSync also has
//   public slots that communicate with other instances of the same class
//   over a network. All operations are initiated by a base instance,
//   which can then invoke the necessary methods of remote instances.
//----------------------------------------------------------------------------- 

class CORENETWORK_EXPORT XSync  : public XServer
{
	Q_OBJECT

public:
	XSync(qint32 serverport,QObject *parent=Q_NULLPTR);
	XSync(qint32 clientport, QHostAddress clientHost, QObject* parent = Q_NULLPTR);
	virtual ~XSync();

	virtual QString serviceName() { return "Sync"; }
	virtual QString serviceVersion() { return "1.0.0.0.0"; }
	virtual QString serviceDescription() { return "Network File Sync"; }

	bool sync(const QString& localpath, const QString& remotepath);
	bool syncRemove(const QString& localfilename, const QString& remotefilename);

	bool upload(const QString& localfilename, const QString& remotefilename);
	bool download(const QString& remotefilename, const QString& localfilename);
	bool remove(const QString& remotefilename);

	bool uploadDirectory(const QString& localpath, const QString& remotepath);
	bool downloadDirectory(const QString& remotepath, const QString& localpath);
	bool removeDirectory(const QString& remotepath);

	QStringList files(const QString& remotepath);
	QStringList tree(const QString& remotepath);
	QStringList treeInfo(const QString& remotepath, QList<QDateTime>& changedlist, QList<qint64>& sizelist, QStringList& hashlist);

public slots:
	QByteArray remoteUpload(const QString& localfilename, const QString& remotefilename, const QString& hash,const QByteArray& data);
	QByteArray remoteDownload(const QString& remotefilename, const QString& localfilename, const QString& hash);
	QByteArray remoteRemove(const QString& remotefilename, const QString& hash);
	QByteArray remoteFiles(const QString& remotepath, const QString& hash);
	QByteArray remoteTree(const QString& remotepath, const QString& hash);
	QByteArray remoteTreeInfo(const QString& remotepath, const QString& hash);
	QByteArray remoteRemoveDirectory(const QString& remotepath, const QString& hash);
	
private:
	qint32 m_ServerPort;
	qint32 m_ClientPort;
	QHostAddress m_ClientHost;
};


};
using namespace CoreNetwork;

