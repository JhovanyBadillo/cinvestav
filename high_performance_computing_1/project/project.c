/*

proceso main:
    *fp = fopen(archivo gigante)
    establecer limites para cada proceso
    fork() por cada nucleo fisico
    a cada proceso hijo le tenemos que dar sus limites del archivo y *fp (via pipe)

subproceso 1:
    crear dos hilos
    determinar, a partir de los limites del subproceso, los limites de lectura
    de cada hilo, inicio_hiloM, fin_hiloM
    hilo 1:
        leer de *fp los datos que le tocan (inicio_hilo1, fin_hilo1)
        escribir a un archivo temporal, tmp_file_1 lo que leyó
    espera a los hilos
    lee una linea de tmp_file_1 -> line_1(10 numeros en crudo)
    lee una linea de tmp_file_2 -> line_2 (10 numeros en crudo)
    interprete un numero double de line_1 -> double numero_1
    interprete un numero double de line_2 -> double numero_2
    comparar(numero_1, numero_2) -> escribir resultado en archivo temporal tmp_subproceso1
    comunicar identificador de archivo temporal a proceso main

...

lo mismo para el resto de subprocesos

proceso main:
    tendra 4 identificadores de archivos temporales con numeros ordenados
    mergesort(archivo1, archivo2) -> ordenado_1
    mergesort(archivo3, archivo4) -> ordenado_2
    mergesort(ordenado_1, ordenado_2) -> ordenado_final
    escribir en un archivo no temporal el ordenamiento final


consideraciones:
    fijar en 4 el numero de nucleos
    subprocesos con fork
    cada línea del archivo gigante tiene SIZE * 8 + (SIZE - 1) + 1 bytes
        SIZE * 8 corresponden a los SIZE doubles
        SIZE - 1 corresponden a los SIZE - 1 espacios en blanco
        1 corresponde al salto de línea

por ver:
    determinar dinámicamente el número de núcleos físicos y con esto
    determinar las particiones

*/