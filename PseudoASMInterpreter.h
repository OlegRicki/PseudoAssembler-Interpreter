#ifndef PSEUDOASMINTERPRETER_H
#define PSEUDOASMINTERPRETER_H

#include <QString>
#include <QHash>
#include <QList>
#include <QRegularExpression>
#include <QFile>
#include <stdint.h>

struct exception
{
    int errCode;
    int numOfStr;
};

QString inputFilePath;


QString outputFilePathResult;


QString outputFilePathRam;


QStringList text;


QList<int8_t> ram;


QHash<QString, int8_t> registers;


void readFromFile(QString filePath, QStringList text);


void writeToFile(const QString filePath, const QStringList text);


void parseAndValidateText(QStringList text, QList<QRegularExpressionMatch> execCommands, QHash<QString, int> labels);


void interpretateProgram(QStringList text, QHash<QString, int8_t> registers, QList<int8_t> ram);


QStringList registersToText(const QHash<QString, int8_t> registers);


QStringList ramToText(const QList<int8_t> ram);


#endif // PSEUDOASMINTERPRETER_H
