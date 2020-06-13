all : main.o Str.o Vec.o Scanner.o CharItr.o Guards.o
	gcc build/main.o build/Str.o build/Vec.o build/Scanner.o build/CharItr.o build/Guards.o -o cIntegration
	mv cIntegration ./build

main.o : src/main.c
	gcc -c -I include/ src/main.c
	mv main.o ./build/

Str.o : src/Str.c
	gcc -c -I include/ src/Str.c
	mv Str.o ./build/

Vec.o : src/Vec.c
	gcc -c -I include/ src/Vec.c
	mv Vec.o ./build/

Scanner.o : src/Scanner.c
	gcc -c -I include/ src/Scanner.c
	mv Scanner.o ./build/

CharItr.o : src/CharItr.c
	gcc -c -I include/ src/CharItr.c
	mv CharItr.o ./build/

Guards.o : src/Guards.c
	gcc -c -I include/ src/Guards.c
	mv Guards.o ./build/

.PHONY: clean
clean:
	rm -rf ./build/
	mkdir build

.PHONY: run
run:
	./build/cIntegration

.PHONY: leak-test
leak-test:
	valgrind --leak-check=full --track-origins=yes ./build/cIntegration
