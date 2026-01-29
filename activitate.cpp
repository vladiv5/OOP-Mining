#include "activitate.h"
#include <climits>

// Default constructor
Activitate::Activitate()
{
    m_rachete = std::vector<Racheta>();
    m_asteroizi = std::vector<Asteroid>();
    m_piata = Piata();
    m_misiuni = std::vector<Misiune>();
}

// Constructor
Activitate::Activitate(const std::vector<Racheta>& rachete, const std::vector<Asteroid>& asteroizi, const Piata& piata, const std::vector<Misiune>& misiuni)
    : m_rachete(rachete), m_asteroizi(asteroizi), m_piata(piata), m_misiuni(misiuni)
{
}

// Getters
std::vector<Racheta> Activitate::getRachete() const
{
    return m_rachete;
}

std::vector<Asteroid> Activitate::getAsteroizi() const
{
    return m_asteroizi;
}

Piata Activitate::getPiata() const
{
    return m_piata;
}

std::vector<Misiune> Activitate::getMisiuni() const
{
    return m_misiuni;
}

// Setters
void Activitate::setRachete(const std::vector<Racheta>& rachete)
{
    m_rachete = rachete;
}

void Activitate::setAsteroizi(const std::vector<Asteroid>& asteroizi)
{
    m_asteroizi = asteroizi;
}

void Activitate::setPiata(const Piata& piata)
{
    m_piata = piata;
}

void Activitate::setMisiuni(const std::vector<Misiune>& misiuni)
{
    m_misiuni = misiuni;
}

bool Activitate::citireActivitate(const std::string& fileNameRachete, const std::string& fileNameAsteroizi, const std::string& fileNamePiata)
{
    std::ifstream fisierRachete(fileNameRachete);
    std::ifstream fisierAsteroizi(fileNameAsteroizi);

    if (!fisierRachete.is_open() || !fisierAsteroizi.is_open())
    {
        std::cout << "Eroare la deschiderea fisierelor" << std::endl;
        return false;
    }
    else
    {
        std::string linie; // variabila in care retin linia citita

        // Ignor primul rand din fisierul cu rachete
        std::getline(fisierRachete, linie);
        while(std::getline(fisierRachete, linie)) // citesc linie cu linie pana la sfarsitul fisierului cu rachete
        {
            try
            {
                Racheta racheta(linie); // creez un obiect de tip Racheta cu datele citite din linia curenta
                m_rachete.push_back(racheta); // adaug racheta in vectorul de rachete
            }
            catch (const std::invalid_argument& e)
            {
                std::cerr << "Eroare de conversie la linia: " << linie << " - " << e.what() << std::endl;
            }
        }

        std::string linie1; // variabila in care retin linia citita
        // Ignor primul rand din fisierul cu asteroizi
        std::getline(fisierAsteroizi, linie1);
        while(std::getline(fisierAsteroizi, linie1)) // citesc linie cu linie pana la sfarsitul fisierului cu asteroizi
        {
            try
            {
                Asteroid asteroid(linie); // creez un obiect de tip Asteroid cu datele citite din linia curenta
                m_asteroizi.push_back(asteroid); // adaug asteroidul in vectorul de asteroizi
            }
            catch (const std::invalid_argument& e)
            {
                std::cerr << "Eroare de conversie la linia: " << linie << " - " << e.what() << std::endl;
            }
        }

        try
        {
            m_piata = Piata(fileNamePiata); // setez piata cu continutul citit
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << "Eroare de conversie la continutul pietei: " << e.what() << std::endl;
        }

        fisierRachete.close(); // inchid fisierul cu rachete
        fisierAsteroizi.close(); // inchid fisierul cu asteroizi
        return true;
    }
}

Racheta& Activitate::alegereRacheta(int index)
{
    // Verificăm dacă indexul este valid pentru vectorul de asteroizi
    if (index < 0 || static_cast<size_t>(index) >= m_asteroizi.size()) {
        std::cerr << "Index invalid pentru asteroid!" << std::endl;
        return m_rachete.at(0);  // Returnăm o rachetă implicită sau primul element din vector
    }

    int costMin = INT_MAX;
    int rachetaIndex = -1;
    for (size_t k = 0; k < m_rachete.size(); k++) {
        if (m_rachete.at(k).getCombustibilDisponibil() > 2 * m_asteroizi.at(index).getDistanta() * m_rachete.at(k).getConsumCombustibil() / 1000) { // Verificăm dacă racheta curentă are suficient combustibil
            float cost = 2 * m_asteroizi.at(index).getDistanta() * m_rachete.at(k).getConsumCombustibil() * m_rachete.at(k).getPretCombustibil() / 1000; // Calculăm costul combustibilului
            if (cost < costMin) {
                costMin = cost;
                rachetaIndex = k;
            }
        }
    }

    // Dacă nu găsim o rachetă validă, ar putea fi un caz de eroare
    if (rachetaIndex == -1) {
        std::cerr << "Nu s-a gasit o racheta adecvata!" << std::endl;
        return m_rachete.at(0);  // Returnăm o rachetă implicită
    }

    return m_rachete.at(rachetaIndex);
}


void Activitate::actualizareCantitateTotalaAsteroid(int index, double cantitateExtrasa)
{
    double cantitateRamasa = m_asteroizi.at(index).getCantitateTotala() - cantitateExtrasa * 100 / m_asteroizi.at(index).getRandamentExtractie(); // calculez cantitatea ramasa de resurse
    m_asteroizi.at(index).setCantitateTotala(cantitateRamasa); // actualizez cantitatea totala de resurse
}

void Activitate::actualizareResursaMinata(std::string resursaMinata, double cantitateExtrasa)
{
    int indexResursa = -1;

    // Cautăm manual resursa în vectorul de resurse
    for (size_t i = 0; i < m_piata.getResurse().size(); i++)
    {
        if (m_piata.getResurse()[i] == resursaMinata)
        {
            indexResursa = static_cast<int>(i); // Salvăm indexul resursei găsite
            break;
        }
    }

    if (indexResursa == -1)
    {
        std::cout << "Eroare: Resursa " << resursaMinata << " nu a fost gasita!" << std::endl;
        return;
    }
    //std::cout << indexResursa <<std::endl;

    // Actualizăm cantitatea resursei minate
    //std::cout << m_piata.getResursaMinata().size() << std::endl;
    m_piata.getResursaMinata().at(indexResursa) += cantitateExtrasa;
}




int Activitate::calculPretResursaMinata(std::string resursa, double cantitateExtrasa)
{
    int i = -1; // Initializam cu un index invalid
    for (size_t j = 0; j < m_piata.getResurse().size(); ++j) // Parcurgem vectorul de resurse
    {
        if (m_piata.getResurse()[j] == resursa) // Dacă găsim resursa
        {
            i = j; // Retinem indexul
            break; // Ieșim din buclă
        }
    }

    if (i == -1) // Dacă resursa nu a fost găsită
    {
        std::cout << "Eroare: Resursa " << resursa << " nu a fost gasita!" << std::endl;
        return 0; // Returnăm 0 pentru a semnala că nu am găsit resursa
    }

    double pretMaxim = m_piata.getMaxPret().at(i);

    // Calculăm numărul de praguri de 10000 kg depășite
    double cantitateTotalaMinata = m_piata.getResursaMinata().at(i);
    int numarPraguri = static_cast<int>(cantitateTotalaMinata / 10000);
    double cantitateTotalaMinataFaraUltimaResursa = m_piata.getResursaMinata().at(i) - cantitateExtrasa;
    int numarPraguriFaraUltimaResursa = static_cast<int>(cantitateTotalaMinataFaraUltimaResursa / 10000);
    double profit = cantitateExtrasa * pretMaxim;

    if (numarPraguri == numarPraguriFaraUltimaResursa)
    {
        for (int i = 0; i < numarPraguri; i++)
        {
            profit *= 0.9;
        }
    }
    else
    {
        double multiplicator = 1;
        for (int i = 0; i < numarPraguriFaraUltimaResursa; i++)
        {
            multiplicator *= 0.9;
        }
        double cantVandutaLaPretulMaiMic = numarPraguri * 10000 - (cantitateTotalaMinata - cantitateExtrasa);
        profit = cantVandutaLaPretulMaiMic * pretMaxim * multiplicator;
        multiplicator *= 0.9;
        profit += (cantitateExtrasa - cantVandutaLaPretulMaiMic) * pretMaxim * multiplicator;
    }

    // Calculăm prețul actualizat

    return profit;
}


void Activitate::creareMisiuni()
{
    try 
    {
        if (m_asteroizi.empty())
        {
            std::cerr << "Vectorul de asteroizi este gol." << std::endl;
            return;
        }

        if (m_rachete.empty())
        {
            std::cerr << "Vectorul de rachete este gol." << std::endl;
            return;
        }

        //std::cout << "Numar de asteroizi: " << m_asteroizi.size() << std::endl;
        //std::cout << "Numar de rachete: " << m_rachete.size() << std::endl;

        for (size_t i = 0; i < m_asteroizi.size(); i++) // parcurg vectorul de asteroizi
        {
            bool continuare = true;
            while (continuare)
            {
                int profitBrut = 0;
                Racheta& rachetaEficienta = alegereRacheta(i); // aleg cea mai eficienta racheta pentru asteroidul curent
                double cantitateTotalaExtrasa = m_asteroizi.at(i).getRandamentExtractie() * m_asteroizi.at(i).getCantitateTotala() / 100; // calculez cantitatea extrasa de resurse
                double cantitateExtrasa;
                std::string resursa = m_asteroizi.at(i).getResursa(); // retin resursa extrasa

                //std::cout << "Procesare asteroid ID: " << m_asteroizi[i].getId() << ", Resursa: " << resursa << std::endl;


                double consumCombustibil = 2 * m_asteroizi.at(i).getDistanta() * rachetaEficienta.getConsumCombustibil() / 1000; // calculez consumul de combustibil
                rachetaEficienta.setCombustibilDisponibil(rachetaEficienta.getCombustibilDisponibil() - consumCombustibil); // actualizez combustibilul disponibil

                if (cantitateTotalaExtrasa > rachetaEficienta.getCapacitate()) // cantitatea extrasa este mai mare decat capacitatea rachetei
                {
                    cantitateExtrasa = rachetaEficienta.getCapacitate(); // cantitatea extrasa este egala cu capacitatea rachetei
                    actualizareResursaMinata(resursa, cantitateExtrasa); // actualizez resursele minate
                    actualizareCantitateTotalaAsteroid(i, cantitateExtrasa); // actualizez cantitatea totala de resurse
                    profitBrut = calculPretResursaMinata(resursa, cantitateExtrasa);
                    m_piata.getProfitBrut().at(i) += profitBrut; // actualizez profitul brut
                }
                else // cantitatea extrasa este mai mica decat capacitatea rachetei
                {
                    cantitateExtrasa = cantitateTotalaExtrasa; // cantitatea extrasa este egala cu cantitatea totala extrasa
                    actualizareResursaMinata(resursa, cantitateExtrasa); // actualizez resursele minate

                    actualizareCantitateTotalaAsteroid(i, cantitateExtrasa); // actualizez cantitatea totala de resurse
                    profitBrut = calculPretResursaMinata(resursa, cantitateExtrasa);
                    m_piata.getProfitBrut().at(i) += profitBrut; // actualizez profitul brut in piata

                    // Verific daca asteroidul mai are resurse
                    if (i + 1 < m_asteroizi.size() && m_asteroizi.at(i + 1).getId() == m_asteroizi.at(i).getId()) // Verific ca nu trec de ultimul asteroid si ca asteroidul urmator are acelasi id
                    {
                        bool ok = false;
                        while (i + 1 < m_asteroizi.size() && cantitateTotalaExtrasa < rachetaEficienta.getCapacitate() && m_asteroizi.at(i + 1).getId() == m_asteroizi.at(i).getId()) // continuăm să extragem de pe același asteroid
                        {
                            // Schimbăm resursa pentru a mina resursa următoare de pe același asteroid
                            if (m_asteroizi.at(i + 1).getResursa() != m_asteroizi.at(i).getResursa()) 
                            {
                                 resursa += "&" + m_asteroizi.at(i + 1).getResursa();  // Adăugăm resursa nouă la lista de resurse extrase
                            }
                        
                        double temp = cantitateTotalaExtrasa;
                        cantitateTotalaExtrasa += m_asteroizi.at(i + 1).getRandamentExtractie() * m_asteroizi.at(i + 1).getCantitateTotala() / 100; // calculăm cantitatea extrasa de resurse
                        cantitateExtrasa = cantitateTotalaExtrasa - temp;
                        actualizareResursaMinata(m_asteroizi.at(i + 1).getResursa(), cantitateExtrasa); // actualizăm resursele minate
                        actualizareCantitateTotalaAsteroid(i + 1, cantitateExtrasa); // actualizăm cantitatea totală de resurse
                        double profitBrut2 = calculPretResursaMinata(m_asteroizi.at(i + 1).getResursa(), cantitateExtrasa); // adăugăm la profitul brut
                        m_piata.getProfitBrut().at(i + 1) += profitBrut2; // actualizăm profitul brut
                        profitBrut += profitBrut2; // adăugăm la profitul brut
        
                        i++; // Trecem la următorul asteroid
                        ok = true;
                    }

                        if (ok) // Am extras resurse de pe un alt asteroid
                        {
                            if (cantitateTotalaExtrasa > rachetaEficienta.getCapacitate())
                            {
                                cantitateExtrasa = rachetaEficienta.getCapacitate();
                            }
                            else
                            {
                                cantitateExtrasa = cantitateTotalaExtrasa;
                                continuare = false; // Nu mai extragem resurse
                            }
                        }
                    }
                    else continuare = false; // Nu mai extragem resurse
                }

                int profitNet = profitBrut - consumCombustibil * rachetaEficienta.getPretCombustibil(); // calculez profitul net
                double costCombustibil = consumCombustibil * rachetaEficienta.getPretCombustibil(); // calculez costul combustibilului
                // Notez toate datele in misiune
                Misiune newMisiune;
                newMisiune.setIdRacheta(rachetaEficienta.getId());
                newMisiune.setIdAsteroid(m_asteroizi.at(i).getId());
                newMisiune.setResursa(resursa);
                newMisiune.setCantitateExtrasa(cantitateExtrasa);
                newMisiune.setCombustibilConsum(consumCombustibil);
                newMisiune.setCostCombustibil(costCombustibil);
                newMisiune.setProfitBrut(profitBrut);
                newMisiune.setProfitNet(profitNet);
                m_misiuni.push_back(newMisiune); // Adaugam misiunea in vectorul de misiuni

                // Debug output
                /*std::cout << "Misiune adaugata: ID Racheta: " << newMisiune.getIdRacheta()
                          << ", ID Asteroid: " << newMisiune.getIdAsteroid()
                          << ", Resursa: " << newMisiune.getResursa()
                          << ", Cantitate Extrasa: " << newMisiune.getCantitateExtrasa()
                          << ", Consum Combustibil: " << newMisiune.getCombustibilConsum()
                          << ", Cost Combustibil: " << newMisiune.getCostCombustibil()
                          << ", Profit Brut: " << newMisiune.getProfitBrut()
                          << ", Profit Net: " << newMisiune.getProfitNet() << std::endl;*/

            }
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "A aparut o exceptie: " << e.what() << std::endl;
    } 
    catch (...) 
    {
        std::cerr << "A aparut o exceptie necunoscută." << std::endl;
    }
}

void Activitate::afisareMisiuni(const std::string& filePath) const
{
    std::ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        std::cerr << "Nu s-a putut deschide fișierul pentru scriere: " << filePath << std::endl;
        return;
    }

    outFile << "ID Rachetă;ID Asteroid;Resursă;Cantitate extrasă (kg);Combustibil consumat (kg);Cost combustibil ($);Profit brut ($);Profit net ($)\n";
    for (size_t i = 0; i < m_misiuni.size(); i++)
    {
        const Misiune& misiune = m_misiuni[i];
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

void Activitate::afisareProfitTotalSiClasament(const std::string& filePath1, const std::string& filePath2)
{
    std::ofstream outFile(filePath1);
    if (!outFile.is_open())
    {
        std::cerr << "Nu s-a putut deschide fișierul pentru scriere: " << filePath1 << std::endl;
        return;
    }

    // Structură pentru a reține informațiile despre rachete pentru clasament
    struct RachetaInfo
    {
        std::string id;
        double profitNet;
        int numarMisiuni;
        double eficienta;
    };

    // Map pentru a reține informațiile despre rachete
    std::vector<RachetaInfo> racheteInfo;

    outFile << "ID Rachetă;Profit brut ($);Cost total combustibil ($);Profit net ($)\n";
    for (size_t i = 0; i < m_rachete.size(); i++)
    {
        double profitBrut = 0;
        double costTotalCombustibil = 0;
        std::string id = m_rachete[i].getId();
        int numarMisiuni = 0;

        for (size_t j = 0; j < m_misiuni.size(); j++)
        {
            if (id == m_misiuni[j].getIdRacheta())
            {
                profitBrut += m_misiuni[j].getProfitBrut();
                costTotalCombustibil += m_misiuni[j].getCostCombustibil();
                numarMisiuni++;
            }
        }

        double profitNet = profitBrut - costTotalCombustibil;
        outFile << id << ";"
                << std::fixed << std::setprecision(0) << profitBrut << ";"
                << std::fixed << std::setprecision(0) << costTotalCombustibil << ";"
                << std::fixed << std::setprecision(0) << profitNet << std::endl;

        // Adăugăm informațiile în map
        racheteInfo.push_back({id, profitNet, numarMisiuni, numarMisiuni > 0 ? profitNet / numarMisiuni : 0.0});

    }
    outFile.close();

    // Sortăm rachetele în funcție de eficiență
    std::sort(racheteInfo.begin(), racheteInfo.end(), [](const RachetaInfo& a, const RachetaInfo& b) {
        return a.eficienta > b.eficienta;
    });

    std::ofstream clasamentFile(filePath2);

    if (!clasamentFile.is_open())
    {
        std::cerr << "Nu s-a putut deschide fișierul pentru scriere: output/clasament_rachete.csv" << std::endl;
        return;
    }

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

void Activitate::afisareProfitPeResursa(const std::string& filePath)
{
    std::ofstream outFile(filePath);
    if (!outFile.is_open())
    {
        std::cerr << "Nu s-a putut deschide fișierul pentru scriere: " << filePath << std::endl;
        return;
    }

    outFile << "Resursă;Cantitate extrasă (kg);Profit brut ($)\n";
    for (size_t i = 0; i < m_piata.getResurse().size(); i++)
    {
        outFile << m_piata.getResurse()[i] << ";"
                << m_piata.getResursaMinata()[i] << ";"
                << m_piata.getProfitBrut()[i] << "\n";
    }

    outFile.close();
}


