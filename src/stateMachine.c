/* Standard library includes */
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "TUM_Event.h"

/* Project includes  */
#include "states.h"
#include "stateMachine.h"
#include "menuScreen.h"
#include "playScreen.h"
#include "gameOver.h"
#include "scoreScreen.h"
#include "multiplayerHandler.h"
#include "multiplayerIPConfigScreen.h"
#include "defines.h"

StateMachine_t stateMachine = {0};

TaskHandle_t StateMachineTask = NULL;

void vStateMachineTask() {
    /* if local variable are needed first increase currnet Stack Size!!!*/
    while(1) {
        states_run();
        xSemaphoreGive(stateMachine.lock);
        vTaskDelay((TickType_t) 50);
    }
}

int initStateMachine(){

    /* Create State Machine Task*/
    xTaskCreate(vStateMachineTask, "StateMachine", mainGENERIC_STACK_SIZE/10, NULL,
                    configMAX_PRIORITIES-5, &StateMachineTask);
 
    /*  Adding States to the State Machine*/
    states_add( (void*) createMenuTask, enterMenuTask, NULL, exitMenuTask, 0, "Menu_Task");
    states_add( (void*) createPlayTask, enterPlayTask, NULL, exitPlayTask, 1, "Play_Task");
    states_add( (void*) createGameOverTask, enterGameOverTask, NULL, exitGameOverTask, 2, "GameOver_Task");
    states_add( (void*) createScoreScreenTask, enterScoreScreenTask, NULL, exitScoreScreenTask, 3, "Score_Task");
    states_add( (void*) createMultiplayerTask, enterMultiplayerTask, NULL, exitMultiplayerTask, 4, "Multiplayer_Task");
    states_add( (void*) createIPConfigTask, enterIPConfigTask, NULL, exitIPConfigTask, 5, "IPConfigTask");
    
    stateMachine.lock = xSemaphoreCreateMutex();
    
    states_init(); //calls probe functions
    states_set_state(0); //sets state (default is first added state)
    
    xSemaphoreTake(stateMachine.lock, portMAX_DELAY);
    stateMachine.last_change = xTaskGetTickCount();
    stateMachine.str = NULL;
    xSemaphoreGive(stateMachine.lock);
    
    states_run();   // checks for changes in states
    return 0;
}

void handleStateInput(char *input, int lastFrameTime) {

    if((lastFrameTime - stateMachine.last_change) >= STATE_DEBOUNCE_DELAY) {

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

        xSemaphoreTake(stateMachine.lock, portMAX_DELAY);
        stateMachine.last_change = xTaskGetTickCount();
        xSemaphoreGive(stateMachine.lock);
    }
}

void deleteStateMachine(){
    vTaskDelete(StateMachineTask);
    deleteMenuTask();
    deletePlayTask();
    deleteGameOverTask();
    deleteScoreScreenTask();
    deleteMultiplayerTask();
    deleteIPConfigTask();
}