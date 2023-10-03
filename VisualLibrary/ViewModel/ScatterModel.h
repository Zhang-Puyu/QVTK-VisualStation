#pragma once

#include "ViewModel_2D.h"

#include <QtCharts/QScatterSeries>

using QtCharts::QScatterSeries;

#include "visuallibrary_global.h"

class VISUALLIBRARY_EXPORT ScatterModel  : public ViewModel_2D
{
	Q_OBJECT

public:
	ScatterModel(QObject* parent = nullptr);
	~ScatterModel();

	void readFile(QString filePath);
	void readCsvFile(QString filePath);

	void viewColourful(QString xName, QString yName);
	void viewColourful(unsigned int xIndex, unsigned int yIndex);

	void setPointsSize(unsigned int size);
	void setColor(QColor color);
	void setName(QString name);

	virtual ViewModelType modelType() override { return ViewModelType::SCATTER; }

	bool isEmpty() { return matrix_.isEmpty(); }

	QScatterSeries* series() { return series_; };
	QString			name()   { return name_; }
	QStringList   featureNames() { return featureNames_; }
	unsigned int  featureCount() { return featureNames_.size(); }
	int			  pointsCount()  { return matrix_.size(); }
	Matrix		  matrix()	     { return matrix_; }

	float xMin() { return xMin_; } float xMax() { return xMax_; }
	float yMin() { return yMin_; } float yMax() { return yMax_; }

private:
	QStringList featureNames_;
	Matrix		matrix_;

	float xMin_ = 0, xMax_ = 0, yMin_ = 0, yMax_ = 0;

	QString name_ = "Undefine";

	unsigned int pointsSize_ = 5;

	QScatterSeries* series_ = new QScatterSeries();
};
