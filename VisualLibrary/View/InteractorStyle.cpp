
#include "InteractorStyle.h"

#include <vtkSphere.h>
#include <vtkSphereSource.h>
#include <vtkRendererCollection.h> 
#include <vtkObjectFactory.h>  
#include <vtkProperty.h>
#include <vtkPointPicker.h>
#include <vtkPropPicker.h>
#include <vtkCellPicker.h>
#include <vtkPolyDataMapper.h>

#ifndef VTKSTANDARDNEWMACRO_POINTPICKERINTERACTORSTYLE
#define VTKSTANDARDNEWMACRO_POINTPICKERINTERACTORSTYLE
vtkStandardNewMacro(FreePointPickerInteractorStyle);
#endif 

void FreePointPickerInteractorStyle::OnLeftButtonDown()
{
	//打印鼠标左键像素位置
	qDebug() << "Picking pixel: " << this->Interactor->GetEventPosition()[0]
		<< " " << this->Interactor->GetEventPosition()[1];
	//注册拾取点函数
	this->Interactor->GetPicker()->Pick(
		this->Interactor->GetEventPosition()[0],
		this->Interactor->GetEventPosition()[1], 0,  // always zero.
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()
	);
	//打印拾取点空间位置
	double picked[3];
	this->Interactor->GetPicker()->GetPickPosition(picked);
	qDebug() << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2];
	//对拾取点进行标记
	vsp<vtkSphereSource> sphereSource = vsp<vtkSphereSource>::New();
	sphereSource->Update();

	vsp<vtkPolyDataMapper> mapper = vsp<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());

	vsp<vtkActor> actor = vsp<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->SetPosition(picked);
	//actor->SetScale(0.05);
	actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
	actor->GetProperty()->SetPointSize(15);
	this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);

	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}


#ifndef VTKSTANDARDNEWMACRO_PROPPICKERINTERACTORSTYLE
#define VTKSTANDARDNEWMACRO_PROPPICKERINTERACTORSTYLE
vtkStandardNewMacro(PropPickerInteractorStyle);
#endif

void PropPickerInteractorStyle::OnLeftButtonDown()
{
	int* clickPos = this->GetInteractor()->GetEventPosition();

    // Pick from this location.
    vsp<vtkPropPicker> picker = vsp<vtkPropPicker>::New();
    picker->Pick(clickPos[0], clickPos[1], 0, 
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

    double* pos = picker->GetPickPosition();
	QVector<double> positionVector = { pos[0], pos[1], pos[2] };
	pickedPropSignalSender->sendPickedPostion(positionVector);

    // If we picked something before, reset its property
    if (this->lastPickedActor_)
    {
        this->lastPickedActor_->GetProperty()->DeepCopy(this->lastPickedProperty_);
    }

    this->lastPickedActor_ = picker->GetActor();

	pickedPropSignalSender->sendPickedProp(picker->GetActor());

    if (this->lastPickedActor_)
    {
        // Save the property of the picked actor so that we can restore it next time
        this->lastPickedProperty_->DeepCopy(this->lastPickedActor_->GetProperty());
        // Highlight the picked actor by changing its properties
        this->lastPickedActor_->GetProperty()->SetColor(0.8, 0.0, 0.0);
        this->lastPickedActor_->GetProperty()->SetDiffuse(1.0);
        this->lastPickedActor_->GetProperty()->SetSpecular(0.0);
    }

    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}


#ifndef VTKSTANDARDNEWMACRO_CELLPICKERINTERACTORSTYLE
#define VTKSTANDARDNEWMACRO_CELLPICKERINTERACTORSTYLE
vtkStandardNewMacro(CellPickerInteractorStyle);
#endif

void CellPickerInteractorStyle::OnLeftButtonDown()
{
	auto clickPos = this->GetInteractor()->GetEventPosition();
	//auto picker = this->GetInteractor()->GetPicker();
	vsp<vtkCellPicker> picker = vsp<vtkCellPicker>::New();

	vsp<vtkRenderer> render = 
		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();

	picker->Pick(clickPos[0], clickPos[1], 0, render);

	double* pointPosition;

	if (picker->GetCellId() != -1 && mesh_ != nullptr)
	{
		picker->GetPickPosition();
		picker->GetCellId();
		picker->GetPointId();

		pointPosition = mesh_->GetPoint(picker->GetPointId());

		vsp<vtkSphereSource> sphereSource = vsp<vtkSphereSource>::New();
		sphereSource->SetCenter(pointPosition);
		sphereSource->SetRadius(0.2);

		// Create a mapper and actor
		vsp<vtkPolyDataMapper> mapper = vsp<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(sphereSource->GetOutputPort());

		vsp<vtkActor> actor = vsp<vtkActor>::New();
		actor->SetMapper(mapper);
		actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
		actor->GetProperty()->SetPointSize(3);
	}
	// Forward events
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

PickedPropSignalSender::PickedPropSignalSender(QObject* parent)
	: QObject(parent)
{

}