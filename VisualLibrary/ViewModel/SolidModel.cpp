
#include "SolidModel.h"

#include "vtkPolyDataMapper.h"

SolidModel::SolidModel(QObject *parent)
	: ViewModel_3D(parent)
{}

SolidModel::~SolidModel()
{}

void SolidModel::readFile(QString filePath)
{
	this->readStlFile(filePath);
}

void SolidModel::readStlFile(QString filePath)
{
	if (filePath.isEmpty()) return;

	vsp<vtkSTLReader> stlReader = vsp<vtkSTLReader>::New();
	stlReader->SetFileName(filePath.toLatin1());
	stlReader->Update();

	vsp<vtkPolyDataMapper> solidMapper = vsp<vtkPolyDataMapper>::New();
	solidMapper->SetInputConnection(stlReader->GetOutputPort());

	actor_->SetMapper(solidMapper);

	vsp<vtkPolyData> polyData = stlReader->GetOutput();
	for (int i = 0; i < polyData->GetNumberOfPoints(); i++)
	{
		double p[] = { 0, 0, 0 };
		polyData->GetPoint(i, p);   // 获取顶点坐标

		QVector<float> node = { (float)p[X], (float)p[Y], (float)p[Z] };
		matrix_.push_back(node);
	}
}
