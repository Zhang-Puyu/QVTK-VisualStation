#pragma once

#include <QtCharts/QChartView>
#include <QMouseEvent>
#include <QGraphicsSimpleTextItem>

#include "visuallibrary_global.h"

class VISUALLIBRARY_EXPORT FlexibleChartView : public QtCharts::QChartView
{
	Q_OBJECT

public:
	FlexibleChartView(QWidget* parent = nullptr);
	~FlexibleChartView();

protected:
	virtual void mouseMoveEvent(QMouseEvent* pEvent) override;
	virtual void mousePressEvent(QMouseEvent* pEvent) override;
	virtual void mouseReleaseEvent(QMouseEvent* pEvent) override;
	virtual void wheelEvent(QWheelEvent* pEvent) override;
private:
	bool bMiddleButtonPressed =false;
	bool bHorizontalZoom = false;
	bool bVerticalZoom = false;
	QPoint oPrePos;
};
