#ifndef PIATA_H
#define PIATA_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class Piata
{
private:
    std::vector<std::string> m_resurse;
    std::vector<int> MIN_PRET;
    std::vector<int> MAX_PRET;
    std::vector<double> m_resursaMinata;
    std::vector<double> m_profitBrut;

public:
    // Constructors
    Piata();
    Piata(const std::vector<std::string>& resurse, const std::vector<int>& minPret, const std::vector<int>& maxPret);
    Piata(const std::string& fileName);

    // Getters
    std::vector<std::string> getResurse() const;
    std::vector<int> getMinPret() const;
    std::vector<int> getMaxPret() const;
    std::vector<double>& getResursaMinata();
    std::vector<double>& getProfitBrut();

    // Setters
    void setResursa(const std::vector<std::string>& resurse);
    void setMinPret(const std::vector<int>& minPret);
    void setMaxPret(const std::vector<int>& maxPret);
    void setResursaMinata(const std::vector<double>& resursaMinata);
};

#endif // PIATA_H