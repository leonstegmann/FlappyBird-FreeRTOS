/**
 * @file Draw.h
 * @author Valentin Heubach
 * @brief provides the all functions necessary to print objects on the screen 
 */
#ifndef DRAW_H
#define DRAW_H

#include "TUM_Draw.h"

/* FPS Font */
#define FPS_FONT "scifieb.ttf"

/* Defines for drawButton */
#define BUTTON_FONT "Minecraft.ttf"
#define BUTTON_FONT_SIZE 22
#define BOX_WIDTH 130
#define BOX_HEIGHT 40

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

/**
 * @brief Prints the Logo on the screen 
 * @param pos x and y coordinates of the logo position
 * @return 1 on success and 0 when failed
 */
int drawLogo(coord_t pos);

/**
 * @brief Prints the numper of FPS on the screen  
 */
void drawFPS(void);

/**
 * @brief Initializes the animations  
 */
void drawInitAnnimations(void);

/**
 * @brief Draws the the moving floor
 * @param xLastFrameTime time of the last drawn frame   
 */
void drawFloorAnnimations(TickType_t xLastFrameTime);

/**
 * @brief Draws the the flapping bird
 * @param xLastFrameTime time of the last drawn frame   
 */
void drawBirdAnnimations(TickType_t xLastFrameTime);

#endif //__DRAW_H__