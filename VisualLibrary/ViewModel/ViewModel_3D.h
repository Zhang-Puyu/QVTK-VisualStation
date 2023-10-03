#pragma once

#include "visuallibrary_global.h"

class VISUALLIBRARY_EXPORT ViewModel_3D : public QObject
{
	Q_OBJECT

public:
	ViewModel_3D(QObject* parent = nullptr);
	~ViewModel_3D();

	virtual void readFile(QString filePath) = 0;
	virtual ViewModelType modelType() = 0;

	virtual bool isEmpty() { return matrix_.isEmpty(); }
	virtual vsp<vtkActor> actor() { return actor_; }
	virtual QString name() { return name_; }
	virtual Matrix matrix() { return matrix_; }
	
protected:
	vsp<vtkActor> actor_ = vsp<vtkActor>::New();
	QString name_ = "Undefine";
	Matrix matrix_;

	static const unsigned int X = 0;
	static const unsigned int Y = 1;
	static const unsigned int Z = 2;

signals:
	void addedToView();
	void removedFromView();
};
