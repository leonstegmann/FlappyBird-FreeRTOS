/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h" // for buttons.lock
#include"task.h" // for xTaskGetTickCount()
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* Project includes  */
#include "playScreen.h"
#include "defines.h" //for Screencenter, stack_size and Task_priority
#include "main.h" // for DrawSignal
#include "buttons.h"
#include "draw.h"

TaskHandle_t PlayScreen = NULL;

void vPlayScreen(){

   TickType_t xLastFrameTime = xTaskGetTickCount(); //  Time of the last drawn frame
 
    while(1){
        if(DrawSignal) {
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
                drawBackround();
                drawFloorAnnimations(xLastFrameTime);
                drawBirdAnnimations(xLastFrameTime);

                TickType_t xLastFrameTime = xTaskGetTickCount(); //  Time of the last drawn frame

                xGetButtonInput();                
                if(checkButton(KEYCODE(SPACE))){
                    tumDrawClear(White);

                }

            }
        }
    vTaskDelay((TickType_t) 50);
    }
}


int createPlayTask(){
    if(!xTaskCreate(vPlayScreen, "PlayScreen",  mainGENERIC_STACK_SIZE , NULL, mainGENERIC_PRIORITY, &PlayScreen)) {
        return 1;
    }
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