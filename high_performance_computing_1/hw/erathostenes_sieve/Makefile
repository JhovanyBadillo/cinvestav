PROGRAM=parallel

compile:
	clang -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include -L/usr/local/opt/libomp/lib -lomp ${PROGRAM}.c -o ${PROGRAM}.o
run:
	./${PROGRAM}.o ${ARGS}