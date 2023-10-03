#pragma once
#include "PointsModel.h"
#include "visuallibrary_global.h"

struct VISUALLIBRARY_EXPORT Tool
{
	QString name = "undefine";
	float D;  // Ö±¾¶
	float R1; // ÏÂ°ë¾¶
	float L;  // ³¤¶È
	float A;  // ¼â½Ç
	float B;  // ×¶½Ç
	QString toString()
	{
		QString str;
		str += "name: " + name + "\n";
		str += "D: " + QString::number(D) + "\n";
		str += "R1: " + QString::number(R1) + "\n";
		str += "L: " + QString::number(L) + "\n";
		str += "A: " + QString::number(A) + "\n";
		str += "B: " + QString::number(B) + "\n";
		return str;
	}
};

class VISUALLIBRARY_EXPORT ToolpathModel : public PointsModel
{
	Q_OBJECT;

public:
	ToolpathModel(QObject* parent = nullptr);
	~ToolpathModel();

	void readFile(QString filePath) override;
	void readClsFile(QString filePath);
	void readMpfFile(QString filePath);

	QString toGCodeString(int spSpeed, bool bCool = true, bool bSoft = true,
		int pcsCoor = 3, int pcsFeed = 0);

	virtual ViewModelType modelType() override { return ViewModelType::TOOLPATH; }

	Tool tool() { return tool_; }

private:
	Tool tool_;

	static const unsigned int F = 3;

signals:
	void clsRead();

	void findSecondToolpathInClsFile();
};

float cutDecimal(float f, int n = 3);