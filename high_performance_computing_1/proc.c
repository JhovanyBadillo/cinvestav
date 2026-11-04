#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#define TAMANO 8

typedef struct
{
    int *arreglo;
    int inicio;
    int fin;
    int resultado_max;
} DatosHilo;

// Algortimo adaptado
void *buscar_max_hilo(void *arg)
{
    DatosHilo *datos = (DatosHilo *)arg;
    int max = datos->arreglo[datos->inicio];
    for (int i = datos->inicio + 1; i < datos->fin; i++)
    {
        if (datos->arreglo[i] > max)
        {
            max = datos->arreglo[i];
        }
    }
    datos->resultado_max = max;
    pthread_exit(NULL);
}

int main()
{
    int arreglo[TAMANO] = {8, 7, 44, 8, 58, 99, 587, 47};
    int tuberia[2];

    if (pipe(tuberia) == -1)
    {
        perror("Error de pipe");
        return EXIT_FAILURE;
    }

    printf("[Padre] lanzando proceso hijo con fork()...\n");
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("Error fork");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        close(tuberia[0]);
        printf("[Hijo] Soy el proceso hijo crearé 2 hilos para dividir el arreglo...\n");

        pthread_t hilo1, hilo2;

        DatosHilo datos1, datos2;

        // primer hilo

        // configuracion hilo 1
        datos1.arreglo = arreglo;
        datos1.inicio = 0;
        datos1.fin = TAMANO / 2;

        // CONFIGURACION HLO 2
        datos2.arreglo = arreglo;
        datos2.inicio = TAMANO / 2;
        datos2.fin = TAMANO;

        // PROCESO HIJO CREA HILOS PROPIOS
        pthread_create(&hilo1, NULL, buscar_max_hilo, (void *)&datos1);
        pthread_create(&hilo2, NULL, buscar_max_hilo, (void *)&datos2);

        // EL PROCESO HIJO ESPERA A QUE SUS HILOS TERMINEN
        pthread_join(hilo1, NULL);
        pthread_join(hilo2, NULL);

        printf("[Hijo] Hilos finalizados. Hilo 1 encontro: %d, Hilo 2 encontro %d\n", datos1.resultado_max, datos2.resultado_max);

        // proceso hijo toma decision.
        int max_final = (datos1.resultado_max > datos2.resultado_max) ? datos1.resultado_max : datos2.resultado_max;

        printf("[Hijo] Enviando el maximo absoluto (%d) al padre a traves del pipe...\n", max_final);
        write(tuberia[1], &max_final, sizeof(max_final));

        close(tuberia[1]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(tuberia[1]);

        int resultado_hijo;

        printf("[Padre] Esperando respuesta en el pipe...\n");

        // bloqueando hasta que el hijo (y sus hilo) terminen
        read(tuberia[0], &resultado_hijo, sizeof(resultado_hijo));

        printf("[Padre] ¡Recibido! El valor máximo procesado por mi hijo e hilos es: %d\n", resultado_hijo);

        close(tuberia[0]);

        wait(NULL);
    }
    return EXIT_SUCCESS;
}
