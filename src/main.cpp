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
    Assignations assignation;

    Ensolement ensolement(argv[1]);
    bool result = ensolement.computeEnsolement(assignation);
    
    if (result == true) {
        ensolement.printResult(assignation);
    }
    return 0;
}