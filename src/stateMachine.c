/* Standard library includes */
#include "stdio.h"

/* Project includes  */
#include "states.h"
#include "stateMachine.h"
#include "menuScreen.h"
#include "playScreen.h"

int initStateMachine(){
    states_add( (void*) createMenuTask, enterMenuTask, NULL, exitMenuTask, 0, "Menu_Task");
    states_add( (void*) createPlayTask, enterPlayTask, NULL, exitPlayTask, 1, "Play_Task");
    states_init();
    states_set_state(0);
    return 0;
}

void deleteStateMachine(){
    states_clear_input();
    deleteMenuTask();
    deletePlayTask();
}