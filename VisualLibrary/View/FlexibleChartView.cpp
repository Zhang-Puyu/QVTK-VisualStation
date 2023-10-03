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
	qreal rVal = std::pow(0.999, pEvent->delta()); // ���ñ���
	// 1. ��ȡ��ͼ������Ϣ
	QRectF oPlotAreaRect = this->chart()->plotArea();
	QPointF oCenterPoint = oPlotAreaRect.center();
	// 2. ˮƽ����
	if(bHorizontalZoom)
		oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
	// 3. ��ֱ����
	if(bVerticalZoom)
		oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
	// ͬʱ����
	if ((!bHorizontalZoom) && (!bVerticalZoom))
	{
		oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
		oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
	}
	// 4.1 �����ӵ㣬�ӵ㲻�䣬Χ����������
	//QPointF oNewCenterPoint(oCenterPoint);
	// 4.2 �����ӵ㣬���������λ���ƶ�����������
	//QPointF oNewCenterPoint(pEvent->pos());
	// 4.3 �����ӵ㣬���������λ�þ������ֲ���(�ȱȻ��㣬����һ�����)
	QPointF oNewCenterPoint(2 * oCenterPoint - pEvent->pos() - (oCenterPoint - pEvent->pos()) / rVal);
	// 5. �����ӵ�
	oPlotAreaRect.moveCenter(oNewCenterPoint);
	// 6. �ύ���ŵ���
	this->chart()->zoomIn(oPlotAreaRect);
	__super::wheelEvent(pEvent);
}
