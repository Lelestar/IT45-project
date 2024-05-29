CXX = g++
CXXFLAGS = -O2 -std=c++11
SRC = src/
OBJ = obj/
BIN = bin/
RESULTS = results/

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

all: directories $(BIN)little_algorithm$(EXT)

directories:
	$(MKDIR_BIN)
	$(MKDIR_OBJ)
	$(MKDIR_RESULTS)

$(BIN)little_algorithm$(EXT): $(SRC)little_algorithm.cpp $(SRC)utils.cpp $(SRC)utils.h
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) $(BIN) $(OBJ) $(RESULTS)
