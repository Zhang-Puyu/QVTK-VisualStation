#include "ScatterModel.h"

#include "qfile.h"
#include "qfileinfo.h"
#include "qtextstream.h"

ScatterModel::ScatterModel(QObject *parent)
	: ViewModel_2D(parent)
{
	series_->setMarkerSize(pointsSize_);
	series_->setBorderColor(series_->color());
}

ScatterModel::~ScatterModel()
{
}

void ScatterModel::readFile(QString filePath)
{
	 
}

void ScatterModel::readCsvFile(QString filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "open file failed";
		return;
	}

	QFileInfo fileInfo(filePath);
	this->setName(fileInfo.baseName());

	QTextStream in(&file);
	matrix_.clear();
	featureNames_.clear();

	QString line = in.readLine();
	if (line.contains(QRegExp("[\\x4e00-\\x9fa5]|[a-zA-Z]")))
		featureNames_ = line.split(",");
	else
		for (int i = 0; i < line.split(",").size(); i++)
			featureNames_.push_back("colum-" + QString::number(i));

	line = in.readLine();
	while (!line.isNull())
	{
		QVector<float> rowArray;
		for (const QString& cell : line.split(","))
			rowArray.push_back(cell.toFloat());
		matrix_.push_back(rowArray);
		line = in.readLine();
	}

	file.close();
}

void ScatterModel::viewColourful(QString xName, QString yName)
{
	viewColourful(featureNames_.indexOf(xName), featureNames_.indexOf(yName));
}

void ScatterModel::viewColourful(unsigned int xIndex, unsigned int yIndex)
{
	series_->clear();
	series_->setName(featureNames_[xIndex] + " - " + featureNames_[yIndex]);

	for (const QVector<float>& row : matrix_)
		series_->append(row[xIndex], row[yIndex]);

	xMin_ = std::min_element(matrix_.begin(), matrix_.end(),
		[=](QVector<float> p, QVector<float> q)->bool {return p[xIndex] < q[xIndex]; })
		->at(xIndex);
	xMax_ = std::max_element(matrix_.begin(), matrix_.end(),
		[=](QVector<float> p, QVector<float> q)->bool {return p[xIndex] < q[xIndex]; })
		->at(xIndex);
	yMin_ = std::min_element(matrix_.begin(), matrix_.end(),
		[=](QVector<float> p, QVector<float> q)->bool {return p[yIndex] < q[yIndex]; })
		->at(yIndex);
	yMax_ = std::max_element(matrix_.begin(), matrix_.end(),
		[=](QVector<float> p, QVector<float> q)->bool {return p[yIndex] < q[yIndex]; })
		->at(yIndex);
}

void ScatterModel::setPointsSize(unsigned int size)
{
	pointsSize_ = size;
	series_->setMarkerSize(size);
}

void ScatterModel::setColor(QColor color)
{
	series_->setColor(color);
	series_->setBorderColor(color);
}

void ScatterModel::setName(QString name)
{
	name_ = name;
	series_->setName(name_);
}