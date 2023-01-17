#ifndef __PLAYSCREEN_H__
#define __PLAYSCREEN_H__

#include "FreeRTOS.h" // must appear in source files before include task.h"
#include "task.h" // for Taskhandle

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

extern TaskHandle_t PlayScreen; // to be able to notify Task

#endif // __PLAYSCREEN_H__