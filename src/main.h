/*
Petit exécutable pour permettre de calculer l'ensolement'
*/

#include <ctime>
#include <vector>
#include <string>
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

class Ensolement {

protected:
    Parcelles parcelles;
    Cultures cultures;
    unsigned int maxYear;
    std::clock_t begin;

public:
    Ensolement();
    bool computeEnsolement();

private:
    bool initParcelles ();
    bool initCultures ();
    bool computeOneYear(Cultures& culturesRestantes, Parcelles& parcellesRestantes, Assignations& assignation, unsigned int currentYear, int& nbLeaf, std::string prefix);
    bool validateFromPreviousYear(const std::string& parcelleName, const Assignation& curentCulture, const Culture& defCulture, const int& currentYear) const;
    void printResult(const Assignations& assignation, const int nbLeaf) const;
};