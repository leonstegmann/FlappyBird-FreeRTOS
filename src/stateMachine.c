/* Standard library includes */
#include "stdio.h"

/* Project includes  */
#include "stateMachine.h"
#include "menuScreen.h"

int initStateMachine(){
    states_add( (void*) createMenuTask, enterMenuTask, NULL, exitMenuTask, 0, "Menu_Screen");
    states_init();
    states_set_state(0);
    return 0;
}