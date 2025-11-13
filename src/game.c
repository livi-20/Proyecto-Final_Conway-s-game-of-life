#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/game.h"

/*
================================================
Conway's Game of Life - Lógica del Juego
================================================
Este módulo implementa la lógica central del Juego de la Vida de Conway:
    - Gestión de memoria para manipular la cuadrícula de células vivas y muertas.
    - Aplicación de las reglas de evolución de las células.
    - Cálculo de las generaciones sucesivas.

CARACTERÍSTICAS PRINCIPALES:
1. Doble Buffering:
    - genActual: Matriz que representa la generación actual de células (el estado actual de la cuadrícula).
    - genSiguiente: Buffer de escritura para la siguiente generación
    - Esto permite evitar conflictos al leer y escribir el estado de las células durante la actualización (es decir, el paso de una generación a la siguiente).

2. Wrapping Toroidal:
    La cuadrícula se comporta como un toroide (se envuelve en los bordes) para realizar el cálculo de los vecinos vivos.
    - Una célula en el borde derecho tiene vecinos en el borde izquierdo, y viceversa.
    - Una célula en el borde superior tiene vecinos en el borde inferior, y viceversa.

3. Inicialización Aleatoria:
    - La cuadrícula se inicializa con un 20% de células vivas, distribuidas aleatoriamente.
    - Se utiliza la función rand() para generar posiciones aleatorias.
*/


// Función para crear una nueva cuadrícula con las dimensiones especificadas y un 20% de células vivas iniciales (aleatorias).
Cuadricula* crearCuadricula(unsigned short ancho, unsigned short alto) {
    // Asignamos memoria para la estructura Cuadricula
    Cuadricula* cuadricula = (Cuadricula*)malloc(sizeof(Cuadricula));

    // Inicializamos los primeros atributos de la estructura.
    cuadricula->ancho = ancho;
    cuadricula->alto = alto;
    cuadricula->numGeneracion = 0;

    // Asignamos memoria para la matriz de células actual. Para ello, se multiplica el tamaño de un puntero a un booleano (que corresponde a una fila de la matriz) por el número de filas (alto).
    cuadricula->genActual = (bool**)malloc(alto * sizeof(bool*));
    if (cuadricula->genActual == NULL) {
        free(cuadricula);
        return NULL;
    }

    // Asignamos memoria para cada fila de la matriz de células actual.
    for (unsigned short i = 0; i < alto; i++) {
        cuadricula->genActual[i] = (bool*)malloc(ancho * sizeof(bool));
        // Liberamos la memoria correspondiente a las filas asignadas y a la cuadrícula en caso de error.
        if (cuadricula->genActual[i] == NULL) {
            for (unsigned short j = 0; j < i; j++) {
                free(cuadricula->genActual[j]);
            }
            free(cuadricula->genActual);
            free(cuadricula);
            return NULL;
        }
    }
    return cuadricula;
}
