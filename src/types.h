
/*
Fichier permettant de définir les types utilisé par le programme
*/

#ifndef ENSOLEMENT_TYPES_H
#define ENSOLEMENT_TYPES_H

#include <string>
#include <vector>
#include <ostream>
#include <unordered_map>

typedef std::unordered_map<std::string, float> Parcelles;

struct Culture {
    std::string name;
    float minSurface;
    float maxSurface;
    int anneeEntreCulture;
    std::vector<std::string> precedentesCulturesInterdites;
};

typedef std::unordered_map<std::string, Culture> Cultures;

struct Assignation {
    std::string nameCulture;
    std::vector<std::vector<std::string> > parcelles;
    std::vector<float> surfaceCouverte;
    Assignation(){};
};

typedef std::unordered_map<std::string, Assignation> Assignations;

inline std::ostream& operator<< (std::ostream & out, Culture const& culture) {
    out << "Culture: " << culture.name << " entre " << culture.minSurface <<
    " et " << culture.maxSurface << " hectare, pause avant de re-semer " << culture.anneeEntreCulture << " ans";
    if (culture.precedentesCulturesInterdites.size()>0) {
        out << ", ne peut venir après : ";
        for (unsigned int i(0); i<culture.precedentesCulturesInterdites.size(); i++) {
            out << culture.precedentesCulturesInterdites[i] << " ";
        }
    }
    return out;
}

#endif