#include "flexiblechartview.h"

#include <cmath>

FlexibleChartView::FlexibleChartView(QWidget *parent)
	: QChartView(parent)
{
	bMiddleButtonPressed = false;
	bHorizontalZoom = false;
	bVerticalZoom = false;
}

FlexibleChartView::~FlexibleChartView()
{
}

void FlexibleChartView::mouseMoveEvent(QMouseEvent* pEvent)
{
	if (bMiddleButtonPressed)
	{
		QPoint oDeltaPos = pEvent->pos() - oPrePos;
		this->chart()->scroll(-oDeltaPos.x(), oDeltaPos.y());
		oPrePos = pEvent->pos();
	}
	__super::mouseMoveEvent(pEvent);
}

void FlexibleChartView::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::MiddleButton)
	{
		bMiddleButtonPressed = true;
		oPrePos = pEvent->pos();
		this->setCursor(Qt::SizeAllCursor);
	}
	if (pEvent->button() == Qt::LeftButton)
	{
		bVerticalZoom = true;
		oPrePos = pEvent->pos();
		this->setCursor(Qt::SplitVCursor);
	}
	if (pEvent->button() == Qt::RightButton)
	{
		bHorizontalZoom = true;
		oPrePos = pEvent->pos();
		this->setCursor(Qt::SplitHCursor);
	}
	__super::mousePressEvent(pEvent);
}

void FlexibleChartView::mouseReleaseEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::MiddleButton)
	{
		bMiddleButtonPressed = false;
		this->setCursor(Qt::ArrowCursor);
	}
	if (pEvent->button() == Qt::LeftButton)
	{
		bVerticalZoom = false;
		this->setCursor(Qt::ArrowCursor);
	}
	if (pEvent->button() == Qt::RightButton)
	{
		bHorizontalZoom = false;
		this->setCursor(Qt::ArrowCursor);
	}
	__super::mouseReleaseEvent(pEvent);
}

void FlexibleChartView::wheelEvent(QWheelEvent* pEvent)
{
	qreal rVal = std::pow(0.999, pEvent->delta()); // 设置比例
	// 1. 读取视图基本信息
	QRectF oPlotAreaRect = this->chart()->plotArea();
	QPointF oCenterPoint = oPlotAreaRect.center();
	// 2. 水平调整
	if(bHorizontalZoom)
		oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
	// 3. 竖直调整
	if(bVerticalZoom)
		oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
	// 同时调整
	if ((!bHorizontalZoom) && (!bVerticalZoom))
	{
		oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
		oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
	}
	// 4.1 计算视点，视点不变，围绕中心缩放
	//QPointF oNewCenterPoint(oCenterPoint);
	// 4.2 计算视点，让鼠标点击的位置移动到窗口中心
	//QPointF oNewCenterPoint(pEvent->pos());
	// 4.3 计算视点，让鼠标点击的位置尽量保持不动(等比换算，存在一点误差)
	QPointF oNewCenterPoint(2 * oCenterPoint - pEvent->pos() - (oCenterPoint - pEvent->pos()) / rVal);
	// 5. 设置视点
	oPlotAreaRect.moveCenter(oNewCenterPoint);
	// 6. 提交缩放调整
	this->chart()->zoomIn(oPlotAreaRect);
	__super::wheelEvent(pEvent);
}
