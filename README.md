# Asteroid Mining Simulator

## Description

This project simulates a space mining operation where I modeled the economics of extracting resources from asteroids. The application manages a fleet of rockets, calculates optimal mining strategies based on fuel efficiency and cargo capacity, and simulates a dynamic market economy where resource prices fluctuate based on supply saturation.

I focused on applying Object-Oriented Programming (OOP) principles, using C++ STL containers (vectors), and implementing robust exception handling for CSV parsing.

---

## Data Structures (Classes)

- **Racheta (Rocket)**
  - Represents a mining vessel.
  - Stores properties like fuel capacity, consumption rate, and operational costs.
  - I implemented logic here to track fuel usage per mission.

- **Asteroid**
  - Represents the target celestial body.
  - Contains:
    - Resource type (e.g., Gold, Iron)
    - Total quantity available
    - Extraction yield percentage
    - Distance from the base

- **Piata (Market)**
  - Manages the economy of the simulation.
  - I implemented a dynamic pricing algorithm here: prices drop as more resources are mined (supply & demand), constrained by minimum and maximum market values.

- **Misiune (Mission)**
  - Acts as a Data Transfer Object (DTO).
  - Logs the details of every successful trip: specific rocket used, costs incurred, and net profit generated.

- **Activitate (Activity)**
  - The main controller class.
  - Manages vectors of Rockets, Asteroids, and Missions.
  - Contains the core algorithms for vehicle selection and simulation orchestration.

---

## Program Flow

1. **Initialization**  
   I read configuration data (rockets, asteroids, market prices) from three separate CSV files using `std::ifstream` and parse them into objects.

2. **Simulation Loop**  
   I iterate through the list of asteroids to perform mining operations.

3. **Optimal Selection**  
   For each asteroid, I calculate which available rocket is the most cost-efficient (lowest fuel cost) for the round trip using a pointer-based selection method.

4. **Extraction & Logistics**
   - If a rocket is found, I calculate the extracted amount based on yield.
   - I handle cases where the rocket capacity is smaller than the asteroid's yield (partial extraction) or where one rocket mines multiple asteroids in a cluster.

5. **Economic Simulation**
   - I calculate the gross profit using a dynamic formula: the price drops by 10% for every 10,000 kg of a resource previously mined.
   - I compute the net profit by subtracting fuel costs.

6. **Reporting**
   - Every action is logged into a `Misiune` object.
   - Finally, I generate detailed CSV reports: a general mission log, a profit breakdown per resource, and a leaderboard ranking rockets by efficiency.

---

## Project Structure

Asteroid-Mining-Simulator/
│
├── main.cpp                  # Program entry point and argument validation
├── activitate.cpp / .h        # Main controller and business logic
├── racheta.cpp / .h           # Rocket entity definition
├── asteroid.cpp / .h          # Asteroid entity definition
├── piata.cpp / .h             # Market economy logic
├── misiune.cpp / .h           # Mission logging structure
├── Makefile                   # Automation for Linux/Unix builds
├── run_all.bat                # Automation script for Windows (Build & Test)
├── input/
│   ├── test0/ .. test6/       # Test scenarios provided for validation
│   └── output/                # Generated CSV reports (created automatically)

The `input/` directory contains various scenarios, from basic single-rocket tests to complex multi-asteroid simulations.

---

## Build & Run

I have provided two ways to build and run the project, ensuring cross-platform compatibility.

### Option 1: Windows (Recommended)

A batch script that compiles the code, creates the output directory, and runs all 7 test cases sequentially:

.\run_all.bat


### Option 2: Linux / Manual (Makefile)

To build the project:

make

To run a specific test case (e.g., Test 3):

./program input/test3/rachete.csv input/test3/asteroizi.csv input/test3/piata.csv


---

## Verification

The simulation generates four output files in the `output/` folder (or specific test folders if using the batch script):

- `misiuni.csv` – Detailed logs of every trip
- `profit_total.csv` – Summary of costs and revenues per rocket
- `profit_pe_resursa.csv` – Total quantity and value extracted per resource type
- `clasament_rachete.csv` – Rockets ranked by efficiency score (Profit / Number of Missions)

Correctness can be verified by checking if the net profit matches the logical calculations based on distance, fuel cost, and market saturation rules.

---

## Notes

- **Language Standard**: C++17
- **Memory Management**: I utilized `std::vector` for automatic memory management, but also used raw pointers in the rocket selection logic to efficiently handle "not found" scenarios without object copying.
- **Robustness**: Try-catch blocks are implemented throughout the file reading process to prevent crashes on malformed CSV data.
- **Cross-Platform**: The inclusion of both a Makefile and a `.bat` script demonstrates consideration for different development environments.
