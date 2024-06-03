CXX = g++
CXXFLAGS = -O2 -std=c++11
SRC = src/
OBJ = obj/
BIN = bin/
RESULTS = results/
TABU_SRC = src/tabu_search/
GENETIC_SRC = src/genetic_algorithm/

# Detect platform
ifeq ($(OS),Windows_NT)
    RM = rmdir /S /Q
    MKDIR_BIN = if not exist $(subst /,\,$(BIN)) mkdir $(subst /,\,$(BIN))
    MKDIR_OBJ = if not exist $(subst /,\,$(OBJ)) mkdir $(subst /,\,$(OBJ))
	MKDIR_RESULTS = if not exist $(subst /,\,$(RESULTS)) mkdir $(subst /,\,$(RESULTS))
    EXT = .exe
else
    RM = rm -rf
    MKDIR_BIN = mkdir -p $(BIN)
    MKDIR_OBJ = mkdir -p $(OBJ)
	MKDIR_RESULTS = mkdir -p $(RESULTS)
    EXT =
endif

all: directories $(BIN)little_algorithm$(EXT) $(BIN)tabu_search$(EXT) $(BIN)genetic_algorithm$(EXT)

directories:
	$(MKDIR_BIN)
	$(MKDIR_OBJ)
	$(MKDIR_RESULTS)

$(BIN)little_algorithm$(EXT): $(SRC)little_algorithm.cpp $(SRC)utils.cpp $(SRC)utils.h $(SRC)thread_pool.cpp $(SRC)thread_pool.h
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN)tabu_search$(EXT): $(TABU_SRC)main.cpp $(TABU_SRC)tabu_search.cpp $(TABU_SRC)tabu_search.h $(TABU_SRC)solution.cpp $(TABU_SRC)solution.h $(TABU_SRC)random.h
	$(CXX) $(CXXFLAGS) -o $@ $(TABU_SRC)main.cpp $(TABU_SRC)tabu_search.cpp $(TABU_SRC)solution.cpp

$(BIN)genetic_algorithm$(EXT): $(GENETIC_SRC)main.cpp $(GENETIC_SRC)ae.cpp $(GENETIC_SRC)ae.h $(GENETIC_SRC)population.cpp $(GENETIC_SRC)population.h $(GENETIC_SRC)random.h $(GENETIC_SRC)chromosome.cpp $(GENETIC_SRC)chromosome.h
	$(CXX) $(CXXFLAGS) -o $@ $(GENETIC_SRC)main.cpp $(GENETIC_SRC)ae.cpp $(GENETIC_SRC)population.cpp $(GENETIC_SRC)chromosome.cpp

clean:
	$(RM) $(BIN) $(OBJ) $(RESULTS)