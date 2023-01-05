/* Standard library includes */
#include "stdio.h"
#include <stdlib.h> // for malloc()
#include "math.h" // for % operator

/* FreeRTOS includes  */
#include "FreeRTOS.h" // for TickType_t
#include "semphr.h"
#include "task.h" 

/* Project includes  */
#include "objects.h"
#include "defines.h"

/* Defining and initializing "Object"*/
bird_t bird = {.height = 34, .width = 34 , .velocityY = 0, .pos = (coord_t) {SCREEN_WIDTH/2 - 34/2, SCREEN_HEIGHT - 300}, .dead = false };

/* creating pointer to "object" for extern usage */
bird_t* player = &bird;

int initPlayer(){
    player->lock = xSemaphoreCreateMutex(); // Locking mechanism
        if (!player->lock) {
        printf("Failed to create bird lock");
        return -1;
    }

    return 0;
}

void updateBirdPosition( TickType_t xLastTimeUpdated){
    TickType_t xtimepassed =  xTaskGetTickCount() - xLastTimeUpdated;
    if( xSemaphoreTake(player->lock, portMAX_DELAY )== pdTRUE){
        player->pos.y += player->velocityY * (int) xtimepassed /100;   // devided by 1000 milliseconds
        player->velocityY+=GRAVITY;
        xSemaphoreGive(player->lock);
    }    

}

pipes_t* newPipe(){

    pipes_t* ret = malloc(sizeof(pipes_t));
    
    ret->lowerPipeImage = tumDrawLoadScaledImage(LOWER_PIPE_FILENAME, 0.2);
    ret->upperPipeImage = tumDrawLoadScaledImage(UPPER_PIPE_FILENAME, 0.2);
    ret->image_height = tumDrawGetLoadedImageHeight(ret->lowerPipeImage);
    ret->image_width = tumDrawGetLoadedImageWidth(ret->lowerPipeImage);
    ret->gap_center = (SCREEN_HEIGHT-FLOOR_HEIGHT)/2;
    ret->positionX = SCREEN_WIDTH;
    ret->velocityX = PIPE_VELOCITY;
    ret->lock = xSemaphoreCreateMutex(); // Locking mechanism
  
    return ret;

}

short updatePipePosition( TickType_t xLastTimeUpdated, pipes_t* pipe){
    short ret = 0;
    if(pipe != NULL){
        TickType_t xtimepassed =  xTaskGetTickCount() - xLastTimeUpdated;
        if( xSemaphoreTake(pipe->lock, portMAX_DELAY) == pdTRUE){
            pipe->positionX -= pipe->velocityX* xtimepassed /1000;
            if (pipe->positionX + pipe->image_width < 0){
                pipe->gap_center = randomGenerator( GAP_LOWEST, GAP_HIGHEST); // generate Random Gap center
                pipe->positionX += SCREEN_WIDTH + pipe->image_width; //respawn Pipe at Right end of the Screen
            }
            xSemaphoreGive(pipe->lock);
            ret = 1;
        }

    }
    return ret;
}

short randomGenerator(short min, short max){
    short ret = min + (rand() % max);
    return  ret;
}