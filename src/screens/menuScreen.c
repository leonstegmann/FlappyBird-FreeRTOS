/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h" // for buttons.lock
#include"task.h" // for xTaskGetTickCount()
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* TUM_Library includes  */
#include "TUM_FreeRTOS_Utils.h" //for tumFUtilPrintTaskStateList

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

    while(1) {
        if(DrawSignal) {
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
                
                drawBackround();
                drawButton(LEFT_BUTTON_POSITION, "Play");
                drawButton(RIGHT_BUTTON_POSITION, "Score");
                drawButton(LOWER_MIDDLE_BUTTON_POSITION, "Multiplayer");
                drawLogo(LOGO_POSITION);
                drawFloorAnnimations(xLastFrameTime);
                drawBirdAnnimations(xLastFrameTime);

                // Time after everything is drawn
                xLastFrameTime = xTaskGetTickCount();

                // Show fps 
                drawFPS();
            }
        }
        xGetButtonInput();                
        if(checkButton(KEYCODE(P))){            
            states_set_state(1);                 
        }
        if(checkButton(KEYCODE(S))){
            states_set_state(3);                       
        }
        if(checkButton(KEYCODE(M))){
            states_set_state(4);                       
        }        
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

void deleteMenuTask(){
    if (MenuScreen)
        vTaskDelete(MenuScreen);
}

void enterMenuTask(void){
    printf("Enter Menu\n");
    vTaskResume(MenuScreen);
    tumFUtilPrintTaskStateList();
}

void exitMenuTask(){
    vTaskSuspend(MenuScreen);
}
