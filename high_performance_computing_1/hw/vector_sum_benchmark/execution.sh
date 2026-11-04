gcc -o vector_sum.o -fopenmp vector_sum.c

for i in $(seq 1 100);
do
    echo -n "$i\t" >> data.txt
    ./vector_sum.o $i >> data.txt
done
