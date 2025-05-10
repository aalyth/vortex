

executable := vortex

include_dir := ./include
output_dir := ./output

srcs := $(wildcard *.cpp) $(wildcard */*.cpp)
objs := $(patsubst %.cpp,$(output_dir)/%.o, $(srcs))

std := c++20
flags := -O2 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion 

CXX := g++
CXXFLAGS := -I $(include_dir) -std=$(std) $(flags)

.PHONY: clean

$(executable): $(objs)
	$(CXX) $(objs) -o $@

$(output_dir)/%.o: %.cpp
	@ echo "Compiling $<"
	@ mkdir -p $(dir $@)
	@ $(CXX) $(CXXFLAGS) -c $< -o $@

$(output_dir)/%.o: */*.cpp
	@ echo "Compiling $<"
	@ mkdir -p $(dir $@)
	@ $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@ rm $(executable) | true
	@ rm -rf $(output_dir) | true
