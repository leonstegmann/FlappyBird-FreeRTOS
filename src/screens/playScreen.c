/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h" // for buttons.lock
#include"task.h" // for xTaskGetTickCount()
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* TUM_Library includes  */
#include "TUM_Event.h" // for tumEventFetchEvents();

/* Project includes  */
#include "playScreen.h"
#include "defines.h" //for Screencenter, stack_size and Task_priority
#include "main.h" // for DrawSignal
#include "buttons.h"
#include "draw.h"
#include "objects.h"

TaskHandle_t PlayScreen = NULL;

void vPlayScreen(){

   TickType_t xLastFrameTime = xTaskGetTickCount(); //  Time of the last drawn frame

    tumDrawBindThread();

    drawInitAnnimations();

    while(1){
        if(DrawSignal) {
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
                tumEventFetchEvents(FETCH_EVENT_NONBLOCK);

                drawBackround();
                drawFloorAnnimations(xLastFrameTime);
                drawBirdAnnimationsInGame(xLastFrameTime);
                updateBirdPosition(xLastFrameTime);
                xLastFrameTime = xTaskGetTickCount(); //  Actualize Time of the last drawn frame
            }
        }
        xGetButtonInput();                
            if(checkButton(KEYCODE(SPACE))){
                printf("Spacebar pressed");
        }
    }
}


int createPlayTask(){
    if(!xTaskCreate(vPlayScreen, "PlayScreen",  mainGENERIC_STACK_SIZE*2 , NULL, mainGENERIC_PRIORITY + 6, &PlayScreen)) {
        return 1;
    }
    vTaskSuspend(PlayScreen);
    return 0;
}

void deletePlayTask(){
 if (PlayScreen)
        vTaskDelete(PlayScreen);
}

void enterPlayTask(){
    vTaskResume(PlayScreen);
}

void exitPlayTask(){
    vTaskSuspend(PlayScreen);
}