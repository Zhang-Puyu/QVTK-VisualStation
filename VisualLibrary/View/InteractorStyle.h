#pragma once

#include <vtkAutoInit.h>
#ifndef VTKRENDERINGOPENGL2
#define VTKRENDERINGOPENGL2
VTK_MODULE_INIT(vtkRenderingOpenGL2)
#endif 
#ifndef VTKRENDERINGFREETYPE
#define VTKRENDERINGFREETYPE
VTK_MODULE_INIT(vtkRenderingFreeType)
#endif
#ifndef VTKINTERACTIONSTYLE
#define VTKINTERACTIONSTYLE
VTK_MODULE_INIT(vtkInteractionStyle)
#endif 

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkOrientationMarkerWidget.h>

#include <vtkPointPicker.h>
#include <vtkRenderWindowInteractor.h>

#include "visuallibrary_global.h"

class VISUALLIBRARY_EXPORT PickedPropSignalSender : public QObject
{
    Q_OBJECT

public:
    PickedPropSignalSender(QObject* parent = nullptr);
    ~PickedPropSignalSender() {}

    void sendPickedProp(vsp<vtkActor> actor) { emit actorPicked(actor); }
    void sendPickedPostion(QVector<double> position) { emit positionPicked(position); }

signals:
    void actorPicked(vsp<vtkActor> actor);
    void positionPicked(QVector<double> position);

};

class VISUALLIBRARY_EXPORT FreePointPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static FreePointPickerInteractorStyle* New();
	vtkTypeMacro(FreePointPickerInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown();
};

class VISUALLIBRARY_EXPORT PropPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static PropPickerInteractorStyle* New();
    vtkTypeMacro(PropPickerInteractorStyle, vtkInteractorStyleTrackballCamera);

	virtual void OnLeftButtonDown();

    PickedPropSignalSender* pickedPropSignalSender = new PickedPropSignalSender();

private:

    vsp<vtkActor> lastPickedActor_ = nullptr;
    vsp<vtkProperty> lastPickedProperty_ = vsp<vtkProperty>::New();
};

class VISUALLIBRARY_EXPORT CellPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static CellPickerInteractorStyle* New();
    vtkTypeMacro(CellPickerInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown();

    void setMesh(vsp<vtkPolyData> polyData) { mesh_ = polyData; };

private:
    vsp<vtkPolyData> mesh_ = nullptr;

    vsp<vtkActor> lastPickedActor_ = nullptr;
    vsp<vtkProperty> lastPickedProperty_ = vsp<vtkProperty>::New();
};

