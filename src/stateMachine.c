/* Standard library includes */
#include "stdio.h" // for sprintf()

/* Project includes  */
#include "states.h"
#include "stateMachine.h"
#include "menuScreen.h"
#include "playScreen.h"
#include "gameOver.h"
#include "scoreScreen.h"
#include "multiplayerHandler.h"
#include "multiplayerIPConfigScreen.h"

int initStateMachine(){
    states_add( (void*) createMenuTask, enterMenuTask, NULL, exitMenuTask, 0, "Menu_Task");
    states_add( (void*) createPlayTask, enterPlayTask, NULL, exitPlayTask, 1, "Play_Task");
    states_add( (void*) createGameOverTask, enterGameOverTask, NULL, exitGameOverTask, 2, "GameOver_Task");
    states_add( (void*) createScoreScreenTask, enterScoreScreenTask, NULL, exitScoreScreenTask, 3, "Score_Task");
    states_add( (void*) createMultiplayerTask, enterMultiplayerTask, NULL, exitMultiplayerTask, 4, "Multiplayer_Task");
    states_add( (void*) createIPConfigTask, enterIPConfigTask, NULL, exitIPConfigTask, 5, "IPConfigTask");
    states_init(); //calls probe functions
    states_set_state(0); //sets state (default is first added state)
    states_run();   // checks for changes in states
    return 0;
}

void deleteStateMachine(){
    deleteMenuTask();
    deletePlayTask();
    deleteGameOverTask();
    deleteScoreScreenTask();
    deleteMultiplayerTask();
    deleteIPConfigTask();
}