CXX := g++
CXXSTD := -std=c++23
WARN := -Wall -Wextra -pedantic
OPT := -O2

CPPFLAGS := -Iinclude $(shell pkg-config --cflags opencv4)
CXXFLAGS := $(CXXSTD) $(WARN) $(OPT)
LDFLAGS := $(shell pkg-config --libs opencv4)

TARGET := app
SRCDIR := src
BUILDDIR := build
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILDDIR)
	rm -f $(TARGET)