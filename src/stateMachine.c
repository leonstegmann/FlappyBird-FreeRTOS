/* Standard library includes */
#include "stdio.h"

/* Project includes  */
#include "states.h"
#include "stateMachine.h"
#include "menuScreen.h"
#include "playScreen.h"
#include "gameOver.h"

int initStateMachine(){
    states_add( (void*) createMenuTask, enterMenuTask, NULL, exitMenuTask, 0, "Menu_Task");
    states_add( (void*) createPlayTask, enterPlayTask, NULL, exitPlayTask, 1, "Play_Task");
    states_add( (void*) createGameOverTask, enterGameOverTask, NULL, exitGameOverTask, 2, "GameOver_Task");
    states_init(); //calls probe functions
    states_set_state(0); //sets state (default is first added state)
    states_run();   // checks for changes in states
    return 0;
}

void deleteStateMachine(){
    deleteMenuTask();
    deletePlayTask();
    deleteGameOverTask();
}