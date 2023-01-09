/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h" // for Semaphore take and give
#include"task.h" // for Taskhandle_t
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes
#include "TUM_Utils.h"

/* Project includes  */
#include "main.h" // for Semaphore drawSignal
#include "buttons.h" // for checkButton()
#include "draw.h"   // for drawButtons() and drawGameOver()
#include "states.h" //for set_stages()
#include "scoreScreen.h"
#include "defines.h"

TaskHandle_t ScoreScreen = NULL;

void vScoreScreen() {

    TickType_t xLastFrameTime = xTaskGetTickCount();

    while(1) {
        if(DrawSignal)
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
            drawBackround();
            drawHighscore(HIGHSCORE_POSITION);
            drawLogo(LOGO_POSITION);
            drawButton(RIGHT_BUTTON_POSITION, "Menu");
            drawButton(LEFT_BUTTON_POSITION, "Play");
            drawFloorAnnimations(xLastFrameTime);

            // Time after everything is drawn
            xLastFrameTime = xTaskGetTickCount();

            // Show fps 
            drawFPS();

            xGetButtonInput();                
            if(checkButton(KEYCODE(P))){
                states_set_state(1);
            }
            else if(checkButton(KEYCODE(M))){
                states_set_state(0);
            }
        }
        
    }
}

int createScoreScreenTask(void) {

    if(!xTaskCreate(vScoreScreen, "ScoreScreen",  mainGENERIC_STACK_SIZE, NULL,
			        mainGENERIC_PRIORITY + 6, &ScoreScreen)) {
                        return 1;
    }
    vTaskSuspend(ScoreScreen); 
    return 0;
}

void deleteScoreScreenTask(){
    if (ScoreScreen)
        vTaskDelete(ScoreScreen);
}

void enterScoreScreenTask(void){
    printf("Enter highscore\n");
    tumFUtilPrintTaskStateList();
    vTaskResume(ScoreScreen);
}

void exitScoreScreenTask(){
    vTaskSuspend(ScoreScreen);
}
