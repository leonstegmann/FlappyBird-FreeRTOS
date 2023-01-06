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

bird_t* createNewPlayer(){

    bird_t* ret = malloc(sizeof(bird_t));

    ret->height = 24;
    ret->width = 34;
    ret->velocityY = 0;
    ret->max_velocity = GRAVITY*10;
    ret->pos = (coord_t) {(SCREEN_WIDTH - ret->width)/2, (SCREEN_HEIGHT-FLOOR_HEIGHT)/2 };
    ret->lock = xSemaphoreCreateMutex(); // Locking mechanism
    ret->dead = false;

    return ret;
}

void updateBirdPosition( TickType_t xLastTimeUpdated, bird_t* player){
    TickType_t xtimepassed =  xTaskGetTickCount() - xLastTimeUpdated;
    if( xSemaphoreTake(player->lock, portMAX_DELAY )== pdTRUE){
        player->pos.y += player->velocityY * (int) xtimepassed /1000;   // devided by 1000 milliseconds
            
        if(player->velocityY <= player->max_velocity) {
            player->velocityY+=GRAVITY;
        }

        if(player->pos.y >= SCREEN_HEIGHT - FLOOR_HEIGHT - player->height/2) {
            player->pos.y = SCREEN_HEIGHT - FLOOR_HEIGHT - player->height/2;
        } 

        if(player->pos.y <= 0) {
            player->pos.y = 0;
            player->velocityY = 3*GRAVITY;
        }

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