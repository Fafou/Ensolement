/*
Classe de test pour l'ensolement
*/

#include "test.h"
#include "ensolement.h"
#include <vector>
#include <iostream>

Test::Test() {
    tests["Test simple cas 1"] = &Test::test_simple1;
    tests["Test simple cas 2"] = &Test::test_simple2;
    tests["Test simple cas 3"] = &Test::test_simple3;
    tests["Test solution impossible simple cas 1"] = &Test::test_impossible_simple1;
    tests["Test solution impossible simple cas 2"] = &Test::test_impossible_simple2;
    tests["Test solution impossible simple cas 3"] = &Test::test_impossible_simple3;
}

void Test::runTests() {
    std::vector<std::string> failedTests;
    for (auto& it : tests) {
        Tests t = it.second;
        if (((this)->*t)() == false) {
            failedTests.push_back(it.first);
        }
    }

    std::cout << "-----------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Total tests : " << tests.size() << ", OK : " << tests.size()-failedTests.size() << ", Failed : " << failedTests.size() << std::endl;
    std::cout << "-----------------------------------------------------------------------------------------------" << std::endl;
    for (auto it : failedTests) {
        std::cout << it << std::endl;
    }
}

bool Test::test_simple1() {
    Ensolement esolement("");
    Parcelles parcelles = {{"A", 2}, {"B", 3}, {"C", 1}};
    Cultures cultures = {{"blé", {"blé", 1, 3, 1, {"orge"}}}, {"orge", {"orge", 1, 3, 1, {"colza"}}}, {"colza", {"colza", 1, 3, 1, {}}}};
    unsigned int maxYear(1);
    esolement.initDatas(parcelles, cultures, maxYear);
    Assignations assignation;
    bool result = esolement.computeEnsolement(assignation);
    return result;
}

bool Test::test_impossible_simple1() {
    Ensolement esolement("");
    Parcelles parcelles = {{"A", 2}};
    Cultures cultures = {{"blé", {"blé", 1, 3, 1, {"orge"}}}, {"orge", {"orge", 1, 3, 1, {"colza"}}}, {"colza", {"colza", 1, 3, 1, {}}}};
    unsigned int maxYear(1);
    esolement.initDatas(parcelles, cultures, maxYear);
    Assignations assignation;
    bool result = esolement.computeEnsolement(assignation);
    return result == false;
}

bool Test::test_simple2() {
    Ensolement esolement("");
    Parcelles parcelles = {{"A", 2}, {"B", 3}};
    Cultures cultures = {{"blé", {"blé", 1, 3, 1, {}}}};
    unsigned int maxYear(2);
    esolement.initDatas(parcelles, cultures, maxYear);
    Assignations assignation;
    bool result = esolement.computeEnsolement(assignation);
    return result;
}

bool Test::test_impossible_simple2() {
    Ensolement esolement("");
    Parcelles parcelles = {{"A", 2}, {"B", 3}};
    Cultures cultures = {{"blé", {"blé", 1, 3, 2, {}}}};
    unsigned int maxYear(3);
    esolement.initDatas(parcelles, cultures, maxYear);
    Assignations assignation;
    bool result = esolement.computeEnsolement(assignation);
    return result == false;
}

bool Test::test_impossible_simple3() {
    Ensolement esolement("");
    Parcelles parcelles = {{"A", 2}, {"B", 3}};
    Cultures cultures = {{"blé", {"blé", 1, 3, 1, {"orge"}}}, {"orge", {"orge", 1, 3, 1, {}}}};
    unsigned int maxYear(2);
    esolement.initDatas(parcelles, cultures, maxYear);
    Assignations assignation;
    bool result = esolement.computeEnsolement(assignation);
    return result == false;
}

bool Test::test_simple3() {
    Ensolement esolement("");
    Parcelles parcelles = {{"A", 6}, {"B", 4}, {"C", 7}, {"D", 8}, {"E", 3}, {"F", 1}};
    Cultures cultures = {{"blé", {"blé", 7, 7, 1, {}}}, {"orge", {"orge", 7, 7, 2, {}}}};
    unsigned int maxYear(3);
    esolement.initDatas(parcelles, cultures, maxYear);
    Assignations assignation;
    bool result = esolement.computeEnsolement(assignation);
    return result;
}
