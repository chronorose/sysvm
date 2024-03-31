CC := g++ -fsanitize=address
build := build/
parser := parser/
vm := vm/

.PHONY: all test clean parser

parser: $(clean)
	$(CC) $(vm)*.cpp $(parser)*.cpp -o $(build)sysvm
	./$(build)sysvm examples/Factorial.class

clean:
	rm -f $(build)*
