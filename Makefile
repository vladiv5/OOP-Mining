# Variabile pentru compilare
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Directoare
INPUT_DIR = input
OUTPUT_DIR = output

# Surse și obiecte
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Executabil
EXECUTABLE = program

# Detectăm sistemul de operare
ifeq ($(OS),Windows_NT)
    RM_CMD = del
else
    RM_CMD = rm -f
endif

# Regula implicită
all: $(EXECUTABLE)

# Compilare program
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rulare script Python
run_script: $(EXECUTABLE)
	@python3 process_asteroids.py ./$(EXECUTABLE) || (echo "Test failed!"; exit 1)

# Curățare
clean:
	@$(RM_CMD) -f $(OBJECTS) $(EXECUTABLE) misiuni.csv profit_resursa.csv profit_total.csv clasament_rachete.csv

# Recompilare
rebuild: clean all

.PHONY: all clean rebuild run_script
