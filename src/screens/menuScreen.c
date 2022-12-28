/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h" // for buttons.lock
#include"task.h" // for xTaskGetTickCount()
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* Project includes  */
#include "menuScreen.h"
#include "main.h"
#include "buttons.h"
#include "draw.h"

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

                /*Testing buttons*/
                xGetButtonInput();                
                if(checkButton(KEYCODE(X)))
                    tumDrawClear(White);
                
                //vTaskDelay((TickType_t) 50);
            }
        }
    }
}

int createMenuTask() {

    if(!xTaskCreate(vMenuScreen, "MenuScreen",  mainGENERIC_STACK_SIZE , NULL,
			        mainGENERIC_PRIORITY, &MenuScreen)) {
                        return 1;
    }
    
    return 0;
}