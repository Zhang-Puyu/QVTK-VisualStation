
#include "FileModel.h"

#include "qfile.h"

void readCsv(QString filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "open file failed";
		return;
	}
	
	QTextStream in(&file);

	QString line = in.readLine();
	QStringList featureNames = line.split(",");

	Matrix matrix;
	while (!line.isNull())
	{
		QString line = in.readLine();
		QVector<float> rowArray;
		for (const QString& cell : line.split(","))
			rowArray.push_back(cell.toFloat());
		matrix.push_back(rowArray);
	}

	file.close();
}
