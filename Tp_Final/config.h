#ifndef CONFIG_H
#define CONFIG_H

#define ASCII       '0'
#define STR_LONG    6
#define TOP_PLAYERS 5

#ifdef RASPBERRY

typedef int elements_t;     //para coordenadas

#define SCREEN_W    16
#define SCREEN_H    16
#define MOVE_RATE   1
#define BASE_SIZE   1

#define FILAS_ALIENS            3   
#define COLUMNAS_ALIENS         6   
#define CANT_ALIENS             FILAS_ALIENS*COLUMNAS_ALIENS
#define PRIMERA_FILA_ALIENS     2   
#define PRIMERA_COLUMNA_ALIENS  3   
#define DISTANCIA_ALIENS_X      2   
#define DISTANCIA_ALIENS_Y      2   

#define CANT_BLOQUES        4
#define PRIMER_BLOQUE       1   
#define DISTANCIA_BLOQUES   4   
#define BLOQUES_Y           12  

#define NAVE_Y          SCREEN_H - BASE_SIZE
#define NAVE_X          SCREEN_W/2 - BASE_SIZE
#define MYSTERY_SHIP_Y  BASE_SIZE

#else

typedef float elements_t;   //para coordenadas

#define SCREEN_W    800
#define SCREEN_H    640
#define MOVE_RATE   4.0
#define BASE_SIZE   20

#define FILAS_ALIENS            5 
#define COLUMNAS_ALIENS         10
#define CANT_ALIENS             FILAS_ALIENS*COLUMNAS_ALIENS
#define PRIMERA_FILA_ALIENS     SCREEN_H/10  
#define PRIMERA_COLUMNA_ALIENS  3*SCREEN_W/15  
#define DISTANCIA_ALIENS_X      SCREEN_W/15     
#define DISTANCIA_ALIENS_Y      SCREEN_H/10     

#define CANT_BLOQUES        4
#define PRIMER_BLOQUE       SCREEN_W/6.5 - 2*BASE_SIZE     
#define DISTANCIA_BLOQUES   3*SCREEN_W/13                
#define BLOQUES_Y           3*SCREEN_H/4 + 2.5*BASE_SIZE   

#define NAVE_Y          SCREEN_H - 2*BASE_SIZE
#define NAVE_X          SCREEN_W/2 - 1.5*BASE_SIZE
#define BULLET_W        3
#define MYSTERY_SHIP_Y  1.5*BASE_SIZE

#endif

#define TIMERTICKRBP    1
#define SAMPLES         5
#define FONTS           2
#define BACKGROUNDS     22
#define FPS             60.0

#define ALIEN 1

#define BLOQUE  25
#define LIVES   3

#define CLOSE_DISPLAY   -1
#define RESET_GAME      -2
#define EXIT_MENU       -3
#define FATAL_ERROR     -4

#define EASY_CODE   "30"
#define NORMAL_CODE "25"
#define HARD_CODE   "20"

#define EASY            30
#define NORMAL          25
#define HARD            20
#define MAX_DIFFICULTY  15

#endif//CONFIG_H