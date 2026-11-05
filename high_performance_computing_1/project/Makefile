RNG_RELATIVE_PATH=../../programming/hw/differential_evolution
APP=matrix_generator

compile-generator:
	g++ -std=c++23 -I${RNG_RELATIVE_PATH} -o ${APP}.o ${APP}.cpp ${RNG_RELATIVE_PATH}/rng.cpp

run-generator:
	time ./${APP}.o

compile-reader:
	gcc -o reader.o reader.c -lpthread

run-reader:
	./reader.o

compile-project:
	gcc -o project.o project.c
run-project:
	./project.o