#ifndef MENU_DISPLAY_ALLEGRO_H
#define MENU_DISPLAY_ALLEGRO_H

/*
 * @Brief Create the main menu. 
 * @Param param1: pointer to the display.
 * @Param param2: pointer to samples array.
 * @Param param3: pointer to the event queue.
 * @Param param4: pointer to fonts array.
 * @param param5: pointer to backgrounds array.
 * @Return  CLOSE_DISPLAY close the game.
 *          1 first button was pressed.
 *          2 second button was pressed.
 *          3 third button was pressed. 
 *          FATAL_ERROR if something went wrong.
 * @Comment This fuction doesn't stop the music.
 */

int8_t menu_display(char *str0, char *str1, char *str2, char flag, uint8_t pause);
/*
 * @Brief Create the Difficulty menu. 
 * @Param param1: pointer to the display.
 * @Param param2: pointer to samples array.
 * @Param param3: pointer to the event queue.
 * @Param param4: pointer to fonts array.
 * @param param5: pointer to backgrounds array.
 * @Return  0 close the game.
 *          1 first button was pressed.
 *          2 second button was pressed.
 *          3 third button was pressed. 
 *          FATAL_ERROR if something  gone wrong.
 * @Coment This fuctions don't stop the music
 */ 
int8_t Difficulty(char *str0, char *str1, char *str2);

#endif /* MENU_DISPLAY_ALLEGRO_H */
