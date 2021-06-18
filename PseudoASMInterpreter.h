#ifndef PSEUDOASMINTERPRETER_H
#define PSEUDOASMINTERPRETER_H

#include <QString>
#include <QHash>
#include <QList>
#include <QRegularExpression>
#include <QFile>
#include <QCoreApplication>
#include <stdint.h>

// Количество регистров РОН
#define NUM_OF_REGISTERS 8

// Количество ячеек памяти RAM
#define NUM_OF_RAM_CELLS 256

// Кастомное исключение
struct exception
{
    // Код ошибки
    int errCode;
    // Номер строки с ошибкой, если ошибка в пользовательском коде на языке псевдоассемблера
    int numOfStr;
};

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

/*  Читает содержимое текстового файла, лежащего в filePath, и записывает считанное в список text
  * filePath - полный путь к файлу
  * text - список строк, в который помещается содержимое файла
*/
void readFromTextFile(QString filePath, QStringList * text);

/*  Записывает содержимое списка строк text в файл, лежащего в filePath
  * filePath - полный путь к файлу
  * text - список строк, записываемый в файл
*/
void writeToFile(const QString filePath, const QStringList text);

/*  Сверяет строки текста программы с регулярными выражениями и разбивает их на лексемы (название команды и аргументы, если они имеются).
    Метки, пустые строки и комментарии отбрасываются. При наличии несовпадений выдает сообщение о наличии синтаксической ошибки.
  * text - текст программы
  * execCommands - список исполняемых команд с аргументами
  * labels - хэш-таблица, где ключ – название метки, а значение – номер в списке исполняемых команд исполняемой команды, стоящей в тексте программы перед меткой
*/
void parseAndValidateText(QStringList * text, QList<QRegularExpressionMatch> * execCommands, QHash<QString, int> * labels);


/*  Интерпретирует текст программы text, написанной на языке псевдоассемблера, и возвращает
    состояние регистров registers R0, R1, .., Rn и состояние памяти, измененные в ходе интерпретации
  * text - текст программы
  * registers - состояния регистров РОН
  * ram - список целых чисел - ячеек памяти RAM
*/
void interpretateProgram(QStringList * text, QHash<QString, int8_t> * registers, QList<int8_t> * ram);

/*  Преобразование хэш-таблицу целых чисел в текст в виде R0 = registers[0]\nR1 = registers[1] и т.д.
  * registers - состояния регистров РОН
  * return - текст вида R0 = registers[0]\nR1 = registers[1] и т.д.
*/
QStringList registersToText(const QHash<QString, int8_t> registers);

/*  Преобразовывает вектор целых чисел в текст - матрицу целых чисел в шестнадцатиричной системе счисления
  * ram - список целых чисел - ячеек памяти RAM
  * returns - текст - матрица целых чисел в шестнадцатиричной системе счисления
*/
QStringList ramToText(const QList<int8_t> ram);

/*  Инициализирует память RAM нулями
  * ram - список целых чисел - ячеек памяти RAM
*/
void initRam(QList<int8_t> * ram);

/*  Инициализирует регистры РОН нулями
  * registers - состояния регистров РОН
*/
void initRegisters(QHash<QString, int8_t> * registers);

#endif // PSEUDOASMINTERPRETER_H
