CXX := g++
CXXSTD := -std=c++23
WARN := -Wall -Wextra -pedantic
OPT := -O2
CPPFLAGS := -Iinclude $(shell pkg-config --cflags opencv4)
CXXFLAGS := $(CXXSTD) $(WARN) $(OPT)
LDFLAGS := $(shell pkg-config --libs opencv4)

TARGET := app
SRCDIR := src
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)