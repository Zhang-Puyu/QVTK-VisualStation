#pragma once

#include "qfilesystemwatcher.h"
#include "qset.h"

#include "visuallibrary_global.h"

class VISUALLIBRARY_EXPORT FileWatcher  : public QObject
{
	Q_OBJECT

public:
	FileWatcher(QObject *parent);
	~FileWatcher();

	void setWatchPath(QString watchPath);
	void setFileType(QString fileType);

	void startWatch(QString watchPath = "", QString fileType = "");
	void stopWatch();

	QString getWatchPath() { return watchPath_; }

private:
	QFileSystemWatcher* fileSystemWatcher_
		= new QFileSystemWatcher();

	QString fileType_ = ".csv";
	QString watchPath_ = "";

	QSet<QString> existFiles_ = {};

	void onWatchedDirectoryChanged(QString path);

signals:
	void filesAdded(QSet<QString> addedFilePathes);
};
