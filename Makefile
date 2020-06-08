all : main.o
	gcc build/main.o -o cIntegration
	mv cIntegration ./build

main.o : src/main.c
	gcc -c -I include/ src/main.c
	mv main.o ./build/

.PHONY: clean
clean:
	rm ./build/*

.PHONY: run
run:
	./build/cIntegration

.PHONY: leak-test
leak-test:
	valgrind ./build/cIntegration
