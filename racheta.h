#ifndef RACHETA_H
#define RACHETA_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class Racheta
{
private:
    std::string m_id;
    int m_capacitate;
    int m_consumCombustibil;
    int m_combustibilDisponibil;
    float m_pretCombustibil;

public:
    // Constructors
    Racheta();
    Racheta(std::string id, int capacitate, int consumCombustibil, int combustibilDisponibil, float pretCombustibil);
    Racheta(const std::string& line);
    Racheta& operator=(const Racheta& other);
    // Getters
    std::string getId() const;
    int getCapacitate() const;
    int getConsumCombustibil() const;
    int getCombustibilDisponibil() const;
    float getPretCombustibil() const;

    // Setters
    void setId(std::string id);
    void setCapacitate(int capacitate);
    void setConsumCombustibil(int consumCombustibil);
    void setCombustibilDisponibil(int combustibilDisponibil);
    void setPretCombustibil(float pretCombustibil);
};

#endif // RACHETA_H