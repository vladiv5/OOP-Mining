CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = program

SRCS = main.cpp activitate.cpp racheta.cpp asteroid.cpp piata.cpp misiune.cpp
OBJS = $(SRCS:.cpp=.o)

ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    EXE = program.exe
    MKDIR_OUTPUT = if not exist output mkdir output
else
    RM = rm -f
    EXE = ./program
    MKDIR_OUTPUT = mkdir -p output
endif

# Regula implicita: Compileaza tot
all: $(TARGET)

# Linkare executabil
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compilare fisiere obiect
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- COMENZI TESTARE ---

# Ruleaza TOATE testele (0-6)
run_all: $(TARGET)
	@echo "========================================"
	@echo "       STARTING ALL TESTS (0-6)         "
	@echo "========================================"
	@$(MKDIR_OUTPUT)
	
	@echo "[RUNNING TEST 0]"
	$(EXE) input/test0/rachete.csv input/test0/asteroizi.csv input/test0/piata.csv
	@echo "----------------------------------------"
	
	@echo "[RUNNING TEST 1]"
	$(EXE) input/test1/rachete.csv input/test1/asteroizi.csv input/test1/piata.csv
	@echo "----------------------------------------"
	
	@echo "[RUNNING TEST 2]"
	$(EXE) input/test2/rachete.csv input/test2/asteroizi.csv input/test2/piata.csv
	@echo "----------------------------------------"
	
	@echo "[RUNNING TEST 3]"
	$(EXE) input/test3/rachete.csv input/test3/asteroizi.csv input/test3/piata.csv
	@echo "----------------------------------------"
	
	@echo "[RUNNING TEST 4]"
	$(EXE) input/test4/rachete.csv input/test4/asteroizi.csv input/test4/piata.csv
	@echo "----------------------------------------"
	
	@echo "[RUNNING TEST 5]"
	$(EXE) input/test5/rachete.csv input/test5/asteroizi.csv input/test5/piata.csv
	@echo "----------------------------------------"
	
	@echo "[RUNNING TEST 6]"
	$(EXE) input/test6/rachete.csv input/test6/asteroizi.csv input/test6/piata.csv
	@echo "========================================"
	@echo "       ALL TESTS COMPLETED SUCCESSFULY  "
	@echo "========================================"

# Teste Individuale (pentru verificare punctuala)
test0: $(TARGET)
	@$(MKDIR_OUTPUT)
	$(EXE) input/test0/rachete.csv input/test0/asteroizi.csv input/test0/piata.csv

test1: $(TARGET)
	@$(MKDIR_OUTPUT)
	$(EXE) input/test1/rachete.csv input/test1/asteroizi.csv input/test1/piata.csv

test2: $(TARGET)
	@$(MKDIR_OUTPUT)
	$(EXE) input/test2/rachete.csv input/test2/asteroizi.csv input/test2/piata.csv

test3: $(TARGET)
	@$(MKDIR_OUTPUT)
	$(EXE) input/test3/rachete.csv input/test3/asteroizi.csv input/test3/piata.csv

test4: $(TARGET)
	@$(MKDIR_OUTPUT)
	$(EXE) input/test4/rachete.csv input/test4/asteroizi.csv input/test4/piata.csv

test5: $(TARGET)
	@$(MKDIR_OUTPUT)
	$(EXE) input/test5/rachete.csv input/test5/asteroizi.csv input/test5/piata.csv

test6: $(TARGET)
	@$(MKDIR_OUTPUT)
	$(EXE) input/test6/rachete.csv input/test6/asteroizi.csv input/test6/piata.csv

# Curatenie
clean:
	$(RM) *.o $(TARGET)