/***************************** Standard libraries *****************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************/

/******************************* Locals headers ********************************/

#include "config.h"
#include "back_end.h"

/*******************************************************************************/

/************************* Header of local functions ***************************/

/*
 * @Brief Al introducir al 6to usuario en ".Top_Score.txt", esta funcion se 
 *        encarga de reordenar el archivo y eliminar al de menor puntaje.
 * @Note Solo hay que llamar a esta funcion cuando el jugador ponga su nombre.
 */

static void reorder_top_score(void);

/*
 * @Brief convierte un string en un entero. 
 * @Param1: string a convertir.
 * @Return  el valor correspondiente del string recibido.
 */

static uint32_t string_to_int(char str[STR_LONG]);

/*******************************************************************************/

/*************************** Global functions **********************************/

uint8_t create_Top_Score(void) {

    if (!fopen(".Top_Score.txt", "r")) { //en caso de no encontrar el archivo del top score
        int i, j;
        FILE* fp;
        fp = fopen(".Top_Score.txt", "w"); //crea el top score vacío, con el formato apropiado
        if (!fp) {
            fprintf(stderr, "Hubo un error en la creación de Top_Score.\n");
            return EXIT_FAILURE;
        }
        for (i = 0; i < TOP_PLAYERS; i++) {
            for (j = 0; j < STR_LONG - 1; j++) { //SCORE
                fputc('0', fp);
            }
            fputc('\n', fp);
            for (j = 0; j < STR_LONG - 1; j++) { //NAME
                fputc(' ', fp);
            }
            fputc('\n', fp);
        }
        fclose(fp);
    }
    return EXIT_SUCCESS;
}

int8_t get_top_score(uint32_t score) { //devuelve la posicion del jugador si esta en el top 5, sino un 0

    static FILE* fp;
    char str[STR_LONG];
    uint8_t i;
    fp = fopen(".Top_Score.txt", "r");

    if (!fp) {
        fprintf(stderr, "Hubo un error al leer el top score.\n");
        return FATAL_ERROR;
    }

    for (i = 1; i <= TOP_PLAYERS; i++) { //en caso que se encuentre el archivo
        //SCORE
        fgets(str, STR_LONG, fp);
        if (string_to_int(str) < score) { //si el puntaje del top es menor al obtenido en la partida
            fclose(fp);
            return i; //entonces devuelve la posición del nuevo jugador en el top
        }
        fgetc(fp); //"aumento" el fp a la siguiente linea 
        //NAME
        fgets(str, STR_LONG, fp); //salteo el nombre, no interesa en esta función
        fgetc(fp); //"aumento" el fp a la siguiente linea 
    }

    fclose(fp);
    return EXIT_SUCCESS; //si sale del ciclo significa que no entró al top
}

void put_on_top_score(uint32_t score, char *name_str) {

    FILE* fp;
    uint32_t j, aux = 0;
    int8_t i;

    char score_str[STR_LONG] = {'0', '0', '0', '0', '0', '\0'};
    for (i = STR_LONG - 2, j = 1; i >= 0; i--, j *= 10) { //ya que el último dígito está en penúltima posición
        aux = score / j; //algoritmo int -> char
        score_str[i] = (char) ((aux % 10) + ASCII);
    }

    fp = fopen(".Top_Score.txt", "a"); //escribo al final del archivo.
    if(!fp) {
        fprintf(stderr, "Hubo un error al sobreescribir el top score.\n");
    }
    else {
        fputs(score_str, fp); //SCORE
        fputc('\n', fp);
        fputs(name_str, fp); //NAME
        fputc('\n', fp);
        fclose(fp);
        reorder_top_score();
    }
}

uint8_t create_difficulty(void){
    
    FILE* fp;     
    if(!fopen(".Difficulty.txt", "r")){         
        fp=fopen(".Difficulty.txt", "w");         
        if (!fp) {
            fprintf(stderr, "Hubo un error en la creación del archivo de dificultad.\n");
            return EXIT_FAILURE;
        }
        switch_difficulty(2);   //Setea la dificultad en normal, en caso de no encontrar el archivo
        fclose(fp);
    }
    return EXIT_SUCCESS;
}

int8_t read_difficulty(void){
    
    FILE* fp = fopen(".Difficulty.txt", "r");
    if(!fp){
        fprintf(stderr, "Hubo un error al leer la dificultad.\n");
        return FATAL_ERROR;
    }
    int8_t difficulty;
    difficulty=(fgetc(fp)-ASCII)*10;    //Convierto en la decena
    difficulty+=(fgetc(fp)-ASCII);      //Le sumo la unidad
    fclose(fp);
    return difficulty;
}

void switch_difficulty(uint8_t option){

    FILE* fp=fopen(".Difficulty.txt", "w");  //Abro el archivo donde guardo el nivel de dificultad.
    
    if (!fp) {
        fprintf(stderr, "Hubo un error al modificar la dificultad.\n");
    }
    else {
        switch(option) {
            case 1:
            {
                fputs(EASY_CODE, fp);
                break;
            }
            case 2:
            {
                fputs(NORMAL_CODE, fp);
                break;
            }
            case 3:
            {
                fputs(HARD_CODE, fp);
                break;
            }
            default:
            {
                fprintf(stderr, "La dificultad elegida está mal configurada.\n");
                break;
            }
        }
        fclose(fp);
    }
}

/*******************************************************************************/

/******************************* Local functions *******************************/

static void reorder_top_score(void) {

    typedef struct {
        char score[STR_LONG];
        char name[STR_LONG];
        int32_t score_num;
    } user_t;

    FILE* fp = fopen(".Top_Score.txt", "r");
    bool its_before_enter = false;
    char c;
    int8_t all_names = TOP_PLAYERS + 1;
    uint8_t i, j;
    user_t users[TOP_PLAYERS + 1], user_aux;
    user_t *puser = users;

    while (all_names > 0) {
        for (i = 0; i < 2*STR_LONG; i++) { 
            c = fgetc(fp);
            if (c != '\n') {
                if (!its_before_enter) {
                    puser->score[i] = c;
                } else {
                    puser->name[i - STR_LONG] = c; 
                }
            } else {
                if (!its_before_enter) {
                    puser->score[i] = '\0';
                    its_before_enter = true;
                } else {
                    puser->name[i - STR_LONG] = '\0';
                    its_before_enter = false;
                }
            }
        }
        ++puser;
        --all_names;
    }
    for (i = 0; i < TOP_PLAYERS + 1; i++) {     //ya que tendremos un jugador de más al ejecutarse la función
        users[i].score_num = string_to_int(users[i].score);
    }
    for (i = 0; i < TOP_PLAYERS + 1; i++) {
        for (j = 1 + i; j < TOP_PLAYERS + 1; j++) {
            if ((users[i].score_num) < (users[j].score_num)) { //compara al nuevo jugador con los demás
                user_aux = users[i];                           //para llevarlo a su posición correspondiente
                users[i] = users[j];
                users[j] = user_aux;
            }
        }
    }
    fclose(fp);
    fp = fopen(".Top_Score.txt", "w");
    for (i = 0; i < TOP_PLAYERS; i++) { //copio solo los usarios que entran en el top
        fputs(users[i].score, fp);
        fputc('\n', fp);
        fputs(users[i].name, fp);
        fputc('\n', fp);
    }
    fclose(fp);
}

static uint32_t string_to_int(char str[STR_LONG]) {

    int8_t i;
    uint32_t aux = 0, j;

    for (i = STR_LONG - 2, j = 1; i >= 0; i--, j *= 10) { //por estar el uĺtimo dígito en el penúltimo lugar.
        aux += (((int32_t) str[i] - ASCII) * j);
    }

    return aux;
}

/********************************** END FILE ***********************************/