#include "XSync.h"

#include "XClient.h"

#include "../CoreBase/XDebug.h"

#include "../CoreStorage/XFile.h"
#include "../CoreStorage/XDirectory.h"
#include "../CoreStorage/XCrypt.h"

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XFileTransfer
// Method:  XFileTransfer
// Params:  QObject* parent
//----------------------------------------------------------------------------- 

XSync::XSync(qint32 serverport, QObject *parent): XServer(parent)
{
	m_ServerPort=serverport;
	m_ClientPort=serverport;
	m_ClientHost=QHostAddress::LocalHost;

	if (m_ServerPort > 0)
		open(m_ServerPort);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  XSync
// Params:  QHostAddress clientHost
// Params:  qint32 clientport
// Params:  QObject* parent
//----------------------------------------------------------------------------- 

XSync::XSync(qint32 clientport, QHostAddress clientHost, QObject* parent)
{
	m_ServerPort = -1;
	m_ClientPort = clientport;
	m_ClientHost = clientHost;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    24.10.2022
// Context: DefaultNamespace
// Class:   XFileTransfer
// Method:  ~XFileTransfer
//----------------------------------------------------------------------------- 
XSync::~XSync()
{
	
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  sync
// Params:  const QString& localpath
// Params:  const QString& remotepath
//----------------------------------------------------------------------------- 
bool XSync::sync(const QString& localpath, const QString& remotepath)
{
	QList<QDateTime> remotechanged;
	QList<qint64> remotesize;
	QStringList remotehash;
	QStringList remotefiles = treeInfo(remotepath, remotechanged, remotesize, remotehash);
	QStringList files = XDirectory::tree(rootPath(localpath));

	QSet<QString> used;

	// Local Files
	for(const QString&filename:files)
	{
		used.insert(filename);

		bool up= false;
		bool down= false;
		
		qint32 pos = remotefiles.indexOf(filename);
		if (pos>=0)
		{
			QFileInfo fileinfo(rootPath(localpath) + QDir::separator() + filename);
			fileinfo.refresh();
			if (remotesize[pos]==fileinfo.size()&&remotehash[pos]== XService::hash(XFile::readBinaryCache(rootPath(localpath) + QDir::separator() + filename)))
			{
				// Equal
				up=false;
				down=false;
			}
			else
			{
				if (fileinfo.lastModified() > remotechanged[pos])
					up = true;
				else
					down= true;
			}
		}
		else
			up=true;

		if (up)
		{
			upload(localpath + QDir::separator() + filename, remotepath + QDir::separator() + filename);
		}
		if (down)
		{
			download(remotepath + QDir::separator() + filename, localpath + QDir::separator() + filename);
		}
	}

	// New Remote Files
	for (const QString& filename : remotefiles)
	{
		if (!used.contains(filename))
		{
			download(remotepath + QDir::separator() + filename, localpath + QDir::separator() + filename);
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    26.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  syncRemove
// Params:  const QString& localfilename
// Params:  const QString& remotefilename
//----------------------------------------------------------------------------- 
bool XSync::syncRemove(const QString& localfilename, const QString& remotefilename)
{
	XFile::remove(rootPath(localfilename));
	return remove(remotefilename);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  upload
// Params:  const QString& localfilename
// Params:  const QString& remotefilename
//----------------------------------------------------------------------------- 
bool XSync::upload(const QString& localfilename, const QString& remotefilename)
{
	if (!XFile::exists(rootPath(localfilename)))
		return false;

	QMap<QString, QVariant> values;
	values["localfilename"] = localfilename;
	values["remotefilename"] = remotefilename;
	const QByteArray& filedata= XFile::readBinaryCache(rootPath(localfilename));

	QString filehash = XService::hash(filedata);
	values["hash"] = filehash;
	QByteArray data = XCrypt::crypt(qCompress(filedata),filehash+localfilename+remotefilename);
	
	QMap<QString, QVariant> ret = XClient::action(m_ClientHost,m_ClientPort, "remoteUpload", values, data);
	return ret["return"].toBool();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  serverUpload
// Params:  const QString& localfilename
// Params:  const QString& remotefilename
// Params:  const QString& hash
// Params:  const QByteArray& data
//----------------------------------------------------------------------------- 
QByteArray XSync::remoteUpload(const QString& localfilename, const QString& remotefilename,const QString& hash, const QByteArray& data)
{
	const QByteArray& filedata = qUncompress(XCrypt::unCrypt(data, hash + localfilename + remotefilename));
	if (XService::hash(filedata) !=hash)
		return params("return", (bool)false);
	XFile::writeBinary(rootPath(remotefilename),filedata,true);
	return params("return", (bool)true);
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  download
// Params:  const QString& remotefilename
// Params:  const QString& localfilename
//----------------------------------------------------------------------------- 
bool XSync::download(const QString& remotefilename, const QString& localfilename)
{
	QMap<QString, QVariant> values;
	values["localfilename"] = localfilename;
	values["remotefilename"] = remotefilename;
	values["hash"] = XService::hash(localfilename + remotefilename);

	QMap<QString, QVariant> ret = XClient::action(m_ClientHost, m_ClientPort, "remoteDownload", values);
	if (!ret["return"].toBool())
		return false;
	QString downloadhash = ret["hash"].toString();
	if (downloadhash.isEmpty())
		return false;

	const QByteArray& data = ret["data"].toByteArray();
	const QByteArray& filedata = qUncompress(XCrypt::unCrypt(data, downloadhash));
	if (downloadhash!=XService::hash(filedata))
		return false;

	XFile::writeBinary(rootPath(localfilename), filedata, true);
	
	return true;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  remoteDownload
// Params:  const QString& remotefilename
// Params:  const QString& localfilename
//----------------------------------------------------------------------------- 
QByteArray XSync::remoteDownload(const QString& remotefilename, const QString& localfilename, const QString& hash)
{
	if (!XFile::exists(rootPath(remotefilename)))
		return params("return", (bool)false);

	if (XService::hash(localfilename + remotefilename) !=hash)
		return params("return", (bool)false);

	const QByteArray& filedata = XFile::readBinaryCache(rootPath(remotefilename));
	QString filehash = XService::hash(filedata);
	const QByteArray& data = XCrypt::crypt(qCompress(filedata), filehash);

	return params("hash", filehash, "return", (bool)true,"data",data);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  remove
// Params:  const QString& remotefilename
//----------------------------------------------------------------------------- 
bool XSync::remove(const QString& remotefilename)
{
	QMap<QString, QVariant> values;
	values["remotefilename"] = remotefilename;
	values["hash"] = XService::hash(remotefilename);
	QMap<QString, QVariant> ret = XClient::action(m_ClientHost, m_ClientPort, "remoteRemove", values);
	return ret["return"].toBool();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  uploadDirectory
// Params:  const QString& localpath
// Params:  const QString& remotepath
//----------------------------------------------------------------------------- 
bool XSync::uploadDirectory(const QString& localpath, const QString& remotepath)
{
	for (QString filename :XDirectory::tree(rootPath(localpath)))
	{
		upload(localpath+ QDir::separator() + filename, remotepath+ QDir::separator() + filename);
	}
	return true;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  downloadDirectory
// Params:  const QString& remotepath
// Params:  const QString& localpath
//----------------------------------------------------------------------------- 

bool XSync::downloadDirectory(const QString& remotepath, const QString& localpath)
{
	for (QString filename : tree(remotepath))
	{
		download(remotepath + QDir::separator() + filename, localpath + QDir::separator() + filename);
	}
	return true;
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  removeDirectory
// Params:  const QString& remotepath
//----------------------------------------------------------------------------- 

bool XSync::removeDirectory(const QString& remotepath)
{
	QMap<QString, QVariant> values;
	values["remotepath"] = remotepath;
	values["hash"] = XService::hash(remotepath);
	QMap<QString, QVariant> ret = XClient::action(m_ClientHost, m_ClientPort, "remoteRemoveDirectory", values);
	return ret["return"].toBool();

	return false;
}
//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  remoteDirectory
// Params:  const QString& remotepath
// Params:  const QString& hash
//----------------------------------------------------------------------------- 

QByteArray XSync::remoteRemoveDirectory(const QString& remotepath, const QString& hash)
{
	if (XService::hash(remotepath) != hash)
		return params("return", (bool)false);
	XDirectory::remove(rootPath(remotepath), true);
	return params("return", (bool)true);

}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  remoteRemove
// Params:  const QString& remotefilename
//----------------------------------------------------------------------------- 
QByteArray XSync::remoteRemove(const QString& remotefilename, const QString& hash)
{
	if (XService::hash(remotefilename) != hash)
		return params("return", (bool)false);
	XFile::remove(rootPath(remotefilename),  true);
	return params("return", (bool)true);
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  files
// Params:  const QString& remotepath
//----------------------------------------------------------------------------- 
QStringList XSync::files(const QString& remotepath)
{
	QMap<QString, QVariant> values;
	values["remotepath"] = remotepath;
	values["hash"] = XService::hash(remotepath);
	QMap<QString, QVariant> ret = XClient::action(m_ClientHost, m_ClientPort, "remoteFiles", values);
	if (!ret["return"].toBool())
		return QStringList();
	return ret["files"].toStringList();
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  remoteFiles
// Params:  const QString& remotepath
// Params:  const QString& hash
// Params:  
//----------------------------------------------------------------------------- 
QByteArray XSync::remoteFiles(const QString& remotepath, const QString& hash)
{
	if (XService::hash(remotepath) != hash)
		return params("return", (bool)false);
	QStringList files=XDirectory::files(rootPath(remotepath));
	return params("return", (bool)true,"files",files);
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  tree
// Params:  const QString& remotepath
//----------------------------------------------------------------------------- 
QStringList XSync::tree(const QString& remotepath)
{
	QMap<QString, QVariant> values;
	values["remotepath"] = remotepath;
	values["hash"] = XService::hash(remotepath);
	QMap<QString, QVariant> ret = XClient::action(m_ClientHost, m_ClientPort, "remoteTree", values);
	if (!ret["return"].toBool())
		return QStringList();
	return ret["files"].toStringList();
}


//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    25.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  remoteTree
// Params:  const QString& remotepath
// Params:  const QString& hash
//----------------------------------------------------------------------------- 

QByteArray XSync::remoteTree(const QString& remotepath, const QString& hash)
{
	if (XService::hash(remotepath) != hash)
		return params("return", (bool)false);
	QStringList files = XDirectory::tree(rootPath(remotepath));
	return params("return", (bool)true, "files", files);
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    26.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  treeInfo
// Params:  const QString& remotepath
// Params:  QStringList& changedlist
// Params:  QStringList& sizelist
// Params:  QStringList& hashList
// Params:  
//----------------------------------------------------------------------------- 

QStringList XSync::treeInfo(const QString& remotepath, QList<QDateTime>& changedlist, QList<qint64>& sizelist, QStringList& hashlist)
{
	QMap<QString, QVariant> values;
	values["remotepath"] = remotepath;
	values["hash"] = XService::hash(remotepath);
	QMap<QString, QVariant> ret = XClient::action(m_ClientHost, m_ClientPort, "remoteTreeInfo", values);
	if (!ret["return"].toBool())
		return QStringList();

	for(const QString&datetime:ret["filechanged"].toStringList())
		changedlist << QDateTime::fromString(datetime,Qt::ISODate);
	for (const QString& size: ret["sizelist"].toStringList())
		sizelist << size.toLongLong();

	hashlist = ret["hashlist"].toStringList();

	return ret["files"].toStringList();
}

//-----------------------------------------------------------------------------
// Author:  Tobias Post
// Company: CUBE4DEV GmbH
// Date:    26.10.2022
// Context: DefaultNamespace
// Class:   XSync
// Method:  remoteTreeInfo
// Params:  const QString& remotepath
// Params:  const QString& hash
//----------------------------------------------------------------------------- 

QByteArray XSync::remoteTreeInfo(const QString& remotepath, const QString& hash)
{
	if (XService::hash(remotepath) != hash)
		return params("return", (bool)false);

	QStringList files = XDirectory::tree(rootPath(remotepath));

	QStringList changedlist;
	QStringList sizelist;
	QStringList hashlist;

	for(const QString& filename:files)
	{
		QFileInfo fileinfo(rootPath(remotepath) + QDir::separator() + filename);
		fileinfo.refresh();

		changedlist << fileinfo.lastModified().toString(Qt::ISODate);
		sizelist << QString("%1").arg(fileinfo.size());
		hashlist << XService::hash(XFile::readBinaryCache(rootPath(remotepath) + QDir::separator() + filename));
		//hashlist << "XXX";
	}

	//return params("return", (bool)true, "files", files,"filechanged",changedlist,"sizelist",sizelist,"hashlist", hashlist);
	return params("return", (bool)true, "files", files, "filechanged", changedlist, "sizelist", sizelist, "hashlist", hashlist);
}

