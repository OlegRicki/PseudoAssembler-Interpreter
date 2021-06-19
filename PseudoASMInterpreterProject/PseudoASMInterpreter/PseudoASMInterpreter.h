#ifndef PSEUDOASMINTERPRETER_H
#define PSEUDOASMINTERPRETER_H

#include <QString>
#include <QHash>
#include <QList>
#include <QRegularExpression>
#include <QFile>
#include <QCoreApplication>
#include <stdint.h>

// Путь к файлу с текстом ошибки входных данных
QString inputErrorFilePath;

// Путь к входному файлу с пользовательским кодом программы на языке псевдоассемблера
QString inputFilePath;

// Путь к выходному результирующему файлу (с состоянием регистров при успешной интерпретации, при неуспешной - с текстом ошибки)
QString outputFilePathResult;

// Путь к выходному файлу с состоянием памяти RAM
QString outputFilePathRam;

// Текст пользовательской программы на языке псевдоассемблера
QStringList text;

// Список восьмиразрядных знаковых ячеек памяти RAM
QList<int8_t> ram;

// Таблица восьмиразрядных знаковых регистров РОН
QHash<QString, int8_t> registers;


#endif // PSEUDOASMINTERPRETER_H
