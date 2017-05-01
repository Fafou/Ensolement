/*
Classe de test pour l'ensolement
*/

#ifndef ENSOLEMENT_TEST_H
#define ENSOLEMENT_TEST_H

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
    bool test_simple1();
    bool test_simple2();
    bool test_simple3();
    bool test_impossible_simple1();
    bool test_impossible_simple2();
    bool test_impossible_simple3();
};

#endif