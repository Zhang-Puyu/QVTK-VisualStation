#pragma once
#include "QVTKOpenGLWidget.h"

#include "vtkSmartPointer.h"
#include "vtkActor.h"
#include "vtkMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h" 
#include "vtkAxesActor.h"
#include "vtkScalarBarActor.h"
#include "vtkTextProperty.h"

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

#include "qurl.h"
#include "qaction.h"
#include "qtreeview.h"

#include "PointsModel.h"
#include "SolidModel.h"
#include "InteractorStyle.h"

#include "visuallibrary_global.h"

class VISUALLIBRARY_EXPORT ViewWidget_3D : public QVTKOpenGLWidget
{
	Q_OBJECT

public:
	ViewWidget_3D(QWidget* parent);
	~ViewWidget_3D();

	void loadFile(QString filePath);

	void loadStlFile(QString filePath);
	void loadCsvFile(QString filePath);
	void loadClsFile(QString filePath);

	void addModel(ViewModel_3D* viewModel);

	void setScalarRange(double min, double max);
	void setColorNum(int num);

	void removeScalarBar();
	void removeModel(ViewModel_3D* viewModel);
	void removePickedActor();

	void clear();

protected:

	//virtual void contextMenuEvent(QContextMenuEvent* event) override;

	virtual void dragEnterEvent(QDragEnterEvent* ev) override;
	virtual void dropEvent(QDropEvent* ev) override;

	void setScalarbar();
	void setLookupTable();
	void setBackground();
	void setAxes();

private:

	vsp<vtkRenderer>	   renderer_	   = vsp<vtkRenderer>::New();
	vsp<vtkAxesActor>      axesActor_	   = vsp<vtkAxesActor>::New();
	vsp<vtkScalarBarActor> scalarbarActor_ = vsp<vtkScalarBarActor>::New();
	vsp<vtkLookupTable>	   lut_			   = vsp<vtkLookupTable>::New();

	vsp<vtkActor> pickedActor_ = nullptr;
	QVector<double> pickedPosition_;
	QHash<ViewModel_3D*, vsp<vtkActor>> modelHash_;

	double minScalar_ = 0.0;
	double maxScalar_ = 0.0;
	int colorNum_ = 0;

signals:
	void fileDropped(QList<QUrl> urls);

	void pointsModelAdded(PointsModel* pointsModel);
	void pointsModelRemoved(PointsModel* pointsModel);
	void solidModelAdded(SolidModel* solidModel);
	void solidModelRemoved(SolidModel* solidModel);

	void actorPicked(vsp<vtkActor> actor);
	void positionPicked(QVector<double> position);
};
