

TARGET := vortex

INCDIR := include
OBJDIR := output
SRCDIR := src

define get_object_name
$(patsubst %.cpp,$(OBJDIR)/%.o, $1)
endef

SOURCES := $(shell find $(SRCDIR) -type f \( -name '*.cpp' -o -name '*.hpp' \) ) main.cpp
OBJECTS := $(foreach source_file, $(SOURCES), $(call get_object_name, $(source_file)))

std := c++20
flags := -O2 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wunused-function

CXX := g++
CXXFLAGS := -I $(INCDIR) -std=$(std) $(flags)

VPATH := $(shell find $(SRCDIR) -type d) .

define compile_object 
$(call get_object_name, $1): $1
	@ echo "Compiling $1"
	@ mkdir -p $(OBJDIR)/$(dir $1)
	@ $(CXX) $(CXXFLAGS) -c $(1) -o $(call get_object_name, $1) 
endef

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(foreach source_file, $(SOURCES), $(eval $(call compile_object, $(source_file))))

clean:
	@ rm $(TARGET) | true
	@ rm -rf $(OBJDIR) | true
