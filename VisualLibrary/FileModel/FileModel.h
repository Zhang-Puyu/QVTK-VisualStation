#pragma once

#include "visuallibrary_global.h"

void readCsv(QString filePath);
void readCls(QString filePath);
void readMpf(QString filePath);

void writeCsv(QString filePath, const Matrix& matrix);
void writeCls(QString filePath, const Matrix& matrix);
void writeMpf(QString filePath, const Matrix& matrix);