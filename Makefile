all : main.o
	gcc build/main.o -o cIntegration
	mv cIntegration ./build

main.o : src/main.c
	gcc -c -I include/ src/main.c
	mv main.o ./build/

.PHONY: clean
clean:
	rm ./build/*

.PHONE: run
run:
	./build/cIntegration
