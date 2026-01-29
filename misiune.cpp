#include "misiune.h"

// Constructor
// I initialize the mission object with default empty values.
Misiune::Misiune()
    : m_idRacheta(""), m_idAsteroid(""), m_resursa(""), m_cantitateExtrasa(0), m_combustibilConsum(0), m_costCombustibil(0), m_profitBrut(0), m_profitNet(0)
{
}

// Parameterized Constructor
// I allow creating a fully populated mission object in one step.
Misiune::Misiune(std::string idRacheta, std::string idAsteroid, std::string resursa, int cantitateExtrasa, int combustibilConsum, double costCombustibil, int profitBrut, int profitNet)
    : m_idRacheta(idRacheta), m_idAsteroid(idAsteroid), m_resursa(resursa), m_cantitateExtrasa(cantitateExtrasa), m_combustibilConsum(combustibilConsum), m_costCombustibil(costCombustibil), m_profitBrut(profitBrut), m_profitNet(profitNet)
{
}

// Getters
// I provide accessors for all mission details to be used in report generation.
std::string Misiune::getIdRacheta() const
{
    return m_idRacheta;
}

std::string Misiune::getIdAsteroid() const
{
    return m_idAsteroid;
}

std::string Misiune::getResursa() const
{
    return m_resursa;
}

int Misiune::getCantitateExtrasa() const
{
    return m_cantitateExtrasa;
}

int Misiune::getCombustibilConsum() const
{
    return m_combustibilConsum;
}

double Misiune::getCostCombustibil() const
{
    return m_costCombustibil;
}

int Misiune::getProfitBrut() const
{
    return m_profitBrut;
}

int Misiune::getProfitNet() const
{
    return m_profitNet;
}

// Setters
// I implemented setters to allow modification of mission details if calculations need adjustment.
void Misiune::setIdRacheta(std::string idRacheta)
{
    m_idRacheta = idRacheta;
}

void Misiune::setIdAsteroid(std::string idAsteroid)
{
    m_idAsteroid = idAsteroid;
}

void Misiune::setResursa(std::string resursa)
{
    m_resursa = resursa;
}

void Misiune::setCantitateExtrasa(int cantitateExtrasa)
{
    m_cantitateExtrasa = cantitateExtrasa;
}

void Misiune::setCombustibilConsum(int combustibilConsum)
{
    m_combustibilConsum = combustibilConsum;
}

void Misiune::setCostCombustibil(double costCombustibil)
{
    m_costCombustibil = costCombustibil;
}

void Misiune::setProfitBrut(int profitBrut)
{
    m_profitBrut = profitBrut;
}

void Misiune::setProfitNet(int profitNet)
{
    m_profitNet = profitNet;
}