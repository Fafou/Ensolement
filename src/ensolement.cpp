/*
Petit exécutable pour permettre de calculer l'ensolement
*/

#include "ensolement.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include "configuration/parseur.h"

#ifdef ENSOLEMENT_VERBOSE // Ajoute des logs si le flag ENSOLEMENT_VERBOSE est ajouté à la compilation
    #define LOG(message) std::cout << message << std::endl
#else
    #define LOG(message) // message Ne fait rien du message sans le flag
#endif

Ensolement::Ensolement(const std::string& fichierConfiguration) : parseur(fichierConfiguration), begin(0), nbLeaf(0) {
    parcelles = parseur.getParcelles();
    cultures = parseur.getCultures();
    maxYear = parseur.getDuree();
}

void Ensolement::initDatas(Parcelles& parcelles, Cultures& cultures, unsigned int maxYear) {
    this->parcelles = parcelles;
    this->cultures = cultures;
    this->maxYear = maxYear;
    nbLeaf = 0;
}

bool Ensolement::computeEnsolement(Assignations& assignation) {

    // Initialisation des données
    int currentYear = 0;
    nbLeaf = 0;

    std::cout << "Début du calcul de l'ensolement." << std::endl;
    Cultures culturesRestantes = cultures;
    Parcelles parcellesRestantes = parcelles;
    assignation.clear();
    for (auto& it: cultures) {
        assignation[it.second.name].parcelles.resize(maxYear);
        assignation[it.second.name].surfaceCouverte.resize(maxYear);
        for (unsigned int index(0); index < maxYear; index++) {
            assignation[it.second.name].surfaceCouverte[index] = 0;
        }
    }
    begin = std::clock();
    
    // Calcul de l'ensolement
    LOG("+" << culturesRestantes.begin()->first);
    bool result = computeOneYear(culturesRestantes, parcellesRestantes, assignation, currentYear, "");

    // Affichage du résultat
    if (result == true) {
        std::cout << "Solution trouvée ! Chemins parcourus : " << nbLeaf << ", en " <<
        double(std::clock() - begin)/CLOCKS_PER_SEC << "s" << std::endl;
    } else {
        std::cout << "Contraintes trop fortes, il n'y a pas de solutions ! Chemins parcourus : " << nbLeaf << ", en " <<
        double(std::clock() - begin)/CLOCKS_PER_SEC << "s" << std::endl;
    }
    
    return result;
}

bool Ensolement::computeOneYear(Cultures& culturesRestantes, Parcelles& parcellesRestantes, Assignations& assignation, unsigned int currentYear, std::string prefix) {
    bool result;

    // Récupération de la prochaine culture a traiter
    const Culture& culture = cultures.at(culturesRestantes.begin()->first);

    // Récupération de l'historique des parcelles de cette culture
    Assignation& current = assignation[culture.name];
    current.nameCulture = culture.name;

    // Parcours des parcelles restant a attribuer
    unsigned int curIter(0);
    while(parcellesRestantes.size()>0 && curIter < parcellesRestantes.size()) {
        const std::string& parcelleName = std::next(parcellesRestantes.begin(), curIter)->first;
        float parcelleSize = parcellesRestantes[parcelleName];
        if (validateFromPreviousYear(parcelleName, current, culture, currentYear, assignation) == true) {
            float taille = current.surfaceCouverte[currentYear] + parcelleSize;

            if (taille <= culture.maxSurface) {
                LOG(prefix+"->" << parcelleName);
                current.parcelles[currentYear].push_back(parcelleName);
                current.surfaceCouverte[currentYear] = taille;
                parcellesRestantes.erase(parcelleName);
                if (taille >= culture.minSurface) {
                    culturesRestantes.erase(culture.name);
                    if (culturesRestantes.size() == 0) {
                        if ((currentYear + 1) < maxYear) {
                            Cultures culturesRestantesBis = cultures;
                            Parcelles parcellesRestantesBis = parcelles;
                            currentYear++;
                            LOG(prefix+"  +" << cultures.begin()->first);
                            result = computeOneYear(culturesRestantesBis, parcellesRestantesBis, assignation, currentYear, prefix+"  ");
                            if (result == false) {
                                LOG(prefix+"  *");
                                currentYear--;
                                culturesRestantes[current.nameCulture] = cultures.at(current.nameCulture);
                                current.surfaceCouverte[currentYear] -= parcelleSize;
                                parcellesRestantes[current.parcelles[currentYear].back()] = parcelles.at(current.parcelles[currentYear].back());
                                curIter++;
                                LOG(prefix+"X");
                                current.parcelles[currentYear].pop_back();
                            } else {
                                return true;
                            }
                        } else {
                            nbLeaf++;
                            if (nbLeaf%100000 == 0) {
                                std::cout << "Nb leaf : " << nbLeaf << " en " << double(std::clock() - begin)/CLOCKS_PER_SEC << "s" << std::endl;
                            }
                            return true;
                        }
                    } else {
                        LOG(prefix+"+" << culturesRestantes.begin()->first);
                        result = computeOneYear(culturesRestantes, parcellesRestantes, assignation, currentYear, prefix);
                        if (result == false) {
                            LOG(prefix+"*");
                            culturesRestantes[current.nameCulture] = cultures.at(current.nameCulture);
                            current.surfaceCouverte[currentYear] -= parcelleSize;
                            parcellesRestantes[current.parcelles[currentYear].back()] = parcelles.at(current.parcelles[currentYear].back());
                            curIter++;
                            LOG(prefix+"X");
                            current.parcelles[currentYear].pop_back();
                        } else {
                            return true;
                        }
                    }
                } else {
                    result = computeOneYear(culturesRestantes, parcellesRestantes, assignation, currentYear, prefix);
                    if (result == false) {
                        current.surfaceCouverte[currentYear] -= parcelleSize;
                        parcellesRestantes[current.parcelles[currentYear].back()] = parcelles.at(current.parcelles[currentYear].back());
                        curIter++;
                        LOG(prefix+"X");
                        current.parcelles[currentYear].pop_back();
                    } else {
                        return true;
                    }
                }
            } else {
                curIter++;
            }
        } else {
            curIter++;
        }
    }
    nbLeaf++;
    if (nbLeaf%100000 == 0) {
        std::cout << "Nb leaf : " << nbLeaf << " en " << double(std::clock() - begin)/CLOCKS_PER_SEC << "s" << std::endl;
    }
    return false;
}

bool Ensolement::validateFromPreviousYear(const std::string& parcelleName, const Assignation& curentCulture, const Culture& defCulture, const int& currentYear, const Assignations& assignations) const {
    if (currentYear > 0) {
        // Vérification de ne pas avoir la même culture n années de suite
        for (int yearBefore(1); yearBefore <= defCulture.anneeEntreCulture && currentYear - yearBefore >= 0; yearBefore++) {
            int year = currentYear - yearBefore;
            if (std::find(curentCulture.parcelles[year].begin(), curentCulture.parcelles[year].end(), parcelleName) !=
                    curentCulture.parcelles[year].end()) {
                return false;
            }
        }
        // Vérification que la culture précedente ne soit pas interdite
        for (auto& it: defCulture.precedentesCulturesInterdites) {
            const std::vector<std::string>& forbidenCulture = assignations.at(it).parcelles[currentYear - 1];
            if (std::find(forbidenCulture.begin(), forbidenCulture.end(), parcelleName) != forbidenCulture.end()) {
                return false;
            }
        }
    }
    return true;
}

void Ensolement::printResult(const Assignations& assignation) const {
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
