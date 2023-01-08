/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h" // for Semaphore take and give
#include"task.h" // for Taskhandle_t
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* Project includes  */
#include "main.h" // for Semaphore drawSignal
#include "buttons.h" // for checkButton()
#include "draw.h"   // for drawButtons() and drawGameOver()
#include "states.h" //for set_stages()

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)

TaskHandle_t GameOverScreen = NULL;

void vGameOverScreen() {
  /* 
    if(DrawSignal)
        if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE)
        
   */
    while(1) {
        if(DrawSignal)
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
            drawGameOver();
            drawButton(LEFT_BUTTON_POSITION, "Retry");
            drawButton(RIGHT_BUTTON_POSITION, "Menu");
            xGetButtonInput();                
            if(checkButton(KEYCODE(R))){
                printf("changing states\n");
                states_set_state(1);
            }
            else if(checkButton(KEYCODE(M))){
                printf("changing states\n");
                states_set_state(0);
            }
        }
        
    }
}

int createGameOverTask(void) {

    if(!xTaskCreate(vGameOverScreen, "GameOverScreen",  mainGENERIC_STACK_SIZE, NULL,
			        mainGENERIC_PRIORITY + 6, &GameOverScreen)) {
                        return 1;
    }
    vTaskSuspend(GameOverScreen); 
    return 0;
}

void deleteGameOverTask(){
    if (GameOverScreen)
        vTaskDelete(GameOverScreen);
}

void enterGameOverTask(void){
    vTaskResume(GameOverScreen);
}

void exitGameOverTask(){
    vTaskSuspend(GameOverScreen);
}
