#ifndef IPConfig_H
#define IPConfig_H

#define NONE 'N'
#define LEFT 'L'
#define RIGHT 'R'
#define UP 'U'
#define DOWN 'D'

/**
 * @brief Creates the FreeRTOS Task.
 * @return Returns 0 on Succes, 1 on Error.
 */
int createIPConfigTask();

/* Enter and Exit functions for the State Machine */
void deleteIPConfigTask();
void enterIPConfigTask();
void exitIPConfigTask();


#endif // IPConfig_H