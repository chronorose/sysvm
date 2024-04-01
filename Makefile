CC := g++ -fsanitize=address
build := build/
parser := parser/
vm := vm/

.PHONY: all test clean parser empty factorial debug milestone1

milestone1:
	$(CC) $(vm)*.cpp $(parser)*.cpp -o $(build)sysvm -DMILESTONE1

debug:
	$(CC) $(vm)*.cpp $(parser)*.cpp -o $(build)sysvm -DDEBUG

all: $(clean)
	$(CC) $(vm)*.cpp $(parser)*.cpp -o $(build)sysvm
	./$(build)sysvm examples/Factorial.class

	


clean:
	rm -f $(build)*
