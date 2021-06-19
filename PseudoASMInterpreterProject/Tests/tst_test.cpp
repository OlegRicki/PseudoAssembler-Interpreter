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
    void test_case1();

};

Test::Test()
{

}

Test::~Test()
{

}

void Test::test_case1()
{
    QList<int8_t> inputList;
    QList<int8_t> expectedList;
    initRam(&inputList);
    initRam(&expectedList);
    QVERIFY2(ramToText(inputList) == ramToText(expectedList), "ploho");
}

QTEST_APPLESS_MAIN(Test)

#include "tst_test.moc"
