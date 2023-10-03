#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VisualStation.h"
#pragma execution_character_set("utf-8")

#include "vtkScalarBarActor.h"
#include "vtkLookupTable.h"

#include "PointsModel.h"
#include "SolidModel.h"

#include "vtkAutoInit.h"
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
#ifndef VTKRENDERINGVOLUMEOPENGL2
#define VTKRENDERINGVOLUMEOPENGL2
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
#endif 

QT_BEGIN_NAMESPACE
namespace Ui { class VisualStationClass; };
QT_END_NAMESPACE

class VisualStation : public QMainWindow
{
    Q_OBJECT

public:
    VisualStation(QWidget* parent = nullptr);
    ~VisualStation();

private:
    Ui::VisualStationClass* ui;
};
