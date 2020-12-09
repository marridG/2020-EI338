mem_alloc: mem_alloc.o
	gcc -o mem_alloc mem_alloc.o -lstdc++ -std=c++11
mem_alloc.o: mem_alloc.cpp
	gcc -c mem_alloc.cpp -lstdc++ -std=c++11
clean:
	rm mem_alloc mem_alloc.o