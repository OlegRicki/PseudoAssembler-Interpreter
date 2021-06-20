#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QString>
#include <QHash>
#include <QList>
#include <QRegularExpression>
#include <QFile>
#include <QCoreApplication>
#include <stdint.h>

/// Количество регистров РОН
#define NUM_OF_REGISTERS 8

/// Количество ячеек памяти RAM
#define NUM_OF_RAM_CELLS 256

/// Кастомное исключение
struct exception
{
    /// Код ошибки
    int errCode;
    /// Номер строки с ошибкой, если ошибка в пользовательском коде на языке псевдоассемблера
    int numOfStr;
};

/*!  Читает содержимое текстового файла, лежащего в filePath, и записывает считанное в список text
  \param[in] filePath - полный путь к файлу
  \param[out] text - список строк, в который помещается содержимое файла
*/
void readFromTextFile(QString filePath, QStringList * text);

/*!  Записывает содержимое списка строк text в файл, лежащего в filePath
  \param[in] filePath - полный путь к файлу
  \param[out] text - список строк, записываемый в файл
*/
void writeToFile(const QString filePath, const QStringList text);

/*!  Сверяет строки текста программы с регулярными выражениями и разбивает их на лексемы (название команды и аргументы, если они имеются).
    Метки, пустые строки и комментарии отбрасываются. При наличии несовпадений выдает сообщение о наличии синтаксической ошибки.
  \param[in] text - текст программы
  \param[in|out] execCommands - список исполняемых команд с аргументами
  \param[in|out] labels - хэш-таблица, где ключ – название метки, а значение – номер в списке исполняемых команд исполняемой команды, стоящей в тексте программы перед меткой
*/
void parseAndValidateText(QStringList * text, QList<QRegularExpressionMatch> * execCommands, QHash<QString, int> * labels);


/*!  Интерпретирует текст программы text, написанной на языке псевдоассемблера, и возвращает
    состояние регистров registers R0, R1, .., Rn и состояние памяти, измененные в ходе интерпретации
  \param[in] text - текст программы
  \param[in|out] registers - состояния регистров РОН
  \param[in|out] ram - список целых чисел - ячеек памяти RAM
*/
void interpretateProgram(QStringList * text, QHash<QString, int8_t> * registers, QList<int8_t> * ram);

/*!  Преобразование хэш-таблицу целых чисел в текст в виде R0 = registers[R0]\nR1 = registers[R1] и т.д.
    За регистры принимаются значения в registers по ключам R0, R1, ..., Rn, где n = numOfRegisters - 1
  \param[in] registers - состояния регистров РОН
  \param[in] numOfRegisters - количество регистров РОН
  \return - текст вида R0 = registers[0]\nR1 = registers[1] и т.д.
*/
QStringList registersToText(const QHash<QString, int8_t> registers, int numOfRegisters);

/*!  Преобразовывает вектор целых чисел в текст - матрицу целых чисел в шестнадцатиричной системе счисления
  \param[in] ram - список целых чисел - ячеек памяти RAM
  \returns - текст - матрица целых чисел в шестнадцатиричной системе счисления
*/
QStringList ramToText(const QList<int8_t> ram);

/*!  Инициализирует память RAM нулями. Количество инициализированных ячеек регулируется макросом NUM_OF_RAM_CELLS
  \param[in|out] ram - список целых чисел - ячеек памяти RAM
*/
void initRam(QList<int8_t> * ram);

/*!  Инициализирует регистры РОН нулями.Количество инициализированных регистров регулируется макросом NUM_OF_REGISTERS
  \param[in|out] registers - состояния регистров РОН
*/
void initRegisters(QHash<QString, int8_t> * registers);

#endif // FUNCTIONS_H
