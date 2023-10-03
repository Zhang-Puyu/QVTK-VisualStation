#pragma once
#include "vtkSTLReader.h"
#include "vtkActor.h"

#include "ViewModel_3D.h"

#include "visuallibrary_global.h"

class VISUALLIBRARY_EXPORT SolidModel : public ViewModel_3D
{
	Q_OBJECT

public:
	SolidModel(QObject* parent = nullptr);
	~SolidModel();

	virtual void readFile(QString filePath) override;
	void readStlFile(QString filePath);

	virtual ViewModelType modelType() override { return ViewModelType::SOLID; }

};
