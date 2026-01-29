#ifndef ACTIVITATE_H
#define ACTIVITATE_H

#include "racheta.h"
#include "asteroid.h"
#include "piata.h"
#include "misiune.h"
#include <vector>
#include <algorithm>
#include <iomanip>


class Activitate
{
private:
    std::vector<Racheta> m_rachete;
    std::vector<Asteroid> m_asteroizi;
    Piata m_piata;
    std::vector<Misiune> m_misiuni;

public:
    // Constructor
    Activitate();
    Activitate(const std::vector<Racheta>& rachete, const std::vector<Asteroid>& asteroizi, const Piata& piata, const std::vector<Misiune>& misiuni);

    // Getters
    std::vector<Racheta> getRachete() const;
    std::vector<Asteroid> getAsteroizi() const;
    Piata getPiata() const;
    std::vector<Misiune> getMisiuni() const;

    // Setters
    void setRachete(const std::vector<Racheta>& rachete);
    void setAsteroizi(const std::vector<Asteroid>& asteroizi);
    void setPiata(const Piata& piata);
    void setMisiuni(const std::vector<Misiune>& misiuni);

    bool citireActivitate(const std::string& fileNameRachete, const std::string& fileNameAsteroizi, const std::string& fileNamePiata);
    void creareMisiuni();
    Racheta& alegereRacheta(int index);
    void actualizareCantitateTotalaAsteroid(int index, double cantitateExtrasa);
    void actualizareResursaMinata(std::string resursaMinata, double cantitateExtrasa);
    int calculPretResursaMinata(std::string resursa, double cantitateExtrasa);
    void afisareMisiuni(const std::string& filePath) const;
    void afisareProfitTotalSiClasament(const std::string& filePath1, const std::string& filePath2);
    void afisareProfitPeResursa(const std::string& filePath);
    void clasamentRachete(const std::string& filePath);
};

#endif // ACTIVITATE_H