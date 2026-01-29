#include "piata.h"

// Default constructor
// I initialize the market with empty containers, preparing it to be populated later.
Piata::Piata()
    : m_resurse(), MIN_PRET(), MAX_PRET(), m_resursaMinata()
{
}

// Parameterized Constructor
// I use this to manually set up a market state, useful for unit testing specific scenarios.
Piata::Piata(const std::vector<std::string>& resurse, const std::vector<int>& minPret, const std::vector<int>& maxPret)
    : m_resurse(resurse), MIN_PRET(minPret), MAX_PRET(maxPret), m_resursaMinata(resurse.size(), 0)
{
}

// Constructor that reads data from a CSV file
// I implemented this to load market configurations directly from file, making the simulation data-driven.
Piata::Piata(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cout << "Eroare la deschiderea fisierului " << fileName << std::endl;
    }
    else
    {
        std::string line; 

        // I skip the CSV header line.
        std::getline(file, line);

        while (std::getline(file, line)) 
        {
            std::stringstream stream(line); 
            std::string value; 

            // I parse the resource name.
            std::getline(stream, value, ';'); 
            m_resurse.push_back(value); 

            // I parse the minimum price constraint.
            std::getline(stream, value, ';'); 
            MIN_PRET.push_back(std::stoi(value)); 

            // I parse the maximum price constraint.
            std::getline(stream, value, ';'); 
            MAX_PRET.push_back(std::stoi(value)); 
        }
    
    // I initialize the tracking vectors (mined amount and gross profit) with zeros, matching the number of resources.
    m_resursaMinata.resize(m_resurse.size(), 0.0);
    m_profitBrut.resize(m_resurse.size(), 0.0);
    }
}

// Getters
// Standard accessors for market data.
std::vector<std::string> Piata::getResurse() const
{
    return m_resurse;
}

std::vector<int> Piata::getMinPret() const
{
    return MIN_PRET;
}

std::vector<int> Piata::getMaxPret() const
{
    return MAX_PRET;
}

// I return references to allow direct modification of these tracking vectors during the simulation.
std::vector<double>& Piata::getResursaMinata()
{
    return m_resursaMinata;
}

std::vector<double>& Piata::getProfitBrut()
{
    return m_profitBrut;
}

// Setters
void Piata::setResursa(const std::vector<std::string>& resurse)
{
    m_resurse = resurse;
    // When updating resources, I resize the tracking vector to ensure consistency.
    m_resursaMinata.resize(resurse.size(), 0); 
}

void Piata::setMinPret(const std::vector<int>& minPret)
{
    MIN_PRET = minPret;
}

void Piata::setMaxPret(const std::vector<int>& maxPret)
{
    MAX_PRET = maxPret;
}

void Piata::setResursaMinata(const std::vector<double>& resursaMinata)
{
    m_resursaMinata = resursaMinata;
}