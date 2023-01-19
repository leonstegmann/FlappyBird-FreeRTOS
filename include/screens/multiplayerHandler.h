#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

/**
 * @brief Creates the FreeRTOS Task.
 * @return Returns 0 on Succes, 1 on Error.
 */
int createMultiplayerTask();

/* Enter and Exit functions for the State Machine */
void deleteMultiplayerTask();
void enterMultiplayerTask();
void exitMultiplayerTask();

#endif // MULTIPLAYER_H