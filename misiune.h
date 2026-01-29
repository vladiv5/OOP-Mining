#ifndef MISIUNE_H
#define MISIUNE_H
#include <string>
#include "racheta.h"
#include "asteroid.h"

// I defined the Mission class to act as a data structure (DTO) for logging completed operations.
class Misiune
{
private:
    std::string m_idRacheta;
    std::string m_idAsteroid;
    std::string m_resursa;
    int m_cantitateExtrasa;
    int m_combustibilConsum;
    double m_costCombustibil;
    int m_profitBrut;
    int m_profitNet;

public:
    // Constructor
    Misiune();
    Misiune(std::string idRacheta ,std::string idAsteroid ,std::string resursa, int cantitateExtrasa, int combustibilConsum, double costCombustibil, int profitBrut, int profitNet);

    // Getters
    std::string getIdRacheta() const;
    std::string getIdAsteroid() const;
    std::string getResursa() const;
    int getCantitateExtrasa() const;
    int getCombustibilConsum() const;
    double getCostCombustibil() const;
    int getProfitBrut() const;
    int getProfitNet() const;

    // Setters
    void setIdRacheta(std::string idRacheta);
    void setIdAsteroid(std::string idAsteroid);
    void setResursa(std::string resursa);
    void setCantitateExtrasa(int cantitateExtrasa);
    void setCombustibilConsum(int combustibilConsum);
    void setCostCombustibil(double costCombustibil);
    void setProfitBrut(int profitBrut);
    void setProfitNet(int profitNet);
};

#endif // MISIUNE_H