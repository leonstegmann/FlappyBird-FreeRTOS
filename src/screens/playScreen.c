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

    bird_t* player1 = createNewPlayer();

    /* create Pipes (here decided for having a maxium amount of 2 pipes on the screen)*/
    pipes_t* pipe1 = newPipe();
    pipes_t* pipe2 = newPipe();
    pipe2->positionX += SCREEN_WIDTH/2; // to ensure the Offset bewteen the 2 pipes 

    tumDrawBindThread();

    drawInitAnnimations();

    while(1){
        if(DrawSignal) {
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
                tumEventFetchEvents(FETCH_EVENT_NONBLOCK);

                drawBackround();
                drawPipe(pipe1);
                drawPipe(pipe2);
                drawFloorAnnimations(xLastFrameTime);
                drawBirdAnnimationsInGame(xLastFrameTime, player1);
                updateBirdPosition(xLastFrameTime, player1);
                updatePipePosition(xLastFrameTime, pipe1);
                updatePipePosition(xLastFrameTime, pipe2);
                xLastFrameTime = xTaskGetTickCount(); //  Actualize Time of the last drawn frame
            }
        }
        xGetButtonInput();                
            if(checkButton(KEYCODE(SPACE))){
                player1->velocityY -= UPWARDS_PUSH;
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