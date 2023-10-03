#pragma once

#include <QtCore/qglobal.h>

//#ifndef BUILD_STATIC
//# if defined(VISUALLIBRARY_LIB)
//#  define VISUALLIBRARY_EXPORT Q_DECL_EXPORT
//# else
//#  define VISUALLIBRARY_EXPORT Q_DECL_IMPORT
//# endif
//#else
//# define VISUALLIBRARY_EXPORT
//#endif

#ifndef VISUALLIBRARY_EXPORT
#define VISUALLIBRARY_EXPORT
#endif // !# define VISUALLIBRARY_EXPORT


#include "vtkSmartPointer.h"
#include "vtkActor.h"
#include "vtkMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "qvector.h"
#include "qstring.h"
#include "qdebug.h"

typedef QVector<QVector<float>> Matrix;
template<typename T> using vsp = vtkSmartPointer<T>;

enum class ViewModelType
{
	TOOLPATH, POINTS, SOLID, SCATTER
};