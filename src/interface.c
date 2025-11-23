#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "../include/game.h" //Usar cuadrícula puesta en el game.h

//Crear función para la cuadrícula
void dibujarCuadricula(WINDOW *win, Cuadricula *cuad) {
    int maxY, maxX;
    getmaxyx(win, maxY, maxX);

    //dejamos el espacio para el titulo (fila arriba) y los botones (fila abajo)
    int offsetY = 1;
    int limiteY = maxY -2;

    //recorrer cada fila de la cuadrícula sin salirse de la matriz ni ponerse encima del titulo ni los botones
    for (unsigned short y = 0; y < cuad->alto && (y + offsetY) < limiteY; y++) {
        for (unsigned short x = 0; x < cuad->ancho && x < (unsigned short)maxX - 2; x++) {
            if (cuad->genActual[y][x])
                mvwprintw(win, y + offsetY, x + 1, "0"); //célula viva
            else
                mvwprintw(win, y + offsetY, x + 1, " "); //célula muerta
        }
    }
}


int main (int argc, char ** argv) 
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    //Añadimos la cuadrícula
    Cuadricula *cuadricula = crearCuadricula(ANCHO_CUADRICULA, ALTO_CUADRICULA);
    if (cuadricula == NULL) {
        endwin();
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

    dibujarCuadricula(ventana, cuadricula);
    wrefresh(ventana);

    //Variables para el estado del juego
    int correr = 0; // 0 = pausa, 1 = play
    int delay_ms = 200; //Tiempo entre generaciones en ms (Velocidad inicial = 200 ms)
    nodelay(ventana, TRUE); //Para que el wgetch no se bloquee esperando una tecla
    
    //Funcionamiento de las teclas completo
    int ch;
    while (1) {
        ch = wgetch(ventana);
        //Entrar solo si hay una tecla
        if (ch != ERR) {
            if (ch == 'q') { //q Salir
                break;
            } else if (ch == ' ') { //SPACE avanzar una generación
                //calcularCuadriculaSiguiente(cuadricula);
                dibujarCuadricula(ventana, cuadricula);
                wrefresh(ventana);
            } else if (ch == 'p') { //p Play o Pausa 
                correr = !correr; //Alternanr entre 0 y 1
            } else if (ch == '+') { //+ Acelerar (se reduce el delay)
                if (delay_ms > 50 ){
                    delay_ms -= 50;
                }
            } else if (ch == '-') { //- Desaelerar (aumenta el delay)
                if (delay_ms < 1000 ){
                    delay_ms += 50;
                }
            }
        }
        //Cuando esta en modo correr, que avance automáticamente
        if (correr) {
            //calcularCuadriculaSiguiente(cuadricula);
            dibujarCuadricula(ventana, cuadricula);
            wrefresh(ventana);
        }
        napms(delay_ms);
    }

    //Para liberar la memoria de la cuadricula
    liberarCuadricula(cuadricula);
    endwin();
    return 0;
}