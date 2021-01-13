/************************* Standard libraries **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************/

/************************* Locals headers **************************************/

#include "config.h"
#include "Top_Score.h"
#include "play.h"
#include "allegro_setup.h"

/*******************************************************************************/

/**************************** Header of local functions ************************/

/*
 * @brief Al introducir al 6to usuario en ".Top_Score.txt", esta funcion se 
 *        encarga de reordenar el archivo y eliminar al de menor puntaje.
 */
static void reorder_top_score(void);

/*******************************************************************************/

/*************************** Global functions***********************************/

int8_t get_top_score(uint32_t score){  //devuelve la posicion del jugador si esta en el top 5, sino un 0
    
    static FILE* fp;
    char str[STR_LONG];
    uint8_t i;
    fp = fopen(".Top_Score.txt", "r");
    
    if(!fp){
        fprintf(stderr, "Hubo un error al leer .Top_Score.txt");
        return FATAL_ERROR;
    }
    for(i=1; i<=TOP_PLAYERS; i++){
        //SCORE
        fgets(str, STR_LONG, fp);
        if(string_to_number(str) < score)
            fclose(fp);
            return i;
        fgetc(fp);  // "aumento" el fp a la siguiente linea 
        //NAME
        fgets(str, STR_LONG, fp);
        fgetc(fp);
    }
    
    fclose(fp);
    return EXIT_SUCCESS;
}

void put_on_top_score(uint32_t score, char *str){
    
    FILE* fp;
    uint32_t  j, aux=0;
    int8_t i;
    
    char str1[STR_LONG]={'0','0','0','0','0','\0'};
    for(i = STR_LONG-2, j=1; i>=0; i--, j*=10){ //ya que el último dígito está en penúltima posición
        aux = score/j;
        str1[i]=(char)((aux%10)+ASCII);        
    }

    fp = fopen(".Top_Score.txt", "a");   //Escribo al final del archivo.
    fputs(str1,fp); //SCORE
    fputc('\n',fp);
    fputs(str,fp);  //NAME
    fputc('\n',fp);
    fclose(fp);
    
    reorder_top_score();
}

int8_t create_Top_Score(){
    
    if(!fopen(".Top_Score.txt", "r")){  //En caso de no encontrar .Top_Score.txt
        int i, j;
        FILE* fp;
        fp = fopen(".Top_Score.txt", "w");
        if(!fp){
            fprintf(stderr, "Hubo un error en la creación de Top_Score");
            return EXIT_FAILURE;
        }
        //crear reset_top_score()
        for(i=0; i<TOP_PLAYERS; i++){
            for(j=0; j < STR_LONG-1; j++){
                fputc('0', fp);
            }
            fputc('\n', fp);
            for(j=0; j < STR_LONG-1; j++){
                fputc(' ', fp);
            }
            fputc('\n', fp);
        }
        fclose(fp);
    }
    
    return EXIT_SUCCESS;
}

/*******************************************************************************/

/******************************* Local functions *******************************/

static void reorder_top_score(void){
    //Solo hay que llamar a esta funcion cuando el jugador ponga su nombre.
    typedef struct{
	char score[STR_LONG];
        char name[STR_LONG];
        int32_t score_num;
    } user_t;
    
    FILE* fp = fopen(".Top_Score.txt", "r");
    bool its_before_enter=false;
    char c;
    int8_t all_names = TOP_PLAYERS+1;
    uint8_t i,j;
    user_t users[TOP_PLAYERS+1], user_aux;
    user_t *puser=users;
    
    while(all_names>0){   
        for(i=0; i<12; i++){    //no se que significa este 12
            c = fgetc(fp);
            if(c!='\n'){
                if(!its_before_enter){
                    puser->score[i]=c;
                }
                else{
                   puser->name[i-6]=c;  //ni este 6
                }
            }
            else{
		if(!its_before_enter){
                    puser->score[i]='\0';
		    its_before_enter=true;
            	}
		else{
                    puser->name[i-6]='\0';  //ni este 6
                    its_before_enter=false;
		}
            }
        }   
        ++puser;
        --all_names;
    }
    for(i=0; i < TOP_PLAYERS+1; i++){
        users[i].score_num = string_to_number(users[i].score);
    }
    for(i=0; i < TOP_PLAYERS+1; i++){
        for(j=1+i; j < TOP_PLAYERS+1; j++){
            if((users[i].score_num) < (users[j].score_num)){
                user_aux=users[i];
                users[i]=users[j];
                users[j]=user_aux;
            }
        }
    }
    fclose(fp);
    fp = fopen(".Top_Score.txt", "w");
    for(i=0; i<TOP_PLAYERS; i++){               //Copio los usarios que entran.
        fputs(users[i].score, fp);    
        fputc('\n', fp);
        fputs(users[i].name, fp);
        fputc('\n',fp);
    }
    fclose(fp);
}

/****************************** END FILE ***************************************/