# Compiler and flags
CXX = g++
CXXFLAGS = -g -I. $(shell pkg-config --cflags gtkmm-3.0)
LDFLAGS = $(shell pkg-config --libs gtkmm-3.0) -lxlsxwriter -lcjson -lsigc-3.0 -ljsoncpp

# Source files
SOURCES = recipe_manager.cpp main.c cJSON.c
OBJECTS = $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.cpp=.o)

# Targets
.PHONY: all clean

all: recipe_manager clean

recipe_manager: $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C files with C++ compiler (for compatibility)
%.o: %.c
	$(CXX) $(CXXFLAGS) -x c -c $< -o $@

clean:
	rm -f $(OBJECTS)
