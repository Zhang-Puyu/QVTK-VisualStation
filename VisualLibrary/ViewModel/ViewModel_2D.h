#pragma once

#include "visuallibrary_global.h"

class VISUALLIBRARY_EXPORT ViewModel_2D  : public QObject
{
	Q_OBJECT

public:
	ViewModel_2D(QObject* parent = nullptr);
	~ViewModel_2D();

	virtual void readFile(QString filePath) = 0;
	virtual void readCsvFile(QString filePath) = 0;

	virtual ViewModelType modelType() = 0;
};
