#ifndef __PLAYSCREEN_H__
#define __PLAYSCREEN_H__

#include "FreeRTOS.h" // must appear in source files before other includes
#include "objects.h" // for bird_t

/**
 * @brief Creates the FreeRTOS Task.
 * @return Returns 0 on Succes, 1 on Error.
 */
int createPlayTask();

/* Enter and Exit functions for the State Machine */
void deletePlayTask();
void enterPlayTask();
void exitPlayTask();

void pauseGame();

/**
 * @brief Enables the GodMode for this player.
 */
void godMode(bird_t* player);

/**
 * @brief Enables the Cheat Mode for this player. Here he can set the score he want using the arrow keys.
 */
void setScoreCheat(bird_t* player);

#endif // __PLAYSCREEN_H__