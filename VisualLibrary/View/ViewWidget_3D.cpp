
#include "ViewWidget_3D.h"

#include "vtkCaptionActor2D.h"
#include "vtkScalarBarActor.h"
#include "vtkActor2DCollection.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"

#include "ToolpathModel.h"
#include "PointsModel.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QUrl>

ViewWidget_3D::ViewWidget_3D(QWidget* parent)
	: QVTKOpenGLWidget(parent)
{
	this->setAcceptDrops(true);	

	this->setBackground();
	this->setAxes();
	this->setScalarbar();
	this->setLookupTable();

	vsp<PropPickerInteractorStyle> style = vsp<PropPickerInteractorStyle>::New();

	style->SetDefaultRenderer(renderer_);
	this->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);

	qRegisterMetaType<vsp<vtkActor>>("vsp<vtkActor>");
	qRegisterMetaType<PointsModel*>("PointsModel*");
	qRegisterMetaType<SolidModel*>("SolidModel*");

	connect(style->pickedPropSignalSender, &PickedPropSignalSender::actorPicked, [=](vsp<vtkActor> pickedActor) {
		pickedActor_ = pickedActor;
		emit this->actorPicked(pickedActor);
		});
	connect(style->pickedPropSignalSender, &PickedPropSignalSender::positionPicked, [=](QVector<double> pickedPostion) {
		pickedPosition_ = pickedPostion;
		emit this->positionPicked(pickedPostion);
		});

	this->GetRenderWindow()->AddRenderer(renderer_);
	this->GetRenderWindow()->Render();

};

ViewWidget_3D::~ViewWidget_3D()
{

}

void ViewWidget_3D::loadFile(QString filePath)
{
	if (filePath.isEmpty()) return;
	if (filePath.endsWith(".stl"))
		this->loadStlFile(filePath);
	else if (filePath.endsWith(".csv"))
		this->loadCsvFile(filePath);
	else if (filePath.endsWith(".cls"))
		this->loadClsFile(filePath);
	else
		QMessageBox::warning(this, "Warning", "Unsupported file type!");
}

void ViewWidget_3D::loadStlFile(QString filePath)
{
	SolidModel* solidModel = new SolidModel();
	solidModel->readFile(filePath);
	this->addModel(solidModel);

	emit solidModelAdded(solidModel);
	//emit solidModel->addedToView();
}

void ViewWidget_3D::loadCsvFile(QString filePath)
{
	PointsModel* pointsModel = new ToolpathModel();
	pointsModel->readCsvFile(filePath);
	pointsModel->setLut(this->lut_);
	pointsModel->viewColourful(3);
	this->addModel(pointsModel);

	connect(pointsModel, &PointsModel::colouredScalarRange, this, &ViewWidget_3D::setScalarRange);
	connect(pointsModel, &PointsModel::colouredFeatureName, [=](QString featureName) {scalarbarActor_->SetTitle(featureName.toLatin1()); });
	connect(pointsModel, &PointsModel::colouredPointsCount, this, &ViewWidget_3D::setColorNum);

	connect(pointsModel, &PointsModel::colored, [=]() {this->GetRenderWindow()->Render(); });

	emit pointsModelAdded(pointsModel);
	//emit pointsModel->addedToView();
}

void ViewWidget_3D::loadClsFile(QString filePath)
{
	ToolpathModel* toolpathModel = new ToolpathModel();
	toolpathModel->readClsFile(filePath);
	toolpathModel->setLut(this->lut_);
	toolpathModel->viewColourful("feed rate");
	this->addModel(toolpathModel);

	connect(toolpathModel, &PointsModel::colouredScalarRange, this, &ViewWidget_3D::setScalarRange);
	connect(toolpathModel, &PointsModel::colouredFeatureName,
		[=](QString featureName) {scalarbarActor_->SetTitle(featureName.toLatin1()); });
	connect(toolpathModel, &PointsModel::colouredPointsCount, this, &ViewWidget_3D::setColorNum);

	connect(toolpathModel, &ToolpathModel::colored, [=]() {this->GetRenderWindow()->Render(); });

	emit pointsModelAdded(toolpathModel);
	//emit toolpathModel->addedToView();
}

void ViewWidget_3D::addModel(ViewModel_3D* viewModel)
{
	renderer_->AddActor(viewModel->actor());
	modelHash_.insert(viewModel, viewModel->actor());

	emit viewModel->addedToView();

	this->GetRenderWindow()->Render();
}

void ViewWidget_3D::setScalarbar()
{
	scalarbarActor_->GetTitleTextProperty()->SetColor(0.1, 0.1, 0.1);
	scalarbarActor_->GetTitleTextProperty()->SetFontSize(3);
	scalarbarActor_->GetTitleTextProperty()->ItalicOff();
	// scalarbarActor_->GetTitleTextProperty()->BoldOff();
	scalarbarActor_->SetNumberOfLabels(5); //设置5个标签
	scalarbarActor_->SetHeight(0.70);
	scalarbarActor_->SetWidth(0.10);

	scalarbarActor_->SetLookupTable(lut_);

	renderer_->AddActor2D(scalarbarActor_);
}

void ViewWidget_3D::setLookupTable()
{
	lut_->SetNumberOfColors(256);
	lut_->SetHueRange(0.7, 0);       // 颜色色调
	lut_->SetAlphaRange(1.0, 1.0);   // 颜色饱和度
	lut_->SetValueRange(1.0, 1.0);
	lut_->SetSaturationRange(0.8, 0.8);  // 颜色亮度
	lut_->Build();
}

void ViewWidget_3D::setScalarRange(double min, double max)
{
	maxScalar_ = maxScalar_ > max ? maxScalar_ : max;
	minScalar_ = minScalar_ < min ? minScalar_ : min;

	lut_->SetRange(minScalar_, maxScalar_);
	lut_->Build();
}

void ViewWidget_3D::setColorNum(int num)
{
	colorNum_ = colorNum_ > num ? colorNum_ : num;

	lut_->SetNumberOfTableValues(colorNum_);
	lut_->Build();
}

void ViewWidget_3D::removeScalarBar()
{
	if (scalarbarActor_ != nullptr)
	{
		renderer_->RemoveActor2D(scalarbarActor_);
		scalarbarActor_->Delete();
		scalarbarActor_ = nullptr;
	};
}

void ViewWidget_3D::removeModel(ViewModel_3D* viewModel)
{
	renderer_->RemoveActor(viewModel->actor());
	modelHash_.remove(viewModel);
	emit viewModel->removedFromView();

	/*if (viewModel->modelType() == ViewModelType::SOLID)
		emit solidModelRemoved((SolidModel*)viewModel);
	else if (viewModel->modelType() == ViewModelType::POINTS
		|| viewModel->modelType() == ViewModelType::TOOLPATH)
		emit pointsModelRemoved((PointsModel*)viewModel);*/

	this->GetRenderWindow()->Render();
}

void ViewWidget_3D::removePickedActor()
{
	if (pickedActor_ != nullptr)
		removeModel(modelHash_.key(pickedActor_));
		
	this->GetRenderWindow()->Render();
}

void ViewWidget_3D::clear()
{
	for (auto viewModel : modelHash_.keys())
		removeModel(viewModel);
	modelHash_.clear();

	this->GetRenderWindow()->Render();
}

void ViewWidget_3D::dragEnterEvent(QDragEnterEvent* ev)
{
	if (ev->mimeData()->hasUrls())
		ev->acceptProposedAction();
}

void ViewWidget_3D::dropEvent(QDropEvent* ev)
{
	if (ev->mimeData()->hasUrls())
	{
		emit fileDropped(ev->mimeData()->urls());
		for (auto url : ev->mimeData()->urls())
			if (url.toString().endsWith(".stl"))
				this->loadStlFile(url.toString());
			else if (url.toString().endsWith(".csv"))
				this->loadCsvFile(url.toString());
			else if (url.toString().endsWith(".cls"))
				this->loadClsFile(url.toString());
		ev->acceptProposedAction();
	}
}

void ViewWidget_3D::setBackground()
{
	// 设置渐变背景
	renderer_->SetBackground(0.1, 0.2, 0.4);
	renderer_->SetBackground2(1, 1, 1);
	renderer_->SetGradientBackground(1);
}

void ViewWidget_3D::setAxes()
{
	// 设置坐标轴
	axesActor_->SetTotalLength(20, 20, 20);
	axesActor_->SetTipTypeToCone();
	axesActor_->AxisLabelsOn();

	axesActor_->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	axesActor_->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
	axesActor_->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);

	axesActor_->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	axesActor_->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
	axesActor_->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 1, 0);

	axesActor_->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	axesActor_->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
	axesActor_->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 1);

	renderer_->AddActor(axesActor_);
}
