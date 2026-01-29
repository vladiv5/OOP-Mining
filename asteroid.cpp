#include "asteroid.h"
#include <sstream>

// Constructor
// I use member initializer lists here for better performance compared to assignment in the body.
Asteroid::Asteroid(std::string id, std::string resursa, double cantitateTotala, int randamentExtractie, int distanta)
    : m_id(id), m_resursa(resursa), m_cantitateTotala(cantitateTotala), m_randamentExtractie(randamentExtractie), m_distanta(distanta)
{
}

// Constructor that reads data from a single line
// I implemented this custom constructor to parse CSV lines directly into Asteroid objects.
Asteroid::Asteroid(const std::string& line)
{
    std::stringstream s(line); // I use stringstream to treat the string as a stream for easy tokenization.
    std::string value; // temporary variable to hold the parsed token

    std::getline(s, m_id, ';'); // I parse the ID up to the delimiter ';'

    std::getline(s, m_resursa, ';'); // I parse the resource name

    std::getline(s, value, ';'); // I read the total quantity as a string...
    m_cantitateTotala = std::stoi(value); // ...and convert it to an integer using stoi.

    std::getline(s, value, ';'); // Parsing extraction yield
    m_randamentExtractie = std::stoi(value); 

    std::getline(s, value, ';'); // Parsing distance
    m_distanta = std::stoi(value); 
}

// Getters
// Standard getters to access private members.
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
// Standard setters to modify the state of the asteroid during the simulation.
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