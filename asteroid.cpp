#include "asteroid.h"
#include <sstream>

// Constructor
Asteroid::Asteroid(std::string id, std::string resursa, double cantitateTotala, int randamentExtractie, int distanta)
    : m_id(id), m_resursa(resursa), m_cantitateTotala(cantitateTotala), m_randamentExtractie(randamentExtractie), m_distanta(distanta)
{
}

// Constructor that reads data from a single line
Asteroid::Asteroid(const std::string& line)
{
    std::stringstream s(line); // transform linia intr-un flux de date
    std::string value; // variabila in care retin temporar valorile citite

    std::getline(s, m_id, ';'); // citesc valoarea pt id

    std::getline(s, m_resursa, ';'); // citesc valoarea pt resursa

    std::getline(s, value, ';'); // citesc valoarea pt cantitateTotala
    m_cantitateTotala = std::stoi(value); // convertesc valoarea citita dintr-un string in int

    std::getline(s, value, ';'); // citesc valoarea pt randamentExtractie
    m_randamentExtractie = std::stoi(value); // convertesc valoarea citita dintr-un string in int

    std::getline(s, value, ';'); // citesc valoarea pt distanta
    m_distanta = std::stoi(value); // convertesc valoarea citita dintr-un string in int
}

// Getters
std::string Asteroid::getId() const
{
    return m_id;
}

std::string Asteroid::getResursa() const
{
    return m_resursa;
}

double Asteroid::getCantitateTotala() const
{
    return m_cantitateTotala;
}

int Asteroid::getRandamentExtractie() const
{
    return m_randamentExtractie;
}

int Asteroid::getDistanta() const
{
    return m_distanta;
}

// Setters
void Asteroid::setId(std::string id)
{
    m_id = id;
}

void Asteroid::setResursa(std::string resursa)
{
    m_resursa = resursa;
}

void Asteroid::setCantitateTotala(double cantitateTotala)
{
    m_cantitateTotala = cantitateTotala;
}

void Asteroid::setRandamentExtractie(int randamentExtractie)
{
    m_randamentExtractie = randamentExtractie;
}

void Asteroid::setDistanta(int distanta)
{
    m_distanta = distanta;
}