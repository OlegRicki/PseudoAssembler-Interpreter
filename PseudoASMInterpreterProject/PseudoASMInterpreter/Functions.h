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


/// Код ошибки: неверно заданных ключ (отличный от "-d")
#define ERR_INPUT_ARG_KEY 1

/// Код ошибки: программе передано недопустимое количество аргументов
#define ERR_INPUT_NUM_ARGS 7

/// Код ошибки: невозможно открыть файл для чтения
#define ERR_OPENING_FILE_READ 2

/// Код ошибки: невозможно открыть файл для записи
#define ERR_OPENING_FILE_WRITE 4

/// Код ошибки: передана пустая программа
#define ERR_PROG_EMPTY_PROG 3

/// Код ошибки: в тексте программы обнаружена синтаксическая ошибка
#define ERR_PROG_SYNTAX 5

/// Код ошибки: в тексте программы осуществлена попытка перехода по несуществующей метке
#define ERR_PROG_INVALID_LABEL 6

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

/*!  Конвертирует аргумент, представляющий собой константное значение, значение из регистра или значение из памяти RAM в целое число
 * \param arg - аргумент из текста программы в виде строки, который необходимо конвертировать в целое число
 * \param registers - состояния регистров РОН
 * \param ram - список целых чисел - ячеек памяти RAM
 * \return - целое число
 */
int8_t convertArgFromStr(QString arg, const QHash<QString, int8_t> * registers, const QList<int8_t> * ram);

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

/*!  Обрабатывает исключения
 * \param[in] exception - структура, содержащая код ошибки и номер строки с ошибкой
 * \param[in] inputErrorFilePath - путь к файлу, в который должно выводиться сообщение о некорректности входных данных
 * \param[in] ProgramErrorFilePath - путь к файлу, в который должно выводиться сообщение об ошибке в программе
 */
void handleCustomException(exception exception, QString inputErrorFilePath, QString ProgramErrorFilePath);

#endif // FUNCTIONS_H
