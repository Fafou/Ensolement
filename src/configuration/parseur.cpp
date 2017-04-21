/*
Fichier permettant de parser le fichier de configuration du composant
*/

#include <sstream>
#include "parseur.h"
#include <iostream>
#include <algorithm>

Parseur::Parseur(const std::string& fichierConfiguration) : fichier(fichierConfiguration, std::ifstream::in) {
    if (fichier) {
        lireConfiguration();
    }
}

Parseur::~Parseur() {
    fichier.close();
}

Cultures& Parseur::getCultures() {
    return cultures;
}

Parcelles& Parseur::getParcelles() {
    return parcelles;
}

int Parseur::getDuree() {
    return duree;
}

void Parseur::lireConfiguration() {
    std::string line;
    while(std::getline(fichier, line)) {
        if (validerLigne(line)) {
            if (line == SECTION_CULTURES) {
                parseCultures();
            } else if (line == SECTION_PARCELLES) {
                parseParcelles();
            }  else if (line == SECTION_CONFIGURATION) {
                parseDuree();
            } else {
                std::cout << "ERROR -> " << line << " <-" << std::endl;
            }
        }
    }
}

void Parseur::parseCultures() {
    std::string line;
    std::string token;
    std::cout << "Ajout des cultures:" <<std::endl;
    while(std::getline(fichier, line)) {
        if (validerLigne(line)) {
            if (line.front() == '[') {
                fichier.seekg(-line.size() - 1, fichier.cur);
                return;
            }
            std::stringstream ss(line);
            Culture c;
            while(std::getline(ss, token, ',')) {
                // Enlevement des espaces tab et autre
                token.erase(std::remove_if(token.begin(), token.end(), [](char x){return std::isspace(x);}), token.end());
                // Parse token
                if (token.find(SECTION_NOM) == 0) {
                    token.erase(token.begin(), token.begin()+SECTION_NOM.size());
                    c.name = token;
                } else if (token.find(SECTION_MIN) == 0) {
                    token.erase(token.begin(), token.begin()+SECTION_MIN.size());
                    c.minSurface = std::stof(token);
                } else if (token.find(SECTION_MAX) == 0) {
                    token.erase(token.begin(), token.begin()+SECTION_MAX.size());
                    c.maxSurface = std::stof(token);
                } else if (token.find(SECTION_PAUSE) == 0) {
                    token.erase(token.begin(), token.begin()+SECTION_PAUSE.size());
                    c.anneeEntreCulture = std::stoi(token);
                } else if (token.find(SECTION_INTERDITES) == 0) {
                    token.erase(token.begin(), token.begin()+SECTION_INTERDITES.size());
                    c.precedentesCulturesInterdites.push_back(token);
                } else {
                    c.precedentesCulturesInterdites.push_back(token);
                }
            }
            std::cout << c << std::endl;
            cultures[c.name] = c;
        }
    }
}

void Parseur::parseParcelles() {
    std::string line;
    std::string token;
    std::string nom;
    float taille(0);
    std::cout << "Ajout des parcelles:" << std::endl;
    while(std::getline(fichier, line)) {
        if (validerLigne(line)) {
            if (line.front() == '[') {
                fichier.seekg(-line.size() - 1, fichier.cur);
                return;
            }
            std::stringstream ss(line);
            while(std::getline(ss, token, ',')) {
                // Enlevement des espaces tab et autre
                token.erase(std::remove_if(token.begin(), token.end(), [](char x){return std::isspace(x);}), token.end());
                // Parse token
                if (token.find(SECTION_NOM) == 0) {
                    token.erase(token.begin(), token.begin()+SECTION_NOM.size());
                    nom = token;
                } else if (token.find(SECTION_TAILLE) == 0) {
                    token.erase(token.begin(), token.begin()+SECTION_TAILLE.size());
                    taille = std::stof(token);
                }
            }
            std::cout << nom << " de taille " << taille << " hectares." << std::endl;
            parcelles[nom] = taille;
        }
    }
}

void Parseur::parseDuree() {
    std::string line;
    std::string token;
    while(std::getline(fichier, line)) {
        if (validerLigne(line)) {
            if (line.front() == '[') {
                fichier.seekg(-line.size() - 1, fichier.cur);
                return;
            }
            std::stringstream ss(line);
            while(std::getline(ss, token, ',')) {
                // Enlevement des espaces tab et autre
                token.erase(std::remove_if(token.begin(), token.end(), [](char x){return std::isspace(x);}), token.end());
                // Parse token
                if (token.find(SECTION_DUREE) == 0) {
                    token.erase(token.begin(), token.begin()+SECTION_DUREE.size());
                    duree = std::stoi(token);
                }
            }
            std::cout << "Durée de calcul de l'ensolement " << duree << " ans." << std::endl;
        }
    }
}

bool Parseur::validerLigne(std::string& line) const {
    // Enlevement des espaces tab et autre
    line.erase(std::remove_if(line.begin(), line.end(), [](char x){return std::isspace(x);}), line.end());
    if (line.size() == 0 || line.front() == '#'){
        return false;
    }
    return true;
}