#include "ViewWidget_2D.h"

#include <QtCharts/QValueAxis>
#include <QSlider>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QVBoxLayout>
#include <QMimeData>

QT_CHARTS_USE_NAMESPACE

ViewWidget_2D::ViewWidget_2D(QWidget* parent)
	: QWidget(parent)
{
    //chart_->setTitle("Title");
    chart_->setTitleFont(QFont("SimHei", 20));

    chart_->addAxis(xAxis_, Qt::AlignBottom);
    chart_->addAxis(yAxis_, Qt::AlignLeft);

    xAxis_->setRange(0, 1);
    yAxis_->setRange(0, 1);

    chartView_->setChart(chart_);
    this->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->insertWidget(0, chartView_);
}

ViewWidget_2D::~ViewWidget_2D()
{

}

void ViewWidget_2D::loadFile(QString filePath)
{
    if (filePath.isEmpty()) return;
	if (filePath.endsWith(".csv"))
		loadCsvFile(filePath);
	else
		QMessageBox::warning(this, "Warning", "Unsupported file type!");
}

void ViewWidget_2D::loadCsvFile(QString filePath)
{
    ScatterModel* scatterModel = new ScatterModel();
    scatterModel->readCsvFile(filePath);
    scatterModel->viewColourful(0, 1);

    this->addModel(scatterModel);
}

void ViewWidget_2D::addModel(ScatterModel* scatterModel)
{
    chart_->addSeries(scatterModel->series()); 
    setXRange(scatterModel->xMin(), scatterModel->xMax());
    setYRange(scatterModel->yMin(), scatterModel->yMax());

    modelHash_.insert(scatterModel, scatterModel->series());
}

void ViewWidget_2D::setTitle(QString title)
{
    chart_->setTitle(title);
}

void ViewWidget_2D::setXRange(float min, float max)
{
    xMin_ = std::min(min, xMin_);
    xMax_ = std::max(max, xMax_);
    xAxis_->setRange(xMin_, xMax_);

    if (QString::number(xMin_).length() > 3 || QString::number(xMax_).length() > 3)
        xAxis_->setLabelFormat("%.2e");
    else
        xAxis_->setLabelFormat("%f");
}

void ViewWidget_2D::setYRange(float min, float max)
{
    yMin_ = std::min(min, yMin_);
	yMax_ = std::max(max, yMax_);
	yAxis_->setRange(yMin_, yMax_);

    if (QString::number(yMin_).length() > 3 || QString::number(yMax_).length() > 3)
        yAxis_->setLabelFormat("%.2e");
    else
        yAxis_->setLabelFormat("%g");
}

void ViewWidget_2D::removeModel(ScatterModel* scatterModel)
{
    chart_->removeSeries(scatterModel->series());
    modelHash_.remove(scatterModel);
}

void ViewWidget_2D::clear()
{
	chart_->removeAllSeries();
	modelHash_.clear();
	xMin_ = xMax_ = yMin_ = yMax_ = 0;
}

void ViewWidget_2D::dragEnterEvent(QDragEnterEvent* ev)
{
    if (ev->mimeData()->hasUrls())
        ev->acceptProposedAction();
}

void ViewWidget_2D::dropEvent(QDropEvent* ev)
{
    if (ev->mimeData()->hasUrls())
    {
        emit fileDropped(ev->mimeData()->urls());
        for (auto url : ev->mimeData()->urls())
            if(url.toString().endsWith(".csv"))
                loadCsvFile(url.toLocalFile());
        ev->acceptProposedAction();
    }
}
