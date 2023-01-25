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
#include "states.h" //for set_states()
#include "stateMachine.h"
#include "defines.h"

TaskHandle_t MenuScreen = NULL;

short changeBirdColour(short colour) {

    switch (vCheckArrowInput()){
        case 'R':
            if(colour == BLUE) {
                colour = YELLOW;
            } else {
                colour++;
            }
            return colour;

        case 'L':
            if(colour == YELLOW) {
                colour = BLUE;
            } else {
                colour--;
            }
            return colour;

        default:
            return colour;
    }
}

void vMenuScreen() {

    TickType_t xLastFrameTime = xTaskGetTickCount();
    
    short colour = 1;

    drawInitAnnimations();

    while(1) {
        if(DrawSignal) {
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
                
                drawBackround();
                drawButton(LEFT_BUTTON_POSITION, "Play", xLastFrameTime);
                drawButton(RIGHT_BUTTON_POSITION, "Score", xLastFrameTime);
                drawLogo(LOGO_POSITION);
                drawFloorAnnimations(xLastFrameTime);
                drawBirdAnnimations(xLastFrameTime, colour);

                // Time after everything is drawn
                xLastFrameTime = xTaskGetTickCount();

                // Show fps 
                drawFPS();
            }
        }

        xGetButtonInput();
        // Check which bird colour is chosen and send it to queue 
        colour = changeBirdColour(colour);   
        xQueueOverwrite(ColourQueue, &colour);           
        
        if(checkButton(KEYCODE(P))){ // Play           
            states_set_state(1);                 
        }
        if(checkButton(KEYCODE(S))){ // Score
            states_set_state(3);                       
        }
        
    }
}

int createMenuTask(void) {

    if(!xTaskCreate(vMenuScreen, "MenuScreen",  mainGENERIC_STACK_SIZE *2, NULL,
			        mainGENERIC_PRIORITY + 6, &MenuScreen)) {
                        return 1;
    }
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
