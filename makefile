CXX = g++
CXXFLAGS = -O2 -std=c++11
SRC = src/
OBJ = obj/
BIN = bin/
RESULTS = results/
LITTLE_SRC = src/little_algorithm/
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

$(BIN)little_algorithm$(EXT): $(LITTLE_SRC)little_algorithm.cpp $(LITTLE_SRC)utils.cpp $(LITTLE_SRC)utils.h $(LITTLE_SRC)thread_pool.cpp $(LITTLE_SRC)thread_pool.h
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN)tabu_search$(EXT): $(TABU_SRC)main.cpp $(TABU_SRC)tabu_search.cpp $(TABU_SRC)tabu_search.h $(TABU_SRC)solution.cpp $(TABU_SRC)solution.h $(TABU_SRC)random.h
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN)genetic_algorithm$(EXT): $(GENETIC_SRC)main.cpp $(GENETIC_SRC)ae.cpp $(GENETIC_SRC)ae.h $(GENETIC_SRC)population.cpp $(GENETIC_SRC)population.h $(GENETIC_SRC)random.h $(GENETIC_SRC)chromosome.cpp $(GENETIC_SRC)chromosome.h
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
ifeq ($(OS),Windows_NT)
	$(RM) $(subst /,\,$(BIN))
	$(RM) $(subst /,\,$(OBJ))
	$(RM) $(subst /,\,$(RESULTS))
else
	$(RM) $(BIN) $(OBJ) $(RESULTS)
endif

# Empty the results directory
clean_results:
ifeq ($(OS),Windows_NT)
	$(RM) $(subst /,\,$(RESULTS))
	$(MKDIR_RESULTS)
else
	$(RM) $(RESULTS)
	$(MKDIR_RESULTS)
endif