#ifndef __PLAYSCREEN_H__
#define __PLAYSCREEN_H__

/**
 * @brief Creates the FreeRTOS Task.
 * @return Returns 0 on Succes, 1 on Error.
 */
int createPlayTask();

/**
 * @brief Deletes the FreeRTOS Task.
 */
void deletePlayTask();

/* Enter and Exit functions for the State Machine */
void enterPlayTask();
void exitPlayTask();

#endif // __PLAYSCREEN_H__