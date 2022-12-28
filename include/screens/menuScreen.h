#ifndef __MENUSCREEN_H__
#define __MENUSCREEN_H__

/**
 * @brief Creates the FreeRTOS Task.
 * @return Returns 0 on Succes, 1 on Error.
 */
int createMenuTask();

/* Enter and Exit functions for the State Machine */
void deleteMenuTask();
void enterMenuTask();
void exitMenuTask();

#endif // __MENUSCREEN_H__