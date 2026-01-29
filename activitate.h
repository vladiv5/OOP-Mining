#ifndef ACTIVITATE_H
#define ACTIVITATE_H

#include "racheta.h"
#include "asteroid.h"
#include "piata.h"
#include "misiune.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm> // for sort
#include <iomanip>   // for setprecision

// I defined the main controller class that manages the entire simulation state.
class Activitate
{
private:
    // I store the simulation entities in vectors for dynamic sizing.
    std::vector<Racheta> m_rachete;
    std::vector<Asteroid> m_asteroizi;
    Piata m_piata;
    std::vector<Misiune> m_misiuni;

public:
    // Constructors
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

    // Methods
    bool citireActivitate(const std::string& fileNameRachete, const std::string& fileNameAsteroizi, const std::string& fileNamePiata);
    
    // I changed the return type to a pointer (Racheta*) to handle cases where no rocket is suitable (returning nullptr).
    Racheta* alegereRacheta(int index);
    
    // I implemented these methods to update the simulation state after each mining step.
    void actualizareCantitateTotalaAsteroid(int index, double cantitateExtrasa);
    void actualizareResursaMinata(std::string resursaMinata, double cantitateExtrasa);
    int calculPretResursaMinata(std::string resursa, double cantitateExtrasa);
    
    // This is the core logic method that drives the simulation loop.
    void creareMisiuni();
    
    // Reporting methods
    void afisareMisiuni(const std::string& filePath) const;
    void afisareProfitTotalSiClasament(const std::string& filePath1, const std::string& filePath2);
    void afisareProfitPeResursa(const std::string& filePath);
};

#endif // ACTIVITATE_H