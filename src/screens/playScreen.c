/* Standard library includes */
#include <stdio.h> // for scanf

/* FreeRTOS includes  */
#include "FreeRTOS.h"
#include "semphr.h" // for buttons.lock
#include"task.h" // for xTaskGetTickCount()
#include <SDL2/SDL_scancode.h>  // Defines keyboard scancodes

/* TUM_Library includes  */
#include "TUM_Event.h" // for tumEventFetchEvents();
#include "TUM_FreeRTOS_Utils.h" // for tumFUtilPrintTaskStateList()
#include "TUM_Sound.h"

/* Project includes  */
#include "playScreen.h"
#include "defines.h" //for Screencenter, stack_size and Task_priority
#include "main.h" // for DrawSignal
#include "buttons.h"
#include "draw.h"
#include "objects.h"
#include "states.h"

TaskHandle_t PlayScreen = NULL;

void setScoreCheat(bird_t* player){    
    printf("in CHEAT Score Mode\n");
    short newScore = player->score;
    bool finished = false;
    do{ 
        if(DrawSignal)
            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
                tumEventFetchEvents(FETCH_EVENT_NONBLOCK);
                tumDrawClear(Grey); // Clear screen
                drawPause();
                drawScore(newScore);                
            }
        xGetButtonInput();
        if(checkButton(KEYCODE(S)))
            finished = true;
        switch (vCheckArrowInput()){
            case 'U':
                newScore++;
                break;
            case 'D':
                newScore--;
                break;
            default:
                break;
        }
        vTaskDelay(10);   
    } while(finished == false);
    player->score = newScore;
}

void godMode(bird_t* player){
    player->godMode = !player->godMode; // turn GodMode on/ off
    if(player->godMode == false) 
        printf("Back to Normal MODE\n");
    else 
        printf("GOD MODE ACTIVATED!\n");
}

void pauseGame(){
    bool pause = true;  
    do {
        xGetButtonInput();
        vTaskDelay((TickType_t) 10);
        if (checkButton(KEYCODE(P)))
            pause = false;
    } while(pause);
}

void vPlayScreen(){

    tumSoundLoadUserSample("../resources/waveforms/wing.wav");
    tumSoundLoadUserSample("../resources/waveforms/hit.wav");
    tumSoundLoadUserSample("../resources/waveforms/point.wav");
    tumSoundLoadUserSample("../resources/waveforms/die.wav");

    TickType_t xLastFrameTime = xTaskGetTickCount(); //  Time of the last drawn frame

    drawInitAnnimations();
    initHighscore();

    while(1){
        xGetButtonInput();
        if(DrawSignal) {

            if(xSemaphoreTake(DrawSignal, portMAX_DELAY) == pdTRUE) {
                tumEventFetchEvents(FETCH_EVENT_NONBLOCK);

                drawBackround();
                drawPipe(pipe1);
                drawPipe(pipe2);
                drawFloorAnnimations(xLastFrameTime);
                drawBirdAnnimationsInGame(xLastFrameTime, player1);

                if(checkScore(player1, pipe1, pipe2)) {
                    tumSoundPlayUserSample("point.wav");
                }

                drawScore(player1->score);
                
                if(!player1->godMode && checkCollision(player1, pipe1, pipe2)) {
                    tumSoundPlayUserSample("hit.wav");
                    resetPlayer(player1);
                    resetPipes(pipe1, pipe2);
                    tumSoundPlayUserSample("die.wav");
                    states_set_state(2);

                } 
                else if(checkButton(KEYCODE(P))){
                        drawPause();
                        pauseGame();
                }
                else if (checkButton(KEYCODE(G))){
                    godMode(player1);
                }
                else if (checkButton(KEYCODE(S))){
                    setScoreCheat(player1);
                }
                else {
                    if(checkButton(KEYCODE(SPACE))){
                        if(player1->velocityY >= -player1->max_velocity) {
                            xSemaphoreTake(player1->lock, portMAX_DELAY);
                            player1->velocityY -= UPWARDS_PUSH;
                            xSemaphoreGive(player1->lock);
                            tumSoundPlayUserSample("wing.wav");
                        }
                    }

                    updateBirdPosition(xLastFrameTime, player1);
                    updatePipePosition(xLastFrameTime, pipe1);
                    updatePipePosition(xLastFrameTime, pipe2);
                }
            
                drawFPS();

                xLastFrameTime = xTaskGetTickCount(); // Time of the last drawn frame
            }
        }    
    }
}


int createPlayTask(){

    createObjects();

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
    printf("Start game\n");
    vTaskResume(PlayScreen);
    tumFUtilPrintTaskStateList();
}

void exitPlayTask(){
    vTaskSuspend(PlayScreen);
}