// Program to multiply two square matrices using pthreads
#include <iostream>
#include <chrono>
#include <fstream>
#include <pthread.h>

using namespace std;

// size of matrices
#define MAX 1500

// maximum number of threads
int MAX_THREAD;
int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];

ofstream file("N_1500.txt");

void *multi(void *arg)
{
    int *core = (int *)arg;
    //  Each thread computes 1/MAX_THREAD of matrix multiplication
    for (int i = *core * MAX / MAX_THREAD; i < (*core + 1) * MAX / MAX_THREAD; i++)
    {
        for (int j = 0; j < MAX; j++)
            for (int k = 0; k < MAX; k++)
            {
                matC[i][j] += matA[i][k] * matB[k][j];
            }
    }
    pthread_exit(NULL);
}

void *trial(void *arg)
{
    int id[MAX_THREAD];

    // Generating random values in matA and matB
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }

    // declaring threads
    pthread_t threads[MAX_THREAD];
    pthread_attr_t atr;
    int r;

    auto start = chrono::high_resolution_clock::now();

    pthread_attr_init(&atr);
    pthread_attr_setdetachstate(&atr, PTHREAD_CREATE_JOINABLE);
    // Creating threads, each evaluating its own part
    for (int i = 0; i < MAX_THREAD; i++)
    {
        id[i] = i;
        r = pthread_create(&threads[i], &atr, multi, (void *)(id + i));
        if (r)
        {
            printf("Error en la creación del hilo %d, código: %d\n", i, r);
            exit(-1);
        }
    }

    /*Se libera atr*/
    pthread_attr_destroy(&atr);
    // joining and waiting for all threads to complete
    for (int i = 0; i < MAX_THREAD; i++)
    {
        r = pthread_join(threads[i], NULL);
        if (r)
        {
            printf("Error en pthread_join() del hilo %d, codígo: %d\n", i, r);
            exit(-1);
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    file << duration << endl;

    pthread_exit(NULL);
}

int main()
{
    file << "N = " << MAX << endl;
    int i, T = 100;

    pthread_t thread;
    pthread_attr_t attr;
    int r;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (i = 1; i <= T; i++)
    {
        file << i << " ";
        MAX_THREAD = i;

        r = pthread_create(&thread, &attr, trial, nullptr);

        if (r)
        {
            exit(1);
        }

        r = pthread_join(thread, nullptr);

        if (r)
        {
            exit(1);
        }
    }
    pthread_attr_destroy(&attr);
    file.close();
}