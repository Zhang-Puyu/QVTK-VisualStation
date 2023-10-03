#pragma once

#include "ViewModel_3D.h"

#include "vtkLookupTable.h"

#include "qmenu.h"
#include "qstandarditemmodel.h"

#include "visuallibrary_global.h"

class VISUALLIBRARY_EXPORT PointsModel : public ViewModel_3D
{
	Q_OBJECT

public:
	PointsModel(QObject* parent = nullptr);
	~PointsModel();

	virtual void readFile(QString filePath) override;
	void readCsvFile(QString filePath);
	void writeCsvFile(QString filePath);

	void viewUncolourful();
	void viewColourful(QString featureName);
	void viewColourful(unsigned int featureIndex);
	void viewColourful();

	void setPointsSize(float size)		 { pointsSize_ = size; };
	void setLut(vsp<vtkLookupTable> lut) { lut_ = lut; }

	QMenu*		   buildFeatureMenu();
	QStandardItem* buildFeatureTreeItem();

	virtual ViewModelType modelType() override { return ViewModelType::POINTS;}

	QStringList   featureNames() { return featureNames_; }
	unsigned int  featureCount() { return featureNames_.size(); }
	int			  pointsCount()	 { return matrix_.size(); }
	
protected:
	QStringList featureNames_;

	vsp<vtkLookupTable> lut_	= vsp<vtkLookupTable>::New();

	int lastColouredFeatureIndex_ = -1;
	float pointsSize_ = 3;

signals:
	void colouredFeatureName(QString featureName);
	void colouredScalarRange(double min, double max);
	void colouredPointsCount(int num);

	void colored();
};

