//Version 3; más botones
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "../include/game.h" //Usar cuadrícula puesta en el game.h

int main (int argc, char ** argv) 
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    //Añadimso la cuadrícula
    Cuadricula *cuadricula = crearCuadricual(ANCHO_CUADRICULA, ALTO_CUADRICULA);
    if (cuadricula == NULL) {
        edwin();
        fprintf(stderr, "No se pudo crear la cuadrícula.\n");
        return 1;
    }

    int altura, ancho;
    getmaxyx(stdscr, altura, ancho);
    
    int margen = 1;
    WINDOW * ventana = newwin(altura -2*margen, ancho - 2*margen, margen, margen);
    box(ventana, 0, 0);
    mvwprintw(ventana, 0, 2, "Conway's Game of Life");
    
    //Se hace un cambio para usar el tamaño de la ventana y no la terminal como lo tenía anteriormente.
    int win_altura, win_ancho;
    getmaxyx(ventana, win_altura, win_ancho);
    
    //4 -> 2 por que es la penúltima fila dentro de la ventana
    int botones = altura - 2;
    mvwprintw(ventana, botones, 2, "q: Salir");
    mvwprintw(ventana, botones, 12, "p: Play/Pausa");
    mvwprintw(ventana, botones, 27, "+: Acelerar");
    mvwprintw(ventana, botones, 40, "-: Desacelerar");
    mvwprintw(ventana, botones, 56, "SPACE: Avanzar gen");
    wrefresh(ventana);
    
    int ch;
    while ((ch = wgetch(ventana)) != 'q'){
    }

    //Para liberar la memoria de la cuadricula
    liberarCuadricula(cuadricula);
    endwin();
    return 0;
}