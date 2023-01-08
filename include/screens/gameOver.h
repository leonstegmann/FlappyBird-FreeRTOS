#ifndef __GAMEOVERSCREEN_H__
#define __GAMEOVERSCREEN_H__

/**
 * @brief Creates the FreeRTOS Task.
 * @return Returns 0 on Succes, 1 on Error.
 */
int createGameOverTask();

/* Enter and Exit functions for the State Machine */
void deleteGameOverTask();
void enterGameOverTask();
void exitGameOverTask();

#endif // __GAMEOVERSCREEN_H__