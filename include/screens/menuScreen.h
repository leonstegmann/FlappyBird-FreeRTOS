#ifndef __MENUSCREEN_H__
#define __MENUSCREEN_H__

/**
 * @brief creates the FreeRTOS Task. Returns 0 on Succes, 1 on Error
 */
int createMenuTask();

/* Enter and Exit functions for the State Machine */
void deleteMenuTask();
void enterMenuTask();
void exitMenuTask();

#endif // __MENUSCREEN_H__