#include <QtTest>
#include "../PseudoASMInterpreter/Functions.h"

// add necessary includes here

class Test : public QObject
{
    Q_OBJECT

public:
    Test();
    ~Test();

private slots:
    // Тестирование функции ramToText
    void ramToText_typical_test();
    void ramToText_negative_value_in_cell();
    void ramToText_eightBit_positive_value();
    void ramToText_pos_and_neg_values_in_cells();

    // Тестирование функции registersToText
    void registersToText_typical_test();
    void registersToText_nine_registers();
    void registersToText_negative_value();
    void registersToText_trash_in_hashtable();
    void registersToText_not_fully_initialized();

    // Тестирование функции parseAndValidateText
    void parseAndValidateText_MOVR_binary_const();
    void parseAndValidateText_MOVR_dec_const();
    void parseAndValidateText_MOVR_hex_const();
    void parseAndValidateText_MOVR_register();
    void parseAndValidateText_MOVR_ram();
    void parseAndValidateText_MOVA_binary_const();
    void parseAndValidateText_MOVA_dec_const();
    void parseAndValidateText_MOVA_hex_const();
    void parseAndValidateText_MOVA_register();
    void parseAndValidateText_JMP();
    void parseAndValidateText_LABEL();
    void parseAndValidateText_COMMENT();
    void parseAndValidateText_empty_string();
    void parseAndValidateText_comment_string();
    void parseAndValidateText_few_spaceSymbol_between_ags();
    void parseAndValidateText_different_symbols_in_comment();
    void parseAndValidateText_TwoRegistersArgs_command();

    // Тестирование функции interpretateProgram
    void interpretateProgram_MOVR_binary_const();
    void interpretateProgram_MOVR_dec_const();
    void interpretateProgram_MOVR_hex_const();
    void interpretateProgram_MOVR_register();
    void interpretateProgram_MOVR_ram();
    void interpretateProgram_MOVA_binary_const();
    void interpretateProgram_MOVA_dec_const();
    void interpretateProgram_MOVA_hex_const();
    void interpretateProgram_MOVA_register();
    void interpretateProgram_ADD();
    void interpretateProgram_SUB();
    void interpretateProgram_AND();
    void interpretateProgram_OR();
    void interpretateProgram_NEG();
    void interpretateProgram_ROL();
    void interpretateProgram_ROR();
    void interpretateProgram_JMP();
    void interpretateProgram_SBRC_succsessfully();
    void interpretateProgram_SBRC_unsuccsessfully();
    void interpretateProgram_SBRS_succsessfully();
    void interpretateProgram_SBRS_unsuccsessfully();
    void interpretateProgram_EQU_succsessfully();
    void interpretateProgram_EQU_unsuccsessfully();
};

Test::Test()
{

}

Test::~Test()
{

}

//************************************* ramToText ***********************************************************

void Test::ramToText_typical_test()
{
    QList<int8_t> inputList;
    initRam(&inputList);
    inputList[1] = 1;

    QStringList expectedValue = {"0x00 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00"};

    QVERIFY2(ramToText(inputList) == expectedValue, "Error in ramToText function");
}

void Test::ramToText_negative_value_in_cell()
{
    QList<int8_t> inputList;
    initRam(&inputList);
    inputList[1] = -1;

    QStringList expectedValue = {"0x00 0xFF 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00"};

    QVERIFY2(ramToText(inputList) == expectedValue, "Error in ramToText function");
}

void Test::ramToText_eightBit_positive_value()
{
    QList<int8_t> inputList;
    initRam(&inputList);
    inputList[1] = 127;

    QStringList expectedValue = {"0x00 0x7F 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00"};

    QVERIFY2(ramToText(inputList) == expectedValue, "Error in ramToText function");
}

void Test::ramToText_pos_and_neg_values_in_cells()
{
    QList<int8_t> inputList;
    initRam(&inputList);
    inputList[1] = -1;
    inputList[2] = 3;

    QStringList expectedValue = {"0x00 0xFF 0x03 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00",
                               "0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00"};

    QVERIFY2(ramToText(inputList) == expectedValue, "Error in ramToText function");
}

//************************************* registersToText ***********************************************************

void Test::registersToText_typical_test()
{
    QHash<QString, int8_t> inputRegisters;
    initRegisters(&inputRegisters);
    inputRegisters["R0"] = 2;
    QStringList expectedText = {"R0 = 2",
                               "R1 = 0",
                               "R2 = 0",
                               "R3 = 0",
                               "R4 = 0",
                               "R5 = 0",
                               "R6 = 0",
                               "R7 = 0"};

    QVERIFY2(registersToText(inputRegisters, 8) == expectedText, "Error in registersToText function");
}
void Test::registersToText_nine_registers()
{
    QHash<QString, int8_t> inputRegisters;
    initRegisters(&inputRegisters);
    inputRegisters["R0"] = 2;
    inputRegisters.insert("R8", 0);
    QStringList expectedText = {"R0 = 2",
                               "R1 = 0",
                               "R2 = 0",
                               "R3 = 0",
                               "R4 = 0",
                               "R5 = 0",
                               "R6 = 0",
                               "R7 = 0",
                               "R8 = 0"};

    QVERIFY2(registersToText(inputRegisters, 9) == expectedText, "Error in registersToText function");
}
void Test::registersToText_negative_value()
{
    QHash<QString, int8_t> inputRegisters;
    initRegisters(&inputRegisters);
    inputRegisters["R0"] = -2;
    QStringList expectedText = {"R0 = -2",
                               "R1 = 0",
                               "R2 = 0",
                               "R3 = 0",
                               "R4 = 0",
                               "R5 = 0",
                               "R6 = 0",
                               "R7 = 0"};

    QVERIFY2(registersToText(inputRegisters, 8) == expectedText, "Error in registersToText function");
}
void Test::registersToText_trash_in_hashtable()
{
    QHash<QString, int8_t> inputRegisters;
    initRegisters(&inputRegisters);
    inputRegisters["R0"] = 2;
    inputRegisters.insert("R8", 0);
    QStringList expectedText = {"R0 = 2",
                               "R1 = 0",
                               "R2 = 0",
                               "R3 = 0",
                               "R4 = 0",
                               "R5 = 0",
                               "R6 = 0",
                               "R7 = 0"};

    QVERIFY2(registersToText(inputRegisters, 8) == expectedText, "Error in registersToText function");
}
void Test::registersToText_not_fully_initialized()
{
    QHash<QString, int8_t> inputRegisters;
    inputRegisters.insert("R0", 2);
    inputRegisters.insert("R1", 0);
    QStringList expectedText = {"R0 = 2",
                               "R1 = 0",
                               "R2 = 0",
                               "R3 = 0",
                               "R4 = 0",
                               "R5 = 0",
                               "R6 = 0",
                               "R7 = 0"};

    QVERIFY2(registersToText(inputRegisters, 8) == expectedText, "Error in registersToText function");
}

//************************************* parseAndValidateText ***********************************************************

void Test::parseAndValidateText_MOVR_binary_const()
{
    QStringList inputText = {"MOVR R0, 0b1"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVR R0, 0b1", "MOVR", "R0", "0b1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_MOVR_dec_const()
{
    QStringList inputText = {"MOVR R0, 0d1"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVR R0, 0d1", "MOVR", "R0", "0d1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_MOVR_hex_const()
{
    QStringList inputText = {"MOVR R0, 0x1"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVR R0, 0x1", "MOVR", "R0", "0x1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_MOVR_register()
{
    QStringList inputText = {"MOVR R0, R0"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVR R0, R0", "MOVR", "R0", "R0"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_MOVR_ram()
{
    QStringList inputText = {"MOVR R0, 0"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVR R0, 0", "MOVR", "R0", "0"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_MOVA_binary_const()
{
    QStringList inputText = {"MOVA 0, 0b1"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVA 0, 0b1", "MOVA", "0", "0b1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_MOVA_dec_const()
{
    QStringList inputText = {"MOVA 0, 0d1"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVA 0, 0d1", "MOVA", "0", "0d1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_MOVA_hex_const()
{
    QStringList inputText = {"MOVA 0, 0x1"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVA 0, 0x1", "MOVA", "0", "0x1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_MOVA_register()
{
    QStringList inputText = {"MOVA 0, R0"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVA 0, R0", "MOVA", "0", "R0"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_JMP()
{
    QStringList inputText = {"JMP ABC"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"JMP ABC", "JMP", "ABC"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_LABEL()
{
    QStringList inputText = {"ABC:",
                            "MOVR R0, 0x1"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    expectedLabels.insert("ABC", -1);
    QList<QStringList> expectedCapturedTexts = {{"MOVR R0, 0x1", "MOVR", "R0", "0x1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_COMMENT()
{
    QStringList inputText = {"MOVR R0, 0x1 ;comment"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVR R0, 0x1 ;", "MOVR", "R0", "0x1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_empty_string()
{
    QStringList inputText = {"MOVR R0, 0x1",
                             "   ",
                            "MOVR R1, 0x1"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVR R0, 0x1", "MOVR", "R0", "0x1"},
                                               {"MOVR R1, 0x1", "MOVR", "R1", "0x1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_comment_string()
{
    QStringList inputText = {";comment"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts;

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        QStringList tmp = inputExecCommands[i].capturedTexts();
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }
    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_few_spaceSymbol_between_ags()
{
    QStringList inputText = {"MOVR R0   ,    0x1"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVR R0   ,    0x1", "MOVR", "R0", "0x1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_different_symbols_in_comment()
{
    QStringList inputText = {"MOVR R0, 0x1 ; 123 абв abc @ !"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"MOVR R0, 0x1 ;", "MOVR", "R0", "0x1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

void Test::parseAndValidateText_TwoRegistersArgs_command()
{
    QStringList inputText = {"ADD R0, R1"};
    QHash<QString, int> inputLabels;
    QList<QRegularExpressionMatch> inputExecCommands;

    QHash<QString, int> expectedLabels;
    QList<QStringList> expectedCapturedTexts = {{"ADD R0, R1", "ADD", "R0", "R1"}};

    parseAndValidateText(&inputText, &inputExecCommands, &inputLabels);

    bool isValidCapturedTexts = false;
    int i = 0;
    while (i < expectedCapturedTexts.size() && !isValidCapturedTexts)
    {
        isValidCapturedTexts = expectedCapturedTexts[i] == inputExecCommands[i].capturedTexts();
        i++;
    }

    if (isValidCapturedTexts)
    {
        isValidCapturedTexts = isValidCapturedTexts && inputExecCommands.size() == expectedCapturedTexts.size();
    }
    else
    {
        isValidCapturedTexts = inputExecCommands.size() == expectedCapturedTexts.size();
    }
    QVERIFY2(inputLabels == expectedLabels && isValidCapturedTexts == true, "Error in parseAndValidate function");
}

//************************************* interpretateProgram ***********************************************************

void Test::interpretateProgram_MOVR_binary_const()
{
    QStringList inputText = {"MOVR R0, 0b1010"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 10;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_MOVR_dec_const()
{
    QStringList inputText = {"MOVR R0, 0d10"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 10;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_MOVR_hex_const()
{
    QStringList inputText = {"MOVR R0, 0xA"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 10;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_MOVR_register()
{
    QStringList inputText = {"MOVR R0, 0xA",
                            "MOVR R1, R0"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 10;
    expectedRegisters["R1"] = 10;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_MOVR_ram()
{
    QStringList inputText = {"MOVA 0, 0xA",
                            "MOVR R0, 0"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 10;
    expectedRam[0] = 10;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_MOVA_binary_const()
{
    QStringList inputText = {"MOVA 0, 0b1010"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRam[0] = 10;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_MOVA_dec_const()
{
    QStringList inputText = {"MOVA 0, 0d10"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRam[0] = 10;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_MOVA_hex_const()
{
    QStringList inputText = {"MOVA 0, 0xA"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRam[0] = 10;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_MOVA_register()
{
    QStringList inputText = {"MOVR R0, 0xA",
                            "MOVA 0, R0"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 10;
    expectedRam[0] = 10;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_ADD()
{
    QStringList inputText = {"MOVR R0, 0b01",
                            "MOVR R1, 0b10",
                            "ADD R0, R1"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 3;
    expectedRegisters["R1"] = 2;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_SUB()
{
    QStringList inputText = {"MOVR R0, 0b01",
                            "MOVR R1, 0b10",
                            "SUB R0, R1"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = -1;
    expectedRegisters["R1"] = 2;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_AND()
{
    QStringList inputText = {"MOVR R0, 0b01",
                            "MOVR R1, 0b10",
                            "AND R1, R0"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_OR()
{
    QStringList inputText = {"MOVR R0, 0b01",
                            "MOVR R1, 0b10",
                            "OR R1, R0"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    expectedRegisters["R1"] = 3;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_NEG()
{
    QStringList inputText = {"MOVR R0, 0b01",
                            "NEG R0"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = -2;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_ROL()
{
    QStringList inputText = {"MOVR R0, 0b01",
                            "MOVR R1, 0b10",
                            "ROL R1, R0"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    expectedRegisters["R1"] = 4;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_ROR()
{
    QStringList inputText = {"MOVR R0, 0b01",
                            "MOVR R1, 0b10",
                            "ROR R1, R0"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    expectedRegisters["R1"] = 1;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_JMP()
{
    QStringList inputText = {"MOVR R0, 0b01",
                             "JMP LABEL",
                             "MOVR R1, 0b10",
                             "MOVR R2, 0b10",
                             "LABEL:"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_SBRC_succsessfully()
{
    QStringList inputText = {"MOVR R0, 0b01",
                             "MOVR R1, 0b10",
                             "SBRC R0, R1",
                             "MOVR R2, 0b1",
                             "MOVR R3, 0b1"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    expectedRegisters["R1"] = 2;
    expectedRegisters["R2"] = 0;
    expectedRegisters["R3"] = 1;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_SBRC_unsuccsessfully()
{
    QStringList inputText = {"MOVR R0, 0b01",
                             "MOVR R1, 0b10",
                             "SBRC R1, R0",
                             "MOVR R2, 0b1",
                             "MOVR R3, 0b1"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    expectedRegisters["R1"] = 2;
    expectedRegisters["R2"] = 1;
    expectedRegisters["R3"] = 1;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_SBRS_succsessfully()
{
    QStringList inputText = {"MOVR R0, 0b01",
                             "MOVR R1, 0b10",
                             "SBRS R1, R0",
                             "MOVR R2, 0b1",
                             "MOVR R3, 0b1"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    expectedRegisters["R1"] = 2;
    expectedRegisters["R2"] = 0;
    expectedRegisters["R3"] = 1;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_SBRS_unsuccsessfully()
{
    QStringList inputText = {"MOVR R0, 0b01",
                             "MOVR R1, 0b10",
                             "SBRS R0, R1",
                             "MOVR R2, 0b1",
                             "MOVR R3, 0b1"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    expectedRegisters["R1"] = 2;
    expectedRegisters["R2"] = 1;
    expectedRegisters["R3"] = 1;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_EQU_succsessfully()
{
    QStringList inputText = {"MOVR R0, 0b01",
                             "MOVR R1, 0b01",
                             "EQU R0, R1",
                             "MOVR R2, 0b1",
                             "MOVR R3, 0b1"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    expectedRegisters["R1"] = 1;
    expectedRegisters["R2"] = 0;
    expectedRegisters["R3"] = 1;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}

void Test::interpretateProgram_EQU_unsuccsessfully()
{
    QStringList inputText = {"MOVR R0, 0b01",
                             "MOVR R1, 0b11",
                             "EQU R0, R1",
                             "MOVR R2, 0b1",
                             "MOVR R3, 0b1"};
    QHash<QString, int8_t> inputRegisters;
    QList<int8_t> inputRam;
    initRegisters(&inputRegisters);
    initRam(&inputRam);
    QHash<QString, int8_t> expectedRegisters;
    QList<int8_t> expectedRam;
    initRegisters(&expectedRegisters);
    initRam(&expectedRam);

    interpretateProgram(&inputText, &inputRegisters, &inputRam);

    expectedRegisters["R0"] = 1;
    expectedRegisters["R1"] = 3;
    expectedRegisters["R2"] = 1;
    expectedRegisters["R3"] = 1;
    QVERIFY2(inputRam == expectedRam && inputRegisters == expectedRegisters, "Error in interpretateProgram function");
}
//************************************* interpretateProgram ***********************************************************
QTEST_APPLESS_MAIN(Test)

#include "tst_test.moc"
