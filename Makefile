CXX=clang++
CXXFLAGS=-std=c++17

file :=
ifdef file
obj_file := $(file).o
else
obj_file :=
endif
comp_files := $(obj_file) fiber.o scheduler.o context/context.s

all: main

main: $(comp_files)
ifdef file
	$(CXX) -o $@ $^
else
	@echo "Error: input file not defined"
endif

test: unit_tests.o $(comp_files) simpletest/simpletest.cpp
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm *.o main test