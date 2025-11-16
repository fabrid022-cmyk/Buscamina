#include "buscaminas_header.h"


void liberarMinas(int ***minas, int tam)
{
    if (!minas || !*minas) return;

    for (int i = 0; i < tam; ++i)
    {
        if ((*minas)[i])
        {
            free((*minas)[i]);
            (*minas)[i] = NULL;
        }
    }
    free(*minas);
    *minas = NULL;
}


int inicializacionJuego(const char* nomArchBuscaminaConf,
                        SDL_Event* event,
                        const int(*numeros[])[8],
                        Uint32* inicioTiempo,
                        int* minasRestante,
                        int* cheat,
                        tGuardado* guardar,
                        int* TAM_GRILLA,
                        tMina* minaConf,
                        int*** minas,
                        SDL_Window** ventana,
                        SDL_Renderer** renderer,
                        int const dibujo[][TAM_PIXEL])
{
    int opcion;

    // CONFIG BÁSICA
    if(configBuscamina(nomArchBuscaminaConf, TAM_GRILLA, minaConf) == ERROR_ARCHIVO)
        return ERROR_ARCHIVO;

    if(configMinas(minas, TAM_GRILLA) == ERROR_MEMORIA)
        return ERROR_MEMORIA;

    if(crearVentana(ventana, TAM_GRILLA) != OK)
        return ERROR_VENTANA;

    SDL_Surface* icono = SDL_LoadBMP("logo.bmp");  // mejor usar BMP
    if (icono)
    {
        SDL_SetWindowIcon(*ventana, icono);
        SDL_FreeSurface(icono);  // liberar la superficie después de asignarla
    }
    else
    {
        SDL_Log("No se pudo cargar el icono: %s", SDL_GetError());
    }


    crearRender(renderer, ventana);

MENU_LOOP:
    opcion = menu(*ventana, *renderer, event, TAM_GRILLA, minas,
                  minaConf, numeros, inicioTiempo, minasRestante, cheat, guardar);


    // AJUSTES
    if(opcion == MENU_AJUSTES)
    {
        if(ajustesSDL(*renderer, TAM_GRILLA, minaConf, event) != OK)
            return FIN_JUEGO;

        FILE* conf = fopen(nomArchBuscaminaConf, "w");
        if(!conf) return ERROR_ARCHIVO;
        fprintf(conf,"dimension=%d\n",*TAM_GRILLA);
        if(minaConf->porcentaje)
            fprintf(conf,"minas=%d%%\n",minaConf->cantMinas);
        else
            fprintf(conf,"minas=%d\n",minaConf->cantMinas);
        fclose(conf);

        // reconfigurar memoria
        configBuscamina(nomArchBuscaminaConf,TAM_GRILLA,minaConf);
        configMinas(minas,TAM_GRILLA);

        goto MENU_LOOP;
    }

    // NUEVA PARTIDA
    if(opcion == MENU_NUEVA)
    {
        ultimosDiez(*ventana, *renderer, *TAM_GRILLA);
        dibujarBoton(*ventana, *renderer, dibujo, TAM_BOTONX, TAM_BOTONY, *TAM_GRILLA);

        //INICIAR TIEMPO AL COMENZAR EL JUEGO
        *inicioTiempo = SDL_GetTicks();

        save(*minas, guardar, TAM_GRILLA, inicioTiempo, cheat, minasRestante);
        return OK;
    }


    if (opcion == MENU_CONT)
    {
        //Limpiar cola de eventos viejos
        SDL_Event ev;
        while (SDL_PollEvent(&ev));

        //Cargar partida
        Uint32 tiempoGuardado = load(*minas, guardar, *TAM_GRILLA);

        //ajustar tiempo
        *inicioTiempo = SDL_GetTicks() - (tiempoGuardado * 1000);

        *minasRestante  = guardar->minas_restantes;
        *cheat          = guardar->cheat;


        ultimosDiez(*ventana, *renderer, *TAM_GRILLA);
        dibujarBoton(*ventana, *renderer, dibujo, TAM_BOTONX, TAM_BOTONY, *TAM_GRILLA);
        mostrarInfoVentana(*ventana, *renderer, inicioTiempo, minasRestante, guardar);

        // Volver al juego
        return MENU_CONT;
    }


    return FIN_JUEGO;
}


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
         tGuardado* guardar)
{
    int menuAct = 1;

    SDL_Rect rectNP = {PADDING, PADDING, TAM_VENTANA - 2*PADDING, TAM_VENTANA_INFO/3 - PADDING};
    SDL_Rect rectC  = {PADDING, TAM_VENTANA_INFO/3 + PADDING, TAM_VENTANA - 2*PADDING, TAM_VENTANA_INFO/3 - PADDING};
    SDL_Rect rectA  = {PADDING, 2*(TAM_VENTANA_INFO/3) + PADDING, TAM_VENTANA - 2*PADDING, TAM_VENTANA_INFO/3 - PADDING};

    int continuar = 0;
    FILE* fp = fopen("ultima_partida","rb");
    if(fp)
    {
        fclose(fp);
        continuar = 1;
    }

    if(TTF_WasInit() == 0 && TTF_Init() != 0)
    {
        SDL_Log("TTF error");
        return ERROR_TEXTO;
    }

    TTF_Font* font = TTF_OpenFont("arialbd.ttf",24);
    if(!font) return ERROR_TEXTO;

    SDL_Color color = {255,255,255,255};

    while(menuAct)
    {
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        // NUEVA PARTIDA
        SDL_SetRenderDrawColor(renderer,128,128,128,255);
        SDL_RenderFillRect(renderer,&rectNP);
        SDL_Surface* surf = TTF_RenderText_Solid(font,"NUEVA PARTIDA",color);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FreeSurface(surf);
        SDL_Rect dst = {rectNP.x + 20, rectNP.y + rectNP.h/4, rectNP.w-40, rectNP.h/2};
        SDL_RenderCopy(renderer,tex,NULL,&dst);
        SDL_DestroyTexture(tex);

        // CONTINUAR
        if(continuar)
        {
            SDL_SetRenderDrawColor(renderer,128,128,128,255);
            SDL_RenderFillRect(renderer,&rectC);
            surf = TTF_RenderText_Solid(font,"CONTINUAR",color);
            tex = SDL_CreateTextureFromSurface(renderer,surf);
            SDL_FreeSurface(surf);
            dst = (SDL_Rect)
            {
                rectC.x+20,rectC.y+rectC.h/4,rectC.w-40,rectC.h/2
            };
            SDL_RenderCopy(renderer,tex,NULL,&dst);
            SDL_DestroyTexture(tex);
        }

        // AJUSTES
        SDL_SetRenderDrawColor(renderer,128,128,128,255);
        SDL_RenderFillRect(renderer,&rectA);
        surf = TTF_RenderText_Solid(font,"AJUSTES",color);
        tex = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FreeSurface(surf);
        dst = (SDL_Rect)
        {
            rectA.x+20,rectA.y+rectA.h/4,rectA.w-40,rectA.h/2
        };
        SDL_RenderCopy(renderer,tex,NULL,&dst);
        SDL_DestroyTexture(tex);

        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(event))
        {
            if(event->type == SDL_QUIT)
            {
                TTF_CloseFont(font);
                return MENU_SALIR;
            }

            if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
            {
                int x = event->button.x;
                int y = event->button.y;

                // NUEVA PARTIDA
                if(x >= rectNP.x && x <= rectNP.x+rectNP.w &&
                        y >= rectNP.y && y <= rectNP.y+rectNP.h)
                {
                    *minasRestante = minaConf->porcentaje ?
                                     (minaConf->cantMinas*(*TAM_GRILLA)*(*TAM_GRILLA))/100 :
                                     minaConf->cantMinas;
                    *cheat = 1;

                    int res = inicioDelJuego(renderer,*minas,TAM_GRILLA,minaConf,guardar);
                    if (res == 0)
                    {
                        TTF_CloseFont(font);
                        return TERMINAR;
                    }

                    TTF_CloseFont(font);
                    return MENU_NUEVA;
                }

                // CONTINUAR
                if (continuar &&
                        x >= rectC.x && x <= rectC.x + rectC.w &&
                        y >= rectC.y && y <= rectC.y + rectC.h)
                {
                    FILE* fLoad = fopen("ultima_partida","rb");
                    if(!fLoad) continue;

                    tGuardado header;
                    if (fread(&header, sizeof(tGuardado), 1, fLoad) != 1)
                    {
                        fclose(fLoad);
                        continue;
                    }
                    fclose(fLoad);

                    // SI EL TAM GRILLA CAMBIÓ → REALLOCAR MATRIZ
                    if (header.tam_matriz != *TAM_GRILLA)
                    {
                        liberarMinas(minas, *TAM_GRILLA);
                        *TAM_GRILLA = header.tam_matriz;

                        if (configMinas(minas, TAM_GRILLA) == ERROR_MEMORIA)
                            return ERROR_MEMORIA;
                    }

                    // CARGAR MATRIZ
                    if (load(*minas, guardar, *TAM_GRILLA) == 1)
                    {
                        printf("Error al cargar partida\n");
                        continue;
                    }

                    *minasRestante = guardar->minas_restantes;
                    *cheat = guardar->cheat;
                    *inicioTiempo = SDL_GetTicks() - guardar->tiempo * 1000;

                    continuarJuego(ventana, renderer, *minas, TAM_GRILLA, numeros);
                    return MENU_CONT;
                }


                // AJUSTES
                if(x >= rectA.x && x <= rectA.x + rectA.w &&
                        y >= rectA.y && y <= rectA.y + rectA.h)
                {
                    TTF_CloseFont(font);
                    return MENU_AJUSTES;
                }
            }
        }
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    return MENU_SALIR;
}


int ajustesSDL(SDL_Renderer* renderer, int* TAM_GRILLA, tMina* minaConf, SDL_Event* event)
{
    int terminar = 0;

    if(TTF_WasInit() == 0 && TTF_Init() != 0)
    {
        SDL_Log("Error TTF: %s", TTF_GetError());
        return FIN_JUEGO;
    }

    TTF_Font* font = TTF_OpenFont("arialbd.ttf",20);
    if(!font) return ERROR_TEXTO;

    SDL_Color color = {255,255,255,255};
    SDL_Color btnColor = {0,0,200,255};
    SDL_Color okColor = {0,200,0,255};
    SDL_Color warnColor = {200,100,0,255};

    // Botones + - y toggle
    SDL_Rect btnDimMas     = {100,100,50,50};
    SDL_Rect btnDimMenos   = {160,100,50,50};
    SDL_Rect btnMinasMas   = {100,200,50,50};
    SDL_Rect btnMinasMenos = {160,200,50,50};
    SDL_Rect btnTogglePct  = {100,300,110,50};

    // Botones Guardar y Volver
    SDL_Rect btnGuardar = {100, 400, 140, 50};
    SDL_Rect btnVolver  = {260, 400, 140, 50};

    while(!terminar)
    {
        while(SDL_PollEvent(event))
        {
            if(event->type == SDL_QUIT)
            {
                TTF_CloseFont(font);
                return FIN_JUEGO;
            }

            if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE)
            {
                terminar = 1; // volver al menu sin guardar
            }

            if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
            {
                int x = event->button.x;
                int y = event->button.y;

                // AUMENTAR DIMENSIÓN +
                if(x>=btnDimMas.x && x<=btnDimMas.x+btnDimMas.w &&
                        y>=btnDimMas.y && y<=btnDimMas.y+btnDimMas.h)
                {
                    if(*TAM_GRILLA < 32) (*TAM_GRILLA)++;
                    // ajustar minas si exceden nuevas celdas
                    int maxCells = (*TAM_GRILLA)*(*TAM_GRILLA) - 1;
                    if(!minaConf->porcentaje && minaConf->cantMinas > maxCells) minaConf->cantMinas = maxCells;
                }

                // DISMINUIR DIMENSIÓN -
                if(x>=btnDimMenos.x && x<=btnDimMenos.x+btnDimMenos.w &&
                        y>=btnDimMenos.y && y<=btnDimMenos.y+btnDimMenos.h)
                {
                    if(*TAM_GRILLA > 5) (*TAM_GRILLA)--; // mínimo 5 para jugar
                    int maxCells = (*TAM_GRILLA)*(*TAM_GRILLA) - 1;
                    if(!minaConf->porcentaje && minaConf->cantMinas > maxCells) minaConf->cantMinas = maxCells;
                }

                // AUMENTAR MINAS
                if(x>=btnMinasMas.x && x<=btnMinasMas.x+btnMinasMas.w &&
                        y>=btnMinasMas.y && y<=btnMinasMas.y+btnMinasMas.h)
                {
                    if(minaConf->porcentaje)
                    {
                        if(minaConf->cantMinas < 90) minaConf->cantMinas++; // porcentaje hasta 90%
                    }
                    else
                    {
                        int maxCells = (*TAM_GRILLA)*(*TAM_GRILLA) - 1;
                        if(minaConf->cantMinas < maxCells) minaConf->cantMinas++;
                    }
                }

                // DISMINUIR MINAS
                if(x>=btnMinasMenos.x && x<=btnMinasMenos.x+btnMinasMenos.w &&
                        y>=btnMinasMenos.y && y<=btnMinasMenos.y+btnMinasMenos.h)
                {
                    if(minaConf->cantMinas > 1) minaConf->cantMinas--;
                }

                // CAMBIAR A PORCENTAJE
                if(x>=btnTogglePct.x && x<=btnTogglePct.x+btnTogglePct.w &&
                        y>=btnTogglePct.y && y<=btnTogglePct.y+btnTogglePct.h)
                {
                    minaConf->porcentaje = !minaConf->porcentaje;
                    // si cambiamos y las minas > MÁXIMO DE CELDAS, reducir
                    if(!minaConf->porcentaje)
                    {
                        int maxCells = (*TAM_GRILLA)*(*TAM_GRILLA) - 1;
                        if(minaConf->cantMinas > maxCells) minaConf->cantMinas = maxCells;
                    }
                    else
                    {
                        if(minaConf->cantMinas > 90) minaConf->cantMinas = 90;
                    }
                }

                // GUARDAR
                if(x>=btnGuardar.x && x<=btnGuardar.x+btnGuardar.w &&
                        y>=btnGuardar.y && y<=btnGuardar.y+btnGuardar.h)
                {
                    // VALIDACIONES antes de guardar
                    int ok = 1;
                    int maxCells = (*TAM_GRILLA)*(*TAM_GRILLA) - 1;
                    if(!minaConf->porcentaje)
                    {
                        if(minaConf->cantMinas < 1 || minaConf->cantMinas > maxCells)
                            ok = 0;
                    }
                    else
                    {
                        if(minaConf->cantMinas < 1 || minaConf->cantMinas > 90) // 1%..90%
                            ok = 0;
                    }

                    if(!ok)
                    {
                        // mostrar un mensaje breve: dibujamos texto "ERROR" encima del botón guardar por unos frames
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);
                        // dibujado mínimo para ver el error
                        SDL_SetRenderDrawColor(renderer, btnColor.r, btnColor.g, btnColor.b, btnColor.a);
                        SDL_RenderFillRect(renderer, &btnGuardar);
                        SDL_SetRenderDrawColor(renderer, warnColor.r, warnColor.g, warnColor.b, warnColor.a);
                        SDL_Rect r = {btnGuardar.x, btnGuardar.y-30, 200, 24};
                        SDL_RenderFillRect(renderer, &r);

                        SDL_Surface* s = TTF_RenderText_Solid(font,"ERROR: Minas invalidas", color);
                        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
                        SDL_FreeSurface(s);
                        SDL_Rect d = {btnGuardar.x, btnGuardar.y-30, 200, 24};
                        SDL_RenderCopy(renderer, t, NULL, &d);
                        SDL_DestroyTexture(t);
                        SDL_RenderPresent(renderer);
                        SDL_Delay(900);
                        continue; // volver sin guardar
                    }

                    // Escribir archivo de config
                    FILE* conf = fopen("buscaminas.config.txt","w");
                    if(!conf)
                    {
                        // mostrar error simple y volver
                        printf("No se pudo abrir buscaminas.config.txt para escribir.\n");
                        continue;
                    }

                    fprintf(conf,"dimension=%d\n", *TAM_GRILLA);
                    if(minaConf->porcentaje)
                        fprintf(conf,"minas=%d%%\n", minaConf->cantMinas);
                    else
                        fprintf(conf,"minas=%d\n", minaConf->cantMinas);
                    fclose(conf);

                    // todo OK: volvemos al menu
                    TTF_CloseFont(font);
                    return OK;
                }

                // VOLVER (sin guardar)
                if(x>=btnVolver.x && x<=btnVolver.x+btnVolver.w &&
                        y>=btnVolver.y && y<=btnVolver.y+btnVolver.h)
                {
                    terminar = 1;
                }
            }
        }

        // DIBUJADO
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        // botones +/-
        SDL_SetRenderDrawColor(renderer, btnColor.r, btnColor.g, btnColor.b, btnColor.a);
        SDL_RenderFillRect(renderer, &btnDimMas);
        SDL_RenderFillRect(renderer, &btnDimMenos);
        SDL_RenderFillRect(renderer, &btnMinasMas);
        SDL_RenderFillRect(renderer, &btnMinasMenos);
        SDL_RenderFillRect(renderer, &btnTogglePct);

        // Botones Guardar/Volver
        SDL_SetRenderDrawColor(renderer, okColor.r, okColor.g, okColor.b, okColor.a);
        SDL_RenderFillRect(renderer, &btnGuardar);
        SDL_SetRenderDrawColor(renderer, btnColor.r, btnColor.g, btnColor.b, btnColor.a);
        SDL_RenderFillRect(renderer, &btnVolver);

        // Dibujar textos
        SDL_Surface* s;
        SDL_Texture* t;
        SDL_Rect d;

        // + y - texto en botones
        s = TTF_RenderText_Solid(font, "+", color);
        t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_FreeSurface(s);
        d = (SDL_Rect)
        {
            btnDimMas.x+12, btnDimMas.y+6, 26, 38
        };
        SDL_RenderCopy(renderer, t, NULL, &d);
        SDL_DestroyTexture(t);

        s = TTF_RenderText_Solid(font, "-", color);
        t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_FreeSurface(s);
        d = (SDL_Rect)
        {
            btnDimMenos.x+12, btnDimMenos.y+6, 26, 38
        };
        SDL_RenderCopy(renderer, t, NULL, &d);
        SDL_DestroyTexture(t);

        s = TTF_RenderText_Solid(font, "+", color);
        t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_FreeSurface(s);
        d = (SDL_Rect)
        {
            btnMinasMas.x+12, btnMinasMas.y+6, 26, 38
        };
        SDL_RenderCopy(renderer, t, NULL, &d);
        SDL_DestroyTexture(t);

        s = TTF_RenderText_Solid(font, "-", color);
        t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_FreeSurface(s);
        d = (SDL_Rect)
        {
            btnMinasMenos.x+12, btnMinasMenos.y+6, 26, 38
        };
        SDL_RenderCopy(renderer, t, NULL, &d);
        SDL_DestroyTexture(t);

        // CAMBIAR texto
        char buf[32];
        snprintf(buf, sizeof(buf), "%s", minaConf->porcentaje ? "Porcentaje" : "Cantidad");
        s = TTF_RenderText_Solid(font, buf, color);
        t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_FreeSurface(s);
        d = (SDL_Rect)
        {
            btnTogglePct.x + 6, btnTogglePct.y+10, btnTogglePct.w-12, btnTogglePct.h-20
        };
        SDL_RenderCopy(renderer, t, NULL, &d);
        SDL_DestroyTexture(t);

        // Mostrar tamaño y minas actuales
        char info[64];
        snprintf(info, sizeof(info), "Dimension: %d", *TAM_GRILLA);
        s = TTF_RenderText_Solid(font, info, color);
        t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_FreeSurface(s);
        d = (SDL_Rect)
        {
            220,100,200,40
        };
        SDL_RenderCopy(renderer, t, NULL, &d);
        SDL_DestroyTexture(t);

        if(minaConf->porcentaje)
            snprintf(info, sizeof(info), "Minas: %d%%", minaConf->cantMinas);
        else
            snprintf(info, sizeof(info), "Minas: %d", minaConf->cantMinas);
        s = TTF_RenderText_Solid(font, info, color);
        t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_FreeSurface(s);
        d = (SDL_Rect)
        {
            220,200,200,40
        };
        SDL_RenderCopy(renderer, t, NULL, &d);
        SDL_DestroyTexture(t);

        // Botones Guardar/Volver texto
        s = TTF_RenderText_Solid(font, "GUARDAR", color);
        t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_FreeSurface(s);
        d = (SDL_Rect)
        {
            btnGuardar.x+8, btnGuardar.y+8, btnGuardar.w-16, btnGuardar.h-16
        };
        SDL_RenderCopy(renderer, t, NULL, &d);
        SDL_DestroyTexture(t);

        s = TTF_RenderText_Solid(font, "VOLVER", color);
        t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_FreeSurface(s);
        d = (SDL_Rect)
        {
            btnVolver.x+8, btnVolver.y+8, btnVolver.w-16, btnVolver.h-16
        };
        SDL_RenderCopy(renderer, t, NULL, &d);
        SDL_DestroyTexture(t);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    return OK;
}



int procesarCheat(int* cheat, int* minasRestante, SDL_Window** ventana,SDL_Renderer** renderer,int*** minas,const int bandera[][TAM_PIXEL],int* TAM_GRILLA)
{
    int buscar=1;
    int i=0;
    int j=0;
    while( buscar )
    {
        if( (*minas)[i][j]==99 )
        {
            mostrarCuadrado(*ventana,*renderer,*minas,i,j,bandera,*TAM_GRILLA);
            (*minas)[i][j]=( (*minas)[i][j]*100 );
            *cheat=0;
            buscar=0;
            (*minasRestante)--;
        }
        j++;
        if ( j==*TAM_GRILLA )
        {
            j=0;
            i++;
        }
        if( i == *TAM_GRILLA )
            buscar=0;
    }
}

int procesarRedimension(SDL_Window** ventana, SDL_Renderer** renderer,
                        int* minasRestante, const int botonAmp[][TAM_PIXEL],
                        int*** minas, const int (*numerosAux[])[8],
                        tMina* minaConf, int* TAM_GRILLA)
{
    if (!minas || !*minas || !TAM_GRILLA || !minaConf || !minasRestante)
        return ERROR_MEMORIA;

    int viejoTam = *TAM_GRILLA;
    int nuevoTam = viejoTam + 2;

    if (nuevoTam > 32)
    {
        printf("NO SE PUDO AMPLIFICAR: limite alcanzado\n");
        return OK;
    }

    //buffer temporal para copiar la vieja grilla
    int minasAux[viejoTam][viejoTam];
    for (int i = 0; i < viejoTam; i++)
        for (int j = 0; j < viejoTam; j++)
            minasAux[i][j] = (*minas)[i][j];

    //Realloc del array de punteros (filas)
    int **rows = realloc(*minas, sizeof(int*) * nuevoTam);
    if (!rows)
    {
        printf("No hay espacio para redimensionar el array de filas.\n");
        return ERROR_MEMORIA;
    }
    *minas = rows;

    //Realloc/malloc de cada fila y inicialización
    for (int i = 0; i < nuevoTam; i++)
    {
        if (i < viejoTam)
        {
            int *r = realloc((*minas)[i], sizeof(int) * nuevoTam);
            if (!r)
            {
                printf("No hay espacio para redimensionar fila %d.\n", i);
                //rollback: liberar nuevas filas creadas y dejar estado consistente
                for (int k = viejoTam; k < i; k++) free((*minas)[k]);
                //no free de filas antiguas ni de *minas: se deja como estaba
                return -1;
            }
            (*minas)[i] = r;
        }
        else
        {
            (*minas)[i] = malloc(sizeof(int) * nuevoTam);
            if (!(*minas)[i])
            {
                printf("No hay espacio para crear fila %d.\n", i);
                for (int k = viejoTam; k < i; k++) free((*minas)[k]);
                return -1;
            }
        }

        //inicializar toda la fila a 0
        for (int j = 0; j < nuevoTam; j++)
            (*minas)[i][j] = 0;
    }

    //Restaurar contenido antiguo en el área correspondiente
    for (int i = 0; i < viejoTam; i++)
    {
        for (int j = 0; j < viejoTam; j++)
            (*minas)[i][j] = minasAux[i][j];
    }

    //Actualizar TAM_GRILLA
    *TAM_GRILLA = nuevoTam;

    //Calcular cuántas minas agregar
    int minasAmp = 0;
    if (minaConf->porcentaje == 1)
    {
        //si porcentaje==1 la cantidad es porcentaje (%) sobre total
        int totalAnt = (minaConf->cantMinas * viejoTam * viejoTam) / 100;
        int totalNuevo = (minaConf->cantMinas * nuevoTam * nuevoTam) / 100;
        minasAmp = totalNuevo - totalAnt;
    }
    else
    {
        //agregar una cantidad fija basada en tamaño
        minasAmp = (nuevoTam * 4 - 4) / 4;
        minaConf->cantMinas += minasAmp;
    }

    *minasRestante += minasAmp;

    //Insertar minas y recalcular números
    insertarMinasAmp(*minas, minasAmp, TAM_GRILLA);
    insertarNumeroTodaGrillaAmp(*minas, TAM_GRILLA);

    //Redibujar la pantalla
    SDL_RenderClear(*renderer);
    crearPantallaBuscamina(*renderer, TAM_GRILLA);
    dibujarBoton(*ventana, *renderer, botonAmp, TAM_BOTONX, TAM_BOTONY, *TAM_GRILLA);

    for (int i = 0; i < (*TAM_GRILLA - 2); i++)
    {
        for (int j = 0; j < (*TAM_GRILLA - 2); j++)
        {
            if ((*minas)[i][j] >= 10 && (*minas)[i][j] <= 80)
            {
                int p = (*minas)[i][j] / 10;
                mostrarCuadrado(*ventana, *renderer, *minas, i, j, numerosAux[p], *TAM_GRILLA);

                if ((*minas)[i][j] == VISTO)
                    mostrarCuadrado(*ventana, *renderer, *minas, i, j, numerosAux[0], *TAM_GRILLA);
            }

            if ((*minas)[i][j] >= 100)
                mostrarCuadrado(*ventana, *renderer, *minas, i, j, numerosAux[9], *TAM_GRILLA);
        }
    }

    ultimosDiez(*ventana, *renderer, *TAM_GRILLA);

    return OK;
}

int procesarClickIzquierdo(SDL_Window* ventana,
                           SDL_Renderer* renderer,
                           int** minas,
                           int pos_X,
                           int pos_Y,
                           const int (*numeros[])[8],
                           int TAM_GRILLA,
                           int* buscamina,
                           const int boom[][TAM_PIXEL],
                           const int boom_2[][TAM_PIXEL],
                           const int boom_3[][TAM_PIXEL],
                           const int boom_4[][TAM_PIXEL])
{
    //REVISAR TODO EL TABLERO POR CASILLEROS ERRÓNEOS
    for(int i = 0; i < TAM_GRILLA; i++)
    {
        for(int j = 0; j < TAM_GRILLA; j++)
        {
            if(numeroMinas(minas, i, j, &TAM_GRILLA) > 10 &&
                    minas[i][j] != HAY_MINA)
            {
                mostrarCuadrado(ventana, renderer, minas,
                                pos_X, pos_Y,
                                numeros[9], TAM_GRILLA);
            }
        }
    }

    //EXPLOTÓ MINA
    if(minas[pos_X][pos_Y] == HAY_MINA)
    {
        mostrarCuadrado(ventana, renderer, minas,
                        pos_X, pos_Y, boom, TAM_GRILLA);

        mostrarMinas(ventana, renderer, minas,
                     boom, boom_2, boom_3, boom_4,
                     TAM_GRILLA);

        vaciarContinuar();//SI PERDIÓ YA NO PUEDE CONTINUAR LA MISMA PARTIDA
        *buscamina = 0;
        return OK;
    }

    //CASILLA VACÍA: HACER CASCADA
    if(minas[pos_X][pos_Y] == 0)
    {
        cascada(ventana, renderer, minas,
                pos_X, pos_Y,
                numeros, TAM_GRILLA);

        return OK;
    }

    //MOSTRAR NÚMERO
    if(minas[pos_X][pos_Y] >= 1 && minas[pos_X][pos_Y] <= 8)
    {
        mostrarCuadrado(ventana, renderer, minas,
                        pos_X, pos_Y,
                        numeros[minas[pos_X][pos_Y]],
                        TAM_GRILLA);

        minas[pos_X][pos_Y] *= 10;   // marcar como visto
    }

    return OK;
}


int procesarClickDerecho(SDL_Window *ventana,
                         SDL_Renderer *renderer,
                         int **minas,
                         int pos_X,
                         int pos_Y,
                         int *minasRestante,
                         int TAM_GRILLA,
                         const int cuadrado[][8],
                         const int bandera[][8])
{
    int valor = minas[pos_X][pos_Y];

    //Quitar bandera
    if ((valor >= 100 && valor <= 800) ||
            valor == (HAY_MINA * 100) ||
            valor == 900)
    {
        if (valor == (HAY_MINA * 100))
            (*minasRestante)++;

        mostrarCuadrado(ventana, renderer, minas,
                        pos_X, pos_Y,
                        cuadrado,
                        TAM_GRILLA);

        if (valor == 900)
            minas[pos_X][pos_Y] = 0;
        else
            minas[pos_X][pos_Y] /= 100;

        return OK;
    }

    //Colocar bandera
    if (valor >= 10 && valor <= 80)
    {
        printf("No se puede poner bandera en un casillero revelado.\n");
        return OK;
    }

    if (valor == 0)
    {
        mostrarCuadrado(ventana, renderer, minas,
                        pos_X, pos_Y,
                        bandera,
                        TAM_GRILLA);

        minas[pos_X][pos_Y] = 900;
        return OK;
    }

    if (valor != VISTO)
    {
        mostrarCuadrado(ventana, renderer, minas,
                        pos_X, pos_Y,
                        bandera,
                        TAM_GRILLA);

        if (valor == HAY_MINA)
            (*minasRestante)--;

        minas[pos_X][pos_Y] *= 100;
        return OK;
    }

    return OK;
}



int configBuscamina(const char* nomArchBuscaminaConf, int *TAM_GRILLA, tMina* minaConf)
{
    char buffer[50];
    char* aux;
    FILE *conf = fopen(nomArchBuscaminaConf, "r+");

    // Si el archivo no existe, crearlo con valores predeterminados
    if(!conf)
    {
        conf = fopen(nomArchBuscaminaConf, "w+");
        if(!conf)
        {
            printf("\nError al crear archivo de configuracion.");
            return ERROR_ARCHIVO;
        }
        fprintf(conf, "dimension=8\nminas=10%%\n"); // predeterminado
        fclose(conf);

        *TAM_GRILLA = 8;
        minaConf->cantMinas = 10;
        minaConf->porcentaje = 1;

        return OK;
    }

    // Leer dimension
    if(fgets(buffer,sizeof(buffer),conf))
    {
        aux = strchr(buffer,'\n');
        if(aux) *aux='\0';

        aux = strrchr(buffer,'=');
        if(!aux)
        {
            fclose(conf);
            return ERROR_ARCHIVO;
        }

        *TAM_GRILLA = atoi(aux+1);
    }
    else
    {
        fclose(conf);
        return ERROR_ARCHIVO;
    }

    // Leer minas
    if(fgets(buffer,sizeof(buffer),conf))
    {
        aux = strchr(buffer,'\n');
        if(aux) *aux='\0';

        if(strchr(buffer,'%'))
            minaConf->porcentaje = 1;
        else
            minaConf->porcentaje = 0;

        aux = strrchr(buffer,'=');
        if(!aux)
        {
            fclose(conf);
            return ERROR_ARCHIVO;
        }

        minaConf->cantMinas = atoi(aux+1);
    }
    else
    {
        fclose(conf);
        return ERROR_ARCHIVO;
    }

    fclose(conf);
    return OK;
}


int configMinas(int*** minas, int* TAM_GRILLA)
{
    *minas = malloc(sizeof(int*) * (*TAM_GRILLA));
    if(!*minas)
    {
        printf("\nEl espacio no era suficiente.");
        return -1;
    }

    for (int i = 0; i < *TAM_GRILLA; i++)
    {
        (*minas)[i] = malloc(sizeof(int) * (*TAM_GRILLA));
        if(!(*minas)[i])
        {
            printf("\nNo hay espacio suficiente.");
            for(int j = 0; j < i; j++)
                free((*minas)[j]);
            free(*minas);
            return ERROR_MEMORIA;
        }
    }

    return OK;
}

int crearVentana(SDL_Window** ventana,int* TAM_GRILLA)
{
    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        printf("Error al intentar abrir SDL\n");
        return ERROR_VENTANA;
    }

    char nombreVentana[100];
    sprintf(nombreVentana, "Buscamina %dx%d",*TAM_GRILLA,*TAM_GRILLA);
    *ventana = SDL_CreateWindow(nombreVentana,
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                (TAM_VENTANA+TAM_VENTANA_INFO), (TAM_VENTANA),
                                SDL_WINDOW_ALWAYS_ON_TOP);

    if(!*ventana)
        return ERROR_VENTANA;

    return OK;
}

int crearRender(SDL_Renderer** renderer, SDL_Window** ventana)
{
    *renderer = SDL_CreateRenderer(*ventana, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        printf("Error al crear el renderer: %s\n", SDL_GetError());
        return ERROR_VENTANA;
    }

    if (SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND) != 0)
    {
        printf("Error al establecer el blend mode: %s\n", SDL_GetError());
        SDL_DestroyRenderer(*renderer);
        return ERROR_VENTANA;
    }

    return OK;
}


void inicializar(int **minas, int *TAM_GRILLA)
{
    for(int i = 0; i < (*TAM_GRILLA); i++)
        for(int j = 0; j < (*TAM_GRILLA); j++)
            minas[i][j] = 0;
}

SDL_Color colores[] =
{
    {0, 0, 0, 255},        // N[0] - Negro
    {128, 128, 128, 255},  // G[1] - Gris
    {255, 255, 0, 255},    // A[2] - Amarillo
    {255, 255, 255, 255},  // B[3] - Blanco
    {0, 150, 60, 255},     // V[4] - Verde
    {255, 0, 0, 255},      // R[5] - Rojo
    {255, 0, 255, 255},    //VI[6] - Violeta
    {0, 0, 255, 255},      //AZ[7] - Azul
    {0, 255, 255, 255},    //C[8] - Celeste
    {255, 128, 0, 255},    //NA[9] - Naranja
    {50, 50, 50, 255},     // X[10] - Gris oscuro
    {128, 64, 0, 255},     //M[11] - Marron
    {252, 208, 180, 255} //NC[12] - Naranja claro

};

int save(int** minas,tGuardado* guardar, int* TAM_GRILLA,Uint32* inicioTiempo,int* cheat,int* minasRestante)
{
    FILE* fSave;
    fSave = fopen("ultima_partida","wb");
    if(!fSave)
    {
        printf("\nHubo un error al guardar el archivo de guardado.");
        return -1;
    }


    guardar->tam_matriz=*TAM_GRILLA;
    guardar->tiempo = (SDL_GetTicks() - *inicioTiempo) / 1000;  // Tiempo jugado en segundos
    guardar->minas_restantes=*minasRestante;
    guardar->cheat=*cheat;

    if (fwrite(guardar, sizeof(tGuardado), 1, fSave) != 1)
    {
        printf("\nError al escribir los datos de guardado.");
        fclose(fSave);
        return -1;
    }

    for(int i = 0; i < (*TAM_GRILLA); i++)
    {
        if (fwrite(minas[i], sizeof(int), (*TAM_GRILLA), fSave) != (*TAM_GRILLA))
        {
            printf("\nError al escribir la fila %d.", i);
            fclose(fSave);
            return -1;
        }
    }

    fclose(fSave);
    return 0;
}

Uint32 load(int **minas, tGuardado *abrir, int TAM_GRILLA)
{
    FILE* fLoad = fopen("ultima_partida", "rb");
    if (!fLoad)
        return 1;

    // LEER
    if (fread(abrir, sizeof(tGuardado), 1, fLoad) != 1)
    {
        fclose(fLoad);
        return 1;
    }

    // LEER MATRIZ
    for (int i = 0; i < TAM_GRILLA; i++)
    {
        if (fread(minas[i], sizeof(int), TAM_GRILLA, fLoad) != TAM_GRILLA)
        {
            fclose(fLoad);
            return 1;
        }
    }

    fclose(fLoad);
    return abrir->tiempo;
}



int vaciarContinuar()
{
    FILE* pf=fopen("ultima_partida","rb");

    if(!pf)
        return OK;

    fclose(pf);
    remove("ultima_partida");
    return OK;
}


void mostrarInfoVentana(SDL_Window *ventana,SDL_Renderer *renderer,Uint32* inicioTiempo,int* minasRestante,tGuardado* ver)
{
    // Armar string del nick
    char verNombre[]="Nombre:";

    // Crear superficie y textura del texto
    SDL_Color color = {255, 255, 255, 255}; // Color blanco
    TTF_Font* font = TTF_OpenFont("arialbd.ttf", 24);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, ver->nick, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Posición y tamaño para el render
    SDL_Rect dstRect = {(TAM_VENTANA+100), 100, textSurface->w, textSurface->h};

    // Copiar al renderer
    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

    textSurface = TTF_RenderText_Solid(font, verNombre, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    dstRect.x=TAM_VENTANA;
    dstRect.y=100;
    dstRect.h=textSurface->h;
    dstRect.w=textSurface->w;

    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);


    char minasStr[32];
    snprintf(minasStr, sizeof(minasStr), "Minas: %d", *minasRestante);
    textSurface = TTF_RenderText_Solid(font, minasStr, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);


    SDL_Rect limpiarRect =
    {
        TAM_VENTANA,
        150,
        150,
        textSurface->h
    };


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &limpiarRect);


    dstRect.x=TAM_VENTANA;
    dstRect.y=150;
    dstRect.h=textSurface->h;
    dstRect.w=textSurface->w;
    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

    // Calcular tiempo transcurrido en segundos
    Uint32 tiempo = (SDL_GetTicks() - (*inicioTiempo)) / 1000;

    // Armar string del cronómetro
    char timeStr[32];
    snprintf(timeStr, sizeof(timeStr), "Tiempo: %02u:%02u", tiempo / 60, tiempo % 60);

    // Crear superficie y textura del texto
    textSurface = TTF_RenderText_Solid(font, timeStr, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Posición y tamaño para el render
    dstRect.x=TAM_VENTANA;
    dstRect.y=1;
    dstRect.h=textSurface->h;
    dstRect.w=textSurface->w;

    // Limpiar solo el área del cronómetro con un rect negro
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Color negro (mismo que el fondo)
    SDL_RenderFillRect(renderer, &dstRect);          // Llenar el área del cronómetro

    // Copiar al renderer
    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

    // Liberar recursos temporales
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    SDL_RenderPresent(renderer);


    SDL_Delay(100);
}

void crearPantallaBuscamina(SDL_Renderer *renderer, int* TAM_GRILLA)
{
    SDL_Rect rect;

    printf("Creando ventana...\n");
    SDL_Delay(100);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int tam_celda = TAM_VENTANA / (*TAM_GRILLA);
    int sobrante = TAM_VENTANA - (tam_celda * (*TAM_GRILLA));
    int margen = sobrante / 2;
    int miniPaso = tam_celda / 8;

    for (int fila = 0; fila < (*TAM_GRILLA); fila++)
    {
        for (int col = 0; col < (*TAM_GRILLA); col++)
        {
            int x = margen + col * tam_celda + miniPaso;
            int y = margen + fila * tam_celda + miniPaso;

            rect.x = x;
            rect.y = y;
            rect.w = tam_celda - 2 * miniPaso;
            rect.h = tam_celda - 2 * miniPaso;

            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
}

void insertarMinas(int **minas,tMina *minaConf, int *TAM_GRILLA)
{
    int pX;
    int pY;
    int cantMinas=minaConf->cantMinas;
    srand(time(NULL));

    if( minaConf->porcentaje==1 )
    {
        cantMinas = ( (minaConf->cantMinas)* (*TAM_GRILLA) * (*TAM_GRILLA) ) / 100;
    }

    while( cantMinas>0 )
    {
        pX=(rand()%(*TAM_GRILLA));
        pY=(rand()%(*TAM_GRILLA));
        while( minas[pX][pY]==99 )
        {
            pX=(rand()%(*TAM_GRILLA));
            pY=(rand()%(*TAM_GRILLA));
        }

        minas[pX][pY]=99;

        cantMinas--;
    }
}

void insertarMinasAmp(int **minas,int minasAmp,int*TAM_GRILLA)
{
    int pX;
    int pY;

    while (minasAmp > 0)
    {
        pX = rand() % (*TAM_GRILLA);
        pY = rand() % (*TAM_GRILLA);

        if ((pX >= (*TAM_GRILLA - 2) || pY >= (*TAM_GRILLA - 2)) && minas[pX][pY] != 99)
        {
            minas[pX][pY] = 99;
            minasAmp--;
        }
    }
}

void fechaHoraActual()
{
    time_t ahora=time(NULL);
    struct tm *tiempoLocal;
    char buffer[100];

    tiempoLocal = localtime(&ahora);
    time(&ahora);
    fflush(stdin);
    strftime(buffer, sizeof(buffer), "Fecha y hora: %d/%m/%Y %H:%M:%S", tiempoLocal);
    printf("\n%s\n", buffer);
    fflush(stdout);
}

int inicioDelJuego(SDL_Renderer *renderer, int **minas, int *TAM_GRILLA, tMina *minaConf, tGuardado *guardar)
{
    SDL_StartTextInput();

    SDL_Event event;
    char nombre[4] = "";
    int pos = 0;
    int seguir = 1;
    guardar->cheat=1;
    if (TTF_Init() != 0)
    {
        SDL_Log("Error al inicializar SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    TTF_Font *font = TTF_OpenFont("arialbd.ttf", 24);
    if (!font)
    {
        SDL_Log("Error al cargar fuente: %s", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    SDL_Rect inputBox = {300, 300, 300, 50};

    while (seguir)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                SDL_StopTextInput();
                TTF_CloseFont(font);
                TTF_Quit();
                return TERMINAR;
            }
            if (event.type == SDL_TEXTINPUT)
            {
                if (pos < sizeof(nombre) - 1)
                {
                    nombre[pos++] = event.text.text[0];
                    nombre[pos] = '\0';
                }
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_BACKSPACE && pos > 0)
                {
                    nombre[--pos] = '\0';
                }
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    seguir = 0;
                    break;
                }
            }
        }

        //Escribir el texto

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &inputBox);

        SDL_Color color = {255, 255, 255};
        SDL_Surface *surface = NULL;
        SDL_Texture *texture = NULL;
        int textoAncho = 0;
        int textoAlto = 0;
        SDL_Rect dstRect;
        char escribir[]="Elija su nick de 3 caracteres maximo y presione enter:";
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, escribir, color);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        dstRect.x=225;
        dstRect.y=250;
        dstRect.h=textSurface->h;
        dstRect.w=textSurface->w;

        SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

        if (strlen(nombre) > 0)
        {
            surface = TTF_RenderText_Blended(font, nombre, color);
            if (surface)
            {
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture)
                {
                    textoAncho = surface->w;
                    textoAlto = surface->h;

                    dstRect.x = inputBox.x + 10;
                    dstRect.y = inputBox.y + (inputBox.h - surface->h) / 2;
                    dstRect.w = surface->w;
                    dstRect.h = surface->h;

                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

                    SDL_DestroyTexture(texture);
                }
                SDL_FreeSurface(surface);
            }
        }
        //Escribir el texto fin

        Uint32 tiempo = SDL_GetTicks();
        if ((tiempo / 500) % 2) // parpadea cada 500 ms
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect cursorRect =
            {
                inputBox.x + 10 + textoAncho,
                inputBox.y + 10,
                2,
                inputBox.h - 20
            };
            SDL_RenderFillRect(renderer, &cursorRect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Para no saturar
    }

    strncpy(guardar->nick, nombre, sizeof(guardar->nick));
    guardar->nick[sizeof(guardar->nick) - 1] = '\0';

    for (int i = 0; i < 4; i++)
    {
        guardar->nick[i] = toupper((unsigned char)guardar->nick[i]);
    }

    SDL_StopTextInput();
    TTF_CloseFont(font);
    TTF_Quit();

    crearPantallaBuscamina(renderer, TAM_GRILLA);
    inicializar(minas, TAM_GRILLA);
    insertarMinas(minas, minaConf, TAM_GRILLA);
    insertarNumeroTodaGrilla(minas, TAM_GRILLA);
    mostrar(minas, TAM_GRILLA);

    printf("\n\nINICIO DEL JUEGO");
    fechaHoraActual();
    fflush(stdout);

    return OK;
}


void continuarJuego(SDL_Window *ventana,SDL_Renderer *renderer,int **minas,int *TAM_GRILLA, const int(*numeros[])[8])
{
    crearPantallaBuscamina(renderer,TAM_GRILLA);

    for( int i=0; i<(*TAM_GRILLA); i++ )
    {
        for(int j=0; j<(*TAM_GRILLA); j++)
        {
            if( minas[i][j]!=99 )
            {
                if( (minas[i][j]/10)<=8 && (minas[i][j]/10)>=1 )
                {
                    mostrarCuadrado(ventana,renderer,minas,i,j,numeros[(minas[i][j]/10)],*TAM_GRILLA);
                }
                if( minas[i][j]==11 )
                {
                    mostrarCuadrado(ventana,renderer,minas,i,j,numeros[0],*TAM_GRILLA);
                }
                if( minas[i][j]!=0 && (minas[i][j]%100)==0 )
                {
                    mostrarCuadrado(ventana,renderer,minas,i,j,numeros[9],*TAM_GRILLA);
                }
            }
        }
    }

    mostrar(minas, TAM_GRILLA);

}

int finDelJuego(int* TAM_GRILLA, SDL_Window** ventana, int*** minas)
{
    if (!TAM_GRILLA || !minas) return -1;

    SDL_Delay(500);

    if (ventana && *ventana)
    {
        SDL_DestroyWindow(*ventana);
        *ventana = NULL;
    }
    SDL_Quit();

    int n = *TAM_GRILLA;
    if (*minas == NULL)
    {
        fprintf(stderr, "Advertencia: *minas == NULL en finDelJuego, nada que liberar.\n");
    }
    else
    {
        printf("Liberando filas...\n");
        for (int i = 0; i < n; ++i)
        {
            //mostrar puntero antes de free
            printf("Fila %d -> %p\n", i, (void*)(*minas)[i]);
            if ((*minas)[i])
            {
                free((*minas)[i]);
                (*minas)[i] = NULL;
            }
            else
            {
                printf("Fila %d ya era NULL\n", i);
            }
        }

        printf("Liberando array de punteros -> %p\n", (void*)(*minas));
        free(*minas);
        *minas = NULL;
    }

    printf("\n\nFIN DEL JUEGO\n");
    fechaHoraActual();
    fflush(stdout);
    return 0;
}




void dibujarGanador(SDL_Window *ventana, SDL_Renderer *renderer,
                    const int dibujo[][10], int posX, int posY, int TAM_GRILLA)
{
    int tam_celda = TAM_VENTANA / TAM_GRILLA;

    // El tamaño de cada “pixel” del dibujo
    int dibujo_celdas = 5;
    int tam_dibujo = tam_celda * dibujo_celdas;
    int tam_pixel = tam_dibujo / 10;

    // Posición base del dibujo
    int offsetX = posX * tam_celda;
    int offsetY = posY * tam_celda;

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            SDL_SetRenderDrawColor(renderer,
                                   colores[dibujo[y][x]].r,
                                   colores[dibujo[y][x]].g,
                                   colores[dibujo[y][x]].b,
                                   255);

            SDL_Rect pixel =
            {
                offsetX + x * tam_pixel,
                offsetY + y * tam_pixel,
                tam_pixel,
                tam_pixel
            };

            SDL_RenderFillRect(renderer, &pixel);
        }
    }

    SDL_RenderPresent(renderer);
}

void dibujar(SDL_Window *ventana, SDL_Renderer *renderer,
             const int dibujo[][TAM_PIXEL], int posX, int posY, int TAM_GRILLA)
{
    int tam_celda = TAM_VENTANA / TAM_GRILLA;
    double escala_precisa = (double)tam_celda / TAM_PIXEL;
    int escala = (int)(escala_precisa + 0.5);  // Redondeamos

    if (escala < 1) escala = 1;

    int sobrante = TAM_VENTANA - (tam_celda * TAM_GRILLA);
    int margen = sobrante / 2;

    int offsetX = margen + posX * tam_celda;
    int offsetY = margen + posY * tam_celda;

    int ajusteX = (tam_celda - escala * TAM_PIXEL) / 2;
    int ajusteY = (tam_celda - escala * TAM_PIXEL) / 2;

    for (int y = 0; y < TAM_PIXEL; y++)
    {
        for (int x = 0; x < TAM_PIXEL; x++)
        {
            SDL_SetRenderDrawColor(renderer,
                                   colores[dibujo[y][x]].r,
                                   colores[dibujo[y][x]].g,
                                   colores[dibujo[y][x]].b,
                                   255);

            SDL_Rect pixel =
            {
                offsetX + ajusteX + x * escala,
                offsetY + ajusteY + y * escala,
                escala,
                escala
            };

            SDL_RenderFillRect(renderer, &pixel);
        }
    }

    SDL_RenderPresent(renderer);
}

void dibujarBoton(SDL_Window *ventana, SDL_Renderer *renderer, int const dibujo[][TAM_PIXEL], int posX, int posY, int TAM_GRILLA)
{

    for (int y = 0; y < TAM_PIXEL; y++)
    {
        for (int x = 0; x < TAM_PIXEL; x++)
        {
            SDL_SetRenderDrawColor(renderer,
                                   colores[dibujo[y][x]].r,
                                   colores[dibujo[y][x]].g,
                                   colores[dibujo[y][x]].b,
                                   255);

            SDL_Rect pixel =
            {
                posX + x * 30,
                posY + y * 30,
                30,
                30
            };

            SDL_RenderFillRect(renderer, &pixel);
        }
    }

    SDL_RenderPresent(renderer);
}

void mostrarCuadrado(SDL_Window *ventana, SDL_Renderer *renderer, int **minas,
                     int posX, int posY, const int (*mat)[8], int TAM_GRILLA)
{
    int tam_celda = TAM_VENTANA / TAM_GRILLA;
    int sobrante = TAM_VENTANA - (tam_celda * TAM_GRILLA);
    int margen = sobrante / 2;

    SDL_Rect celdaCompleta =
    {
        margen + posX * tam_celda,
        margen + posY * tam_celda,
        tam_celda,
        tam_celda
    };

    // Pintar fondo negro antes del dibujo
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &celdaCompleta);

    // Luego dibujar sobre la celda ya "limpia"
    dibujar(ventana, renderer, mat, posX, posY, TAM_GRILLA);
    SDL_RenderPresent(renderer);
}

int numeroMinas(int **minas, int x, int y, int *TAM_GRILLA) //RETORNA EL NUMERO DE MINAS ALREDEDOR y listo
{
    int acum;
    int x_ini = (x - 1), x_fin = (x + 1); //Seteamos para que arranque una fila y una columna antes
    int y_ini = (y - 1), y_fin = (y + 1); //asi recorre las 8 celdas de alrededor

    if (x_ini < 1) x_ini = 0;
    if (y_ini < 1) y_ini = 0;

    if(x_fin >= (*TAM_GRILLA) ) x_fin = ( (*TAM_GRILLA) - 1);
    if(y_fin >= (*TAM_GRILLA) ) y_fin = ( (*TAM_GRILLA) - 1);

    acum = 0;

    for(int i = x_ini; i <=x_fin; i++)
    {
        for(int j = y_ini; j <= y_fin; j++)
        {
            if(minas[i][j]==99)
            {
                acum++;
            }
        }
    }
    return acum;
}

void insertarNumeroTodaGrillaAmp(int **minas, int *TAM_GRILLA) //Esto completa toda la grilla
{
    for(int i = 0; i < (*TAM_GRILLA); i++)
    {
        for(int j = 0; j < (*TAM_GRILLA); j++)
        {
            if( ( i>=(*TAM_GRILLA-2) || j>=(*TAM_GRILLA-2) ) && minas[i][j]==0)
            {
                minas[i][j] = numeroMinas(minas, i, j, TAM_GRILLA);
            }
        }
    }

}

void insertarNumeroTodaGrilla(int **minas, int *TAM_GRILLA) //Esto completa toda la grilla
{
    for(int i = 0; i < (*TAM_GRILLA); i++)
    {
        for(int j = 0; j < (*TAM_GRILLA); j++)
        {
            if(minas[i][j]==0)
            {
                minas[i][j] = numeroMinas(minas, i, j, TAM_GRILLA);
            }
        }
    }

}

void mostrar(int **minas, int *TAM_GRILLA)       //Esto muestra por consola la grilla no se usa y usa tam grilla
{
    for(int i = 0; i < (*TAM_GRILLA); i++)
    {
        printf("\n");
        for(int j = 0; j < (*TAM_GRILLA); j++)
        {
            printf("[%d]", minas[j][i]);
        }
    }
}

void cascada(SDL_Window *ventana, SDL_Renderer *renderer,int **minas,int posX, int posY, const int (*numeros[])[8], int TAM_GRILLA)
{
    int p=minas[posX][posY];

//    SDL_Delay( 0.001 );
    mostrarCuadrado(ventana,renderer,minas,posX,posY,numeros[p], TAM_GRILLA);


    if( minas[posX][posY]==0 )
    {
        minas[posX][posY]=VISTO;

        if( (posX-1)>=0 )
        {
            if( minas[posX-1][posY]<=8 )
            {
                cascada(ventana,renderer,minas,posX-1,posY,numeros,TAM_GRILLA);
            }

            if( (posY-1)>=0 )
            {
                if( minas[posX-1][posY-1]<=8 )
                {
                    cascada(ventana,renderer,minas,posX-1,posY-1,numeros,TAM_GRILLA);
                }
                if( minas[posX][posY-1]<=8 )
                {
                    cascada(ventana,renderer,minas,posX,posY-1,numeros,TAM_GRILLA);
                }

            }

            if( (posY+1)<TAM_GRILLA )
            {

                if( minas[posX-1][posY+1]<=8 )
                {
                    cascada(ventana,renderer,minas,posX-1,posY+1,numeros,TAM_GRILLA);
                }

                if( minas[posX][posY+1]<=8 )
                {
                    cascada(ventana,renderer,minas,posX,posY+1,numeros,TAM_GRILLA);
                }

            }

        }

        if( (posX+1)<TAM_GRILLA )
        {

            if( minas[posX+1][posY]<=8 )
            {
                cascada(ventana,renderer,minas,posX+1,posY,numeros,TAM_GRILLA);
            }

            if( (posY-1)>=0 )
            {
                if( minas[posX+1][posY-1]<=8 )
                {
                    cascada(ventana,renderer,minas,posX+1,posY-1,numeros,TAM_GRILLA);
                }

                if( minas[posX][posY-1]<=8 )
                {
                    cascada(ventana,renderer,minas,posX,posY-1,numeros,TAM_GRILLA);
                }
            }

            if( (posY+1)<TAM_GRILLA )
            {

                if( minas[posX+1][posY+1]<=8 )
                {
                    cascada(ventana,renderer,minas,posX+1,posY+1,numeros,TAM_GRILLA);
                }

                if( minas[posX][posY+1]<=8 )
                {
                    cascada(ventana,renderer,minas,posX,posY+1,numeros,TAM_GRILLA);
                }
            }

        }

    }
    else
    {
        minas[posX][posY]=( minas[posX][posY]*10 );
    }


}

void mostrarMinas(SDL_Window *ventana, SDL_Renderer *renderer, int **minas,
                  const int grafico[][TAM_PIXEL], const int grafico2[][TAM_PIXEL],
                  const int grafico3[][TAM_PIXEL], const int grafico4[][TAM_PIXEL], int TAM_GRILLA)
{
    for(int i = 0; i < TAM_GRILLA; i++)
    {
        for(int j = 0; j < TAM_GRILLA; j++)
        {
            if( minas[i][j]==HAY_MINA || minas[i][j]/100==HAY_MINA )
                dibujar(ventana, renderer, grafico,i, j, TAM_GRILLA);
        }
    }

    SDL_Delay( 250 );

    for(int i = 0; i < TAM_GRILLA; i++)
    {
        for(int j = 0; j < TAM_GRILLA; j++)
        {
            if( minas[i][j]==HAY_MINA || minas[i][j]/100==HAY_MINA )
                dibujar(ventana, renderer, grafico2,i, j, TAM_GRILLA);
        }
    }

    SDL_Delay( 250 );

    for(int i = 0; i < TAM_GRILLA; i++)
    {
        for(int j = 0; j < TAM_GRILLA; j++)
        {
            if( minas[i][j]==HAY_MINA || minas[i][j]/100==HAY_MINA )
                dibujar(ventana, renderer, grafico3,i, j, TAM_GRILLA);
        }
    }

    SDL_Delay( 250 );

    for(int i = 0; i < TAM_GRILLA; i++)
    {
        for(int j = 0; j < TAM_GRILLA; j++)
        {
            if( minas[i][j]==HAY_MINA || minas[i][j]/100==HAY_MINA )
                dibujar(ventana, renderer, grafico, i, j, TAM_GRILLA);
        }
    }

    SDL_Delay( 250 );

    for(int i = 0; i < TAM_GRILLA; i++)
    {
        for(int j = 0; j < TAM_GRILLA; j++)
        {
            if( minas[i][j]==HAY_MINA || minas[i][j]/100==HAY_MINA )
                dibujar(ventana, renderer, grafico2,i, j, TAM_GRILLA);
        }
    }

    SDL_Delay( 250 );

    for(int i = 0; i < TAM_GRILLA; i++)
    {
        for(int j = 0; j < TAM_GRILLA; j++)
        {
            if( minas[i][j]==HAY_MINA || minas[i][j]/100==HAY_MINA )
                dibujar(ventana, renderer, grafico3,i, j, TAM_GRILLA);
        }
    }

    SDL_Delay( 250 );

    for(int i = 0; i < TAM_GRILLA; i++)
    {
        for(int j = 0; j < TAM_GRILLA; j++)
        {
            if( minas[i][j]==HAY_MINA || minas[i][j]/100==HAY_MINA )
                dibujar(ventana, renderer, grafico, i, j, TAM_GRILLA);
        }
    }

    SDL_Delay( 300 );

    for(int i = 0; i < TAM_GRILLA; i++)
    {
        for(int j = 0; j < TAM_GRILLA; j++)
        {
            if( minas[i][j]==HAY_MINA || minas[i][j]/100==HAY_MINA )
                dibujar(ventana, renderer, grafico4, i, j, TAM_GRILLA);
        }
    }
}

int verificar(int **minas,int TAM_GRILLA,SDL_Window **ventana)
{
    int i,j;
    int valor=1;
    for(int i = 0; i < TAM_GRILLA; i++)
    {
        for(int j = 0; j < TAM_GRILLA; j++)
        {
            if( minas[i][j]!=HAY_MINA && minas[i][j]!=(HAY_MINA*100) && (minas[i][j]<=8 || (minas[i][j]>=100 && minas[i][j]<=800)) )
            {
                return 0;
            }
        }
    }
    printf("\nGANASTE\n");
    return OK;
}

void trofeo(SDL_Window *ventana, SDL_Renderer *renderer, int TAM_GRILLA, tGuardado* guardar)
{
    const int trof[8][8] =
    {
        {N, N, A, A, A, A, N, N},
        {N, A, X, X, X, X, A, N},
        {N, A, A, X, X, A, A, N},
        {N, N, A, A, A, A, N, N},
        {N, N, N, A, A, N, N, N},
        {N, N, M, A, A, M, N, N},
        {N, N, M, M, M, M, N, N},
        {N, N, N, N, N, N, N, N}
    };

    const int bimo[][8] =
    {
        {AZ, AZ, AZ, AZ, AZ, AZ, AZ, AZ},
        {AZ, C,  C,  C,  C,  C,  C,  AZ},
        {AZ, C,  N,  C,  N,  C,  C,  AZ},
        {AZ, C,  C,  C,  C,  C,  C,  AZ},
        {AZ, C,  C,  C,  C,  C,  C,  AZ},
        {AZ, AZ, AZ, AZ, AZ, AZ, AZ, AZ},
        {AZ, A,  AZ, AZ, AZ, V,  AZ, AZ},
        {AZ, AZ, AZ, AZ, R,  AZ, AZ, AZ}
    };

    const int rick[8][8] =
    {
        {N,  C,  N,  C,  C,  N,  C,  N},
        {C,  C,  A,  A,  A,  A,  C,  C},
        {C,  A,  M,  A,  A,  M,  A,  C},
        {C,  A,  A,  A,  A,  A,  A,  C},
        {N,  A,  A,  M,  M,  A,  A,  N},
        {N,  B,  A,  A,  A,  A,  B,  N},
        {N,  B,  B,  C,  C,  B,  B,  N},
        {N,  N,  B,  B,  B,  B,  N,  N}
    };



    int posX = 5;
    int posY = 2;
    SDL_Color color = {255, 255, 255, 255}; // Blanco

    SDL_RenderClear(renderer);

    dibujar(ventana, renderer, trof, posX, posY, 8);
    dibujar(ventana, renderer, bimo, posX-1, posY, 8);
    dibujar(ventana, renderer, rick, posX+1, posY, 8);

    if (TTF_Init() != 0)
    {
        SDL_Log("Error al inicializar SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        exit(1);
    }

    TTF_Font* font = TTF_OpenFont("arialbd.ttf", 10);
    if (!font)
    {
        SDL_Log("Error al cargar fuente: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }


    // Renderizar texto a superficie
    SDL_Surface* surface = TTF_RenderText_Solid(font, guardar->nick, color);
    if (!surface)
    {
        SDL_Log("Error al renderizar texto: %s", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int texW = 0;
    int texH = 0;

    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    SDL_Rect dstRect;

    dstRect.x=370, dstRect.y=400, dstRect.h=100, dstRect.w=250; //dimensiones

    SDL_RenderCopy(renderer, texture, NULL, &dstRect); //mostrar

    /////////////////

    SDL_Surface* texto2 = TTF_RenderText_Solid(font, "GANADOR", colores[2]);
    if (!texto2)
    {
        SDL_Log("Error al renderizar texto: %s", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(ventana);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, texto2);
    SDL_FreeSurface(texto2);


    SDL_QueryTexture(texture2, NULL, NULL, &texW, &texH);

    dstRect.x=275, dstRect.y=250, dstRect.h=175, dstRect.w=450; //dimensiones y posicion

    SDL_RenderCopy(renderer, texture2, NULL, &dstRect); //mostrar

    /////////////////

    SDL_RenderPresent(renderer);
}

int guardarJugadorTabla(tGuardado partida, int TAM_GRILLA)
{
    char linea[100];
    int contador = 0;

    FILE *arch = fopen("ultDiez.txt", "a+");
    if(!arch)
    {
        printf("Error al abrir archivo de ultimos diez");
        return -1;
    }

    int puntaje = (partida.tam_matriz*100)-(partida.tiempo/10000);

    fprintf(arch, "%s %d %d\n", partida.nick, TAM_GRILLA, puntaje);

    fclose(arch);
}

void ultimosDiez(SDL_Window *ventana, SDL_Renderer *renderer, int TAM_GRILLA)
{
    char buffer[100];
    tGuardado jugador;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("\nError al iniciar SDL");
        return;
    }

    if (TTF_Init() != 0)
    {
        printf("\nError al iniciar SDL_ttf");
        return;
    }

    TTF_Font* fuente = TTF_OpenFont("arialbd.ttf", 12);
    if (!fuente)
    {
        printf("\nNo se pudo cargar la fuente");
        return;
    }

    SDL_Rect dstRect = { TAM_VENTANA, 200, 250, 40 };

    FILE *fDiez = fopen("ultDiez.txt", "r");
    if (!fDiez)
    {
        printf("\nError al abrir archivo ultDiez.txt");
        return;
    }

    // --- Primera pasada: contar cuántas líneas hay ---
    int totalLineas = 0;
    while (fgets(buffer, sizeof(buffer), fDiez))
        totalLineas++;

    rewind(fDiez); // volver al inicio del archivo

    // --- Segunda pasada: leer y mostrar solo las últimas 10 ---
    int lineaActual = 0;
    while (fgets(buffer, sizeof(buffer), fDiez))
    {
        if (lineaActual >= totalLineas - 10)
        {
            sscanf(buffer, "%s %d %u", jugador.nick, &jugador.tam_matriz, &jugador.tiempo);

            unsigned int puntaje = (jugador.tam_matriz * 100) - (jugador.tiempo / 10000);

            sprintf(buffer, "%s %dx%d %d", jugador.nick, jugador.tam_matriz, jugador.tam_matriz, puntaje);
            SDL_Surface* surface = TTF_RenderText_Solid(fuente, buffer, colores[2]);

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_RenderCopy(renderer, texture, NULL, &dstRect);

            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);

            dstRect.y += 50;
        }
        lineaActual++;
    }

    fclose(fDiez);
}
