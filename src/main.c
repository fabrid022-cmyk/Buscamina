#include "buscaminas_header.h"

const int botonAmp[8][8] =
{
    {X, X, X, X, X, X, X, X},
    {X, G, N, V, V, N, G, X},
    {X, N, N, V, V, N, N, X},
    {X, V, V, V, V, V, V, X},
    {X, V, V, V, V, V, V, X},
    {X, N, N, V, V, N, N, X},
    {X, G, N, V, V, N, G, X},
    {X, X, X, X, X, X, X, X},

};

const int cuadrado[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, G, G, G, G, G, G, N},
    {N, G, G, G, G, G, G, N},
    {N, G, G, G, G, G, G, N},
    {N, G, G, G, G, G, G, N},
    {N, G, G, G, G, G, G, N},
    {N, G, G, G, G, G, G, N},
    {N, N, N, N, N, N, N, N},

};

const int cero[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, N, N, N, N, N, N, N},
    {N, N, X, X, X, X, N, N},
    {N, N, X, X, X, X, N, N},
    {N, N, X, X, X, X, N, N},
    {N, N, X, X, X, X, N, N},
    {N, N, N, N, N, N, N, N},
    {N, N, N, N, N, N, N, N},

};

const int uno[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, N, N, N, AZ, N, N, N},
    {N, N, N, AZ, AZ, N, N, N},
    {N, N, N, N, AZ, N, N, N},
    {N, N, N, N, AZ, N, N, N},
    {N, N, N, N, AZ, N, N, N},
    {N, N, N, AZ, AZ, AZ, N, N},
    {N, N, N, N, N, N, N, N}

};

const int dos[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, N, N, A, A, N, N, N},
    {N, N, A, N, N, A, N, N},
    {N, N, N, N, A, N, N, N},
    {N, N, N, A, N, N, N, N},
    {N, N, A, N, N, N, N, N},
    {N, N, A, A, A, A, N, N},
    {N, N, N, N, N, N, N, N}

};

const int tres[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, N, V, V, V, N, N, N},
    {N, N, N, N, N, V, N, N},
    {N, N, N, V, V, N, N, N},
    {N, N, N, N, N, V, N, N},
    {N, N, N, N, N, V, N, N},
    {N, N, V, V, V, N, N, N},
    {N, N, N, N, N, N, N, N}

};

const int cuatro[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, N, N, N, R, R, N, N},
    {N, N, N, R, N, R, N, N},
    {N, N, R, N, N, R, N, N},
    {N, N, R, R, R, R, N, N},
    {N, N, N, N, N, R, N, N},
    {N, N, N, N, N, R, N, N},
    {N, N, N, N, N, N, N, N}

};

const int cinco[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, N, VI, VI, VI, VI, N, N},
    {N, N, VI, N, N, N, N, N},
    {N, N, VI, VI, VI, N, N, N},
    {N, N, N, N, N, VI, N, N},
    {N, N, N, N, N, VI, N, N},
    {N, N, VI, VI, VI, N, N, N},
    {N, N, N, N, N, N, N, N}

};

const int seis[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, N, N, C, C, N, N, N},
    {N, N, C, N, N, N, N, N},
    {N, N, C, C, C, N, N, N},
    {N, N, C, N, N, C, N, N},
    {N, N, C, N, N, C, N, N},
    {N, N, N, C, C, N, N, N},
    {N, N, N, N, N, N, N, N},

};

const int siete[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, N, NA, NA, NA, NA, N, N},
    {N, N, N, N, N, NA, N, N},
    {N, N, N, N, NA, N, N, N},
    {N, N, N, NA, N, N, N, N},
    {N, N, N, NA, N, N, N, N},
    {N, N, N, NA, N, N, N, N},
    {N, N, N, N, N, N, N, N},

};

const int ocho[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, N, N, B, B, N, N, N},
    {N, N, B, N, N, B, N, N},
    {N, N, N, B, B, N, N, N},
    {N, N, B, N, N, B, N, N},
    {N, N, B, N, N, B, N, N},
    {N, N, N, B, B, N, N, N},
    {N, N, N, N, N, N, N, N},

};

const int bandera[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, R, R, R, R, G, N, N},
    {N, R, R, R, R, G, N, N},
    {N, R, R, R, R, G, N, N},
    {N, N, N, N, N, G, N, N},
    {N, N, N, N, N, G, N, N},
    {N, N, N, N, N, G, N, N},
    {N, N, N, N, N, N, N, N},

};

const int boom[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, G, N, G, G, N, G, N},
    {N, N, G, G, G, G, N, N},
    {N, G, G, G, G, G, G, N},
    {N, G, G, G, G, G, G, N},
    {N, N, G, G, G, G, N, N},
    {N, G, N, G, G, N, G, N},
    {N, N, N, N, N, N, N, N}

};

const int boom_2[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, R, N, R, R, N, R, N},
    {N, N, R, R, R, R, N, N},
    {N, R, R, R, R, R, R, N},
    {N, R, R, R, R, R, R, N},
    {N, N, R, R, R, R, N, N},
    {N, R, N, R, R, N, R, N},
    {N, N, N, N, N, N, N, N}

};

const int boom_3[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, N, N, N, N, N, N, N},
    {N, N, NA, NA, NA, NA, N, N},
    {N, N, NA, R, R, NA, N, N},
    {N, N, NA, R, R, NA, N, N},
    {N, N, NA, NA, NA, NA, N, N},
    {N, N, N, N, N, N, N, N},
    {N, N, N, N, N, N, N, N}

};

const int boom_4[8][8] =
{
    {N, N, N, N, N, N, N, N},
    {N, R, N, N, N, N, R, N},
    {N, N, R, N, N, R, N, N},
    {N, N, N, R, R, N, N, N},
    {N, N, N, R, R, N, N, N},
    {N, N, R, N, N, R, N, N},
    {N, R, N, N, N, N, R, N},
    {N, N, N, N, N, N, N, N}

};

int main(int argc,char* argv[])
{
    const char* nomArchBuscaminaConf = "buscaminas.config.txt";
    int TAM_GRILLA;
    tMina minaConf;
    int **minas = NULL;
    int minasAux[32][32];
    SDL_Window *ventana = NULL;
    SDL_Renderer* renderer = NULL;
    const int (*numeros[])[8]= { cero,uno,dos,tres,cuatro,cinco,seis,siete,ocho,bandera,boom,boom_2,boom_3,boom_4 };
    int buscamina;
    int minasRestante = 0;
    int cheat = 0;
    SDL_Event event;
    Uint32 inicioTiempo = 0;
    tGuardado guardar;
    guardar.tiempo = 0;
    const Uint8 *flechaArriba = NULL;
    const int (*numerosAux[])[8]= { cero,uno,dos,tres,cuatro,cinco,seis,siete,ocho,bandera };

    // inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
    {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }
    inicioTiempo = SDL_GetTicks();
    flechaArriba = SDL_GetKeyboardState(NULL);

    // INCIALIZAR JUEGO
    buscamina = inicializacionJuego(nomArchBuscaminaConf,
                                    &event,
                                    numeros,
                                    &inicioTiempo,
                                    &minasRestante,
                                    &cheat,
                                    &guardar,
                                    &TAM_GRILLA,
                                    &minaConf,
                                    &minas,
                                    &ventana,
                                    &renderer,
                                    botonAmp);

    // Si inicializacion devolvió FIN_JUEGO o error, salir limpiamente
    if (buscamina== TERMINAR || buscamina == FIN_JUEGO || buscamina == ERROR_ARCHIVO || buscamina == ERROR_MEMORIA || buscamina == ERROR_VENTANA)
    {
        // Si existen recursos, liberarlos
        if (minas)
            liberarMinas(&minas, TAM_GRILLA);

        if (renderer) { SDL_DestroyRenderer(renderer); renderer = NULL; }
        if (ventana)  { SDL_DestroyWindow(ventana); ventana = NULL; }
        TTF_Quit();
        SDL_Quit();

        system("pause");
        return 0;
    }

    // JUGANDO
    while (buscamina)
    {
        while (SDL_PollEvent(&event))
        {
            // Mostrar info cada vez que hay evento
            if (ventana && renderer)
                mostrarInfoVentana(ventana, renderer, &inicioTiempo, &minasRestante, &guardar);

            // GUARDAR Y SALIR (evento window close)
            if (event.type == SDL_QUIT)
            {
                // Guardar partida (si minas existe)
                if (minas)
                {
                    int r = save(minas, &guardar, &TAM_GRILLA, &inicioTiempo, &cheat, &minasRestante);
                    if (r != 0)
                        printf("\nError al guardar la partida al cerrar.\n");
                }
                printf("\nSaliendo de SDL\n");
                buscamina = TERMINAR;
                break;
            }

            // CHEAT TECLADO
            flechaArriba = SDL_GetKeyboardState(NULL);
            if (flechaArriba && flechaArriba[SDL_SCANCODE_UP] && cheat == 1)
            {
                // procesarCheat
                procesarCheat(&cheat, &minasRestante, &ventana, &renderer, &minas, bandera, &TAM_GRILLA);
            }

            // MOUSE
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;
                int boton = event.button.button;

                if (TAM_GRILLA <= 0) continue;

                int paso = TAM_VENTANA / TAM_GRILLA;
                int grillaAncho = paso * TAM_GRILLA;
                int offsetX = (TAM_VENTANA - grillaAncho) / 2;
                int offsetY = (TAM_VENTANA - grillaAncho) / 2;

                // CLICK DENTRO DE BOTON AMPLIAR
                if (x >= TAM_BOTONX && y >= TAM_BOTONY)
                {
                    // REDIMENSIONAR
                    if (procesarRedimension(&ventana, &renderer, &minasRestante, botonAmp, &minas, numerosAux, &minaConf, &TAM_GRILLA) == OK)
                    {
                        // recalcular para seguir con clicks correctos
                        paso = TAM_VENTANA / TAM_GRILLA;
                        grillaAncho = paso * TAM_GRILLA;
                        offsetX = (TAM_VENTANA - grillaAncho) / 2;
                        offsetY = (TAM_VENTANA - grillaAncho) / 2;
                    }
                    else
                    {
                        buscamina = TERMINAR;
                        break;
                    }
                }

                // CLICK DENTRO DE LA GRILLA
                if (x >= offsetX && x < offsetX + grillaAncho &&
                    y >= offsetY && y < offsetY + grillaAncho)
                {
                    paso = TAM_VENTANA / TAM_GRILLA;
                    grillaAncho = paso * TAM_GRILLA;
                    offsetX = (TAM_VENTANA - grillaAncho) / 2;
                    offsetY = (TAM_VENTANA - grillaAncho) / 2;

                    int pos_X = (x - offsetX) / paso;
                    int pos_Y = (y - offsetY) / paso;

                    // Seguridad: chequear índices dentro de rango
                    if (pos_X < 0 || pos_X >= TAM_GRILLA || pos_Y < 0 || pos_Y >= TAM_GRILLA) continue;
                    if (!minas) continue; // si no hay matriz, evitar crash

                    // MOUSE CLICK IZQUIERDO
                    if (boton == SDL_BUTTON_LEFT)
                    {
                        printf("\n\nI(%d, %d)", pos_Y, pos_X);
                        fechaHoraActual();

                        mostrar(minas, &TAM_GRILLA);

                        procesarClickIzquierdo(ventana, renderer, minas, pos_X, pos_Y, numeros, TAM_GRILLA, &buscamina, boom, boom_2, boom_3, boom_4);
                    }
                    // MOUSE CLICK DERECHO
                    else if (boton == SDL_BUTTON_RIGHT)
                    {
                        printf("\n\nD(%d, %d)", pos_X, pos_Y);
                        fechaHoraActual();

                        procesarClickDerecho(ventana, renderer, minas, pos_X, pos_Y, &minasRestante, TAM_GRILLA, cuadrado, bandera);
                    }

                    // VERIFICAR SI GANO
                    if (verificar(minas, TAM_GRILLA, &ventana) == OK)
                    {
                        guardarJugadorTabla(guardar, TAM_GRILLA);
                        buscamina = TERMINAR;
                        SDL_Delay(1000);
                        trofeo(ventana, renderer, TAM_GRILLA, &guardar);
                        SDL_Delay(5000);
                        vaciarContinuar(); // SI GANO YA NO PUEDE CONTINUAR LA MISMA PARTIDA
                    }
                }
            } // fin mouse
        } // fin pollEvent

        // Si salió por cerrar ventana, rompemos el loop principal
        if (!buscamina) break;

        if (ventana && renderer)
            mostrarInfoVentana(ventana, renderer, &inicioTiempo, &minasRestante, &guardar);

        SDL_Delay(8); // pequeño sleep para evitar CPU al 100%
    } // fin while(buscamina)

    // FINALIZAR EL JUEGO Y DESTRUIR
    // Llamar a finDelJuego para destruir ventana y SDL
    finDelJuego(&TAM_GRILLA, &ventana, &minas);

    system("pause");
    return 0;
}
