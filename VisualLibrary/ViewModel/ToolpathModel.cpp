
#include "ToolpathModel.h"

#include "qfile.h"
#include "qfileinfo.h"

ToolpathModel::ToolpathModel(QObject* parent)
	: PointsModel(parent)
{
}

ToolpathModel::~ToolpathModel()
{
}

void ToolpathModel::readFile(QString filePath)
{
	if (filePath.isEmpty()) return;
	else if (filePath.endsWith(".csv")) readCsvFile(filePath);
	else if (filePath.endsWith(".cls")) readClsFile(filePath);
}

void ToolpathModel::readClsFile(QString filePath)
{
	// read file
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Open failed.";
		return;
	}
	qDebug() << "import apt file: " << filePath;
	QTextStream in(&file);
	QString line;

	// first line
	line = in.readLine();
	if (line.contains("TOOL PATH/"))
	{
		tool_.name = line.split(",").last();
		name_ = line.split(",").first().remove("TOOL PATH/");
	}
	else
	{
		QFileInfo fileInfo(filePath);
		name_ = fileInfo.baseName();
	}

	// second line
	line = in.readLine();
	if (line.contains("TLDATA/MILL,"))
	{
		line = line.remove("TLDATA/MILL,");
		tool_.D = line.split(",").at(0).toFloat();
		tool_.R1 = line.split(",").at(1).toFloat();
		tool_.L = line.split(",").at(2).toFloat();
		tool_.B = line.split(",").at(3).toFloat();
		tool_.A = line.split(",").at(4).toFloat();
	};

	// third line
	line = in.readLine();

	featureNames_.clear();
	featureNames_ << "X" << "Y" << "Z" << "feed rate";

	// centerline data
	float feedRate = 0;
	matrix_.clear();
	while (!line.isNull())
	{
		line = in.readLine();
		if (line.contains("GOTO/"))
		{
			line.remove(("GOTO/"));
			matrix_.push_back(QVector<float>{
				line.split(",").at(X).toFloat(),
					line.split(",").at(Y).toFloat(),
					line.split(",").at(Z).toFloat(),
					feedRate});
			continue;
		}
		if (line.contains("TOOL PATH/"))
		{
			emit findSecondToolpathInClsFile();
			break;
		}
		if (line.contains("FEDRAT/MMPM"))
		{
			feedRate = line.split(",").last().toFloat();
			continue;
		}
		if (line.contains("FEDRAT/"))
		{
			feedRate = line.split("/").last().toFloat();
			continue;
		}
	}

	file.close();

	this->viewUncolourful();
}

void ToolpathModel::readMpfFile(QString filePath)
{
	// read file
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Open failed.";
		return;
	}
	qDebug() << "import mpf file: " << filePath;

	QFileInfo info(filePath);
	name_ = info.baseName();

	QTextStream in(&file);
	QString line;

	matrix_.clear();
	featureNames_.clear();
	featureNames_ << "X" << "Y" << "Z" << "feed rate" << "N";
	float x = 0, y = 0, z = 0, feedrate = 0, n = 0;
	while (!line.isNull())
	{
		line = in.readLine();
		QStringList strs = line.split(" ");
		for (QString str : strs)
		{
			if (str.contains("X"))
				x = str.remove("X").toFloat();
			else if (str.contains("Y"))
				y = str.remove("Y").toFloat();
			else if (str.contains("Z"))
				z = str.remove("Z").toFloat();
			else if (str.contains("F"))
				feedrate = str.remove("F").toFloat();
			else if (str.contains("N"))
				n = str.remove("N").toFloat();
		};
		matrix_.push_back(QVector<float>{x, y, z, feedrate, n});
	}

	this->viewUncolourful();
}

QString ToolpathModel::toGCodeString(int spSpeed, bool bCool, bool bSoft, int pcsCoor, int pcsFeed)
{
	QString gCodeStr;

	int n = 0;
	// first line
	gCodeStr += "N" + QString::number(n)
		+ " G90"	// 绝对坐标系
		+ " G54"	// 指定工件坐标系
		+ " G40\n"; // 取消刀具半径补偿

	n++;
	// second line
	gCodeStr += "N" + QString::number(n)
		+ " M3"				// 主轴正转
		+ " S" + QString::number(spSpeed) // 主轴转速
		+ "\n";

	n++;
	// third line
	if (bCool)
		gCodeStr += "N" + QString::number(n) + " " + "M8\n";  // 冷却液开

	if (bSoft)
	{
		gCodeStr += "FFWON\n";
		gCodeStr += "SOFT\n";
		gCodeStr += "G641 ADIS=0.1\n";
	}

	// first point
	n++;
	auto it = matrix_.begin();
	gCodeStr += "N" + QString::number(n) + " G01"
		+ " X" + QString::number(it->at(X))
		+ " Y" + QString::number(it->at(Y))
		+ " Z" + QString::number(it->at(Z))
		+ " F" + QString::number(it->at(F));

	it++;
	float feedRate = it->at(F);
	float x = it->at(X);
	float y = it->at(Y);
	float z = it->at(Z);
	while (it != matrix_.end())
	{
		if (x == it->at(X) && y == it->at(Y) && z == it->at(Z) 
			&& feedRate == it->at(F))
		{
			it++;
			continue;
		}
		n++;
		gCodeStr += "\nN" + QString::number(n);
		if (x != it->at(X))
		{
			gCodeStr += " X" +
				QString::number(cutDecimal(it->at(X), pcsCoor));
			x = it->at(X);
		}
		if (y != it->at(Y))
		{
			gCodeStr += " Y" +
				QString::number(cutDecimal(it->at(Y), pcsCoor));
			y = it->at(Y);
		}
		if (z != it->at(Z))
		{
			gCodeStr += " Z" +
				QString::number(cutDecimal(it->at(Z), pcsCoor));
			z = it->at(Z);
		}
		if (feedRate != it->at(F))
		{
			gCodeStr += " F" +
				QString::number(cutDecimal(it->at(F), pcsFeed));
			feedRate = it->at(F);
		}
		it++;
	}
	// last line
	n++;
	gCodeStr += "N" + QString::number(n);
	if (bCool) gCodeStr += " M9";    // 冷却液关
	gCodeStr += " M30";	   // 程序结束
	return gCodeStr;
}

float cutDecimal(float f, int n)
{
	if (n < 0)
		return f;
	if (n == 0)
		return round(f);

	int a = pow(10, n);
	return round(f * a) / float(a);
}