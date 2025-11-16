#ifndef BUSCAMINAS_HEADER_H_INCLUDED
#define BUSCAMINAS_HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define OK 1
#define ERROR_ARCHIVO -1
#define ERROR_MEMORIA -2
#define ERROR_VENTANA -3
#define ERROR_TEXTO -4

// MENU
#define MENU_NUEVA   1
#define MENU_CONT    2
#define MENU_AJUSTES 3
#define MENU_SALIR   4

// CÓDIGOS INTERNOS
#define MENU 1
#define AJUSTES 2
#define JUGAR 3
#define TERMINAR 0

#define FIN_JUEGO -1

#define TAM_VENTANA 720
#define TAM_VENTANA_INFO 600
#define TAM_PIXEL 8
#define PADDING 4
#define TAM_BOTONX 1000
#define TAM_BOTONY 0

#define HAY_MINA 99
#define VISTO 11

#define N 0   // Negro
#define G 1   // Gris
#define A 2   // Amarillo
#define B 3   // Blanco
#define V 4   // Verde
#define R 5   // Rojo
#define VI 6  // Violeta
#define AZ 7  // Azul
#define C 8   // Celeste
#define NA 9  // Naranja
#define X 10  // Gris oscuro
#define M 11  // Marron
#define NC 12 // Naranja claro



typedef struct
{
    char nick[4];
    int tam_matriz;
    int minas_restantes;
    int cheat;
    Uint32 tiempo;
}tGuardado;

typedef struct
{
    int cantMinas;
    int porcentaje; //1 si o 0 no
}tMina;


//PREPARAR BUSCAMINA
int inicializacionJuego(const char* nomArchBuscaminaConf,SDL_Event* event,
                        const int(*numeros[])[8],Uint32* inicioTiempo,
                        int* minasRestante,int* cheat,tGuardado* guardar,
                        int* TAM_GRILLA,tMina* minaConf,int*** minas,
                        SDL_Window** ventana,SDL_Renderer** renderer,
                        int const dibujo[][TAM_PIXEL]);

//AJUSTE DE TAMAÑO Y MINAS
int ajustesSDL(SDL_Renderer* renderer, int* TAM_GRILLA, tMina* minaConf, SDL_Event* event);


//PROCESAR EL CHEAT (FLECHA ARRIBA)
int procesarCheat(int* cheat, int* minasRestante,SDL_Window** ventana,
                  SDL_Renderer** renderer,int*** minas,
                  const int bandera[][TAM_PIXEL],int* TAM_GRILLA);



//PROCESAR REDIMENSION (AGREGA 2 FILAS Y 2 COLUMNAS)
int procesarRedimension(SDL_Window** ventana,SDL_Renderer** renderer, int* minasRestante,
                        const int botonAmp[][TAM_PIXEL],int*** minas,const int (*numerosAux[])[8],
                        tMina* minaConf,int* TAM_GRILLA);

//PROCESAR CLICK IZQUIERDO (SI HAY MINA O QUE NÚMERO MOSTRAR)
int procesarClickIzquierdo(SDL_Window* ventana,SDL_Renderer* renderer,int** minas,
                           int pos_X,int pos_Y,const int (*numeros[])[8],int TAM_GRILLA,
                           int* buscamina,const int boom[][TAM_PIXEL],const int boom_2[][TAM_PIXEL],
                           const int boom_3[][TAM_PIXEL],const int boom_4[][TAM_PIXEL]);

//PROCESAR CLICK DERECHO (PONER O SACAR BANDERA)
int procesarClickDerecho(SDL_Window *ventana,
                         SDL_Renderer *renderer,
                         int **minas,
                         int pos_X,
                         int pos_Y,
                         int *minasRestante,
                         int TAM_GRILLA,
                         const int cuadrado[][8],
                         const int bandera[][8]);



//WINDOWS Y SDL
int configMinas(int*** minas,int* TAM_GRILLA);
int crearVentana(SDL_Window** ventana,int* TAM_GRILLA);
int crearRender(SDL_Renderer** renderer,SDL_Window** ventana);
int configBuscamina(const char* nomArchBuscaminaConf,int *TAM_GRILLA,tMina* minaConf);
void crearPantallaBuscamina(SDL_Renderer *renderer, int*);
void insertarMinas(int **minas,tMina *minaConf, int*);
void insertarMinasAmp(int **minas,int,int*);
int inicioDelJuego(SDL_Renderer *,int **minas, int*,tMina*,tGuardado* guardar);
void continuarJuego(SDL_Window *ventana,SDL_Renderer *,int **minas,int *TAM_GRILLA,const int(*numeros[])[8]);
int finDelJuego(int* TAM_GRILLA, SDL_Window** ventana, int*** minas);
void dibujarGanador(SDL_Window *ventana, SDL_Renderer *renderer, int const dibujo[][10], int, int, int);
void dibujar(SDL_Window *ventana, SDL_Renderer *renderer, int const dibujo[][TAM_PIXEL], int, int, int);
void mostrarCuadrado(SDL_Window *, SDL_Renderer *, int**, int, int, const int[][TAM_PIXEL], int);
int numeroMinas(int **minas, int, int, int*);
void insertarNumeroTodaGrilla(int **minas, int*);
void insertarNumeroTodaGrillaAmp(int **minas, int*);
void mostrar(int **minas, int*);
void cascada(SDL_Window *, SDL_Renderer *,int**,int, int, const int (*numeros[])[8],int);
void mostrarMinas(SDL_Window *ventana, SDL_Renderer *renderer, int **minas, const int dibujo[][TAM_PIXEL], const int dibujo2[][TAM_PIXEL], const int dibujo3[][TAM_PIXEL], const int dibujo4[][TAM_PIXEL],int);
int verificar(int **minas,int TAM_GRILLA,SDL_Window **ventana);
void trofeo(SDL_Window*, SDL_Renderer*,int, tGuardado*);
void dibujarBoton(SDL_Window *ventana, SDL_Renderer *renderer, int const dibujo[][TAM_PIXEL], int, int,int);
int vaciarContinuar();
void liberarMinas(int ***minas, int tam);

// MANEJOS DE ARCHIVOS
int save(int** minas,tGuardado* guardar, int* TAM_GRILLA,Uint32* inicioTiempo,int* cheat,int* minasRestante); //recibe puntero a matriz, tam_grilla y struct de guardado
Uint32 load(int** minas, tGuardado* abrir, int tam_expected);
int guardarJugadorTabla(tGuardado, int);
void ultimosDiez(SDL_Window *, SDL_Renderer *, int);

//ESTADISTICAS, TIEMPO Y NOMBRE
void mostrarInfoVentana(SDL_Window *ventana,SDL_Renderer *renderer,Uint32* inicioTiempo,int* minasRestante,tGuardado* guardar);

//MENU PRINCIPAL
int menu(SDL_Window *ventana,
         SDL_Renderer *renderer,
         SDL_Event *event,
         int *TAM_GRILLA,
         int ***minas,
         tMina *minaConf,
         const int(*numeros[])[8],
         Uint32* inicioTiempo,
         int* minasRestante,
         int *cheat,
         tGuardado* guardar);

//PONER MATRIZ EN 0
void inicializar(int**, int*);

//MANEJO DEL TIEMPO
void fechaHoraActual();

#endif // BUSCAMINAS_HEADER_H_INCLUDED
