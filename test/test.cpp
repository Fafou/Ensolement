/*
Classe de test pour l'ensolement
*/

#include "test.h"
#include <vector>
#include <iostream>

Test::Test() {
    tests["test_1"] = &Test::test_1;
    tests["test_2"] = &Test::test_2;
}

void Test::runTests() {
    std::vector<std::string> failedTests;
    for (auto& it : tests) {
        Tests t = it.second;
        if (((this)->*t)() == false) {
            failedTests.push_back(it.first);
        }
    }

    std::cout << "Total tests : " << tests.size() << ", OK : " << tests.size()-failedTests.size() << ", Failed : " << failedTests.size() << std::endl;
    for (auto it : failedTests) {
        std::cout << it << std::endl;
    }
}

bool Test::test_1() {
    return false;
}

bool Test::test_2() {
    return true;
}
