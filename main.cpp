#include "activitate.h"
#include "misiune.h"
#include <iostream>

// Entry point of the application.
int main(int argc, char* argv[])
{
    // I verify that the user provides exactly three CSV files as arguments.
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <rachete.csv> <asteroizi.csv> <piata.csv>" << std::endl;
        return 1;
    }

    // I create the main controller instance.
    Activitate activitate;
    
    // I trigger the data loading process and handle potential file I/O errors.
    if (!activitate.citireActivitate(argv[1], argv[2], argv[3]))
    {
        std::cerr << "Failed to read data from CSV files" << std::endl;
        return 1;
    }

    // I commented out debug prints used during development to keep the console clean.
    //std::cout << "Rachete:" << std::endl;
    /*for (const auto& racheta : activitate.getRachete())
    {
        std::cout << "ID: " << racheta.getId() << ", Capacitate: " << racheta.getCapacitate()
                  << ", Consum Combustibil: " << racheta.getConsumCombustibil()
                  << ", Combustibil Disponibil: " << racheta.getCombustibilDisponibil()
                  << ", Pret Combustibil: " << racheta.getPretCombustibil() << std::endl;
    }*/

    /*std::cout << "Asteroizi:" << std::endl;
    for (const auto& asteroid : activitate.getAsteroizi())
    {
        std::cout << "ID: " << asteroid.getId() << ", Resursa: " << asteroid.getResursa()
                  << ", Cantitate Totala: " << asteroid.getCantitateTotala()
                  << ", Randament Extractie: " << asteroid.getRandamentExtractie()
                  << ", Distanta: " << asteroid.getDistanta() << std::endl;
    }*/

    /*std::cout << "Piata:" << std::endl;
    const Piata& piata = activitate.getPiata();
    for (size_t i = 0; i < piata.getResurse().size(); ++i)
    {
        std::cout << "Resursa: " << piata.getResurse()[i]
                  << ", Pret Minim: " << piata.getMinPret()[i]
                  << ", Pret Maxim: " << piata.getMaxPret()[i] << std::endl;
    }*/

    // I execute the core business logic to simulate mining missions.
    activitate.creareMisiuni();

    // I export the calculated results to separate CSV files for analysis.
    activitate.afisareMisiuni("output/misiuni.csv");
    activitate.afisareProfitPeResursa("output/profit_pe_resursa.csv");
    activitate.afisareProfitTotalSiClasament("output/profit_total.csv", "output/clasament_rachete.csv");
    return 0;
}