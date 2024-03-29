/**
 * @file Draw.h
 * @author Valentin Heubach
 * @brief provides the all functions necessary to print objects on the screen 
 */
#ifndef DRAW_H
#define DRAW_H

#include "TUM_Draw.h"
#include "objects.h"

#define SCORE_FONT /* "SF Automaton.ttf" */"flappy-bird-font.ttf"
#define HIGHSCORE_FONT "SF Automaton.ttf"

/* FPS Font */
#define FPS_FONT "scifieb.ttf"

/* Defines for drawButton */
#define BUTTON_FONT "Minecraft.ttf"
#define BUTTON_FONT_SIZE 22
#define BOX_WIDTH 130
#define BOX_HEIGHT 40

#define LEFT_BUTTON_POSITION  (coord_t) {SCREEN_WIDTH/2 - BOX_WIDTH*1.75, SCREEN_HEIGHT/2 + 100}
#define RIGHT_BUTTON_POSITION (coord_t) {SCREEN_WIDTH/2 + BOX_WIDTH*0.75, SCREEN_HEIGHT/2 + 100}
#define MIDDLE_BUTTON_POSITION (coord_t) {SCREEN_WIDTH/2 - BOX_WIDTH/2, SCREEN_HEIGHT/2 + 100}
#define LOWER_LEFT_BUTTON_POSITION  (coord_t) {SCREEN_WIDTH/2 - BOX_WIDTH*1.75, SCREEN_HEIGHT/2 + 150}
#define LOWER_RIGHT_BUTTON_POSITION (coord_t) {SCREEN_WIDTH/2 + BOX_WIDTH*0.75, SCREEN_HEIGHT/2 + 150}
#define LOWER_MIDDLE_BUTTON_POSITION (coord_t) {SCREEN_WIDTH/2 - BOX_WIDTH/2, SCREEN_HEIGHT/2 + 150}

#define BIRD_MENU_POS (coord_t) {SCREEN_WIDTH/2 - 34/2, SCREEN_HEIGHT - 300}

#define LOGO_POSITION (coord_t) {SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 150}
#define HIGHSCORE_POSITION (coord_t) {SCREEN_WIDTH/2, SCREEN_HEIGHT/2}

#define ARROW_OFFSET 50
#define ARROW_HEIGHT 10
#define ARROW_WIDTH 10

/**
 * @brief Draws the uppeer and lower pipe allocated around the Gap center.
 * @param pipe Pipe Object 
 * @return 1 on success and 0 when failed
 */
void drawPipe(pipes_t* pipe);

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
int drawButton(coord_t pos, char *name, TickType_t lastFrameTime);

/**
 * @brief Prints the Logo on the screen 
 * @param pos x and y coordinates of the logo position
 * @return 1 on success and 0 when failed
 */
int drawLogo(coord_t pos);

/**
 * @brief Prints GAME OVER on the Screen 
 */
void drawGameOver();

/**
 * @brief Prints the Highscore on the Screen
 * @param pos Position on the screen  
 */
int drawHighscore(coord_t pos);

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
 * @brief Draws the the flapping bird while in Menu
 * @param xLastFrameTime time of the last drawn frame   
 */
void drawBirdAnnimations(TickType_t xLastFrameTime, int colour);

/**
 * @brief Draws the the flapping bird while in Game
 * @param xLastFrameTime time of the last drawn frame   
 */
void drawBirdAnnimationsInGame(TickType_t xLastFrameTime, bird_t* player, int colour);
 
/**
 * @brief Draws the the score of the player
 * @param count score of the player   
 */
void drawScore(unsigned short count);

/**
 * @brief Draws an arrow
 * @param orientation UP, DOWN, RIGHT or LEFT
 * @param point coordinat of the position   
 */
void drawArrow(char orientation, coord_t point);

/**
 * @brief Draws Pause Text
 */
void drawPause();

#endif //__DRAW_H__
