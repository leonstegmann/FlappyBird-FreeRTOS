/**
 * @file Draw.h
 * @author Valentin Heubach
 * @brief provides the all functions necessary to print objects on the screen 
 */
#ifndef DRAW_H
#define DRAW_H

#include "TUM_Draw.h"

/**
 * @brief Prints the backround image on the screen 
 * @return 1 on success and 0 when failed
 */
int drawBackround();


/**
 * @brief Prints the backround image on the screen 
 * @param pos x and y coordinates of the button position
 * @param name String of the displayed name
 * @return 1 on success and 0 when failed
 */
int drawButton(coord_t pos, char *name);

#endif //__DRAW_H__