CC := g++ -fsanitize=address
build := build/

%:
	rm ./$(build)$@
	$(CC) src/$@.cpp -o $(build)$@
	./$(build)$@
