/*
Classe de test pour l'ensolement
*/

#ifndef ENSOLEMENT_TEST_H
#define ENSOLEMENT_TEST_H

#include "ensolement.h"
#include <string>
#include <unordered_map>

class Test;

typedef bool (Test::*Tests)();

class Test {
protected:
    std::unordered_map<std::string, Tests> tests;

public:
    Test();
    void runTests();

protected:
    bool test_1();
    bool test_2();
};

#endif