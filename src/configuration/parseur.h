/*
Fichier permettant de parser le fichier de configuration du composant
*/

#ifndef ENSOLEMENT_PARSEUR_H
#define ENSOLEMENT_PARSEUR_H

#include <string>
#include <fstream>
#include "../types.h"

class Parseur {
private:
    const std::string SECTION_CONFIGURATION = "[Configuration]";
    const std::string SECTION_CULTURES = "[Cultures]";
    const std::string SECTION_PARCELLES = "[Parcelles]";
    const std::string SECTION_NOM = "Nom:";
    const std::string SECTION_MIN = "Min:";
    const std::string SECTION_MAX = "Max:";
    const std::string SECTION_PAUSE = "Pause:";
    const std::string SECTION_INTERDITES = "Interdites:";
    const std::string SECTION_TAILLE = "Taille:";
    const std::string SECTION_DUREE = "Duree:";
    std::ifstream fichier;
    Cultures cultures;
    Parcelles parcelles;
    int duree;

public:
    Parseur(const std::string& fichierConfiguration);
    ~Parseur();

    Cultures& getCultures();
    Parcelles& getParcelles();
    int getDuree();

private:
    void lireConfiguration();
    void parseCultures();
    void parseParcelles();
    void parseDuree();
    bool validerLigne(std::string& line) const;
};

#endif