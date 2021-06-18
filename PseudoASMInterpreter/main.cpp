#include "PseudoASMInterpreter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    try {
        if      (argc == 3)
        {
            //Инициализировать входные и выходные файлы
            inputFilePath        = argv[1];
            outputFilePathResult = QString(argv[2]) + "\\result.txt";
        }
        else if (argc == 4 && QString(argv[1]) == "-d")
        {
            //Инициализировать входные и выходные файлы
            inputFilePath        = argv[2];
            outputFilePathResult = QString(argv[3]) + "\\result.txt";
            outputFilePathRam    = QString(argv[3]) + "\\RAM.txt";
        }
        else if (QString(argv[1]) != "-d")
        {
            // Сообщить о том, что в качестве аргумента передан неверный ключ
            exception newException = {1, NULL};
            throw     newException;
        }
        else if (argc < 3 || argc > 4)
        {
            // Сообщить о том, что количество аргументов неверно
            exception newException = {7, NULL};
            throw     newException;
        }

        // Считать переданные пользователем файл
        readFromTextFile(inputFilePath, &text);
        // Инициализировать память RAM и регистры РОН
        initRam(&ram);
        initRegisters(&registers);
        // Произвести интерпретацию программы, написанную на языке псевдоассемблера
        interpretateProgram(&text, &registers, &ram);
        // Вывести результат интерпретации в файл result.txt по пути, заданным пользователем
        writeToFile(outputFilePathResult, registersToText(registers, NUM_OF_REGISTERS));
        // Если пользователь задал ключ "-d"
        if (argv[1] == QString("-d"))
        {
            // Вывести состояние памяти RAM в файл RAM.txt по пути, заданным пользователем
            writeToFile(outputFilePathRam, ramToText(ram));
        }
    }

    // Поймать кастомное исключение
    catch(exception exception)
    {
        // Определить полный путь к файлу, в который выведется сообщение об ошибке, связанной с входными аргументами
        inputErrorFilePath = a.applicationDirPath() + "\\inputError.txt";
        if      (exception.errCode == 1)
        {
            writeToFile(inputErrorFilePath, QStringList("On the command line, an option other than “-d” is passed as an argument."));
        }
        else if (exception.errCode == 2)
        {
            writeToFile(inputErrorFilePath, QStringList("Error opening file for reading."));
        }
        else if (exception.errCode == 3)
        {
            writeToFile(outputFilePathResult, QStringList("The text of the program in which the search should be performed was not found."));
        }
        else if(exception.errCode == 4)
        {
            writeToFile(inputErrorFilePath, QStringList("Error opening file for writing."));
        }
        else if (exception.errCode == 5)
        {
            writeToFile(outputFilePathResult, QStringList("Syntax error. Line number: " + QString::number(exception.numOfStr)));
        }
        else if (exception.errCode == 6)
        {
            writeToFile(outputFilePathResult, QStringList("An attempt was made to navigate to a non-existent label." ));
        }
        else if (exception.errCode == 7)
        {
            writeToFile(inputErrorFilePath, QStringList("Invalid number of input arguments."));
        }
    }
    // Поймать исключение о переполнении
    catch(std::overflow_error &exception)
    {
        writeToFile(outputFilePathResult, QStringList("Overflow error."));
    }

    return 0;
}

void readFromTextFile(const QString filePath, QStringList * text)
{
    QFile file(filePath);
    // Если файл имеет расширение .txt и он успешно открылся для чтения
    if (file.open(QIODevice::ReadOnly) && filePath.endsWith(".txt"))
    {
        // Пока не считаем все данные файла
        while (!file.atEnd())
        {
            // Записать строку из файла
            *text << file.readLine();
        }

        // Если файл был пустой
        if (text->size() == 0)
        {
            // Выдать сообщение о том, что файл пустой
            exception newException = {3, NULL};
            throw newException;
        }
        // Иначе
        else
        {
            // Убрать из каждой строки текста ненужные служебные символы
            QMutableListIterator<QString> text_iterator(*text);
            while (text_iterator.hasNext() && text_iterator.peekNext().contains("\r\n"))
            {
                text_iterator.next().chop(2);
            }
        }
    }
    // Иначе
    else
    {
        // Выдать сообщение о том, что возникли проблемы при открытии файла
        exception newException = {2, NULL};
        throw newException;
    }

    // Закрыть файл
    file.close();
}

void writeToFile(const QString filePath, const QStringList text)
{
    QFile file(filePath);
    // Если файл успешно открылся для записи
    if (file.open(QIODevice::WriteOnly))
    {
        QListIterator<QString> textIterator(text);
        // Записать данные из text в файл
        while (textIterator.hasNext())
        {
            file.write((textIterator.next() + "\n").toLocal8Bit());
        }
    }
    // Иначе
    else
    {
        // Выдать сообщение о том, что возникли проблемы при открытии файла для записи
        exception newException = {4, NULL};
        throw newException;
    }

    // Закрыть файл
    file.close();
}

void interpretateProgram(QStringList * text, QHash<QString, int8_t> * registers, QList<int8_t> * ram)
{
    // Создать копии памяти и регистров
    QHash<QString, int8_t> newRegisters = *registers;
    QList<int8_t> newRam = *ram;

    // Создать список исполняемых команд
    QList<QRegularExpressionMatch> execCommands;
    // Таблицу меток, где ключ - метка, значение - индекс предшествующей метке исполняемой команды в списке исполняемых команд
    QHash<QString, int> labels;
    //Распарсить текст программы, выделить оттуда исполняемые команды и метки
    parseAndValidateText(text, &execCommands, &labels);

    int numOfCommands = execCommands.size();
    // Для каждой исполняемоей команды
    for (int i = 0; i < numOfCommands; ++i)
    {
        // Выделить название команды
        QString curCommand = execCommands[i].captured(1);
        // Если текущая команда - сложение
        if (curCommand == "ADD")
        {
            // Произвести сложение
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] + newRegisters[execCommands[i].captured(3)];
        }
        // Если текущая команда - вычитание
        else if (curCommand == "SUB")
        {
            // Произвести вычитание
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] - newRegisters[execCommands[i].captured(3)];
        }
        // Если текущая команда - побитовое логическое умножение
        else if (curCommand == "AND")
        {
            // Произвести побитовое логическое умножение
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] & newRegisters[execCommands[i].captured(3)];
        }
        // Если текущая команда - побитовое логическое сложение
        else if (curCommand == "OR")
        {
            // Произвести побитовое логическое сложение
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] | newRegisters[execCommands[i].captured(3)];
        }
        // Если текущая команда - побитовое отрицание (инверсия)
        else if (curCommand == "NEG")
        {
            // Произвести побитовое отрицание
            newRegisters[execCommands[i].captured(2)] = ~newRegisters[execCommands[i].captured(2)];
        }
        // Если текущая команда - битовый сдвиг влево
        else if (curCommand == "ROL")
        {
            // Произвести битовый сдвиг влево
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] << newRegisters[execCommands[i].captured(3)];
        }
        // Если текущая команда - битовый сдвиг вправо
        else if (curCommand == "ROR")
        {
            // Произвести битовый сдвиг вправо
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] >> newRegisters[execCommands[i].captured(3)];
        }
        // Если текущая команда - помещение значения в ячейку памяти RAM по адресу
        else if (curCommand == "MOVA")
        {
            // Если помещается значение из регистра в ячейку по заданному адресу
            if (execCommands[i].captured(3).contains("R"))
            {
                // Поместить значение из регистра в ячейку по заданному адресу
                newRam[execCommands[i].captured(2).toInt()] = newRegisters[execCommands[i].captured(3)];
            }
            // Если в ячейку по заданному помещается константа, записанная в двоичной системе счисления
            else if (execCommands[i].captured(3).contains("b"))
            {
                // Поместить константное значение в ячейку по заданному адресу
                newRam[execCommands[i].captured(2).toInt()] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 2);
            }
            // Если в ячейку по заданному помещается константа, записанная в десятичной системе счисления
            else if (execCommands[i].captured(3).contains("d"))
            {
                // Поместить константное значение в ячейку по заданному адресу
                newRam[execCommands[i].captured(2).toInt()] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 10);
            }
            // Если в ячейку по заданному помещается константа, записанная в шестнадцатиричной системе счисления
            else if (execCommands[i].captured(3).contains("x"))
            {
                // Поместить константное значение в ячейку по заданному адресу
                newRam[execCommands[i].captured(2).toInt()] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 16);
            }
        }
        // Если текущая команда - помещения значения в регистр РОН
        else if (curCommand == "MOVR")
        {
            // Если помещается значение из регистра
            if (execCommands[i].captured(3).contains("R"))
            {
                // Поместить значение из регистра
                newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(3)];
            }
            // Если помещается константное значение, записанное в двоичной системе счисления
            else if (execCommands[i].captured(3).contains("b"))
            {
                // Поместить константное значение
                newRegisters[execCommands[i].captured(2)] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 2);
            }
            // Если помещается константное значение, записанное в десятичной системе счисления
            else if (execCommands[i].captured(3).contains("d"))
            {
                // Поместить константное значение
                newRegisters[execCommands[i].captured(2)] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 10);
            }
            // Если помещается константное значение, записанное в шестнадцатиричной системе счисления
            else if (execCommands[i].captured(3).contains("x"))
            {
                // Поместить константное значение
                newRegisters[execCommands[i].captured(2)] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 16);
            }
            // Если помещается значение из ячейки памяти по заданному адресу
            else
            {
                // Поместить значение из ячейки памяти по адресу
                newRegisters[execCommands[i].captured(2)] = newRam[execCommands[i].captured(3).toInt()];
            }
        }
        // Если текущая команда - прыжок по метке
        else if (curCommand == "JMP")
        {
            // Если заданная метка существует
            if (labels.contains(execCommands[i].captured(2)))
            {
                // Произвести прыжок
                i = labels[execCommands[i].captured(2)];
            }
            // Иначе
            else
            {
                // Выдать сообщение о том, что производится попытка совершения прыжка по несуществующей метке
                exception newException = {6, NULL};
                throw newException;
            }
        }
        // Если текущая команда - условный прыжок через следующую команду при условии, что бит сброшен
        else if (curCommand == "SBRC" && !(newRegisters[execCommands[i].captured(2)] & (1 << newRegisters[execCommands[i].captured(3)])))
        {
            // Перепрыгнуть через следующую команду
            i++;
        }
        // Если текущая команда - условный прыжок через следующую команду при условии, что бит установлен
        else if (curCommand == "SBRS" &&   newRegisters[execCommands[i].captured(2)] & (1 << newRegisters[execCommands[i].captured(3)]))
        {
            // Перепрыгнуть через следующую команду
            i++;
        }
        // Если текущая команда - условный прыжок через следующую команду при условии, что два значения равны
        else if (curCommand == "EQU"  &&   newRegisters[execCommands[i].captured(2)]    ==   newRegisters[execCommands[i].captured(3)])
        {
            // Перепрыгнуть через следующую команду
            i++;
        }
    }

    // Сохранить содержимое копий памяти и регистров
    *registers = newRegisters;
    *ram = newRam;
}

void parseAndValidateText(QStringList * text, QList<QRegularExpressionMatch> * execCommands, QHash<QString, int> * labels)
{   
    // Создадим кастомное исключение об ошибке, чтобы использовать при необходимости
    exception newException;
    // Текущая строка
    QString curString;
    // Регулярное выражение
    QRegularExpression regExp;
    // Выделенная группа при сходстве строки с регулярным выражением
    QRegularExpressionMatch newMatch;   
    // Количество строк текста
    int cnt = 0;
    QListIterator<QString> textIterator(*text);
    // Пока не рассмотрим все строки текста
    while (textIterator.hasNext())
    {
        // Рассмотреть текущую строку текста
        curString = textIterator.next();
        // Добавить рассматривающуюся строку в счетчик
        cnt++;

        // Проверить строку на соответствие с регулярным выражением для команд ADD OR AND SUB EQU ROL ROR SBRS SBRC
        regExp = QRegularExpression("^\\h*(ADD|OR|AND|SUB|EQU|ROL|ROR|SBRS|SBRC)\\h+(R[0-7])\\h*,\\h*(R[0-7])\\h*(?:;|$)");
        newMatch = regExp.match(curString);
        // Если соответствует
        if (newMatch.hasMatch())
        {
            // Добавить выделенную из строки сполняемую команду в список
            execCommands->append(newMatch);
            // Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        // Проверить строку на соответствие с регулярным выражением для команды NEG
        regExp = QRegularExpression("^\\h*(NEG)\\h+(R[0-7])\\h*(?:;|$)");
        newMatch = regExp.match(curString);
        // Если соответствует
        if (newMatch.hasMatch())
        {
            // Добавить выделенную из строки сполняемую команду в список
            execCommands->append(newMatch);
            // Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        // Проверить строку на соответствие с регулярным выражением для команды MOVA
        regExp = QRegularExpression("^\\h*(MOVA)\\h+((?:[0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]))\\h*,\\h*(R[0-7]|0(?:d\\d+|b[01]+|x[\\dA-F]+))\\h*(?:;|$)");
        newMatch = regExp.match(curString);
        // Если соответствует
        if (newMatch.hasMatch())
        {
            // Добавить выделенную из строки сполняемую команду в список
            execCommands->append(newMatch);
            // Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        // Проверить строку на соответствие с регулярным выражением для команды MOVR
        regExp = QRegularExpression("^\\h*(MOVR)\\h+(R[0-7])\\h*,\\h*(R[0-7]|0(?:d\\d+|b[01]+|x[\\dA-F]+)|\\d+)\\h*(?:;|$)");
        newMatch = regExp.match(curString);
        // Если соответствует
        if (newMatch.hasMatch())
        {
            // Добавить выделенную из строки сполняемую команду в список
            execCommands->append(newMatch);
            // Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        // Проверить строку на соответствие с регулярным выражением для команды JMP
        regExp = QRegularExpression("^\\h*(JMP)\\h+([A-Za-z\\d]+)(?:;|$)");
        newMatch = regExp.match(curString);
        // Если соответствует
        if (newMatch.hasMatch())
        {
            // Добавить выделенную из строки сполняемую команду в список
            execCommands->append(newMatch);
            // Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        // Проверить строку на соответствие с регулярным выражением для метки
        regExp = QRegularExpression("^\\h*([a-zA-Z\\d]+):\\h*(?:;|$)");
        newMatch = regExp.match(curString);
        // Если соответствует
        if (newMatch.hasMatch() && !labels->contains(newMatch.captured(1)))
        {
            // Добавить метку в качестве ключа в таблицу со значением - индексом команды, после которой стоит метка
            labels->insert(newMatch.captured(1), execCommands->count() - 1);
            // Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        // Проверить строку на соответствие с регулярным выражением для пустой строки
        regExp = QRegularExpression("^\\h*(?:;|$)");
        newMatch = regExp.match(curString);
        // Если соответствует
        if (newMatch.hasMatch())
        {
            // Закончить парсинг строки, не производя никаких действий с данной строкой
            goto END_STRING_PARSE;
        }

        // Если строка не прошла валидацию ни по одному регулярному выражению, выдать сообщение о синтаксической ошибке
        newException = {5, cnt};
        throw newException;

END_STRING_PARSE:;
    }
}

QStringList registersToText(const QHash<QString, int8_t> registers, int numOfRegisters)
{
    // Результирующий текст с состояниями регистров РОН
    QStringList registersText;
    // Новый ключ для таблицы registers
    QString newKey;
    // Для каждого регистра РОН
    for (int i = 0; i < numOfRegisters; ++i)
    {
        // Сгенерировать строку "Rn = *value*"
        newKey = "R" + QString::number(i);
        registersText.append(newKey + " = " + QString::number(registers[newKey]));
    }

    // Вернуть сгенерированный текст
    return registersText;
}

QStringList ramToText(const QList<int8_t> ram)
{
    // Текст со значениями ячеек памяти RAM
    QStringList ramText;
    // Новая сгенерированная строка
    QString newString;
    // Количество рассмотренных ячеек
    int cnt = 0;
    //
    QString curValueStr;
    QListIterator<int8_t> ramIterator(ram);
    // Пока не рассмотрели все ячейки памяти RAM
    while (ramIterator.hasNext())
    {
        // Конвертировать рассматриваемую ячейку в строку
        curValueStr = QString::number(ramIterator.next(), 16).toUpper();
        // Если в рассматриваемой ячейке отрицательное значение
        if (curValueStr.size() > 2)
        {
            // Удалить лишние символы
            curValueStr.remove(0, curValueStr.size() - 2);
        }
        // Если в рассматриваемой ячейке значение, не превышающее 4 бит
        else if (curValueStr.size() < 2)
        {
            // Добавить в начало 0
            curValueStr.insert(0, "0");
        }
        // Добавить обозначение шестнадцатиричной системы счисления
        curValueStr.insert(0, "0x");
        // Добавить к строке значение из рассматриваемой ячейки в 16-ной системе счисления
        newString += curValueStr;
        // Считать, что ячейку рассмотрели, инкрементировать счетчик
        cnt++;
        // Если ячейка последняя в строке
        if (cnt % 16 == 0)
        {
            // Добавить получившуюся строку в текст
            ramText.append(newString);
            // Отчистить текущую строку
            newString.clear();
        }
        // Иначе
        else
        {
            // Добавить пробел
            newString += " ";
        }
    }

    // Вернуть сгенерированный текст
    return ramText;
}

void initRam(QList<int8_t> * ram)
{
    // Для каждой ячейки памяти
    for (int i = 0; i < NUM_OF_RAM_CELLS; ++i)
    {
        // Заполнить 0
        ram->append(0);
    }
}

void initRegisters(QHash<QString, int8_t> * registers)
{
    // Для каждого регистра РОН
    for (int i = 0; i < NUM_OF_REGISTERS; ++i)
    {
        // Добавить по ключу Rn значение 0
        QString curKey = "R" + QString::number(i);
        registers->insert(curKey, 0);
    }
}
