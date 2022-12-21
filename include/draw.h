/**
 * @file Draw.h
 * @author Valentin Heubach
 * @brief provides the all functions necessary to print objects on the screen 
 */
#ifndef DRAW_H
#define DRAW_H

/**
 * @brief Prints the backround image on the screen 
 * @return 1 on success and 0 when failed
 */
int drawBackround();

int drawButton(coord_t pos, char *name);

#endif //__DRAW_H__