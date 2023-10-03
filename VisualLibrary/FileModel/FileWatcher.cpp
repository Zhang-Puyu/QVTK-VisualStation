
#include "FileWatcher.h"

#include "qdir.h"

FileWatcher::FileWatcher(QObject *parent)
	: QObject(parent)
{
	
}

FileWatcher::~FileWatcher()
{
}

void FileWatcher::setWatchPath(QString watchPath)
{
	fileSystemWatcher_->removePath(watchPath_);
	fileSystemWatcher_->addPath(watchPath);
	qDebug() << "set watch path: " << watchPath;

	watchPath_ = watchPath;

	QDir dir(watchPath);
	existFiles_ = dir.entryList(QDir::Files).toSet();
}

void FileWatcher::setFileType(QString fileType)
{
	fileType_ = fileType;
}

void FileWatcher::startWatch(QString watchPath, QString fileType)
{
	if (watchPath != "")
		setWatchPath(watchPath);
	if (fileType != "")
		setFileType(fileType);
	connect(fileSystemWatcher_, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::onWatchedDirectoryChanged);
}

void FileWatcher::stopWatch()
{
	disconnect(fileSystemWatcher_, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::onWatchedDirectoryChanged);
}

void FileWatcher::onWatchedDirectoryChanged(QString path)
{
	QDir dir(path);

	QSet<QString> addedFileSet = dir.entryList(QDir::Files).toSet() - existFiles_;
	QSet<QString> addedFilePathSet;
	for (auto& file : addedFileSet)
		if (file.endsWith(fileType_))
		{
			addedFilePathSet.insert(watchPath_ + "/" + file);
			qDebug() << "find new file: " << file;
		};

	existFiles_ = dir.entryList(QDir::Files).toSet();

	emit filesAdded(addedFilePathSet);
}
