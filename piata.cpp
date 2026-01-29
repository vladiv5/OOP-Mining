#include "piata.h"


// Default constructor
Piata::Piata()
    : m_resurse(), MIN_PRET(), MAX_PRET(), m_resursaMinata()
{
}

// Constructor
Piata::Piata(const std::vector<std::string>& resurse, const std::vector<int>& minPret, const std::vector<int>& maxPret)
    : m_resurse(resurse), MIN_PRET(minPret), MAX_PRET(maxPret), m_resursaMinata(resurse.size(), 0)
{
}

// Constructor that reads data from a CSV file
Piata::Piata(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cout << "Eroare la deschiderea fisierului " << fileName << std::endl;
    }
    else
    {
        std::string line; // variabila in care retin linia citita

        // Ignor primul rand din fisier
        std::getline(file, line);

        while (std::getline(file, line)) // citesc linie cu linie pana la sfarsitul fisierului
        {
            std::stringstream stream(line); // transform linia intr-un flux de date
            std::string value; // variabila in care retin temporar valorile citite

            std::getline(stream, value, ';'); // citesc valoarea pt resursa
            m_resurse.push_back(value); // adaug valoarea citita in vectorul de resurse

            std::getline(stream, value, ';'); // citesc valoarea pt MIN_PRET
            MIN_PRET.push_back(std::stoi(value)); // convertesc valoarea citita dintr-un string in int si o adaug in vectorul de preturi minime

            std::getline(stream, value, ';'); // citesc valoarea pt MAX_PRET
            MAX_PRET.push_back(std::stoi(value)); // convertesc valoarea citita dintr-un string in int si o adaug in vectorul de preturi maxime
        }
    
    // Inițializăm vectorul m_resursaMinata cu aceeași dimensiune ca vectorul m_resurse și toate elementele setate la 0
    m_resursaMinata.resize(m_resurse.size(), 0.0);
    m_profitBrut.resize(m_resurse.size(), 0.0);
    }
}

// Getters
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
    m_resursaMinata.resize(resurse.size(), 0); // Resize m_resursaMinata to match the size of m_resurse and set all elements to 0
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

// Method to get max price of a resource by name

