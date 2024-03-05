CC := g++ -fsanitize=address
build := build/
parser := parser/

.PHONY: all test clean parser

parser: $(clean)
	$(CC) $(parser)*.cpp -o $(build)parser
	./$(build)parser examples/HelloVM.class

clean:
	rm -f $(build)*
