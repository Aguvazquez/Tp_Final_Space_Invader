#ifndef CONFIG_H
#define CONFIG_H

#define ASCII '0'
#define STR_LONG 6

#ifdef RASPBERRY

#define SCREEN_W 16
#define SCREEN_H  16
#define MOVE_RATE  1
#define N 24
#define BASE_SIDE 1

#else

#define SCREEN_W  800
#define SCREEN_H  640
#define MOVE_RATE  4.0
#define N 50

#endif

#define TIMERTICKRBP 1
#define SAMPLES  5
#define FONTS  2
#define BACKGROUNDS 17
#define FPS    60.0
#define BASE_SIZE 20
#define ALIEN 1
#define PRIMERA_FILA 2
#define ULTIMA_FILA 7
#define PRIMERA_COLUMNA 2
#define ULTIMA_COLUMNA 12
#define ESPACIO_ALIENS 1
#define BLOQUE 25
#define FILA_BLOQUES N-3
#define PRIMER_BLOQUE 2
#define ULTIMO_BLOQUE 12
#define ESPACIO_BLOQUES 3

#define CLOSE_DISPLAY  -42
#define RESET_GAME  -22
#define EXIT_MENU -64
#define FATAL_ERROR -71

#define EASY_CODE "27"
#define NORMAL_CODE "22"
#define HARD_CODE "17"

#define EASY 27
#define NORMAL 22
#define HARD 17


#endif//CONFIG_H




