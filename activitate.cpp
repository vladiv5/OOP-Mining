#include "activitate.h"
#include <climits>
#include <cfloat> // I included this to use DBL_MAX for proper price comparison.
#include <iostream>

// Default constructor
// I initialize empty vectors to ensure the simulation starts with a clean state.
Activitate::Activitate()
{
    m_rachete = std::vector<Racheta>();
    m_asteroizi = std::vector<Asteroid>();
    m_piata = Piata();
    m_misiuni = std::vector<Misiune>();
}

// Parameterized Constructor
Activitate::Activitate(const std::vector<Racheta>& rachete, const std::vector<Asteroid>& asteroizi, const Piata& piata, const std::vector<Misiune>& misiuni)
    : m_rachete(rachete), m_asteroizi(asteroizi), m_piata(piata), m_misiuni(misiuni)
{
}

// Getters
std::vector<Racheta> Activitate::getRachete() const { return m_rachete; }
std::vector<Asteroid> Activitate::getAsteroizi() const { return m_asteroizi; }
Piata Activitate::getPiata() const { return m_piata; }
std::vector<Misiune> Activitate::getMisiuni() const { return m_misiuni; }

// Setters
void Activitate::setRachete(const std::vector<Racheta>& rachete) { m_rachete = rachete; }
void Activitate::setAsteroizi(const std::vector<Asteroid>& asteroizi) { m_asteroizi = asteroizi; }
void Activitate::setPiata(const Piata& piata) { m_piata = piata; }
void Activitate::setMisiuni(const std::vector<Misiune>& misiuni) { m_misiuni = misiuni; }

// File Parsing Logic
bool Activitate::citireActivitate(const std::string& fileNameRachete, const std::string& fileNameAsteroizi, const std::string& fileNamePiata)
{
    std::ifstream fisierRachete(fileNameRachete);
    std::ifstream fisierAsteroizi(fileNameAsteroizi);

    // I validate that the input files exist before attempting to read them to prevent runtime crashes.
    if (!fisierRachete.is_open() || !fisierAsteroizi.is_open())
    {
        std::cout << "Eroare la deschiderea fisierelor" << std::endl;
        return false;
    }
    else
    {
        std::string linie; 
        std::getline(fisierRachete, linie); // I skip the CSV header to get straight to the data.

        while(std::getline(fisierRachete, linie)) 
        {
            try {
                // I use a try-catch block here to handle potential formatting errors in the CSV lines safely.
                Racheta racheta(linie); 
                m_rachete.push_back(racheta); 
            } catch (const std::invalid_argument& e) {
                std::cerr << "Eroare conversie racheta: " << e.what() << std::endl;
            }
        }

        std::string linie1; 
        std::getline(fisierAsteroizi, linie1); // Skipping header for asteroids file as well.
        
        while(std::getline(fisierAsteroizi, linie1)) 
        {
            try {
                // I explicitly use 'linie1' here to avoid conflicts with the previous buffer.
                Asteroid asteroid(linie1); 
                m_asteroizi.push_back(asteroid); 
            } catch (const std::invalid_argument& e) {
                std::cerr << "Eroare conversie asteroid: " << e.what() << std::endl;
            }
        }

        try {
            m_piata = Piata(fileNamePiata); 
        } catch (const std::invalid_argument& e) {
            std::cerr << "Eroare piata: " << e.what() << std::endl;
        }

        fisierRachete.close(); 
        fisierAsteroizi.close(); 
        return true;
    }
}

// Rocket Selection Algorithm
// I designed this to return a pointer (Racheta*) so I can return nullptr when no suitable rocket is found.
Racheta* Activitate::alegereRacheta(int index)
{
    // I perform a boundary check on the asteroid index to prevent out-of-bounds access.
    if (index < 0 || static_cast<size_t>(index) >= m_asteroizi.size()) {
        return nullptr;
    }

    double costMin = DBL_MAX; // I use DBL_MAX to ensure any valid calculated cost will be lower.
    int rachetaIndex = -1;

    for (size_t k = 0; k < m_rachete.size(); k++) {
        // I calculate if the rocket has enough fuel for a round trip (distance * 2).
        if (m_rachete.at(k).getCombustibilDisponibil() >= 2.0 * m_asteroizi.at(index).getDistanta() * m_rachete.at(k).getConsumCombustibil() / 1000.0) { 
            double cost = 2.0 * m_asteroizi.at(index).getDistanta() * m_rachete.at(k).getConsumCombustibil() * m_rachete.at(k).getPretCombustibil() / 1000.0; 
            
            // I select the rocket that minimizes the financial cost, not just fuel consumption.
            if (cost < costMin) {
                costMin = cost;
                rachetaIndex = k;
            }
        }
    }

    if (rachetaIndex == -1) {
        // If no rocket is found, I return nullptr to signal the simulation loop to stop for this asteroid.
        return nullptr;
    }

    return &m_rachete.at(rachetaIndex);
}

void Activitate::actualizareCantitateTotalaAsteroid(int index, double cantitateExtrasa)
{
    // I update the remaining resources on the asteroid, accounting for extraction yield.
    double cantitateRamasa = m_asteroizi.at(index).getCantitateTotala() - cantitateExtrasa * 100 / m_asteroizi.at(index).getRandamentExtractie(); 
    if (cantitateRamasa < 0) cantitateRamasa = 0;
    m_asteroizi.at(index).setCantitateTotala(cantitateRamasa); 
}

void Activitate::actualizareResursaMinata(std::string resursaMinata, double cantitateExtrasa)
{
    // I search for the resource in the market and update the total mined quantity.
    for (size_t i = 0; i < m_piata.getResurse().size(); i++)
    {
        if (m_piata.getResurse()[i] == resursaMinata)
        {
            m_piata.getResursaMinata().at(i) += cantitateExtrasa;
            return;
        }
    }
    std::cout << "Eroare: Resursa " << resursaMinata << " nu a fost gasita!" << std::endl;
}

// Dynamic Pricing Logic
int Activitate::calculPretResursaMinata(std::string resursa, double cantitateExtrasa)
{
    int i = -1; 
    // I find the index of the resource in the market data.
    for (size_t j = 0; j < m_piata.getResurse().size(); ++j) 
    {
        if (m_piata.getResurse()[j] == resursa) 
        {
            i = j; 
            break; 
        }
    }

    if (i == -1) return 0;

    double pretMaxim = m_piata.getMaxPret().at(i);
    double cantitateTotalaMinata = m_piata.getResursaMinata().at(i);
    
    // I calculate how many 10,000kg thresholds have been passed to determine the price drop.
    int numarPraguri = static_cast<int>(cantitateTotalaMinata / 10000);
    double cantitateAnterioara = cantitateTotalaMinata - cantitateExtrasa;
    int numarPraguriAnterior = static_cast<int>(cantitateAnterioara / 10000);
    
    // I calculate the current price based on previous saturation.
    double pretCurent = pretMaxim;
    for(int k=0; k<numarPraguriAnterior; k++) pretCurent *= 0.9;
    
    // I ensure the price never drops below the minimum market price.
    if(pretCurent < m_piata.getMinPret().at(i)) pretCurent = m_piata.getMinPret().at(i);

    double profit = 0;
    if (numarPraguri == numarPraguriAnterior) {
        // If we haven't crossed a new threshold, the price remains stable for this batch.
        profit = cantitateExtrasa * pretCurent;
    } else {
        // If we cross a threshold, I split the batch: part is sold at the old price, part at the new lower price.
        double cantitatePanaLaPrag = (numarPraguriAnterior + 1) * 10000 - cantitateAnterioara;
        profit += cantitatePanaLaPrag * pretCurent;
        
        double cantitateDupaPrag = cantitateExtrasa - cantitatePanaLaPrag;
        double pretNou = pretCurent * 0.9;
        if(pretNou < m_piata.getMinPret().at(i)) pretNou = m_piata.getMinPret().at(i);
        profit += cantitateDupaPrag * pretNou;
    }

    return static_cast<int>(profit);
}

// Main Simulation Loop
void Activitate::creareMisiuni()
{
    if (m_asteroizi.empty() || m_rachete.empty()) return;

    for (size_t i = 0; i < m_asteroizi.size(); i++) 
    {
        bool continuare = true;
        while (continuare)
        {
            // I utilize the pointer check to safely handle fleet depletion.
            Racheta* rachetaPtr = alegereRacheta(i);
            if (rachetaPtr == nullptr) {
                continuare = false; 
                break; // Stop loop to avoid infinite cycle or crash if no rockets are available.
            }
            Racheta& rachetaEficienta = *rachetaPtr;

            int profitBrut = 0;
            double cantitateTotalaExtrasa = m_asteroizi.at(i).getRandamentExtractie() * m_asteroizi.at(i).getCantitateTotala() / 100; 
            
            // I added a small threshold check to stop mining if resources are negligible.
            if (cantitateTotalaExtrasa <= 0.001) {
                continuare = false;
                break;
            }

            double cantitateExtrasa;
            std::string resursa = m_asteroizi.at(i).getResursa(); 

            double consumCombustibil = 2.0 * m_asteroizi.at(i).getDistanta() * rachetaEficienta.getConsumCombustibil() / 1000.0; 
            rachetaEficienta.setCombustibilDisponibil(rachetaEficienta.getCombustibilDisponibil() - consumCombustibil); 

            // Logic to handle when the asteroid has more resources than the rocket can carry.
            if (cantitateTotalaExtrasa > rachetaEficienta.getCapacitate()) 
            {
                cantitateExtrasa = rachetaEficienta.getCapacitate(); 
                actualizareResursaMinata(resursa, cantitateExtrasa); 
                actualizareCantitateTotalaAsteroid(i, cantitateExtrasa); 
                profitBrut = calculPretResursaMinata(resursa, cantitateExtrasa);
                
                // I iterate to find the correct index to update gross profit, avoiding vector range errors.
                for(size_t k=0; k<m_piata.getResurse().size(); ++k) {
                    if(m_piata.getResurse()[k] == resursa) {
                        m_piata.getProfitBrut().at(k) += profitBrut;
                        break;
                    }
                }
            }
            else 
            {
                // Logic for when the rocket has more capacity than the asteroid has resources.
                cantitateExtrasa = cantitateTotalaExtrasa; 
                actualizareResursaMinata(resursa, cantitateExtrasa); 
                actualizareCantitateTotalaAsteroid(i, cantitateExtrasa); 
                profitBrut = calculPretResursaMinata(resursa, cantitateExtrasa);
                
                // Same fix here for vector range safety.
                for(size_t k=0; k<m_piata.getResurse().size(); ++k) {
                    if(m_piata.getResurse()[k] == resursa) {
                        m_piata.getProfitBrut().at(k) += profitBrut;
                        break;
                    }
                }

                // I implemented a check to see if the rocket can continue to the next asteroid (if it's the same ID/Cluster).
                if (i + 1 < m_asteroizi.size() && m_asteroizi.at(i + 1).getId() == m_asteroizi.at(i).getId()) 
                {
                    while (i + 1 < m_asteroizi.size() && cantitateExtrasa < rachetaEficienta.getCapacitate() && m_asteroizi.at(i + 1).getId() == m_asteroizi.at(i).getId()) 
                    {
                        // If mining a different resource type in the same cluster, I append it to the name.
                        if (m_asteroizi.at(i + 1).getResursa() != m_asteroizi.at(i).getResursa()) {
                             if(resursa.find(m_asteroizi.at(i+1).getResursa()) == std::string::npos)
                                 resursa += "&" + m_asteroizi.at(i + 1).getResursa(); 
                        }
                    
                        double capacitateRamasa = rachetaEficienta.getCapacitate() - cantitateExtrasa;
                        double cantitateDisponibilaNext = m_asteroizi.at(i + 1).getRandamentExtractie() * m_asteroizi.at(i + 1).getCantitateTotala() / 100;
                        
                        double cantitateLuata = (cantitateDisponibilaNext > capacitateRamasa) ? capacitateRamasa : cantitateDisponibilaNext;
                        
                        if (cantitateLuata > 0) {
                            actualizareResursaMinata(m_asteroizi.at(i + 1).getResursa(), cantitateLuata); 
                            actualizareCantitateTotalaAsteroid(i + 1, cantitateLuata); 
                            double profitBrut2 = calculPretResursaMinata(m_asteroizi.at(i + 1).getResursa(), cantitateLuata); 
                            
                            // I update profit for the specific resource index mined in the cluster.
                            for(size_t k=0; k<m_piata.getResurse().size(); ++k) {
                                if(m_piata.getResurse()[k] == m_asteroizi.at(i+1).getResursa()) {
                                    m_piata.getProfitBrut().at(k) += profitBrut2;
                                    break;
                                }
                            }
                            profitBrut += profitBrut2; 
                            cantitateExtrasa += cantitateLuata;
                        }
    
                        // I move to the next asteroid segment if the current one is depleted.
                        if(cantitateDisponibilaNext <= capacitateRamasa) i++; 
                        else break; 
                    }
                }
                continuare = false; 
            }

            int profitNet = profitBrut - consumCombustibil * rachetaEficienta.getPretCombustibil(); 
            double costCombustibil = consumCombustibil * rachetaEficienta.getPretCombustibil(); 
            
            // I log the mission details.
            Misiune newMisiune;
            newMisiune.setIdRacheta(rachetaEficienta.getId());
            newMisiune.setIdAsteroid(m_asteroizi.at(i).getId());
            newMisiune.setResursa(resursa);
            newMisiune.setCantitateExtrasa(cantitateExtrasa);
            newMisiune.setCombustibilConsum(consumCombustibil);
            newMisiune.setCostCombustibil(costCombustibil);
            newMisiune.setProfitBrut(profitBrut);
            newMisiune.setProfitNet(profitNet);
            m_misiuni.push_back(newMisiune); 
        }
    }
}

// Function to export mission logs to CSV.
void Activitate::afisareMisiuni(const std::string& filePath) const
{
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) return;

    outFile << "ID Rachetă;ID Asteroid;Resursă;Cantitate extrasă (kg);Combustibil consumat (kg);Cost combustibil ($);Profit brut ($);Profit net ($)\n";
    for (const auto& misiune : m_misiuni)
    {
        outFile << misiune.getIdRacheta() << ";"
                << misiune.getIdAsteroid() << ";"
                << misiune.getResursa() << ";"
                << misiune.getCantitateExtrasa() << ";"
                << misiune.getCombustibilConsum() << ";"
                << misiune.getCostCombustibil() << ";"
                << misiune.getProfitBrut() << ";"
                << misiune.getProfitNet() << "\n";
    }
    outFile.close();
}

// Function to generate the financial report and rocket ranking.
void Activitate::afisareProfitTotalSiClasament(const std::string& filePath1, const std::string& filePath2)
{
    std::ofstream outFile(filePath1);
    if (!outFile.is_open()) return;

    // Helper struct to aggregate data for the ranking system.
    struct RachetaInfo
    {
        std::string id;
        double profitNet;
        int numarMisiuni;
        double eficienta;
    };
    std::vector<RachetaInfo> racheteInfo;

    outFile << "ID Rachetă;Profit brut ($);Cost total combustibil ($);Profit net ($)\n";
    for (const auto& racheta : m_rachete)
    {
        double profitBrut = 0;
        double costTotalCombustibil = 0;
        std::string id = racheta.getId();
        int numarMisiuni = 0;

        // I sum up results for each rocket across all missions.
        for (const auto& misiune : m_misiuni)
        {
            if (id == misiune.getIdRacheta())
            {
                profitBrut += misiune.getProfitBrut();
                costTotalCombustibil += misiune.getCostCombustibil();
                numarMisiuni++;
            }
        }

        double profitNet = profitBrut - costTotalCombustibil;
        outFile << id << ";"
                << std::fixed << std::setprecision(0) << profitBrut << ";"
                << std::fixed << std::setprecision(0) << costTotalCombustibil << ";"
                << std::fixed << std::setprecision(0) << profitNet << std::endl;

        racheteInfo.push_back({id, profitNet, numarMisiuni, numarMisiuni > 0 ? profitNet / numarMisiuni : 0.0});

    }
    outFile.close();

    // I sort the rockets based on efficiency score (Profit / Missions).
    std::sort(racheteInfo.begin(), racheteInfo.end(), [](const RachetaInfo& a, const RachetaInfo& b) {
        return a.eficienta > b.eficienta;
    });

    std::ofstream clasamentFile(filePath2);
    if (!clasamentFile.is_open()) return;

    clasamentFile << "Loc;ID Racheta;Profit net ($);Numar misiuni;Scor eficienta\n";
    int loc = 1;
    for (const auto& info : racheteInfo)
    {
        clasamentFile << loc << ";"
                      << info.id << ";"
                      << std::fixed << std::setprecision(0) << info.profitNet << ";"
                      << info.numarMisiuni << ";"
                      << std::fixed << std::setprecision(0) << info.eficienta << "\n";
        loc++;
    }
    clasamentFile.close();
}

// Function to export per-resource profit stats.
void Activitate::afisareProfitPeResursa(const std::string& filePath)
{
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) return;

    outFile << "Resursă;Cantitate extrasă (kg);Profit brut ($)\n";
    for (size_t i = 0; i < m_piata.getResurse().size(); i++)
    {
        outFile << m_piata.getResurse()[i] << ";"
                << m_piata.getResursaMinata()[i] << ";"
                << m_piata.getProfitBrut()[i] << "\n";
    }
    outFile.close();
}