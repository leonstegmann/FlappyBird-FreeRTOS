#include "FreeRTOS.h" // for TickType_t
#include "semphr.h"
#include "task.h" 
#include "stdio.h"

#include "objects.h"

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

short checkCollision(){
    short ret = 0;
     if ( xSemaphoreTake(player->lock, portMAX_DELAY) == pdTRUE ){
        if ((player->pos.y-player->height) > GROUND){
            player->velocityY = 0;
            player->dead = true;
            ret = 1;
        }
        xSemaphoreGive(player->lock);
     }
    return ret;
}