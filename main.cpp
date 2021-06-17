#include "PseudoASMInterpreter.h"

int main(int argc, char *argv[])
{
    try {
        if      (argc == 3)
        {
            inputFilePath        = argv[1];
            outputFilePathResult = QString(argv[2]) + "\\result.txt";
            outputFilePathRam    = QString(argv[2]) + "\\RAM.txt";
        }
        else if (argc == 4 && QString(argv[1]) == "-d")
        {
            inputFilePath        = argv[2];
            outputFilePathResult = QString(argv[3]) + "\\result.txt";
            outputFilePathRam    = QString(argv[3]) + "\\RAM.txt";
        }
        else
        {
            exception newException = {1, NULL};
            throw     newException;
        }

        readFromFile(inputFilePath, &text);

        initRam(&ram);
        interpretateProgram(&text, &registers, &ram);
        writeToFile(outputFilePathResult, registersToText(registers));
        if (argc == 4)
        {
            writeToFile(outputFilePathRam, ramToText(ram));
        }
    }

    catch(exception exception)
    {
        if      (exception.errCode == 1)
        {
            writeToFile(outputFilePathResult, QStringList("On the command line, an option other than “-d” is passed as an argument."));
        }
        else if (exception.errCode == 2)
        {
            writeToFile(outputFilePathResult, QStringList("Error opening file for reading."));
        }
        else if (exception.errCode == 3)
        {
            writeToFile(outputFilePathResult, QStringList("The text of the program in which the search should be performed was not found."));
        }
        else if (exception.errCode == 4)
        {
            writeToFile(outputFilePathResult, QStringList("Syntax error. Line number: " + QString::number(exception.numOfStr)));
        }
        else if (exception.errCode == 5)
        {
            writeToFile(outputFilePathResult, QStringList("An attempt was made to navigate to a non-existent label. Line number: " + QString::number(exception.numOfStr)));
        }
    }
    catch(std::overflow_error &exception)
    {
        writeToFile(outputFilePathResult, QStringList("Overflow error."));
    }

    return 0;
}

void initRam(QList<int8_t> * ram)
{
    for (int i = 0; i < 256; ++i)
    {
        ram->append(0);
    }
}

void readFromFile(const QString filePath, QStringList * text)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        while (!file.atEnd())
        {
            *text << file.readLine();
        }

        if (text->size() == 0)
        {
            exception newException = {3, NULL};
            throw newException;
        }
    }
    else
    {
        exception newException = {2, NULL};
        throw newException;
    }

    file.close();
}

void writeToFile(const QString filePath, const QStringList text)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        QListIterator<QString> textIterator(text);
        while (textIterator.hasNext())
        {
            file.write((textIterator.next() + "\n").toLocal8Bit());
        }
    }
    else
    {
        exception newException = {4, NULL};
        throw newException;
    }
}

void interpretateProgram(QStringList * text, QHash<QString, int8_t> * registers, QList<int8_t> * ram)
{
    QHash<QString, int8_t> newRegisters = *registers;
    QList<int8_t> newRam = *ram;

    QList<QRegularExpressionMatch> execCommands;
    QHash<QString, int> labels;
    parseAndValidateText(text, &execCommands, &labels);

    int numOfCommands = execCommands.size();
    for (int i = 0; i < numOfCommands; ++i)
    {
        QString curCommand = execCommands[i].captured(1);
        if (curCommand == "ADD")
        {
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] + newRegisters[execCommands[i].captured(3)];
        }
        else if (curCommand == "SUB")
        {
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] - newRegisters[execCommands[i].captured(3)];
        }
        else if (curCommand == "AND")
        {
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] & newRegisters[execCommands[i].captured(3)];
        }
        else if (curCommand == "OR")
        {
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] | newRegisters[execCommands[i].captured(3)];
        }
        else if (curCommand == "NEG")
        {
            newRegisters[execCommands[i].captured(2)] = ~newRegisters[execCommands[i].captured(2)];
        }
        else if (curCommand == "ROL")
        {
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] << newRegisters[execCommands[i].captured(3)];
        }
        else if (curCommand == "ROR")
        {
            newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(2)] >> newRegisters[execCommands[i].captured(3)];
        }
        else if (curCommand == "MOVA")
        {
            if (execCommands[i].captured(3).contains("R"))
            {
                newRam[execCommands[i].captured(2).toInt()] = newRegisters[execCommands[i].captured(3)];
            }
            else if (execCommands[i].captured(3).contains("b"))
            {
                newRam[execCommands[i].captured(2).toInt()] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 2);
            }
            else if (execCommands[i].captured(3).contains("d"))
            {
                newRam[execCommands[i].captured(2).toInt()] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 10);
            }
            else if (execCommands[i].captured(3).contains("h"))
            {
                newRam[execCommands[i].captured(2).toInt()] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 16);
            }
        }
        else if (curCommand == "MOVR")
        {
            if (execCommands[i].captured(3).contains("R"))
            {
                newRegisters[execCommands[i].captured(2)] = newRegisters[execCommands[i].captured(3)];
            }
            else if (execCommands[i].captured(3).contains("b"))
            {
                newRegisters[execCommands[i].captured(2)] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 2);
            }
            else if (execCommands[i].captured(3).contains("d"))
            {
                newRegisters[execCommands[i].captured(2)] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 10);
            }
            else if (execCommands[i].captured(3).contains("x"))
            {
                newRegisters[execCommands[i].captured(2)] = execCommands[i].captured(3).remove(0, 2).toInt(nullptr, 16);
            }
            else
            {
                newRegisters[execCommands[i].captured(2)] = newRam[execCommands[i].captured(3).toInt()];
            }

        }
        else if (curCommand == "JMP")
        {
            if (labels.contains(execCommands[i].captured(2)))
            {
                i = labels[execCommands[i].captured(2)];
            }
            else
            {
                exception newException = {5, text->indexOf(execCommands[i].captured(2))};
                throw newException;
            }
        }
        else if (curCommand == "SBRC" && !(newRegisters[execCommands[i].captured(2)] & (1 << newRegisters[execCommands[i].captured(3)])))
        {
            i++;
        }
        else if (curCommand == "SBRS" &&   newRegisters[execCommands[i].captured(2)] & (1 << newRegisters[execCommands[i].captured(3)]))
        {
            i++;
        }
        else if (curCommand == "EQU"  &&   newRegisters[execCommands[i].captured(2)]    ==   newRegisters[execCommands[i].captured(3)])
        {
            i++;
        }
    }

    *registers = newRegisters;
    *ram = newRam;
}

void parseAndValidateText(QStringList * text, QList<QRegularExpressionMatch> * execCommands, QHash<QString, int> * labels)
{   
    exception newException;
    QListIterator<QString> textIterator(*text);
    QString curString;
    QRegularExpressionMatch newMatch;
    QRegularExpression regExp;

    int cnt = 0;
    while (textIterator.hasNext())
    {
        cnt++;
        curString = textIterator.next();

        regExp = QRegularExpression("^\\h*(ADD|OR|AND|SUB|EQU|ROL|ROR|SBRS|SBRC)\\h+(R[0-7])\\h*,\\h*(R[0-7])\\h*(?:;[\\w\\d\\h]*$|$)");
        newMatch = regExp.match(curString);
        if (newMatch.hasMatch())
        {
            execCommands->append(newMatch);
            goto END_STRING_PARSE;
        }

        regExp = QRegularExpression("^\\h*(MOVA)\\h+((?:[0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5]))\\h*,\\h*(R[0-7]|0(?:x\\d+|b[01]+|d[\\dA-F]+))\\h*(?:;[\\w\\d\\s]*$|$)");
        newMatch = regExp.match(curString);
        if (newMatch.hasMatch())
        {
            execCommands->append(newMatch);
            goto END_STRING_PARSE;
        }

        regExp = QRegularExpression("^\\h*(MOVR)\\h+(R[0-7])\\h*,\\h*(R[0-7]|0(?:d\\d+|b[01]+|x[\\dA-F]+)|\\d+)\\h*(?:;[\\w\\d\\s]*$|$)");
        newMatch = regExp.match(curString);
        if (newMatch.hasMatch())
        {
            execCommands->append(newMatch);
            goto END_STRING_PARSE;
        }

        regExp = QRegularExpression("^\\h*(JMP)\\h+([A-Za-z\\d]+)(?:;[\\w\\d\\s]*$|$)");
        newMatch = regExp.match(curString);
        if (newMatch.hasMatch())
        {
            execCommands->append(newMatch);
            goto END_STRING_PARSE;
        }

        regExp = QRegularExpression("^\\h*([a-zA-Z\\d]+):\\h*(?:;[\\w\\d\\s]*$|$)");
        newMatch = regExp.match(curString);
        if (newMatch.hasMatch() && !labels->contains(newMatch.captured(1)))
        {
            labels->insert(newMatch.captured(1), execCommands->count() - 1);
            goto END_STRING_PARSE;
        }

        regExp = QRegularExpression("^\\h*(?:;[\\w\\d\\s]*$|$)");
        newMatch = regExp.match(curString);
        if (newMatch.hasMatch())
        {
            goto END_STRING_PARSE;
        }

        newException = {4, cnt};
        throw newException;

END_STRING_PARSE:;
    }
}

QStringList registersToText(const QHash<QString, int8_t> registers)
{
    QStringList registersText;
    for (int i = 0; i < 8; ++i)
    {
        QString curKey = "R" + QString::number(i);
        registersText.append(curKey + " = " + QString::number(registers[curKey]));
    }

    return registersText;
}

QStringList ramToText(const QList<int8_t> ram)
{
    QStringList ramText;
    int cnt = 0;
    QListIterator<int8_t> ramIterator(ram);
    QString newString;
    while (ramIterator.hasNext())
    {
        newString += QString::number(ramIterator.next(), 16);
        cnt++;
        if (cnt % 16 == 0)
        {
            ramText.append(newString);
            newString.clear();
        }
        else
        {
            newString += " ";
        }
    }

    return ramText;
}




