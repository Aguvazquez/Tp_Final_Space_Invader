#######################Defines################################

ALLEGRO_LIBS = -lallegro -lallegro_primitives -lallegro_color -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_image #Librerias de allegro.
DEF_RPI = -D RASPBERRY -lSDL2 -lpthread
RPI_OBJ = disdrv.o joydrv.o libAudioSDL2.o
OPTIONS = -Wall -g
RPI_HS = disdrv.h joydrv.h audio.h

##########################################################

################Allegro game##############################

all_game: OPTIONS += ${ALLEGRO_LIBS}

all_game: main.o  back_end.o setup.o menus.o logic.o GUI.o 
	gcc main.o back_end.o menus.o setup.o logic.o GUI.o -o Space_Invaders ${OPTIONS} 
	
##########################################################

#############Raspberry pi game############################

rpi_game:OPTIONS += ${DEF_RPI}
rpi_game:main.o  back_end.o setup.o menus.o logic.o GUI.o ${RPI_OBJ}
	gcc main.o back_end.o menus.o setup.o logic.o GUI.o ${RPI_OBJ} -o Space_Invaders ${OPTIONS} 

##########################################################
	
###############Main back###################################
main.o: main.c menus.h setup.h 
	gcc -c main.c ${OPTIONS}

menus.o: menus.c menus.h logic.h GUI.h back_end.h config.h ${RPI_HS}
	gcc -c menus.c ${OPTIONS} 
 
logic.o: logic.c logic.h  GUI.h back_end.h config.h ${RPI_HS}
	gcc -c logic.c ${OPTIONS} 

GUI.o: GUI.c GUI.h config.h ${RPI_HS}
	gcc -c GUI.c ${OPTIONS} 

back_end.o: back_end.c back_end.h config.h
	gcc -c back_end.c ${OPTIONS} 

setup.o: setup.c setup.h config.h ${RPI_HS}
	gcc -c setup.c ${OPTIONS} 
######################################################
#################Clear################################
clr_obj:
	rm *.o
#####################################################
######################End file#######################

