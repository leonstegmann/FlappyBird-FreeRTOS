/* Standard library includes */
#include <stdio.h>
#include <string.h>


#include "TUM_Event.h"

/* Project includes  */
#include "states.h"
#include "stateMachine.h"
#include "menuScreen.h"
#include "playScreen.h"
#include "gameOver.h"
#include "scoreScreen.h"
#include "main.h"

StateMachine_t stateMachine = {0};

int initStateMachine(){
    states_add( (void*) createMenuTask, enterMenuTask, NULL, exitMenuTask, 0, "Menu_Task");
    states_add( (void*) createPlayTask, enterPlayTask, NULL, exitPlayTask, 1, "Play_Task");
    states_add( (void*) createGameOverTask, enterGameOverTask, NULL, exitGameOverTask, 2, "GameOver_Task");
    states_add( (void*) createScoreScreenTask, enterScoreScreenTask, NULL, exitScoreScreenTask, 3, "Score_Task");
    stateMachine.lock = xSemaphoreCreateMutex();
    states_init(); //calls probe functions
    states_set_state(0); //sets state (default is first added state)
    xSemaphoreTake(stateMachine.lock, portMAX_DELAY);
    stateMachine.last_change = xTaskGetTickCount();
    xSemaphoreGive(stateMachine.lock);
    states_run();   // checks for changes in states
    return 0;
}

void handleStateInput(char *input) {
    if (!strcmp(input, "Play") || !strcmp(input, "Retry")) {
        printf("%s button pressed\n", input);
        states_set_state(1); 
    }

    else if (!strcmp(input, "Score")) {
        printf("%s button pressed\n", input);
        states_set_state(3); 
    }

    else if (!strcmp(input, "Menu")) {
        printf("%s button pressed\n", input);
        states_set_state(0); 
    }

}

void checkStateInput(TickType_t lastFrameTime) {
    
    char *input;

    if (lastFrameTime - stateMachine.last_change >= STATE_DEBOUNCE_DELAY) {

        if(tumEventGetMouseLeft() && xQueueReceive(StateQueue, &input, 0)) {
            
            xSemaphoreTake(stateMachine.lock, portMAX_DELAY);
            stateMachine.last_change = xTaskGetTickCount();
            xSemaphoreGive(stateMachine.lock);
            handleStateInput(input);
            xQueueReset(StateQueue);   
        }
    }

}

void deleteStateMachine(){
    deleteMenuTask();
    deletePlayTask();
    deleteGameOverTask();
    deleteScoreScreenTask();
}