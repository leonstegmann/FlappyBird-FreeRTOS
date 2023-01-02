/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h" // for buttons.lock
#include"task.h" // for xTaskGetTickCount()
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* TUM_Library includes  */
#include "TUM_FreeRTOS_Utils.h" //for tumFUtilPrintTaskStateList
#include "TUM_Event.h" // for tumEventFetchEvents(FETCH_EVENT_NONBLOCK);

/* Project includes  */
#include "menuScreen.h"
#include "main.h"
#include "buttons.h"
#include "draw.h"
#include "states.h" //for set_stages()

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)

TaskHandle_t MenuScreen = NULL;

void vMenuScreen() {
    // Time of the last drawn frame
    TickType_t xLastFrameTime = xTaskGetTickCount();

    drawInitAnnimations();

    coord_t playButtonPosition = {SCREEN_WIDTH/2 - BOX_WIDTH*1.5, SCREEN_HEIGHT/2 + 50};
    coord_t scoreButtonPosition = {SCREEN_WIDTH/2 + BOX_WIDTH*0.5, SCREEN_HEIGHT/2 + 50};
    coord_t logoPosition = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 150};

    while(1) {
        if(DrawSignal) {
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
                
                drawBackround();
                drawButton(playButtonPosition, "Play");
                drawButton(scoreButtonPosition, "Score");
                drawLogo(logoPosition);
                drawFloorAnnimations(xLastFrameTime);
                drawBirdAnnimations(xLastFrameTime);

                // Time after everithing is drawn
                xLastFrameTime = xTaskGetTickCount();

                // Show fps 
                drawFPS();
            xSemaphoreGive(DrawSignal);
            }
        }
        xGetButtonInput();                
        if(checkButton(KEYCODE(P))){
            printf("changing states\n");
            states_set_state(1);
            states_run();
            tumFUtilPrintTaskStateList();                
            }

    vTaskDelayUntil(&xLastFrameTime, (TickType_t) 10);
    }
}

int createMenuTask(void) {

    if(!xTaskCreate(vMenuScreen, "MenuScreen",  mainGENERIC_STACK_SIZE *2, NULL,
			        mainGENERIC_PRIORITY + 6, &MenuScreen)) {
                        return 1;
    }
    //vTaskSuspend(MenuScreen); 
    return 0;
}

void deleteMenuTask(void){
    if (MenuScreen)
        vTaskDelete(MenuScreen);
}

void enterMenuTask(void){
    vTaskResume(MenuScreen);
}

void exitMenuTask(void){
    vTaskSuspend(MenuScreen);
}
