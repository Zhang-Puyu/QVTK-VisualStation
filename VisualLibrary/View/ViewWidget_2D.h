#pragma once

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QColor> 
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QGraphicsSimpleTextItem>

#include "flexiblechartview.h"
#include "ScatterModel.h"

using QtCharts::QChart;
using QtCharts::QValueAxis;

#include "visuallibrary_global.h"

class VISUALLIBRARY_EXPORT ViewWidget_2D : public QWidget
{
	Q_OBJECT

public:
	ViewWidget_2D(QWidget* parent = nullptr);
	~ViewWidget_2D();

	void loadFile(QString filePath);
	void loadCsvFile(QString filePath);

	void addModel(ScatterModel* scatterModel);

	void setTitle(QString title = "Undefine Title");
	void setXRange(float min, float max);
	void setYRange(float min, float max);

	void removeModel(ScatterModel* scatterModel);

	void clear();

protected:

	void dragEnterEvent(QDragEnterEvent* ev) override;
	void dropEvent(QDropEvent* ev) override;

private:
	FlexibleChartView* chartView_ = new FlexibleChartView();
	QChart*  chart_     = new QChart();

	QValueAxis* xAxis_ = new QValueAxis();
	QValueAxis* yAxis_ = new QValueAxis();

	QHash<ScatterModel*, QScatterSeries*> modelHash_;
	
	float xMin_=0, xMax_=1, yMin_=0, yMax_=1;

signals:
	void fileDropped(QList<QUrl> urls);
};

