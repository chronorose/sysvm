CC := g++ -fsanitize=address
build := build/
parser := parser/
test := tests/
vm := vm/
exclude := `ls vm/*.cpp | grep -v sysvm.cpp`

.PHONY: all test clean parser empty factorial debug milestone1

test:
	$(CC) $(exclude) $(parser)*.cpp $(test)*.cpp -o $(build)sysvm

valgrind:
	g++ $(vm)*.cpp $(parser)*.cpp -o $(build)sysvm -DDEBUG

milestone1:
	$(CC) $(vm)*.cpp $(parser)*.cpp -o $(build)sysvm -DMILESTONE1

debug:
	$(CC) $(vm)*.cpp $(parser)*.cpp -o $(build)sysvm -DDEBUG

all: $(clean)
	$(CC) $(vm)*.cpp $(parser)*.cpp -o $(build)sysvm
	./$(build)sysvm examples/Factorial.class

clean:
	rm -f $(build)*
