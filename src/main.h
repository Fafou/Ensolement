/*
Petit exécutable pour permettre de calculer l'ensolement
*/

#ifndef ENSOLEMENT_MAIN_H
#define ENSOLEMENT_MAIN_H

#include <ctime>
#include <string>
#include "types.h"
#include "configuration/parseur.h"


class Ensolement {
protected:
    Parseur parseur;
    Parcelles parcelles;
    Cultures cultures;
    unsigned int maxYear;
    std::clock_t begin;

public:
    Ensolement(const std::string& fichierConfiguration);
    bool computeEnsolement();

private:
    bool computeOneYear(Cultures& culturesRestantes, Parcelles& parcellesRestantes, Assignations& assignation, unsigned int currentYear, int& nbLeaf, std::string prefix) const;
    bool validateFromPreviousYear(const std::string& parcelleName, const Assignation& curentCulture, const Culture& defCulture, const int& currentYear, const Assignations& assignations) const;
    void printResult(const Assignations& assignation, const int nbLeaf) const;
};

#endif