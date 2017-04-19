/*
Petit exécutable pour permettre de calculer l'ensolement'
*/

#include "main.h"
#include <iostream>
#include <algorithm>

Ensolement::Ensolement() : parcelles(), cultures(), maxYear(1), begin(0) {

}

bool Ensolement::computeEnsolement() {

    // Initialisation des données
    int currentYear = 0;
    int nbLeaf = 0;
    maxYear = 4;

    // Initialise la taille des parcelles
    initParcelles();

    // Initialise les cultures
    initCultures();

    std::cout << "Début du calcul de l'ensolement." << std::endl;
    Cultures culturesRestantes = cultures;
    Parcelles parcellesRestantes = parcelles;
    Assignations assignation;
    for (auto& it: cultures) {
        assignation[it.second.name].parcelles.resize(maxYear);
        assignation[it.second.name].surfaceCouverte.resize(maxYear);
        for (unsigned int index(0); index < maxYear; index++) {
            assignation[it.second.name].surfaceCouverte[index] = 0;
        }
    }
    begin = std::clock();
    
    // Calcul de l'ensolement
    std::cout << "+" << culturesRestantes.begin()->first << std::endl;
    bool result = computeOneYear(culturesRestantes, parcellesRestantes, assignation, currentYear, nbLeaf, "");

    // Affichage du résultat
    if (result == true) {
        printResult(assignation, nbLeaf);
    } else {
        std::cout << "Contraintes trop fortes, il n'y a pas de solutions ! Chemins parcourus : " << nbLeaf << ", en " <<
        double(std::clock() - begin)/CLOCKS_PER_SEC << "s" << std::endl;
    }
    
    return result;
}

bool Ensolement::initParcelles () {
    //parcelles["A"] = 2;
    parcelles["B"] = 1;
    parcelles["C"] = 1;
   // parcelles["D"] = 5;
    //parcelles["E"] = 3;
    parcelles["F"] = 1;
    //parcelles["G"] = 2;
    //parcelles["H"] = 2;
    return true;
}

bool Ensolement::initCultures () {
    Culture ble = {"ble", 4, 6, 1, {}};
    //Culture orge = {"orge", 2, 5, 2, {"ble"}};
    //Culture colza = {"colza", 4, 7, 3, {"orge"}};
    cultures[ble.name] = ble;
    //cultures[orge.name] = orge;
    //cultures[colza.name] = colza;
    return true;
}

bool Ensolement::computeOneYear(Cultures& culturesRestantes, Parcelles& parcellesRestantes, Assignations& assignation, unsigned int currentYear, int& nbLeaf, std::string prefix) {
    bool result;

    // Récupération de la prochaine culture a traiter
    Culture& culture = cultures[culturesRestantes.begin()->first];

    // Récupération de l'historique des parcelles de cette culture
    Assignation& current = assignation[culture.name];
    current.nameCulture = culture.name;

    // Parcours des parcelles restant a attribuer
    std::vector<std::string> unusedKey;
    unusedKey.reserve(parcellesRestantes.size());
    for (auto& it: parcellesRestantes) {
        unusedKey.push_back(it.first);
    }
    for(unsigned int index(0); index < unusedKey.size(); index++) {
        const std::string& parcelleName = unusedKey[index];
        float parcelleSize = parcellesRestantes[parcelleName];
        if (validateFromPreviousYear(parcelleName, current, culture, currentYear) == true) {
            float taille = current.surfaceCouverte[currentYear] + parcelleSize;

            if (taille <= culture.maxSurface) {
                std::cout << prefix+"->" << parcelleName << std::endl;
                current.parcelles[currentYear].push_back(parcelleName);
                current.surfaceCouverte[currentYear] = taille;
                parcellesRestantes.erase(parcelleName);
                unusedKey.erase(unusedKey.begin() + index);
                index--;
                if (taille >= culture.minSurface) {
                    culturesRestantes.erase(culture.name);
                    if (culturesRestantes.size() == 0) {
                        if ((currentYear + 1) < maxYear) {
                            Cultures culturesRestantesBis = cultures;
                            Parcelles parcellesRestantesBis = parcelles;
                            currentYear++;
                            std::cout << prefix+"  +" << cultures.begin()->first << std::endl;
                            result = computeOneYear(culturesRestantesBis, parcellesRestantesBis, assignation, currentYear, nbLeaf, prefix+"  ");
                            if (result == false) {
                                std::cout << prefix+"  -" << cultures.begin()->first << std::endl;
                                currentYear--;
                                culturesRestantes[current.nameCulture] = cultures[current.nameCulture];
                            }
                        } else {
                            if (nbLeaf%100000 == 0) {
                                std::cout << "Nb leaf : " << nbLeaf << " en " << double(std::clock() - begin)/CLOCKS_PER_SEC << "s" << std::endl;
                            }
                            nbLeaf++;
                            return true;
                        }
                    } else {
                        std::cout << prefix+"+" << culturesRestantes.begin()->first << std::endl;
                        result = computeOneYear(culturesRestantes, parcellesRestantes, assignation, currentYear, nbLeaf, prefix);
                        if (result == false) {
                            std::cout << prefix+"-" << culturesRestantes.begin()->first << std::endl;
                            culturesRestantes[current.nameCulture] = cultures[current.nameCulture];
                        }
                    }
                }
                result = computeOneYear(culturesRestantes, parcellesRestantes, assignation, currentYear, nbLeaf, prefix);
                if (result == false) {
                    current.surfaceCouverte[currentYear] -= parcelleSize;
                    parcellesRestantes[current.parcelles[currentYear].back()] = parcelles[current.parcelles[currentYear].back()];
                    index++;
                    unusedKey.insert(unusedKey.begin()+index, current.parcelles[currentYear].back());
                    std::cout << prefix+"<-" << parcelleName << std::endl;
                    current.parcelles[currentYear].pop_back();
                } else {
                    return true;
                }
            }
        }
    }
    if (nbLeaf%100000 == 0) {
        std::cout << "Nb leaf : " << nbLeaf << " en " << double(std::clock() - begin)/CLOCKS_PER_SEC << "s" << std::endl;
    }
    nbLeaf++;
    return false;
}

bool Ensolement::validateFromPreviousYear(const std::string& parcelleName, const Assignation& curentCulture, const Culture& defCulture, const int& currentYear) const {
    // Vérification de ne pas avoir la même culture n années de suite
    for (int yearBefore(1); yearBefore <= defCulture.anneeEntreCulture && currentYear - yearBefore >= 0; yearBefore++) {
        int year = currentYear - yearBefore;
        if (std::find(curentCulture.parcelles[year].begin(), curentCulture.parcelles[year].end(), parcelleName) !=
                curentCulture.parcelles[year].end()) {
            return false;
        }
    }
    return true;
}

void Ensolement::printResult(const Assignations& assignation, const int nbLeaf) const {
    std::cout << "Voila le résult : " << std::endl;
    for (unsigned int year(0); year < maxYear; year++) {
        std::cout << "Année " << year+1 << " :" << std::endl;
        for (auto& it: assignation) {
            std::cout << "    Culture: " << it.second.nameCulture << ", parcelles : [";
            for (unsigned int i(0); i < it.second.parcelles[year].size(); i++) {
                std::cout << " " << it.second.parcelles[year].at(i);
            }
            std::cout << " ], taille de la culture : " << it.second.surfaceCouverte[year] << std::endl;
        }
    }

    std::cout << nbLeaf << " chemins parcourus en " << double(std::clock() - begin)/CLOCKS_PER_SEC << "s" << std::endl;
}

int main (int argc, char * argv  []) {
    if (argc > 1) {
        std::cout << "Unused arguments : ";
        for (int i(1); i<argc; i++) {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;
    }
    Ensolement ensolement;
    ensolement.computeEnsolement();
    return 0;
}
