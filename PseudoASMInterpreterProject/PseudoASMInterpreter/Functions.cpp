#include "Functions.h"

void readFromTextFile(const QString filePath, QStringList * text)
{
    QFile file(filePath);
    /// Если файл имеет расширение .txt и он успешно открылся для чтения
    if (file.open(QIODevice::ReadOnly) && filePath.endsWith(".txt"))
    {
        /// Пока не считаем все данные файла
        while (!file.atEnd())
        {
            /// Записать строку из файла
            *text << file.readLine();
        }

        /// Если файл был пустой
        if (text->size() == 0)
        {
            /// Выдать сообщение о том, что файл пустой
            exception newException = {3, NULL};
            throw newException;
        }
        /// Иначе
        else
        {
            /// Убрать из каждой строки текста ненужные служебные символы
            QMutableListIterator<QString> text_iterator(*text);
            while (text_iterator.hasNext() && text_iterator.peekNext().contains("\r\n"))
            {
                text_iterator.next().chop(2);
            }
        }
    }
    /// Иначе
    else
    {
        /// Выдать сообщение о том, что возникли проблемы при открытии файла для чтения
        exception newException = {ERR_OPENING_FILE_READ, NULL};
        throw newException;
    }

    /// Закрыть файл
    file.close();
}

void writeToFile(const QString filePath, const QStringList * text)
{
    QFile file(filePath);
    /// Если файл успешно открылся для записи
    if (file.open(QIODevice::WriteOnly))
    {
        QListIterator<QString> textIterator(*text);
        /// Записать данные из text в файл
        while (textIterator.hasNext())
        {
            file.write((textIterator.next() + "\n").toLocal8Bit());
        }
    }
    /// Иначе
    else
    {
        /// Выдать сообщение о том, что возникли проблемы при открытии файла для записи
        exception newException = {ERR_OPENING_FILE_WRITE, NULL};
        throw newException;
    }

    /// Закрыть файл
    file.close();
}

void interpretateProgram(const QStringList * text, QHash<QString, int8_t> * registers, QList<int8_t> * ram)
{
    /// Создать копии памяти и регистров
    QHash<QString, int8_t> newRegisters = *registers;
    QList<int8_t> newRam = *ram;

    /// Создать список исполняемых команд
    QList<QRegularExpressionMatch> execCommands;
    /// Таблицу меток, где ключ - метка, значение - индекс предшествующей метке исполняемой команды в списке исполняемых команд
    QHash<QString, int> labels;
    ///Распарсить текст программы, выделить оттуда исполняемые команды и метки
    parseAndValidateText(text, &execCommands, &labels);

    int numOfCommands = execCommands.size();
    /// Для каждой исполняемоей команды
    for (int i = 0; i < numOfCommands; ++i)
    {
        /// Выделить название команды
        QString curCommand = execCommands[i].captured(1);
        /// Если текущая команда - сложение
        if (curCommand == "ADD")
        {
            /// Произвести сложение
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] + newRegisters[execCommands[i].captured(3)];
        }
        /// Если текущая команда - вычитание
        else if (curCommand == "SUB")
        {
            /// Произвести вычитание
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] - newRegisters[execCommands[i].captured(3)];
        }
        /// Если текущая команда - побитовое логическое умножение
        else if (curCommand == "AND")
        {
            /// Произвести побитовое логическое умножение
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] & newRegisters[execCommands[i].captured(3)];
        }
        /// Если текущая команда - побитовое логическое сложение
        else if (curCommand == "OR")
        {
            /// Произвести побитовое логическое сложение
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] | newRegisters[execCommands[i].captured(3)];
        }
        /// Если текущая команда - побитовое отрицание (инверсия)
        else if (curCommand == "NEG")
        {
            /// Произвести побитовое отрицание
            newRegisters[execCommands[i].captured(2)] = ~newRegisters[execCommands[i].captured(2)];
        }
        /// Если текущая команда - битовый сдвиг влево
        else if (curCommand == "ROL")
        {
            /// Произвести битовый сдвиг влево
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] << newRegisters[execCommands[i].captured(3)];
        }
        /// Если текущая команда - битовый сдвиг вправо
        else if (curCommand == "ROR")
        {
            /// Произвести битовый сдвиг вправо
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] >> newRegisters[execCommands[i].captured(3)];
        }
        /// Если текущая команда - помещение значения в ячейку памяти RAM по адресу
        else if (curCommand == "MOVA")
        {
            newRam[execCommands[i].captured(2).toInt()] = convertArgFromStr(execCommands[i].captured(3), &newRegisters, &newRam);
        }
        /// Если текущая команда - помещения значения в регистр РОН
        else if (curCommand == "MOVR")
        {
            newRegisters[execCommands[i].captured(2)] = convertArgFromStr(execCommands[i].captured(3), &newRegisters, &newRam);
        }
        /// Если текущая команда - прыжок по метке
        else if (curCommand == "JMP")
        {
            exception newException = {ERR_PROG_INVALID_LABEL, NULL};
            /// Если данная метка существует, произвести прыжок
            labels.contains(execCommands[i].captured(2))    ?   i = labels[execCommands[i].captured(2)]
            /// Иначе выдать сообщение об ошибке
                                                            :   throw newException;
        }
        /// Если текущая команда - условный прыжок через следующую команду при условии, что бит сброшен
        else if (curCommand == "SBRC" && !(newRegisters[execCommands[i].captured(2)] & (1 << newRegisters[execCommands[i].captured(3)])))
        {
            /// Перепрыгнуть через следующую команду
            i++;
        }
        /// Если текущая команда - условный прыжок через следующую команду при условии, что бит установлен
        else if (curCommand == "SBRS" &&   newRegisters[execCommands[i].captured(2)] & (1 << newRegisters[execCommands[i].captured(3)]))
        {
            /// Перепрыгнуть через следующую команду
            i++;
        }
        /// Если текущая команда - условный прыжок через следующую команду при условии, что два значения равны
        else if (curCommand == "EQU"  &&   newRegisters[execCommands[i].captured(2)]    ==   newRegisters[execCommands[i].captured(3)])
        {
            /// Перепрыгнуть через следующую команду
            i++;
        }
    }

    /// Сохранить содержимое копий памяти и регистров
    *registers = newRegisters;
    *ram = newRam;
}

int8_t convertArgFromStr(QString arg, const QHash<QString, int8_t> * registers, const QList<int8_t> * ram)
{
    int8_t result;
    /// Если помещается значение из регистра
    if (arg.contains("R"))
    {
        /// Поместить значение из регистра
        result = registers->value(arg);
    }
    /// Если помещается константное значение, записанное в двоичной системе счисления
    else if (arg.contains("b"))
    {
        /// Поместить константное значение
        result = arg.remove(0, 2).toInt(nullptr, 2);
    }
    /// Если помещается константное значение, записанное в десятичной системе счисления
    else if (arg.contains("d"))
    {
        /// Поместить константное значение
        result = arg.remove(0, 2).toInt(nullptr, 10);
    }
    /// Если помещается константное значение, записанное в шестнадцатиричной системе счисления
    else if (arg.contains("x"))
    {
        /// Поместить константное значение
        result = arg.remove(0, 2).toInt(nullptr, 16);
    }
    /// Если помещается значение из ячейки памяти по заданному адресу
    else
    {
        /// Поместить значение из ячейки памяти по адресу
        result = ram->value(arg.toInt());
    }

    /// Вернуть ковертированный в целое число результат
    return result;
}

void parseAndValidateText(const QStringList * text, QList<QRegularExpressionMatch> * execCommands, QHash<QString, int> * labels)
{
    /// Создадим кастомное исключение об ошибке, чтобы использовать при необходимости
    exception newException;
    /// Текущая строка
    QString curString;
    /// Регулярные выражения
    QList<QRegularExpression> regularExpressions =
        {QRegularExpression("^\\h*(ADD|OR|AND|SUB|EQU|ROL|ROR|SBRS|SBRC)\\h+(R[0-7])\\h*,\\h*(R[0-7])\\h*(?:;|$)"),
         QRegularExpression("^\\h*(NEG)\\h+(R[0-7])\\h*(?:;|$)"),
         QRegularExpression("^\\h*(MOVA)\\h+((?:[0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]))\\h*,\\h*(R[0-7]|0(?:d\\d+|b[01]+|x[\\dA-F]+))\\h*(?:;|$)"),
         QRegularExpression("^\\h*(MOVR)\\h+(R[0-7])\\h*,\\h*(R[0-7]|0(?:d\\d+|b[01]+|x[\\dA-F]+)|\\d+)\\h*(?:;|$)"),
         QRegularExpression("^\\h*(JMP)\\h+([A-Za-z\\d]+)(?:;|$)"),
         QRegularExpression("^\\h*([a-zA-Z\\d]+):\\h*(?:;|$)"),
         QRegularExpression("^\\h*(?:;|$)")};
    /// Выделенная группа при сходстве строки с регулярным выражением
    QRegularExpressionMatch newMatch;
    /// Количество строк текста
    int cnt = 0;
    QListIterator<QString> textIterator(*text);
    /// Пока не рассмотрим все строки текста
    while (textIterator.hasNext())
    {
        /// Добавить рассматривающуюся строку в счетчик
        cnt++;

        /// Проверить строку на соответствие с регулярным выражением для команд ADD OR AND SUB EQU ROL ROR SBRS SBRC
        newMatch = regularExpressions[0].match(textIterator.peekNext());
        /// Если соответствует
        if (newMatch.hasMatch())
        {
            /// Добавить выделенную из строки сполняемую команду в список
            execCommands->append(newMatch);
            /// Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        /// Проверить строку на соответствие с регулярным выражением для команды NEG
        newMatch = regularExpressions[1].match(textIterator.peekNext());
        /// Если соответствует
        if (newMatch.hasMatch())
        {
            /// Добавить выделенную из строки сполняемую команду в список
            execCommands->append(newMatch);
            /// Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        /// Проверить строку на соответствие с регулярным выражением для команды MOVA
        newMatch = regularExpressions[2].match(textIterator.peekNext());
        /// Если соответствует
        if (newMatch.hasMatch())
        {
            /// Добавить выделенную из строки сполняемую команду в список
            execCommands->append(newMatch);
            /// Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        /// Проверить строку на соответствие с регулярным выражением для команды MOVR
        newMatch = regularExpressions[3].match(textIterator.peekNext());
        /// Если соответствует
        if (newMatch.hasMatch())
        {
            /// Добавить выделенную из строки сполняемую команду в список
            execCommands->append(newMatch);
            /// Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        /// Проверить строку на соответствие с регулярным выражением для команды JMP
        newMatch = regularExpressions[4].match(textIterator.peekNext());
        /// Если соответствует
        if (newMatch.hasMatch())
        {
            /// Добавить выделенную из строки сполняемую команду в список
            execCommands->append(newMatch);
            /// Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        /// Проверить строку на соответствие с регулярным выражением для метки
        newMatch = regularExpressions[5].match(textIterator.peekNext());
        /// Если соответствует
        if (newMatch.hasMatch() && !labels->contains(newMatch.captured(1)))
        {
            /// Добавить метку в качестве ключа в таблицу со значением - индексом команды, после которой стоит метка
            labels->insert(newMatch.captured(1), execCommands->count() - 1);
            /// Закончить парсинг текущей строки
            goto END_STRING_PARSE;
        }

        /// Проверить строку на соответствие с регулярным выражением для пустой строки
        newMatch = regularExpressions[6].match(textIterator.peekNext());
        /// Если соответствует
        if (newMatch.hasMatch())
        {
            /// Закончить парсинг строки, не производя никаких действий с данной строкой
            goto END_STRING_PARSE;
        }

        /// Если строка не прошла валидацию ни по одному регулярному выражению, выдать сообщение о синтаксической ошибке
        newException = {ERR_PROG_SYNTAX, cnt};
        throw newException;

END_STRING_PARSE:
        /// Перейти к следующей строке
        textIterator.next();
    }
}

void registersToText(const QHash<QString, int8_t> * registers, const int numOfRegisters, QStringList * registersText)
{
    /// Новый ключ для таблицы registers
    QString newKey;
    /// Для каждого регистра РОН
    for (int i = 0; i < numOfRegisters; ++i)
    {
        /// Сгенерировать строку "Rn = *value*"
        newKey = "R" + QString::number(i);
        registersText->append(newKey + " = " + QString::number(registers->value(newKey)));
    }

}

void ramToText(const QList<int8_t> * ram, QStringList * ramText)
{
    /// Новая сгенерированная строка
    QString newString;
    /// Количество рассмотренных ячеек
    int cnt = 0;
    ///
    QString curValueStr;
    QListIterator<int8_t> ramIterator(*ram);
    /// Пока не рассмотрели все ячейки памяти RAM
    while (ramIterator.hasNext())
    {
        /// Конвертировать рассматриваемую ячейку в строку
        curValueStr = QString::number(ramIterator.next(), 16).toUpper();
        /// Если в рассматриваемой ячейке отрицательное значение
        if (curValueStr.size() > 2)
        {
            /// Удалить лишние символы
            curValueStr.remove(0, curValueStr.size() - 2);
        }
        /// Если в рассматриваемой ячейке значение, не превышающее 4 бит
        else if (curValueStr.size() < 2)
        {
            /// Добавить в начало 0
            curValueStr.insert(0, "0");
        }
        /// Добавить обозначение шестнадцатиричной системы счисления
        curValueStr.insert(0, "0x");
        /// Добавить к строке значение из рассматриваемой ячейки в 16-ной системе счисления
        newString += curValueStr;
        /// Считать, что ячейку рассмотрели, инкрементировать счетчик
        cnt++;
        /// Если ячейка последняя в строке
        if (cnt % 16 == 0)
        {
            /// Добавить получившуюся строку в текст
            ramText->append(newString);
            /// Отчистить текущую строку
            newString.clear();
        }
        /// Иначе
        else
        {
            /// Добавить пробел
            newString += " ";
        }
    }
}

void initRam(QList<int8_t> * ram)
{
    /// Для каждой ячейки памяти
    for (int i = 0; i < NUM_OF_RAM_CELLS; ++i)
    {
        /// Заполнить 0
        ram->append(0);
    }
}

void initRegisters(QHash<QString, int8_t> * registers)
{
    /// Для каждого регистра РОН
    for (int i = 0; i < NUM_OF_REGISTERS; ++i)
    {
        /// Добавить по ключу Rn значение 0
        QString curKey = "R" + QString::number(i);
        registers->insert(curKey, 0);
    }
}

void handleCustomException(const exception * exception, const QString * inputErrorFilePath, const QString * ProgramErrorFilePath)
{
    if      (exception->errCode == ERR_INPUT_ARG_KEY)
    {
        QStringList errMsg = QStringList("On the command line, an option other than “-d” is passed as an argument.");
        writeToFile(*inputErrorFilePath, &errMsg);
    }
    else if (exception->errCode == ERR_OPENING_FILE_READ)
    {
        QStringList errMsg = QStringList("Error opening file for reading.");
        writeToFile(*inputErrorFilePath, &errMsg);
    }
    else if (exception->errCode == ERR_PROG_EMPTY_PROG)
    {
        QStringList errMsg = QStringList("The text of the program in which the search should be performed was not found.");
        writeToFile(*ProgramErrorFilePath, &errMsg);
    }
    else if(exception->errCode ==  ERR_OPENING_FILE_WRITE)
    {
        QStringList errMsg = QStringList("Error opening file for writing.");
        writeToFile(*inputErrorFilePath, &errMsg);
    }
    else if (exception->errCode == ERR_PROG_SYNTAX)
    {
        QStringList errMsg = QStringList("Syntax error. Line number: " + QString::number(exception->numOfStr));
        writeToFile(*ProgramErrorFilePath, &errMsg);
    }
    else if (exception->errCode == ERR_PROG_INVALID_LABEL)
    {
        QStringList errMsg = QStringList("An attempt was made to navigate to a non-existent label.");
        writeToFile(*ProgramErrorFilePath, &errMsg);
    }
    else if (exception->errCode == ERR_INPUT_NUM_ARGS)
    {
        QStringList errMsg = QStringList("Invalid number of input arguments.");
        writeToFile(*inputErrorFilePath, &errMsg);
    }
}
