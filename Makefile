

executable := vortex

include_dir := include
output_dir := output
source_dir := src

# srcs := $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
srcs := $(shell find $(source_dir) -type f -name '*.cpp') main.cpp
objs := $(patsubst %.cpp,$(output_dir)/%.o, $(srcs))

std := c++20
flags := -O2 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion 

CXX := g++
CXXFLAGS := -I $(include_dir) -std=$(std) $(flags)

VPATH := $(shell find $(source_dir) -type d) .

define compile-object
	@ echo "Compiling $<"
	@ mkdir -p $(dir $@)
	@ $(CXX) $(CXXFLAGS) -c $< -o $@
endef

.PHONY: clean

$(executable): $(objs)
	$(CXX) $(objs) -o $@

$(output_dir)/%.o: %.cpp 
	$(compile-object)

$(output_dir)/%.o: */%.cpp 
	$(compile-object)

$(output_dir)/%.o: src/*/%.cpp 
	$(compile-object)

clean:
	@ rm $(executable) | true
	@ rm -rf $(output_dir) | true
