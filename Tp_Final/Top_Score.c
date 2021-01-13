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
 *        encarga de reordenar el archivo en cuestion y eliminar al de menor
 *        puntaje.
 */
static void reorder_top_score(void);

/*******************************************************************************/

/*************************** Global functions***********************************/

void put_on_top_score(uint32_t score, char *str){
    
    FILE* fp;
    uint32_t  j, aux=0;
    int8_t i;
    
    char str1[6]={'0','0','0','0','0','\0'};
    for(i=4, j=1; i>=0; i--,j*=10){
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
    
    if(!fopen(".Top_Score.txt", "r")){  //En caso que no lo pueda abrir, es que no
        int i, j;                        //se creo o se borro .Top_Score.txt
        FILE* fp;
        fp=fopen(".Top_Score.txt", "w");
        if(!fp){
            fprintf(stderr, "Hubo un error en la creación de Top_Score");
            return EXIT_FAILURE;
        }
        for(i=0; i<5; i++){
            for(j=0; j<5; j++){
                fputc('0', fp);
            }
            fputc('\n', fp);
            for(j=0; j<5; j++){
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
    bool its_before_enter=0;
    char c;
    int8_t all_names=6, i,j;
    user_t users[6], user_aux;
    user_t *puser=users;
    
    while(all_names>0){   
        for(i=0; i<12; i++){
            c = fgetc(fp);
            if(c!='\n'){
                if(!its_before_enter){
                    puser->score[i]=c;
                }
                else{
                   puser->name[i-6]=c;
                }
            }
            else{
		if(!its_before_enter){
                    puser->score[i]='\0';
		    its_before_enter=1;
            	}
		else{
                    puser->name[i-6]='\0';
                    its_before_enter=0;
		}
            }
        }   
        ++puser;
        --all_names;
    }
    for(i=0; i<6; i++){
        users[i].score_num=string_to_number(users[i].score);
    }
    for(i=0; i<6; i++){
        for(j=1+i; j<6; j++){
            if((users[i].score_num)<(users[j].score_num)){
                user_aux=users[i];
                users[i]=users[j];
                users[j]=user_aux;
            }
        }
    }
    fclose(fp);
    fp = fopen(".Top_Score.txt", "w");
    for(i=0; i<5; i++){               //Copio los 5 usarios.
        fputs(users[i].score, fp);    
        fputc('\n', fp);
        fputs(users[i].name, fp);
        fputc('\n',fp);
    }
    fclose(fp);
}

/****************************** END FILE ***************************************/