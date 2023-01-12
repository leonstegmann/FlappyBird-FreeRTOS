#ifndef __SCORESCREEN_H__
#define __SCORESCREEN_H__

/**
 * @brief Creates the FreeRTOS Task.
 * @return Returns 0 on Succes, 1 on Error.
 */
int createScoreScreenTask();

/* Enter and Exit functions for the State Machine */
void deleteScoreScreenTask();
void enterScoreScreenTask();
void exitScoreScreenTask();

#endif // __SCORESCREEN_H__