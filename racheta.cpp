#include "racheta.h"

// Default constructor
Racheta::Racheta() : m_id(""), m_capacitate(0), m_consumCombustibil(0), m_combustibilDisponibil(0), m_pretCombustibil(0.0f)
{
}

// Parameterized Constructor
// I use this for initializing rockets with explicit values, useful for testing or manual creation.
Racheta::Racheta(std::string id, int capacitate, int consumCombustibil, int combustibilDisponibil, float pretCombustibil)
    : m_id(id), m_capacitate(capacitate), m_consumCombustibil(consumCombustibil), m_combustibilDisponibil(combustibilDisponibil), m_pretCombustibil(pretCombustibil)
{
}

// Constructor that reads data from a CSV line
// I implemented this to parse rocket configuration directly from a CSV string.
Racheta::Racheta(const std::string& line)
{
    std::stringstream s(line); 
    std::string value; 

    std::getline(s, m_id, ';'); // Parsing ID

    std::getline(s, value, ';'); 
    m_capacitate = std::stoi(value); // Parsing cargo capacity

    std::getline(s, value, ';'); 
    m_consumCombustibil = std::stoi(value); // Parsing fuel consumption rate

    std::getline(s, value, ';'); 
    m_combustibilDisponibil = std::stoi(value); // Parsing initial fuel tank

    std::getline(s, value, ';'); 
    m_pretCombustibil = std::stof(value); // Parsing fuel cost
}

// Assignment operator
// I overloaded the assignment operator to ensure deep copying of rocket properties if needed.
Racheta& Racheta::operator=(const Racheta& other)
{
    if (this != &other)
    {
        m_id = other.m_id;
        m_capacitate = other.m_capacitate;
        m_consumCombustibil = other.m_consumCombustibil;
        m_combustibilDisponibil = other.m_combustibilDisponibil;
        m_pretCombustibil = other.m_pretCombustibil;
    }
    return *this;
}

// Getters
std::string Racheta::getId() const
{
    return m_id;
}

int Racheta::getCapacitate() const
{
    return m_capacitate;
}

int Racheta::getConsumCombustibil() const
{
    return m_consumCombustibil;
}

int Racheta::getCombustibilDisponibil() const
{
    return m_combustibilDisponibil;
}

float Racheta::getPretCombustibil() const
{
    return m_pretCombustibil;
}

// Setters
void Racheta::setId(std::string id)
{
    m_id = id;
}

void Racheta::setCapacitate(int capacitate)
{
    m_capacitate = capacitate;
}

void Racheta::setConsumCombustibil(int consumCombustibil)
{
    m_consumCombustibil = consumCombustibil;
}

void Racheta::setCombustibilDisponibil(int combustibilDisponibil)
{
    m_combustibilDisponibil = combustibilDisponibil;
}

void Racheta::setPretCombustibil(float pretCombustibil)
{
    m_pretCombustibil = pretCombustibil;
}