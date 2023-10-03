#pragma once

#include "visuallibrary_global.h"

#include "qtcpserver.h"

class FilePathTcpWatcher : public QObject
{
	Q_OBJECT

public:
	FilePathTcpWatcher(QObject* parent = nullptr);
	~FilePathTcpWatcher();

	void startWatch(int port = 8848);
	void stopWatch();

private:
	QTcpServer* server_;

signals:
	void receiveFilePath(QString filePath);
};
