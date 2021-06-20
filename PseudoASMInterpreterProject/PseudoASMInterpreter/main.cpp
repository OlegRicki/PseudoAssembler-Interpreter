#include "Functions.h"

/*! \mainpage Интерпретатор для программ, написанных на языке псевдоассемблера
 *
 * Эта программа может интерпретировать программу, написанную на языке псевдоассемблера с заданной системой команд
 *
 * Автор: Воробьев Александр, ПрИн-266
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /// Путь к файлу с текстом ошибки входных данных
    QString inputErrorFilePath;

    /// Путь к входному файлу с пользовательским кодом программы на языке псевдоассемблера
    QString inputFilePath;

    /// Путь к выходному результирующему файлу (с состоянием регистров при успешной интерпретации, при неуспешной - с текстом ошибки)
    QString outputFilePathResult;

    /// Путь к выходному файлу с состоянием памяти RAM
    QString outputFilePathRam;

    /// Текст пользовательской программы на языке псевдоассемблера
    QStringList text;

    /// Список восьмиразрядных знаковых ячеек памяти RAM
    QList<int8_t> ram;

    /// Таблица восьмиразрядных знаковых регистров РОН
    QHash<QString, int8_t> registers;

    try {
        if      (argc == 3)
        {
            ///Инициализировать входные и выходные файлы
            inputFilePath        = argv[1];
            outputFilePathResult = QString(argv[2]) + "\\result.txt";
        }
        else if (argc == 4 && QString(argv[1]) == "-d")
        {
            ///Инициализировать входные и выходные файлы
            inputFilePath        = argv[2];
            outputFilePathResult = QString(argv[3]) + "\\result.txt";
            outputFilePathRam    = QString(argv[3]) + "\\RAM.txt";
        }
        else if (QString(argv[1]) != "-d")
        {
            /// Сообщить о том, что в качестве аргумента передан неверный ключ
            exception newException = {ERR_INPUT_ARG_KEY, NULL};
            throw     newException;
        }
        else if (argc < 3 || argc > 4)
        {
            /// Сообщить о том, что количество аргументов неверно
            exception newException = {ERR_INPUT_NUM_ARGS, NULL};
            throw     newException;
        }

        /// Считать переданные пользователем файл
        readFromTextFile(inputFilePath, &text);
        /// Инициализировать память RAM и регистры РОН
        initRam(&ram);
        initRegisters(&registers);
        /// Произвести интерпретацию программы, написанную на языке псевдоассемблера
        interpretateProgram(&text, &registers, &ram);
        /// Вывести результат интерпретации в файл result.txt по пути, заданным пользователем
        writeToFile(outputFilePathResult, registersToText(registers, NUM_OF_REGISTERS));
        /// Если пользователь задал ключ "-d"
        if (argv[1] == QString("-d"))
        {
            /// Вывести состояние памяти RAM в файл RAM.txt по пути, заданным пользователем
            writeToFile(outputFilePathRam, ramToText(ram));
        }
    }

    /// Поймать кастомное исключение
    catch(exception exception)
    {     
        handleCustomException(exception, a.applicationDirPath() + "\\inputError.txt", outputFilePathResult);
    }
    /// Поймать исключение о переполнении
    catch(std::overflow_error &exception)
    {
        writeToFile(outputFilePathResult, QStringList("Overflow error."));
    }

    return 0;
}
