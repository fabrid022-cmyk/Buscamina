## 💻 Instalación y configuración

<details>
<summary>Linux</summary>

### Instalación de dependencias
```bash
sudo apt update
sudo apt install build-essential libsdl2-dev libsdl2-ttf-dev
Compilación
bash
Copiar código
gcc main.c funciones_buscaminas.c -o Buscamina `sdl2-config --cflags --libs` -lSDL2_ttf
Ejecutar
bash
Copiar código
./Buscamina
</details> <details> <summary>Windows (MinGW / Code::Blocks)</summary>
Configuración en Code::Blocks
Abrir Code::Blocks → Settings → Compiler → Global compiler settings

En Search directories → Compiler, agregar:

text
Copiar código
C:/SDL2/SDL2-2.0.22/include
C:/SDL2/SDL2_ttf-2.22.0/include
En Search directories → Linker, agregar:

text
Copiar código
C:/SDL2/SDL2-2.0.22/lib
C:/SDL2/SDL2_ttf-2.22.0/lib
En Project → Build options → Linker settings, agregar:

text
Copiar código
SDL2main
SDL2
SDL2_ttf
Copiar los archivos SDL2.dll y SDL2_ttf.dll al directorio del ejecutable.

Compilación desde terminal
bash
Copiar código
gcc main.c funciones_buscaminas.c -o Buscamina \
-IC:/SDL2/SDL2-2.0.22/include \
-IC:/SDL2/SDL2_ttf-2.22.0/include \
-LC:/SDL2/SDL2-2.0.22/lib \
-LC:/SDL2/SDL2_ttf-2.22.0/lib \
-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
Ejecutar
bash
Copiar código
./Buscamina.exe
</details> ```
