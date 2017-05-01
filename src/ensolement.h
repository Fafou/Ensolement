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
    int nbLeaf;

public:
    /*
    Constructeur, utilise le parseur de configuration pour initialiser les attributs en fonction du fichier de configuration passé en argument
    */
    Ensolement(const std::string& fichierConfiguration);

    /*
    Fonction pour initialiser les données, essentiellement utilisé pour les tests
    */
    void initDatas(Parcelles& parcelles, Cultures& cultures, unsigned int maxYear);

    /*
    Fonction pour calculer l'ensolement
    */
    bool computeEnsolement(Assignations& assignation);

    /*
    Fonction pour afficher le résultat du calcul
    */
    void printResult(const Assignations& assignation) const;

protected:
    bool computeOneYear(Cultures& culturesRestantes, Parcelles& parcellesRestantes, Assignations& assignation, unsigned int currentYear, std::string prefix);
    bool validateFromPreviousYear(const std::string& parcelleName, const Assignation& curentCulture, const Culture& defCulture, const int& currentYear, const Assignations& assignations) const;
};

#endif