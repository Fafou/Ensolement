/*
Executable du programme
*/

#include <iostream>
#include "ensolement.h"

int main (int argc, char * argv  []) {
    if (argc < 2) {
        std::cout << "Le programme prend en parametre le chemin vers le fichier de configuration :" << std::endl;
        std::cout << argv[0] << " configuration.bob" << std::endl;
        return -1;
    } else if (argc > 2) {
        std::cout << "Le programme prend en parametre le chemin vers le fichier de configuration :" << std::endl;
        std::cout << argv[0] << " configuration.bob" << std::endl;
        std::cout << "Arguments en trop: ";
        for (int i(2); i<argc; i++) {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;
        return -1;
    }
    Ensolement ensolement(argv[1]);
    ensolement.computeEnsolement();
    return 0;
}