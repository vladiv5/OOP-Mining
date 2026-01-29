#ifndef PIATA_H
#define PIATA_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// I defined the 'Piata' (Market) class to manage resource prices and track global extraction stats.
class Piata
{
private:
    std::vector<std::string> m_resurse;
    std::vector<int> MIN_PRET;
    std::vector<int> MAX_PRET;
    std::vector<double> m_resursaMinata; // Tracks total mined quantity per resource to calculate price drops.
    std::vector<double> m_profitBrut;    // Tracks total revenue generated per resource.

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