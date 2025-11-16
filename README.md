# Buscamina 
Este proyecto es una implementación del clásico juego Buscaminas, desarrollado en C utilizando la biblioteca SDL2 para gráficos y manejo de eventos. Permite al usuario configurar el tamaño del tablero y la cantidad de minas, iniciar partidas nuevas, continuar partidas guardadas y personalizar algunos ajustes del juego.


# 🎮 Buscamina en C con SDL2

[![MIT License](https://img.shields.io/badge/license-MIT-green)](LICENSE)  
[![C](https://img.shields.io/badge/language-C-blue)](https://github.com/fabrid022-cmyk/Buscamina)  
[![SDL2](https://img.shields.io/badge/SDL2-2.0.22-orange)](https://www.libsdl.org/)  

El clásico **Buscaminas** implementado en **C** usando **SDL2**. Compatible con Windows y Linux, incluye menú interactivo, temporizador y guardado de partidas.  

Repositorio: [https://github.com/fabrid022-cmyk/Buscamina](https://github.com/fabrid022-cmyk/Buscamina)

---

## 📖 Descripción
- Tablero configurable por tamaño y número de minas.  
- Menú interactivo con opciones de juego y ajustes.  
- Temporizador y contador de minas restantes.  
- Guardado y carga de partidas.  
- Gráficos y manejo de eventos mediante SDL2.  

---

## ⚡ Características principales
- ✅ Tablero configurable  
- ✅ Menú con ajustes y selección de dificultad  
- ✅ Temporizador y contador de minas  
- ✅ Guardado y carga de partidas  
- ✅ Compatible con Windows y Linux  

---

## 📂 Estructura del proyecto

- **src/** – Código fuente
  - `main.c` – Entrada principal del juego
  - `funciones_buscaminas.c` – Lógica del Buscaminas
  - `buscamina_header.h` – Header con definiciones y estructuras
  - `buscamina.config.txt` – Configuración inicial del tablero

- **resources/** – Recursos gráficos y fuentes
  - `logo.bmp` – Logo del juego
  - `arialbd.ttf` – Fuente utilizada en el juego

- **project/** – Archivos del proyecto de Code::Blocks
  - `buscamina_implementacion.cbp`
  - `buscamina_implementacion.layout`
  - `buscamina_implementacion.depend`

- **saves/** – Archivos de partidas (opcional)
  - `ultDiez.txt` – Registro de los últimos 10 juegos
  - `ultima_partida` – Última partida guardada

- `README.md` – Documentación del proyecto
- `.gitignore` – Archivos y carpetas ignoradas por Git


---

## 🛠 Requisitos
- Compilador **C** (GCC / MinGW)  
- **Code::Blocks** (opcional)  
- **SDL2 2.0.22**  
- **SDL2_ttf 2.22**  

### 🔗 Descarga de SDL2
- SDL2: [https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php) → `SDL2-devel-2.0.22-mingw.tar.gz`  
- SDL2_ttf: [https://www.libsdl.org/projects/SDL_ttf/](https://www.libsdl.org/projects/SDL_ttf/) → `SDL2_ttf-devel-2.22.0-mingw.tar.gz`  

Extraer los archivos en una carpeta accesible, por ejemplo `C:/SDL2`.

---

### 💻 Instalación y configuración

<details>
<summary>🖥️ Windows (MinGW / Code::Blocks)</summary>

#### Configuración en Code::Blocks
1.  Abrir **Code::Blocks → Settings → Compiler → Global compiler settings**.
2.  En **Search directories → Compiler**, agregar:
    ```text
    C:/SDL2/SDL2-2.0.22/include
    C:/SDL2/SDL2_ttf-2.22.0/include
    ```
3.  En **Search directories → Linker**, agregar:
    ```text
    C:/SDL2/SDL2-2.0.22/lib
    C:/SDL2/SDL2_ttf-2.22.0/lib
    ```
4.  En **Project → Build options → Linker settings**, agregar:
    ```text
    SDL2main
    SDL2
    SDL2_ttf
    ```
5.  Copiar los archivos **SDL2.dll** y **SDL2_ttf.dll** al directorio del ejecutable.

#### Compilación desde terminal

gcc main.c funciones_buscaminas.c -o Buscamina \
-IC:/SDL2/SDL2-2.0.22/include \
-IC:/SDL2/SDL2_ttf-2.22.0/include \
-LC:/SDL2/SDL2-2.0.22/lib \
-LC:/SDL2/SDL2_ttf-2.22.0/lib \
-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
Ejecutar

./Buscamina.exe
</details>

<details> <summary>🐧 Linux</summary>

Instalación de dependencias

sudo apt update
sudo apt install build-essential libsdl2-dev libsdl2-ttf-dev
Compilación

gcc main.c funciones_buscaminas.c -o Buscamina `sdl2-config --cflags --libs` -lSDL2_ttf
Ejecutar

./Buscamina
</details>

📂 Archivos importantes
main.c → Entrada principal del programa.

funciones_buscaminas.c / buscaminas_header.h → Contiene la lógica del Buscaminas y las estructuras de datos.

buscamina.config.txt → Archivo para la configuración inicial del tablero.

🎮 Uso
Ejecutar el juego.

Seleccionar la dificultad o personalizar el tablero desde el menú.

Clic izquierdo: descubrir casilla.

Clic derecho: marcar mina.

El juego termina al descubrir todas las casillas seguras o al explotar una mina.

📝 Licencia
MIT License © 2025 Fabrizio Del Vecchio

Se permite usar, modificar y redistribuir el código, siempre mencionando al autor original.
