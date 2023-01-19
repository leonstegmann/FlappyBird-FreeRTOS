/* Standard library includes */
#include "stdio.h"

/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include"task.h" // for Task_handle
#include "semphr.h"
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* AsyncIO includes  */
#include "AsyncIO.h" // for UDP connection

/* TUM_Library includes  */
#include "TUM_Draw.h"
#include "TUM_Event.h" // for FETCH_EVENT_NONBLOCK
#include "TUM_FreeRTOS_Utils.h" //for tumFUtilPrintTaskStateList

/* Project includes  */
#include "multiplayerHandler.h"
#include "defines.h"
#include "main.h"
#include "buttons.h"
#include "draw.h"
#include "multiplayerIPConfigScreen.h"
#include "states.h"

#define UDP_BUFFER_SIZE 2000

static TaskHandle_t MultiplayerScreen = NULL;



void vMultiplayerScreen() {

    while(1) {
        if(DrawSignal)
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
                drawBackround();
                drawButton(LEFT_BUTTON_POSITION, "Host"); // Master
                drawButton(RIGHT_BUTTON_POSITION,"Invite"); // Slave
                vTaskDelay(10);
            }
        xGetButtonInput();
        if(checkButton(KEYCODE(H))){
            printf("Host\n");
            states_set_state(5);
            
        }        
        if(checkButton(KEYCODE(I))){
            printf("Invite\n");
        }    
    }
}


int createMultiplayerTask(void) {

    if(!xTaskCreate(vMultiplayerScreen, "MultiplayerScreen",  mainGENERIC_STACK_SIZE *2, NULL,
			        mainGENERIC_PRIORITY + 6, &MultiplayerScreen)) {
                        return 1;
    }
    vTaskSuspend(MultiplayerScreen); 
    return 0;
}

void deleteMultiplayerTask(){
    if (MultiplayerScreen)
        vTaskDelete(MultiplayerScreen);
}

void enterMultiplayerTask(void){
    printf("Enter Multiplayer\n");
    vTaskResume(MultiplayerScreen);
    tumFUtilPrintTaskStateList();
}

void exitMultiplayerTask(){
    vTaskSuspend(MultiplayerScreen);
}

