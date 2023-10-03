#include "FilePathTcpWatcher.h"

#include "qfileinfo.h"
#include "qtcpserver.h"
#include "qtcpsocket.h"

FilePathTcpWatcher::FilePathTcpWatcher(QObject* parent)
	: QObject(parent)
{
}

FilePathTcpWatcher::~FilePathTcpWatcher()
{
}

void FilePathTcpWatcher::startWatch(int port)
{
	server_ = new QTcpServer();
	server_->listen(QHostAddress::Any, port);
	qDebug() << "socket is listen " << port;

	connect(server_, &QTcpServer::newConnection, [=]() {

		QTcpSocket* socket = server_->nextPendingConnection();

		//��ȡ�Է���IP�Ͷ˿�
		QString ip = socket->peerAddress().toString();
		qint16 port = socket->peerPort();
		qDebug() << QString("[%1:%2]:connect success").arg(ip).arg(port);
		
		connect(socket, &QTcpSocket::readyRead, [=]() {
			//��ͨ���׽�����ȡ������
			QByteArray byteArray = socket->readAll();
			qDebug() << "recieve message: " << byteArray;

			QFileInfo fileInfo(byteArray);
			if (fileInfo.isDir())
				emit receiveFilePath(byteArray);
			});
		});
}

void FilePathTcpWatcher::stopWatch()
{
	server_->disconnect();
	server_->close();
	qDebug() << "socket is close";
}
