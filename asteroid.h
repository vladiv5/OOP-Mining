#ifndef ASTEROID_H
#define ASTEROID_H
#include <string>

class Asteroid
{
private:
    std::string m_id;
    std::string m_resursa;
    double m_cantitateTotala;
    int m_randamentExtractie;
    int m_distanta;

public:
    // Constructors
    Asteroid(std::string id, std::string resursa, double cantitateTotala, int randamentExtractie, int distanta);
    Asteroid(const std::string& line);

    // Getters
    std::string getId() const;
    std::string getResursa() const;
    double getCantitateTotala() const;
    int getRandamentExtractie() const;
    int getDistanta() const;

    // Setters
    void setId(std::string id);
    void setResursa(std::string resursa);
    void setCantitateTotala(double cantitateTotala);
    void setRandamentExtractie(int randamentExtractie);
    void setDistanta(int distanta);
};

#endif // ASTEROID_H