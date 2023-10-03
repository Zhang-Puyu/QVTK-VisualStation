
#include "PointsModel.h"

#include "vtkpoints.h"
#include "vtkCellArray.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkDoubleArray.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkLookupTable.h"
#include "vtkPolyVertex.h"
#include "vtkPointData.h"

#include "qfile.h"
#include "qfileinfo.h"
#include "qurl.h"

PointsModel::PointsModel(QObject* parent)
	: ViewModel_3D(parent)
{
}

PointsModel::~PointsModel()
{
}

void PointsModel::readFile(QString filePath)
{
	if (filePath.isEmpty()) return;
	else if (filePath.endsWith(".csv")) readCsvFile(filePath);
}

void PointsModel::readCsvFile(QString filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "open file failed";
		return;
	}

	QFileInfo fileInfo(filePath);
	name_ = fileInfo.baseName();

	QTextStream in(&file);

	QString line = in.readLine();
	featureNames_ = line.split(",");

	matrix_.clear();
	line = in.readLine();
	int order = 0;
	featureNames_.push_back("points order");
	while (!line.isNull())
	{
		QVector<float> rowArray;
		for (const QString& cell : line.split(","))
			rowArray.push_back(cell.toFloat());
		rowArray.push_back(order++);

		matrix_.push_back(rowArray);
		line = in.readLine();
	}

	file.close();

	this->viewUncolourful();
}

void PointsModel::writeCsvFile(QString filePath)
{
	qDebug() << "export csv to " << filePath;
	// create a file 
	QFile file(filePath);
	file.open(QIODevice::ReadWrite | QIODevice::Text);

	// write file
	QTextStream out(&file);

	// first line
	for (auto feature : featureNames_)
		out << feature << ",";
	out << "\n";

	// first point
	for (auto point : matrix_)
	{
		for (auto value : point)
			out << value << ",";
		out << "\n";
	};

	file.close();
}

void PointsModel::viewUncolourful()
{
	lastColouredFeatureIndex_ = -1;
	
	vsp<vtkPoints>    points   = vsp<vtkPoints>::New();
	vsp<vtkCellArray> vertices = vsp<vtkCellArray>::New();	// 单元数据 顶点类型
	for (auto it = matrix_.begin(); it != matrix_.end(); it++)
	{
		vtkIdType pointId[1];
		pointId[0] = points->InsertNextPoint(it->at(X), it->at(Y), it->at(Z));
		vertices->InsertNextCell(1, pointId);
	}

	//创建vtkPolyData对象
	vsp<vtkPolyData> polyData = vsp<vtkPolyData>::New();
	polyData->SetPoints(points);
	polyData->SetVerts(vertices);

	vsp<vtkPolyDataMapper> pointsMapper = vsp<vtkPolyDataMapper>::New();
	pointsMapper->SetInputData(polyData);

	actor_->SetMapper(pointsMapper);
	actor_->GetProperty()->SetPointSize(pointsSize_);	

	emit colored();
}

void PointsModel::viewColourful(QString featureName)
{
	if (featureNames_.contains(featureName))
		return this->viewColourful(featureNames_.indexOf(featureName));
	else
		return this->viewUncolourful();
}

void PointsModel::viewColourful(unsigned int featureIndex)
{
	if (featureIndex >= featureNames_.count() || featureIndex < 0)
		return this->viewUncolourful();
	
	lastColouredFeatureIndex_ = featureIndex;

	float minScalar = std::min_element(matrix_.begin(), matrix_.end(),
		[=](QVector<float> p, QVector<float> q)->bool {return p[featureIndex] < q[featureIndex]; })
		->at(featureIndex);
	float maxScalar = std::max_element(matrix_.begin(), matrix_.end(),
		[=](QVector<float> p, QVector<float> q)->bool {return p[featureIndex] < q[featureIndex]; })
		->at(featureIndex);

	emit colouredFeatureName(featureNames_.at(featureIndex));
	emit colouredScalarRange(minScalar, maxScalar);
	emit colouredPointsCount(matrix_.count());

	// 几何数据
	vsp<vtkPoints> points = vsp<vtkPoints>::New();
	for (auto it = matrix_.begin(); it != matrix_.end(); it++)
		points->InsertNextPoint(it->at(X), it->at(Y), it->at(Z));

	// 拓扑数据
	vsp<vtkPolyVertex> polyVertex = vsp<vtkPolyVertex>::New();   // 单元：多顶点，是O维单元的组合，
	for (auto it = matrix_.begin(); it != matrix_.end(); it++)
		polyVertex->GetPointIds()->InsertNextId(polyVertex->GetNumberOfPoints());

	// 属性数据
	vsp<vtkDoubleArray> scalars = vsp<vtkDoubleArray>::New();
	for (auto it = matrix_.begin(); it != matrix_.end(); it++)
		// 第1个参数是points点的Id，第2个参数是该点的属性值
		scalars->InsertNextTuple1(it->at(featureIndex));

	// 将以上三部分数据组合成一个结构为vtkUnstructureGrid数据集
	vsp<vtkUnstructuredGrid> grid = vsp<vtkUnstructuredGrid>::New();
	grid->Allocate(1, 1);
	grid->SetPoints(points);
	grid->InsertNextCell(polyVertex->GetCellType(), polyVertex->GetPointIds()); //设置映射器
	grid->GetPointData()->SetScalars(scalars);

	vsp<vtkDataSetMapper> mapper = vsp<vtkDataSetMapper>::New();
	// 根据属性数据的最大、最小值，将颜色表和属性数据关联
	mapper->SetInputData(grid);
	mapper->ScalarVisibilityOn();
	mapper->SetScalarRange(minScalar, maxScalar);
	mapper->SetLookupTable(lut_);
	mapper->SetColorModeToDefault();

	actor_->SetMapper(mapper);
	actor_->GetProperty()->SetRepresentationToPoints();
	actor_->GetProperty()->SetPointSize(pointsSize_);

	emit colored();
}

void PointsModel::viewColourful()
{
	for (int i = 0; i < matrix_.count(); i++)
		matrix_[i].push_back(i);
	featureNames_.push_back("points order");
	viewColourful("points order");	

	emit colored();
}

QMenu* PointsModel::buildFeatureMenu()
{
	QMenu* menu = new QMenu(name());
	for (const QString& feature : featureNames())
	{
		QAction* action = new QAction(feature);
		menu->addAction(action);
	}
	connect(menu, &QMenu::triggered, [=](QAction* action) {this->viewColourful(action->text());});
	return menu;
}

QStandardItem* PointsModel::buildFeatureTreeItem()
{
	QStandardItem* item = new QStandardItem(name_);

	for (QString featureName : featureNames_)
		item->appendRow(new QStandardItem(featureName));
	
	return item;
}
