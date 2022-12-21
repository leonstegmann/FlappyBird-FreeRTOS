/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h" // for buttons.lock
#include"task.h" // for xTaskGetTickCount()
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* Project includes  */
#include "menuScreen.h"
#include "main.h"
#include "buttons.h"

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)

TaskHandle_t MenuScreen = NULL;

void vMenuScreen() {

    while(1) {

        if(DrawSignal) {
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
                
                tumDrawClear(Silver);
                drawBackround();
                //drawButton();

                /*Testing buttons*/
                xGetButtonInput();                
                if(checkButton(KEYCODE(X)))
                    tumDrawClear(White);
                
                vTaskDelay((TickType_t) 100);
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